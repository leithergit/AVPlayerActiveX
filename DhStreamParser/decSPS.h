#ifndef _DECSPS_
#define _DECSPS_

/********************************************************************
Function Name   :	Stream_Analyse
Input Param     :	unsigned char* pBuf	：buf地址
int nSize	：大小
Output Param    :	int* nWidth	：宽
int* nHeight：高
Return          :	-1:failed	0:mpeg4		1:h264
Description     :	码流分析函数
Modified Date   :   2008-1-31   09:04
Modified By     :   Winton
*********************************************************************/

/*
CODEC_H264		 = 0x00,
CODEC_MJPEG		 = 0x01,
CODEC_H265		 = 0x02,
*/
int Stream_Parser(unsigned char* pBuf, int nSize,int &nCodec, int* nWidth, int* nHeight, int* framerate);

int decsps(unsigned char* pBuf, unsigned int nSize, unsigned int* width, unsigned int* height, unsigned int *frmRate);

#endif
