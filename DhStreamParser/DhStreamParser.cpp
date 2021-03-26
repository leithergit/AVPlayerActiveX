// DhStreamParser.cpp: implementation of the DhStreamParser class.
//
//////////////////////////////////////////////////////////////////////
#include "DhStreamParser.h"
#include "StreamParser.h"
#include "newstream.h"
#include "psstream.h"
#include "dhstdstream.h"
#include "svacstream.h"
#include "avistream.h"
#include "xmstream.h"

#include <time.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4305)
#pragma warning(disable:4309)

#ifndef WIN32
#include <stdexcept>
#include <signal.h>
#include <setjmp.h>
#endif

DhStreamParser::DhStreamParser(int nStreamType, int nFlag)
{
	m_nStreamType = nStreamType ;

	m_nStreamType = DH_STREAM_UNKNOWN ;
	m_streamParser = NULL ;

	m_seq = 0;
}

DhStreamParser::~DhStreamParser()
{
	if (m_streamParser)
	{
		delete m_streamParser ;
		m_streamParser = NULL ;
	}
}

#ifndef WIN32 
void sig_handler(int signum, siginfo_t* info, void* arg)
{
	throw "stream parser exception\n"; 
	return;			/* Unreachable code. */
}
#endif

int DhStreamParser::InputData(unsigned char *pData, unsigned long nDataLength)
{
	if (m_nStreamType == DH_STREAM_UNKNOWN 
		|| m_nStreamType == DH_STREAM_AUDIO)
	{
		int iRet = AutoScanStream(pData, nDataLength);
		if ( iRet< 0)
		{
			if (m_nStreamType == DH_STREAM_AUDIO && m_streamParser)//����Ƶ����
			{
				return m_streamParser->ParseData(pData, nDataLength) ;
			}
			else//û�ҵ�
			{
				return -1 ;
			}
		}
	}

	int ret = 0;

#ifdef WIN32
	__try
	{
		m_streamParser->ParseData(pData, nDataLength) ;
	}
	__except(0, 1)
	{
		ret = -1;
	}
#else

	int savesigs;
	jmp_buf env;
	stack_t ss;
	struct sigaction act={0};

	act.sa_sigaction = sig_handler;
	act.sa_flags = SA_ONSTACK | SA_SIGINFO;
	
	if (sigaction(SIGFPE, &act, NULL) != 0) return -1;
	int a,b;
	try
	{
		if (sigsetjmp(env, savesigs) !=0) 
		{
			throw "fault thrown in user context(as opposed to signal contxt!";
		}
		m_streamParser->ParseData(pData, nDataLength) ;
	}
	catch (const char* msg)
	{
		printf("%s", msg);
		ret = -1;
	}
	
#endif
	
	return ret;
}

