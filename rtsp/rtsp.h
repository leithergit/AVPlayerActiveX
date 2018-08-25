#ifndef RTSP_H
#define RTSP_H

#ifdef RTSP_LIB
# define RTSP_EXPORT __declspec(dllexport)
#else
# define RTSP_EXPORT __declspec(dllimport)
#endif
struct _RTSPParam
{
	int nLength;
	int nNalType;
	int nDataType;		// _RTP_Header(0),_NALU(1),_FrameData(2)
};
typedef void (*PFRtspDataCallBack)(long lHandle, char *pBuffer, int nParam, int nUser);
typedef void (*PFRtspDisConnect)(long lHandle, int dwUser);


extern "C"
{
	RTSP_EXPORT long rtsp_login(char* pUrl, const char* user, const char* pass, int httpPort, PFRtspDisConnect cbDisconnectCallBack, void* pUser);
	RTSP_EXPORT long rtsp_play(const char *pUrl, const char* user, const char* pass, int nNetType, int httpPort, 
		PFRtspDataCallBack cbDataCallBack, PFRtspDisConnect cbDisconnectCallBack, void* pUser);
	RTSP_EXPORT long rtsp_setCallback(long lPlayHandle, PFRtspDataCallBack cbDataCallBack, PFRtspDisConnect cbDisconnectCallBack, void* pUser);

	RTSP_EXPORT int	 rtsp_stop(long lHandle);
};


#endif // RTSP_H
