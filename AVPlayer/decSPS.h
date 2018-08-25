#ifndef _DECSPS_
#define _DECSPS_

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
int Stream_Analyse(unsigned char* pBuf,int nSize,int* nWidth,int* nHeight,int* framerate,int *nOffset);

#endif