#define __dhcountof(array) (sizeof(array)/sizeof(array[0]))
#define _crt_va_end(ap) ( ap = (va_list)0 )
#define _crt_va_start(ap,v) ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
#pragma warning (disable:4996)
#include <windows.h>
void DHTraceMsg(const char* pFormat, ...)
{
	va_list args;
	_crt_va_start(args, pFormat);
	int nBuff;
	char szBuffer[0x7fff];
	nBuff = _vsnprintf(szBuffer, __dhcountof(szBuffer), pFormat, args);
	//::wvsprintf(szBuffer, pFormat, args);
	//assert(nBuff >=0);
	OutputDebugStringA(szBuffer);
	_crt_va_end(args);
}
DH_FRAME_INFO *DhStreamParser::GetNextFrame()
{	
	if (m_nStreamType == DH_STREAM_UNKNOWN)
	{
		return NULL ;
	}

	DH_FRAME_INFO * frame = m_streamParser->GetNextFrame() ; 

	if (frame != NULL && m_nStreamType == DH_STREAM_AVI)
	{
		unsigned char IFrameHeader[16] = {0, 0, 1, 0xFD, 0};
		unsigned char PFrameHeader[8]  = {0, 0, 1, 0xFC};
		unsigned char AFrameHeader[8]  = {0, 0, 1, 0xF0};

		if (frame->nType == DH_FRAME_TYPE_VIDEO)
		{
			
			if (frame->nSubType == DH_FRAME_TYPE_VIDEO_I_FRAME)
			{
				IFrameHeader[5] = frame->nFrameRate;
				IFrameHeader[6] = frame->nWidth / 8;
				IFrameHeader[7] = frame->nHeight / 8;
			
				time_t now=time(NULL);
				tm *newtime = localtime(&now); 
				int year = newtime->tm_year + 1900 - 2000;
				int mon = newtime->tm_mon + 1;
				int day = newtime->tm_mday;
				int hour = newtime->tm_hour;
				int min = newtime->tm_min;
				int sec = newtime->tm_sec;
				
				unsigned int timestamp = ( sec & 0x3f)          |
					((min & 0x3f) << 6)	|
					((hour& 0x1f) << 12)	|
					((day & 0x1f) << 17)	|
					((mon & 0x0f) << 22)	|
					((year& 0x3f) << 26)	;
				
				memcpy(IFrameHeader + 8,  &timestamp, 4);
				memcpy(IFrameHeader + 12, &frame->nFrameLength, 4);
				IFrameHeader[15] = m_seq++;

				memcpy(m_FDBuffer, IFrameHeader, 16);
				memcpy(m_FDBuffer+16, frame->pContent, frame->nFrameLength);
				frame->nLength = frame->nFrameLength + 16;
				frame->pContent = m_FDBuffer + 16;
			}
			else
			{
				memcpy(PFrameHeader + 4, &frame->nFrameLength, 4);
				IFrameHeader[7] = m_seq++;
				memcpy(m_FDBuffer, PFrameHeader, 8);
				memcpy(m_FDBuffer + 8, frame->pContent, frame->nFrameLength);
				frame->nLength = frame->nFrameLength + 8;
				frame->pContent = m_FDBuffer + 8;
			}
		}
		else if (frame->nType == DH_FRAME_TYPE_AUDIO)
		{
			AFrameHeader[4] = 15; //AVI��ƵĿǰ����16λ��PCM
			switch (frame->nSamplesPerSecond)
			{
			case 8000:
				AFrameHeader[5] = 2;
				break;
			case 16000:
				AFrameHeader[5] = 4;
				break;
			default:
				AFrameHeader[5] = 4;
				break;
			}
			
			memcpy(AFrameHeader + 6, &frame->nFrameLength, 2);
			memcpy(m_FDBuffer, AFrameHeader, 8);
			memcpy(m_FDBuffer+8, frame->pContent, frame->nFrameLength);
			frame->nLength = frame->nFrameLength + 8;
			frame->pContent = m_FDBuffer + 8;
		}

		frame->pHeader = m_FDBuffer;

	}

	return frame;
}

DH_FRAME_INFO *DhStreamParser::GetNextKeyFrame()
{
	if (m_nStreamType == DH_STREAM_UNKNOWN)
	{
		return NULL ;
	}

	return m_streamParser->GetNextKeyFrame() ;
}

int DhStreamParser::Reset(int nLevel, int streamtype)
{
	if (m_nStreamType == DH_STREAM_UNKNOWN)
	{
		return -1 ;
	}

	if (nLevel == STREAMPARSER_RESET_REFIND)
	{
		m_nStreamType = DH_STREAM_UNKNOWN ;

		return 0 ;
	}

	return m_streamParser->Reset(nLevel) ;
}

int DhStreamParser::GetStreamType()
{
	return m_nStreamType ;
}

