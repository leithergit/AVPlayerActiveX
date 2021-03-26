
/*************************************************************************/
/*                                                                       */
/*                       MPEG4 BitStream�������                         */
/*								winton			                         */
/*							  2008-1-30                                  */
/*                                                                       */
/*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "decSPS.h"

struct Bitstream
{
	unsigned int bufa;
	unsigned int bufb;
	unsigned int buf;
	unsigned int pos;
	unsigned int *tail;
	unsigned int *start;
	unsigned int length;
};

int remove_0x03(unsigned char *src, unsigned char *dst, int size)
{
	int i, rbsp_pos;
	unsigned int tmp;
	//unsigned char *srcPtr, *dstPtr;

    rbsp_pos = 0;
	for(i=0;i<(size-2);i++)
	{
		tmp = (src[i]<<16)|(src[i+1]<<8)|(src[i+2]);
		if(0x03==tmp)
		{
			dst[rbsp_pos] = src[i];
			dst[rbsp_pos+1] = src[i+1];
			i+=2;
			rbsp_pos+=2;
		}	
		else
			dst[rbsp_pos++] = src[i];
	}
	return rbsp_pos;
}

unsigned int get_int(void *ptr)
{
	unsigned char *buffer = (unsigned char *)ptr;
	return (*buffer) | ((*(buffer + 1)) << 8)| ((*(buffer + 2)) << 16)| ((*(buffer + 3)) << 24);
}

int MPEG4_BSWAP(void *v)
{      
	unsigned int x =  *(unsigned int *) v;
	
	*(int*)v = (x&0xFF)<<24|(x&0xFF00)<<8|(x&0xFF0000)>>8|(x&0xFF000000)>>24;	 
	return 0;
}

void MPEG4_BitstreamInit(Bitstream *  bs, void * bitstream, unsigned int length)
{
	unsigned int tmp;

	bs->start = bs->tail = (unsigned int *) bitstream;

	//tmp = *(unsigned int *) bitstream;
	tmp = get_int(bitstream);
	
	MPEG4_BSWAP(&tmp);

	bs->bufa = tmp;

	//tmp = *((unsigned int *) bitstream + 1);
	tmp = get_int((void*)((unsigned long)bitstream + sizeof(int)));
	MPEG4_BSWAP(&tmp);
	bs->bufb = tmp;

	bs->buf = 0;
	bs->pos = 0;
	bs->length = length;
}

unsigned int MPEG4_BitstreamShowBits(Bitstream *  bs, unsigned int bits)
{
	int nbit = (bits + bs->pos) - 32;
	
	if (nbit > 0) {
		return ((bs->bufa & (0xffffffff >> bs->pos)) << nbit) | (bs->bufb >> (32 - nbit));
	} else {
		return (bs->bufa & (0xffffffff >> bs->pos)) >> (32 - bs->pos - bits);
	}
}

void MPEG4_BitstreamSkip(Bitstream * bs, unsigned int bits)
{
	bs->pos += bits;

	if (bs->pos >= 32) 
	{
		unsigned int tmp;

		bs->bufa = bs->bufb;
		//tmp = *((unsigned int *) bs->tail + 2);
		tmp = get_int((void *)((unsigned int *) bs->tail + 2));
		MPEG4_BSWAP(&tmp);
		bs->bufb = tmp;
		bs->tail++;
		bs->pos -= 32;
	}

}

void MPEG4_BitstreamByteAlign(Bitstream * bs)
{
	unsigned int remainder = bs->pos % 8;

	if (remainder) 
	{
		MPEG4_BitstreamSkip(bs, 8 - remainder);
	}
}

unsigned int MPEG4_BitstreamGetBits(Bitstream * const bs,unsigned int n)
{
	unsigned int ret = MPEG4_BitstreamShowBits(bs, n);
	
	MPEG4_BitstreamSkip(bs, n);

	return ret;
}

unsigned int MPEG4_BitstreamGetBit(Bitstream * const bs)
{
	return MPEG4_BitstreamGetBits(bs, 1);
}


unsigned int MPEG4_log2bin(unsigned int value)
{
	int n = 0;

	while (value) 
	{
		value >>= 1;
		n++;
	}
	return n;
}

unsigned int MPEG4_BitstreamPos(Bitstream * bs)
{
	return((unsigned int)(8*((unsigned long)bs->tail - (unsigned long)bs->start) + bs->pos));
}

/*************************************************************************/
/*                                                                       */
/*                         H264 BitStream�������                        */
/*								winton			                         */
/*							  2008-1-30                                  */
/*                                                                       */
/*************************************************************************/


