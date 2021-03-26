// StreamParser.h: interface for the StreamParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STREAMPARSER_H__855E8C45_33DB_4F24_AF2A_CA819E6D11E3__INCLUDED_)
#define AFX_STREAMPARSER_H__855E8C45_33DB_4F24_AF2A_CA819E6D11E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FrameList.h"
#include "DhStreamParser.h"

class StreamParser 
{
public:
	StreamParser(unsigned char* rawBuf) ;
	virtual ~StreamParser();
	
	virtual bool CheckSign(const unsigned int& Code) = 0 ;
	virtual bool ParseOneFrame(){return true ;} ;
	virtual bool CheckIfFrameValid(){return true ;} ;

	virtual int ParseData(unsigned char *data, int datalen) ;	
	virtual DH_FRAME_INFO *GetNextFrame() ;
	virtual DH_FRAME_INFO *GetNextKeyFrame() ;

	virtual int Reset(int level);
	int GetFrameDataListSize();; //chenfeng20090511 -s

public:
	CFrameList<DH_FRAME_INFO> m_FrameInfoList ;//֡��Ϣ����
protected:
	DH_FRAME_INFO* m_FrameInfo ;//֡��Ϣ�ṹ
	DH_FRAME_INFO* m_tmp_Frameinfo;//��ʱ֡��Ϣ
	////////////////////////////////////////////////////////////
	unsigned char* m_rawBuf;//ԭʼ���ݻ���
	unsigned char *m_bufptr;//���ݷ���ָ��
	unsigned int    m_code;//��־
	unsigned long m_frameLen; // ֡��
	unsigned long m_RemainLen; // ʣ�����ݳ���
	long  rest ;//δ��������
	unsigned long m_preShSeq ;
	//2010-1-16�����޸�start
	bool m_frameNotCon;//֡������
	//2010-1-16�����޸�end
protected:
	static void AudioInfoOpr(DH_FRAME_INFO* frameinfo, const unsigned char& samplespersecond) ;
};


#endif // !defined(AFX_STREAMPARSER_H__855E8C45_33DB_4F24_AF2A_CA819E6D11E3__INCLUDED_)
