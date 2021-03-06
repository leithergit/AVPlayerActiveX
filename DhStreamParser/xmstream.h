// newstream.h: interface for the newstream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMSTREAM_H__C00190F5_E1A4_415E_83DC_9259A0765031__INCLUDED_)
#define AFX_XMSTREAM_H__C00190F5_E1A4_415E_83DC_9259A0765031__INCLUDED_

#include "StreamParser.h"

class XMStream : public StreamParser  
{
public:
	XMStream(unsigned char* rawBuf);
	virtual ~XMStream();
	
	bool CheckSign(const unsigned int& Code) ;
	bool ParseOneFrame() ;
	bool CheckIfFrameValid() ; 
};

#endif // !defined(AFX_NEWSTREAM_H__C00190F5_E1A4_415E_83DC_9259A0765031__INCLUDED_)