typedef struct GetBitContext {
    unsigned char *buffer;
	unsigned char *buffer_end;
    int index;
    int size_in_bits;
} GetBitContext;

unsigned char _ff_golomb_vlc_len[32]={
0, 9, 7, 7, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

unsigned char ff_log2_table[128]={
0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
};


unsigned char _ff_ue_golomb_vlc_code[256]={ 
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13,14,14,14,14,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};

char ff_se_golomb_vlc_code_[256]={ 
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8, -8,  9, -9, 10,-10, 11,-11, 12,-12, 13,-13, 14,-14, 15,-15,
  4,  4,  4,  4, -4, -4, -4, -4,  5,  5,  5,  5, -5, -5, -5, -5,  6,  6,  6,  6, -6, -6, -6, -6,  7,  7,  7,  7, -7, -7, -7, -7,
  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  };

int H264_unaligned32_be(void *v)
{      
	//unsigned int x =  *(unsigned int *) v;
	unsigned int x =  get_int(v);
	return  (x&0xFF)<<24|(x&0xFF00)<<8|(x&0xFF0000)>>8|(x&0xFF000000)>>24;	 
}

void H264_init_get_bits(GetBitContext *s, unsigned char *buffer, int bit_size)
{
    int buffer_size= (bit_size+7)>>3;

    s->buffer= buffer;
    s->size_in_bits= bit_size;
    s->buffer_end= buffer + buffer_size;    
	s->index=0;
    {
		int re_index= s->index;
        int re_cache=  H264_unaligned32_be( ((unsigned char *)s->buffer)+(re_index>>3) ) << (re_index&0x07);	
        
		s->index= re_index;
    }
}

void H264_skip_bits(GetBitContext *s, int n)
{
	s->index += n;
}

void H264_skip_one_bits(GetBitContext *s)
{
    H264_skip_bits(s, 1);
}

unsigned int H264_get_one_bit(GetBitContext *s)
{

    int index = s->index;
    unsigned char result = s->buffer[index>>3];
    result <<= (index&0x07);
    result >>= 7;
    index++;
    s->index = index;
    return result;
}
int unaligned32_be(void *v)
{      
	unsigned int x =  *(unsigned int *) v;
	
	return  ((x&0xff)<<24)|((x&0xff00)<<8)|((x&0xff0000)>>8)|((x&0xff000000)>>24);	 
}



unsigned int H264_get_bits(GetBitContext *s, int n)
{

	int tmp;
    int re_index= s->index;
    int re_cache= 0;
    re_cache= unaligned32_be( ((unsigned char *)(s)->buffer)+(re_index>>3) ) << (re_index&0x07);
	tmp= (((unsigned long)(re_cache))>>(32-(n)));
    re_index += n;
    s->index= re_index;
    return tmp;
    
}
unsigned int get_bits_long(GetBitContext *s, int n)
{
    if(n<=17) 
	{
		return H264_get_bits(s, n);
	}
    else
	{
        int ret= H264_get_bits(s, 16) << (n-16);
        return ret | H264_get_bits(s, n-16);
    }
}
int H264_av_log2(unsigned int v)
{
    int n = 0;
    if (v & 0xffff0000) 	
	{
        v >>= 16;
        n += 16;
		if(v & 0xff00)
		{
			v >>= 8;
			n += 8;
		}
    }
	else if (v & 0xff00) 
	{
        v >>= 8;
        n += 8;
    }
	n += ff_log2_table[v>>1];

    return n;
}

int H264_get_ue_golomb(GetBitContext *gb)
{
    unsigned int	buf;
    int				log;
    int				re_index = gb->index;
    int				re_cache = 0;
	re_cache		= H264_unaligned32_be( ((unsigned char *)(gb)->buffer)+(re_index>>3) ) << (re_index&0x07);
    buf				= (unsigned int)re_cache;
    
    if(buf >= (1<<27))
	{
        buf >>= 32 - 9;
		re_index += _ff_golomb_vlc_len[buf>>4];
    	
		gb->index = re_index;  

		if(buf<256)
		{
			return _ff_ue_golomb_vlc_code[buf];
		}
		else
		{
			return 0;
		}    
    }
	else
	{
        log = 2*H264_av_log2(buf) - 31;
        buf>>= log;
        buf--;
        re_index += (32 - log);
		gb->index= re_index;    
        return buf;
    }
}


int H264_get_se_golomb(GetBitContext *gb)
{
    unsigned int	buf;
    int				log;
    int				re_index= gb->index;
    int				re_cache= 0;
    re_cache		= H264_unaligned32_be( ((unsigned char *)(gb)->buffer)+(re_index>>3) ) << (re_index&0x07);    
    buf				= re_cache;
    
    if(buf >= (1<<27))
	{
        buf >>= 32 - 9;       		
		re_index += _ff_golomb_vlc_len[buf>>4];		
		gb->index= re_index;
		if(buf<256)
		{
			return ff_se_golomb_vlc_code_[buf];
		}
		else
		{
			return 0;
		}           
    }
	else
	{
        log = 2*H264_av_log2(buf) - 31;
        buf >>= log;
        re_index += (32 - log);
		gb->index = re_index;
		int nRes = buf;
		if (nRes & 1)
			nRes = -(nRes >> 1);
        else      
			nRes = (nRes >> 1);

		return nRes;
    }
}
/*************************************************************************/
/*                                                                       */
/*                         MPEG4 Analyse function                        */
/*								winton			                         */
/*							  2008-1-30                                  */
/*                                                                       */
/*************************************************************************/
int Mpeg4_Analyse(unsigned char* pBuf,int nSize,int* nWidth,int* nHeight,int*framerate)
{
	Bitstream		bs;
	int				time_increment				= 0;
	int				frame_type					= 0;
	int				shape						= 0;
	int				time_inc_bits				= 0;
	unsigned int	vol_ver_id					= 0;
	unsigned int	time_increment_resolution	= 0;
	unsigned int	coding_type					= 0;
	unsigned int	start_code					= 0;
	unsigned int	time_incr					= 0;	

	MPEG4_BitstreamInit(&bs, pBuf, nSize);

	do 
	{
		MPEG4_BitstreamByteAlign(&bs);
		start_code = MPEG4_BitstreamShowBits(&bs, 32);

		if (start_code == 0x01b0) 
		{	
			MPEG4_BitstreamSkip(&bs, 40);
		}
		else if (start_code == 0x01b1) 
		{
			MPEG4_BitstreamSkip(&bs, 32);	
		} 
		else if (start_code == 0x01b5) 
		{
			MPEG4_BitstreamSkip(&bs, 32);	
			if (MPEG4_BitstreamGetBit(&bs))
			{
				vol_ver_id = MPEG4_BitstreamGetBits(&bs, 4);					
				MPEG4_BitstreamSkip(&bs, 3);	
			} 
			else 
			{
				vol_ver_id = 1;
			}

			if (MPEG4_BitstreamShowBits(&bs, 4) != 1)
			{
				return -1;
			}
			MPEG4_BitstreamSkip(&bs, 4);

			if (MPEG4_BitstreamGetBit(&bs))
			{
				MPEG4_BitstreamSkip(&bs, 4);	
				
				if (MPEG4_BitstreamGetBit(&bs))
				{
					MPEG4_BitstreamSkip(&bs, 24);
				}
			}
		} 
		else if ((start_code & ~0x0000001F) == 0x00000100) 
		{
			MPEG4_BitstreamSkip(&bs, 32);
		} 
		else if ((start_code & ~0x0000000F) == 0x00000120)
		{
			MPEG4_BitstreamSkip(&bs, 33);
			
			if (MPEG4_BitstreamShowBits(&bs, 8) != 1 && 
				MPEG4_BitstreamShowBits(&bs, 8) != 3 && 
				MPEG4_BitstreamShowBits(&bs, 8) != 4 && 
				MPEG4_BitstreamShowBits(&bs, 8) != 0)
			{
				return -1;
			}

			MPEG4_BitstreamSkip(&bs, 8);

			if (MPEG4_BitstreamGetBit(&bs))	
			{
				vol_ver_id = MPEG4_BitstreamGetBits(&bs, 4);	
				MPEG4_BitstreamSkip(&bs, 3);	
			} 
			else 
			{
				vol_ver_id = 1;
			}

			if (MPEG4_BitstreamGetBits(&bs, 4) == 15)	
			{				
				MPEG4_BitstreamSkip(&bs, 16);
			}

			if (MPEG4_BitstreamGetBit(&bs))	
			{				
				MPEG4_BitstreamSkip(&bs, 3);				
				
				if (MPEG4_BitstreamGetBit(&bs))
				{
					MPEG4_BitstreamSkip(&bs, 81);
				}
			}

			shape = MPEG4_BitstreamGetBits(&bs, 2);
			if ( shape == 3 && vol_ver_id != 1) 
			{
				MPEG4_BitstreamSkip(&bs, 4);
			}

			MPEG4_BitstreamSkip(&bs, 1);

			time_increment_resolution = MPEG4_BitstreamGetBits(&bs, 16);	

			*framerate = time_increment_resolution;
			if (time_increment_resolution > 0) 
			{
				time_inc_bits = MPEG4_log2bin(time_increment_resolution-1);
			} 
			else 
			{
				time_inc_bits = 1;
			}

			MPEG4_BitstreamSkip(&bs, 1);

			if (MPEG4_BitstreamGetBit(&bs))
			{
					MPEG4_BitstreamSkip(&bs, time_inc_bits);				
			}

			if (shape != 2) 
			{
				if (shape == 0) 
				{
					MPEG4_BitstreamSkip(&bs, 1);
					*nWidth = MPEG4_BitstreamGetBits(&bs, 13);	
					MPEG4_BitstreamSkip(&bs, 1);
					*nHeight = MPEG4_BitstreamGetBits(&bs, 13);				
					return 0;
				}
				return -1;
			} 
			else
			{
				if (vol_ver_id != 1) 
				{
					if (MPEG4_BitstreamGetBit(&bs))
					{
						return -1;
					}
				}
				MPEG4_BitstreamSkip(&bs, 1);	
			}
		}
		else if (start_code == 0x01b3) 
		{
			MPEG4_BitstreamSkip(&bs, 32);
			{
				MPEG4_BitstreamSkip(&bs, 18);
			}
			MPEG4_BitstreamSkip(&bs, 2);	
		} 
		else if (start_code == 0x01b6) 
		{
			MPEG4_BitstreamSkip(&bs, 32);	
			coding_type = MPEG4_BitstreamGetBits(&bs, 2);
			frame_type	= coding_type;
			if(coding_type==3)
				coding_type=1;

			while (MPEG4_BitstreamGetBit(&bs) != 0)	
			{
				time_incr++;
			}
			
			MPEG4_BitstreamSkip(&bs, 1);

			if (time_inc_bits)
			{
				time_increment = (MPEG4_BitstreamGetBits(&bs, time_inc_bits));
			}

			MPEG4_BitstreamSkip(&bs, 1);

			if (!MPEG4_BitstreamGetBit(&bs))
			{				
				return -1;
			}

			if ((shape != 2) && (coding_type == 1)) 
			{
				MPEG4_BitstreamSkip(&bs,1);				
			}

			if (shape != 0) 
			{
				unsigned int width, height;
				unsigned int horiz_mc_ref, vert_mc_ref;

				width = MPEG4_BitstreamGetBits(&bs, 13);
				MPEG4_BitstreamSkip(&bs,1);
				height = MPEG4_BitstreamGetBits(&bs, 13);
				MPEG4_BitstreamSkip(&bs,1);
				horiz_mc_ref = MPEG4_BitstreamGetBits(&bs, 13);
				MPEG4_BitstreamSkip(&bs,1);
				vert_mc_ref = MPEG4_BitstreamGetBits(&bs, 13);				
				MPEG4_BitstreamSkip(&bs, 2);
				
				if (MPEG4_BitstreamGetBit(&bs))
				{
					MPEG4_BitstreamSkip(&bs, 8);
				}
			}			
			return -1;
		} 
		else if (start_code == 0x01b2) 
		{
			MPEG4_BitstreamSkip(&bs, 48);
		} 
		else
		{
			MPEG4_BitstreamSkip(&bs, 32);	
		}

	}
	while ((MPEG4_BitstreamPos(&bs) >> 3) < bs.length);

	return -1;	
}

/*************************************************************************/
/*                                                                       */
/*                         H264 Analyse function                         */
/*								winton			                         */
/*							  2008-1-31                                  */
/*                                                                       */
/*************************************************************************/
int H264_Analyse(unsigned char* pBuf,int nSize,int* nWidth,int* nHeight,int* framerate)
{
	GetBitContext	gb;    
	int				i					= 0;
	int				poc_type			= 0;
	int				poc_cycle_length	= 0;	
	int				tmp					= 0;
	int				mb_width,mb_height,flag_a,flag_b;
	int				crop_left,crop_right,crop_top,crop_bottom;  
	unsigned int    profile; 
    unsigned char sps_nal[512];
	int sps_size;
    
	if(nSize>512)
		nSize = 512;
    
    sps_size = remove_0x03(pBuf, sps_nal, nSize);

	crop_left= 0;
	crop_right= 0 ;
	crop_top= 0;
	crop_bottom=0;

	H264_init_get_bits(&gb, sps_nal+4, sps_size-4);

	profile=H264_get_bits(&gb,8);
	H264_skip_bits(&gb,16);

	H264_get_ue_golomb(&gb);
	if(profile>=100)
	{
		unsigned int seq_scaling_matrix_present_flag;
		H264_get_ue_golomb(&gb);
		H264_get_ue_golomb(&gb);
		H264_get_ue_golomb(&gb);
		H264_skip_bits(&gb,1);
		seq_scaling_matrix_present_flag=H264_get_bits(&gb,1);
		if(seq_scaling_matrix_present_flag)
		{
			int scal_list=0;
			int idx=0;
			for(idx=0;idx<8;idx++)
			{
				int last = 8, next = 8;
				scal_list=H264_get_bits(&gb,1);
				if(scal_list)
				{
					int j=0;
					int t=(idx>=6?64:16);
					for(j=0;j<t;j++)
					{
						if (next)
							next = (last+H264_get_se_golomb(&gb)) & 0xff;
						else
						{
							int zgf = 0;
						}
						last = next ? next : last;
					}
				}
			}

		}
	}

	H264_get_ue_golomb(&gb);
	poc_type = H264_get_ue_golomb(&gb);    
	if(poc_type == 0)
	{					
		H264_get_ue_golomb(&gb);
	} 
	else if(poc_type == 1)
	{					
		H264_skip_one_bits(&gb);
		H264_get_se_golomb(&gb);	
		H264_get_se_golomb(&gb);
		poc_cycle_length = H264_get_ue_golomb(&gb);
    
		for(i=0; i<poc_cycle_length; i++)
		{
			H264_get_se_golomb(&gb);
		}
	}
	if(poc_type > 2)
	{
		printf("H264_Analyse error poc_type[%d]\n", poc_type);
		return -1;
	}

	H264_get_ue_golomb(&gb);		
	H264_skip_one_bits(&gb);
        
	mb_width= ( H264_get_ue_golomb(&gb)+1);
	mb_height=( H264_get_ue_golomb(&gb) +1);



	flag_a = H264_get_one_bit(&gb);
	if(!flag_a)
	{
		H264_skip_one_bits(&gb);
	}
	H264_skip_one_bits(&gb);

	flag_b=H264_get_one_bit(&gb);
	
	if(flag_b)
	{
		crop_left=H264_get_ue_golomb(&gb);
		crop_right=H264_get_ue_golomb(&gb);
		crop_top=H264_get_ue_golomb(&gb);
		crop_bottom=H264_get_ue_golomb(&gb);
	}
	*nWidth= 16 * mb_width- 2 * (crop_left + crop_right );
	if (flag_a)
	{
		*nHeight  = 16 * mb_height- 2 * (crop_top + crop_bottom);
	}
	else
	{
		*nHeight  = 16 * mb_height - 4 * (crop_top +crop_bottom);
	} 

	if(H264_get_one_bit(&gb))
	{
		tmp =H264_get_one_bit(&gb);
		if(tmp)
		{
			tmp =H264_get_bits(&gb,8);
			if(tmp == 255)
			{
				H264_skip_bits(&gb,32);
			}
		}
		if(H264_get_one_bit(&gb))
		{							
			H264_skip_one_bits(&gb);
		}

		if(H264_get_one_bit(&gb))
		{	
			H264_skip_bits(&gb, 3);	
			H264_skip_one_bits(&gb);
			if(H264_get_one_bit(&gb))
			{						
				H264_skip_bits(&gb, 8);
				H264_skip_bits(&gb, 8);
				H264_skip_bits(&gb, 8);
			}
		}

		if(H264_get_one_bit(&gb))
		{
			H264_get_ue_golomb(&gb);
			H264_get_ue_golomb(&gb);
		}

		if(H264_get_one_bit(&gb))
		{	
			int num_units_in_tick, timeScale ;
			num_units_in_tick  = get_bits_long(&gb,32);
			timeScale = get_bits_long(&gb,32);
			if(num_units_in_tick)
				*framerate = timeScale/(2*num_units_in_tick);
		}
		
	}

	
	return 1;


}


int H265_Analyse(unsigned char* pBuf,int nSize,int* nWidth,int* nHeight,int* framerate)
{
	GetBitContext	gb;    
	int				i, j;
	int				tmp					= 0;
	int				width,height,flag_a,flag_b, num_short_term_rps, maxPicOrderCntLsb;
	int				crop_left,crop_right,crop_top,crop_bottom; 
	unsigned char sps_nal[512];
	int sps_size, num_negative_pics[65], num_positive_pics[65];

	if(nSize>512)
		nSize = 512;

	sps_size = remove_0x03(pBuf, sps_nal, nSize);

	crop_left= 0;
	crop_right= 0 ;
	crop_top= 0;
	crop_bottom=0;

	H264_init_get_bits(&gb, sps_nal+5, sps_size-5);

	H264_skip_bits(&gb,8+8+32+4+32+20);
	H264_get_ue_golomb(&gb);
    H264_get_ue_golomb(&gb);

	width = H264_get_ue_golomb(&gb);
	height = H264_get_ue_golomb(&gb);

	flag_b=H264_get_one_bit(&gb);

	if(flag_b)
	{
		crop_left=H264_get_ue_golomb(&gb);
		crop_right=H264_get_ue_golomb(&gb);
		crop_top=H264_get_ue_golomb(&gb);
		crop_bottom=H264_get_ue_golomb(&gb);
	}

	*nWidth= width- 2 * (crop_left + crop_right );
	*nHeight  = height - 2 * (crop_top +crop_bottom); 

	H264_get_ue_golomb(&gb);
	H264_get_ue_golomb(&gb);
	maxPicOrderCntLsb = 1<<(H264_get_ue_golomb(&gb)+4);
    H264_skip_bits(&gb,1);
	//sub_layer_ordering_info_flg
	H264_get_ue_golomb(&gb);
	H264_get_ue_golomb(&gb);
	H264_get_ue_golomb(&gb);


	H264_get_ue_golomb(&gb);
	H264_get_ue_golomb(&gb);
	H264_get_ue_golomb(&gb);
	H264_get_ue_golomb(&gb);
	H264_get_ue_golomb(&gb);
	H264_get_ue_golomb(&gb);
    //scaling_list_enc_flg	
	//pcm_enc_flag
    H264_skip_bits(&gb,4);
	//num_short_term_rps
    num_short_term_rps = H264_get_ue_golomb(&gb);
	for(i=0;i<num_short_term_rps;i++)
	{
		int inter_ref_pic_set_pred_flg = 0;
		if(i)
			inter_ref_pic_set_pred_flg=(int) H264_get_one_bit(&gb);

		if(inter_ref_pic_set_pred_flg)
		{
			int numDeltaPocs, refRpsIdx, delta_idx;
			if(i==num_short_term_rps)
				delta_idx = H264_get_ue_golomb(&gb)+1;

			refRpsIdx = i - delta_idx;
			H264_skip_bits(&gb, 1);
            H264_get_ue_golomb(&gb);

            numDeltaPocs = num_negative_pics[refRpsIdx] + num_positive_pics[refRpsIdx];
			for(j=0;j<(numDeltaPocs+1);j++)
			{
				flag_a =H264_get_one_bit(&gb);
				if(!flag_a)
					H264_skip_bits(&gb, 1);	
			}
		}
		else
		{
			num_negative_pics[i] = H264_get_ue_golomb(&gb);
			num_positive_pics[i] = H264_get_ue_golomb(&gb);

			if(num_negative_pics[i])
			{
				H264_get_ue_golomb(&gb);
				H264_skip_bits(&gb, 1);
			}

			for(j=1;j<num_negative_pics[i];j++)
			{
				H264_get_ue_golomb(&gb);
				H264_skip_bits(&gb, 1);
			}

			if(num_positive_pics[i])
			{
				H264_get_ue_golomb(&gb);
				H264_skip_bits(&gb, 1);
			}

			for(j=1;j<num_positive_pics[i];j++)
			{
				H264_get_ue_golomb(&gb);
				H264_skip_bits(&gb, 1);
			}
		}
	}
    //long_term_ref_pics_present_flg
    if(H264_get_one_bit(&gb))
	{
		int num_long_term_rps;
		num_long_term_rps=H264_get_ue_golomb(&gb);
		for(i=0;i<num_long_term_rps;i++)
		{
			H264_skip_bits(&gb,1+maxPicOrderCntLsb);
		}
	}

    H264_skip_bits(&gb,2);
    //vui
	if(H264_get_one_bit(&gb))
	{
		tmp =H264_get_one_bit(&gb);
		if(tmp)
		{
			tmp =H264_get_bits(&gb,8);
			if(tmp == 255)
			{
				H264_skip_bits(&gb,32);
			}
		}
		if(H264_get_one_bit(&gb))
		{							
			H264_skip_bits(&gb, 1);
		}

		if(H264_get_one_bit(&gb))
		{	
			H264_skip_bits(&gb, 4);	
			
			if(H264_get_one_bit(&gb))
			{						
				H264_skip_bits(&gb, 24);
			}
		}

		if(H264_get_one_bit(&gb))
		{
			H264_get_ue_golomb(&gb);
			H264_get_ue_golomb(&gb);
		}
		H264_skip_bits(&gb,3);
		if(H264_get_one_bit(&gb))
		{	
			H264_get_ue_golomb(&gb);
			H264_get_ue_golomb(&gb);
			H264_get_ue_golomb(&gb);
			H264_get_ue_golomb(&gb);
		}
        //vui_timing_info_present_flag 
		if(H264_get_one_bit(&gb))
		{	
			int num_units_in_tick, timeScale ;
			num_units_in_tick  = get_bits_long(&gb,32);
			timeScale = get_bits_long(&gb,32);
			if(num_units_in_tick)
				*framerate = timeScale/(2*num_units_in_tick);
		}

	}

	return 2;
}
/********************************************************************
	Function Name   :	Stream_Analyse
	Input Param     :	unsigned char* pBuf	��buf��ַ
						int nSize	����С						
	Output Param    :	int* nWidth	����
						int* nHeight����
	Return          :	-1:failed	0:mpeg4		1:h264
	Description     :	������������
	Modified Date   :   2008-1-31   09:04
	Modified By     :   Winton	
*********************************************************************/
/*
CODEC_H264		 = 0x00,
CODEC_MJPEG		 = 0x01,
CODEC_H265		 = 0x02,
*/
int Stream_Parser(unsigned char* pBuf, int nSize, int &nCodec, int* nWidth, int* nHeight, int* framerate)
{
	int i = 0;
	int ret ;	
    
	if ((!pBuf)||(!nWidth)||(!nHeight))
	{
		return -1;
	}

	*framerate = 0;
	while (i < (nSize-4))
	{
		//mpeg4
		if((*(pBuf+i) == 0x00)&&(*(pBuf+i+1) == 0x00)&&(*(pBuf+i+2) == 0x01)&&(*(pBuf+i+3) == 0xB0))
		{
			ret = Mpeg4_Analyse(pBuf+i, nSize-i, nWidth,nHeight,framerate);
			if(ret >= 0)
			{
				nCodec = 1;
				return ret;
			}
		}

		//ADI h264
		if( (*(pBuf+i) == 0x00)&&(*(pBuf+i+1) == 0x00)&&(*(pBuf+i+2) == 0x01)
			)
		{
			unsigned char naltype = *(pBuf+i+3);
			//!ȡ��5λ����nal type , rfc3984�涨
			naltype = naltype & 0x1f;
			if ( naltype == 7 ) //!sps �ڲ��ܷ�������Ϣ
			{
				ret = H264_Analyse(pBuf+i, nSize-i, nWidth,nHeight,framerate);
				if(ret >= 0)
				{
					nCodec = 0;
					return ret;
				}
			}
		}


		if( (*(pBuf+i) == 0x00)&&(*(pBuf+i+1) == 0x00)&&(*(pBuf+i+2) == 0x01)			)
		{
			unsigned char naltype = *(pBuf+i+3);
			
			naltype = (naltype & 0x7e)>>1;
			if ( naltype == 33 ) //!sps �ڲ��ܷ�������Ϣ
			{
				ret = H265_Analyse(pBuf+i, nSize-i, nWidth,nHeight,framerate);
				if(ret >= 0)
				{
					nCodec = 2;
					return ret;
				}
			}
		}

		i++;
	}

	return -1;
}


int decsps(unsigned char* pBuf, unsigned int nSize, unsigned int* width, unsigned int* height, unsigned int *frmRate)
{
	int rate=0;
	int nCodec = 0;
	int iret = Stream_Parser(pBuf, nSize,nCodec,(int*)width,(int*)height,&rate);
	if(rate)
		*frmRate = rate ;
	return iret>0 ? 0 : -1;
}

#if 0
int main(int argc, char* argv[])
{
	unsigned char* pBuf;
	FILE* f;
	int len = 0;
	int pos = 0;
	int ret = 0;
	int frame_width = 0;
	int frame_height = 0;
	int readLen = 1024*1024;
	int framerate = 0;

	pBuf = (unsigned char*)malloc(1024*1024);
	if(!pBuf)
	{
		printf("malloc failed\r\n");
		return 0;
	}
	else
	{
		printf("ptotal buf 0x%x \n",pBuf);
	}
	
	f = fopen("E:\\decSps\\decSps\\bin\\aa_rtsp.265","rb");
	if(!f)
	{
		free(pBuf);
		printf("file open failed\r\n");
		return 0;
	}
REPEAT:
	len = fread(pBuf+pos,1,readLen,f);

	if (!len)
	{
		fclose(f);
		free(pBuf);
		printf("file empty\r\n");
		return 0;
	}
	pos += len;

	ret = Stream_Analyse(pBuf,len ,&frame_width,&frame_height,&framerate);
	switch(ret)
	{
	case -1:
		goto REPEAT;
		break;
	case 0:
		printf("this file is mpeg4,width=%d,height=%d,framerate=%d\r\n",frame_width,frame_height,framerate);
		break;
	case 1:
		printf("this file is h264,width=%d,height=%d,framerate=%d\r\n",frame_width,frame_height,framerate);
		break;
	case 2:
		printf("this file is h265,width=%d,height=%d,framerate=%d\r\n",frame_width,frame_height,framerate);
		break;
	}
	fclose(f);
	free(pBuf);

	return 0;
}

#endif