int DhStreamParser::AutoScanStream(unsigned char *pData, unsigned long nDataLength)
{
	static const unsigned long MAXSCANLEN = 51200;

	int NEWStreamCounter = 0;
	int XMStreamCounter = 0;
	int AUDIOStreamCounter = 0;
	int AVIStreamCounter = 0;
	int PSStreamCounter = 0;
	int DHSTDStreamCounter = 0;
	int SVACStreamCounter = 0;
	unsigned int Code = 0xFFFFFFFF;
	unsigned char *pScanBuf = pData;
	unsigned long DataRest = nDataLength;
	unsigned long asfpacketlen = 0;
	unsigned int videoStreamID = 1;

	while (DataRest--)
	{
		Code = (Code << 8) | *pScanBuf++;

		if (Code == 0x01EA)
		{
			if (DataRest < 20)
			{
				continue;
			}
			SVACStreamCounter = 1;

			if (nDataLength - DataRest > MAXSCANLEN)
			{
				break;
			}
		}
		else if (Code == 0x01ED || Code == 0x01EC)
		{
			if (DataRest < 20)
			{
				continue;
			}

			int encodeType = pScanBuf[0];
			if (encodeType < 1 || encodeType > 5)//Ŀǰֻ��1-4�����ֱ����ʽ
			{
				continue;
			}

			int width = (pScanBuf[4] | pScanBuf[5] << 8);
			int height = (pScanBuf[6] | pScanBuf[7] << 8);
			int rate = pScanBuf[2];

			if (width < 120 || width > 4096)
			{
				continue;
			}

			if (height < 120 || height > 4096)
			{
				continue;
			}

			if (rate > 100 || rate < 1)
			{
				continue;
			}

			SVACStreamCounter = 1;

			if (nDataLength - DataRest > MAXSCANLEN)
			{
				break;
			}
		}

		if (Code == 0x01FC)
		{
			if (DataRest < 12)
			{
				continue;
			}

			int width = pScanBuf[2] * 8;
			int height = pScanBuf[3] * 8;
			int rate = pScanBuf[1] & 0x1F;

			if (width < 120 || width > 1920)
			{
				continue;
			}
			if (height < 120 || height > 1920)
			{
				continue;
			}

			if (rate > 30 || rate < 1)
			{
				continue;
			}

			XMStreamCounter = 1;

			if (nDataLength - DataRest > MAXSCANLEN)
			{
				break;
			}
		}
		else if (Code == 0x01FD || Code == 0x01FE)
		{
			if (DataRest < 12)
			{
				continue;
			}

			int width = pScanBuf[2] * 8;
			int height = pScanBuf[3] * 8;
			int rate = pScanBuf[1] & 0x1F;

			if (width < 120 || width > 1920)
			{
				continue;
			}
			if (height < 120 || height > 1920)
			{
				continue;
			}

			if (rate > 100 || rate < 1)
			{
				continue;
			}

			NEWStreamCounter = 1;

			if (nDataLength - DataRest > MAXSCANLEN)
			{
				break;
			}
		}
		else if (Code == 0x01F0)
		{
			AUDIOStreamCounter = 1;

			if (nDataLength - DataRest > MAXSCANLEN)
			{
				break;
			}
		}
		else if (Code == 0x52494646)//RIFF
		{
			if (DataRest > 8)
			{
				unsigned int rifftype = pScanBuf[4] << 24 | pScanBuf[5] << 16 | pScanBuf[6] << 8 | pScanBuf[7];
				if (rifftype == 0x41564920) {
					AVIStreamCounter = 1;
					break;
				}
			}
		}
		else if (Code == 0x01BA)
		{
			PSStreamCounter = 1;
			if (nDataLength - DataRest > MAXSCANLEN)
			{
				break;
			}
		}
		else if (Code == 0x44484156)
		{
			if (DataRest < 20)
			{
				continue;
			}

			unsigned char crc = 0;
			for (int i = 0; i < 19; i++)
			{
				crc += pScanBuf[i];
			}
			crc += 0x44;
			crc += 0x48;
			crc += 0x41;
			crc += 0x56;

			if (crc != pScanBuf[19])
			{
				continue;
			}

			DHSTDStreamCounter = 1;
			if (nDataLength - DataRest > MAXSCANLEN)
			{
				break;
			}
		}
	}

	if (AVIStreamCounter || NEWStreamCounter || XMStreamCounter
		|| PSStreamCounter || DHSTDStreamCounter || SVACStreamCounter)
	{
		if (m_streamParser)
		{
			delete m_streamParser;
			m_streamParser = NULL;
		}
	}


	if (AVIStreamCounter > 0)
	{
		DH_FRAME_INFO tmpAviInfo;
		memset(&tmpAviInfo, 0, sizeof(tmpAviInfo));

		int auds = 0;

		while (DataRest--)
		{
			Code = (Code << 8) | *pScanBuf++;

			if (Code == 0x61766968)
			{
				pScanBuf += 4;
				int resultion = *(int*)pScanBuf;
				tmpAviInfo.nFrameRate = 1000000 / resultion;
				pScanBuf += 32;
				tmpAviInfo.nWidth = *(int*)pScanBuf;
				tmpAviInfo.nHeight = *(int*)(pScanBuf + 4);
			}
			else if (Code == 0x61756473)
			{
				auds = 1;
			}
			else if (Code == 0x73747266)
			{
				if (auds == 1 && DataRest > 20)
				{
					tmpAviInfo.nSamplesPerSecond = *((int*)(pScanBuf + 8));
					tmpAviInfo.nBitsPerSample = (pScanBuf[16] | (pScanBuf[17] << 8)) * 8;
					break;
				}
			}
		}

		if (tmpAviInfo.nBitsPerSample != 8 && tmpAviInfo.nBitsPerSample != 16)
		{
			tmpAviInfo.nBitsPerSample = 8;
		}

		if (tmpAviInfo.nSamplesPerSecond < 4000 || tmpAviInfo.nSamplesPerSecond > 48000)
		{
			tmpAviInfo.nSamplesPerSecond = 8000;
		}
		m_nStreamType = DH_STREAM_AVI;
		//m_streamParser = new AVIStream(m_buffer, &tmpAviInfo);
		return 0;
	}
	else if (DHSTDStreamCounter > 0)
	{
		m_nStreamType = DH_STREAM_DHSTD;
		//m_streamParser = new DhStdStream(m_buffer);
		return 0;
	}
	else if (NEWStreamCounter > 0)
	{
		m_nStreamType = DH_STREAM_NEW;
		m_streamParser = new NewStream(m_buffer);
		return 0;
	}
	else if (XMStreamCounter > 0)
	{
		//m_nStreamType = DH_STREAM_XM;
		//m_streamParser = new XMStream(m_buffer);
		return 0;
	}
	else if (SVACStreamCounter > 0)
	{
		m_nStreamType = DH_STREAM_SVAC;
		m_streamParser = new SvacStream(m_buffer);
		return 0;
	}
	else if (PSStreamCounter > 0)
	{
		m_nStreamType = DH_STREAM_PS;
		//m_streamParser = new PSStream(m_buffer);
		return 0;
	}
	else if (AUDIOStreamCounter > 0)
	{
		if (m_nStreamType != DH_STREAM_AUDIO)
		{
			if (m_streamParser)
			{
				delete m_streamParser;
				m_streamParser = NULL;
			}
			m_nStreamType = DH_STREAM_AUDIO;
			//m_streamParser = new NewStream(m_buffer);//����Ƶ��������������
		}

		return 0;
	}

	return -1;
}
//
//int DhStreamParser::AutoScanStream(unsigned char *pData, unsigned long nDataLength)
//{
//	static const unsigned long MAXSCANLEN = 51200 ;
//	int SVACStreamCounter = 0;
//	unsigned int Code = 0xFFFFFFFF;
//	unsigned char *pScanBuf = pData;
//	unsigned long DataRest = nDataLength ;
//	unsigned long asfpacketlen = 0;
//	unsigned int videoStreamID = 1;
//
//	while (DataRest--)
//	{
//		Code = (Code << 8) | *pScanBuf++;
//		
//		if (Code == 0x01ED)
//		{
//			if (DataRest < 20)
//			{
//				continue;
//			}
//			
//			int encodeType = pScanBuf[0];
//			if (encodeType < 1 || encodeType > 5)//Ŀǰֻ��1-4�����ֱ����ʽ
//			{
//				continue;
//			}
//			
//			int width  = (pScanBuf[4] | pScanBuf[5] << 8);
//			int height = (pScanBuf[6] | pScanBuf[7] << 8);
//			int rate   = pScanBuf[2];
//			
//			if (width < 160 || width > 1920)
//			{
//				continue;
//			}
//			
//			if (height < 120 || height > 1216)
//			{
//				continue;
//			}
//			
//			if (rate > 100 || rate < 1)
//			{
//				continue;
//			}
//			
//			SVACStreamCounter = 1 ;
//			
//			if (nDataLength - DataRest > MAXSCANLEN)
//			{
//				break ;
//			}
//		}
//
//	}
//
//	if (SVACStreamCounter > 0)
//	{
//		m_nStreamType = DH_STREAM_SVAC;
//		m_streamParser = new SvacStream(m_buffer);
//		return 0;
//	}
//
//	return -1 ;
//}

//chenf20090511-s
int DhStreamParser::GetFrameDataListSize()
{
	//return m_streamParser->GetDataListSize();
	return m_streamParser->m_FrameInfoList.GetDataListSize();
}
//chenf20090511-e
