#ifndef   _VS300CLIENTSDK_H   
#define   _VS300CLIENTSDK_H   


#ifdef WIN32
#define CALLBACK	__stdcall
#define DLL_API extern "C" _declspec(dllexport)

#else
#define  CALLBACK
#define DLL_API
#endif 

#ifndef uint64
typedef unsigned long long	uint64;
#endif

#ifndef uint32
#if defined(WIN32)
typedef unsigned long uint32;
#else
typedef unsigned int uint32;
#endif
#endif

#define MAX_SCHEME_NAME_LEN 256
#define MAX_ID_LENGTH		32
#define MAX_CHANNEL_DEVICE  256
#define SDK_MAX_NAME_LEN		256
#define MAX_RECORD_NAME_LEN 64 

#define POINT_NAME_LEN		 32	//Ԥ�õ����Ƴ���
#define PTZ_PRESETNUM		 64 //Ԥ�õ�ĸ���
#define PTZ_TOURPOINT		 64 //Ѳ����·��Ԥ�õ������
#define PTZ_TOURNUM			 8  //Ѳ����·����Ŀ




#define _EC(x)						(0x80000000|x)

#define   	SDK_Success						0						//�ɹ�
#define 	SDK_Error_Unknow				_EC(1)			//δ֪����

#define     SDK_Error_NotLogin				_EC(10000)		//δ��¼����δ��ʼ��
#define		SDK_Error_Timeout				_EC(10001)		//��ʱ
#define		SDK_Error_ParamError			_EC(10002)		//��������

//��¼���������ؽ��,SDK_Login�ӿ�ʹ��:
#define SDK_Login_Timeout_Connect		_EC(2)	//��¼��������ʱ;
#define SDK_Login_Timeout_Group			_EC(3)	//��ȡ�û���������Ϣ��ʱ.
#define SDK_Login_Timeout_ORG			_EC(4)	//��ȡ�û���Ȩ����Ϣ��ʱ.
#define SDK_Login_Timeout_SCHEME		_EC(5)	//��ȡ��Ѳ�ƻ���ʱ.
#define SDK_Login_Timeout_ALERTSCHEME	_EC(6)	//��ȡ�������Գ�ʱ.
#define SDK_Login_Timeout_DECPOLLSCHEME _EC(7)	//��ȡ��������ѯ����ʧ��

#define SDK_Login_Fail_IP				_EC(8)	//���ӷ�����ʧ�ܣ�����IP��ַ�Ͷ˿�.
#define SDK_Login_Fail_FILE				_EC(9)	//�ļ�����ʧ�ܣ��������Ա��ѯ.
#define SDK_Login_Fail_DATABASE_USER	_EC(10)	//û���ҵ���ǰ�û�.
#define SDK_Login_Fail_DATABASE_GROUP	_EC(11)	//û������Ϣ.
#define SDK_Login_Fail_USER				_EC(12)	//û���ҵ���ǰ�û�.
#define SDK_Login_Fail_DATA_LONG		_EC(13)	//���ݳ���.
#define SDK_Login_Fail_PASSWORD			_EC(14)	//�������.
#define SDK_Login_Fail_USER_ID			_EC(15)	//��Ч���û�ID.
#define SDK_Login_Fail_USER_EXIST		_EC(16)	//�û��ѵ�¼.
#define SDK_Login_Fail_SESSION_EXIST	_EC(17)	//��ǰ�Ự�Ѿ�����.
#define SDK_Login_Fail_USER_LOCKED		_EC(18)	//�û��ѱ�����.
#define SDK_Login_Fail_NO_GROUP			_EC(19)	//û������Ϣ.

//����Ƶ�ӿڷ��ؽ���ʹ򿪻طŷ���,�Խ���ʱ�ķ��ش���
#define		SDK_Error_Video_Opened				_EC(20)				//��Ƶ�Ѵ�
#define 	SDK_Error_Rtsp_Connect_Failed		_EC(21)				//����RTSP����ʧ��
#define		SDK_Error_Rtsp_Ack_Failed			_EC(22)				//RTSPӦ��ʧ��
#define 	SDK_Error_Rtp_Listen_Failed			_EC(23)				//RTP��������ʧ��
#define		SDK_Error_Rtp_Connect_Failed		_EC(24)				//����RTP����ʧ��
#define		SDK_Error_Player_Port_UseOut		_EC(25)				//����˿�����
#define		SDK_Error_No_Right					_EC(26)				//��Ȩ��
#define		SDK_Error_Not_Found_Device			_EC(27)				//�Ҳ�����Ӧ���豸
#define		SDK_Error_Device_Offline			_EC(28)				//�豸����
#define		SDK_Error_LoginDev_Failed			_EC(29)				//�豸��¼ʧ�� 
#define		SDK_Error_No_DevVisitorRight		_EC(30)				//û��ֱ��Ȩ��
	//ELR_Pdu_Ack_Failed,Э��ջ����ʧ�ܣ�����ԭ��μ�CMS��DMS�����Ӧ����
#define		SDK_Error_CMS_Add_Camera_Failed		_EC(31)				//�������ͷʧ��
#define     SDK_Error_CMS_No_MTS_To_Use			_EC(32)				//û��MTS����
#define		SDK_Error_CMS_No_Device				_EC(33)				//�Ҳ����豸
#define		SDK_Error_CMS_Media_Session_Exist	_EC(34)				//ý��Ự�Ѵ���
#define		SDK_Error_CMS_Server_Reconnect		_EC(35)				//���ܷ�����������
#define		SDK_Error_CMS_No_Right				_EC(36)				//��Ȩ��
#define 	SDK_Error_CMS_Not_Find_Child_Domain	 _EC(37)			//û���ҵ��¼���
#define		SDK_Error_CMS_Not_Login_Child_Domain _EC(38)			//δ��½�¼���
#define		SDK_Error_CMS_Child_Domain_Exception _EC(39)			//�¼����쳣
#define		SDK_Error_CMS_Reach_System_Limit	 _EC(40)			//�ﵽϵͳ����
#define		SDK_Error_Chan						 _EC(41)			//ͨ��������ȷ(������Χ)
#define		SDK_Error_Playback_Opened		     _EC(42)			//�ط��Ѵ�
#define		SDK_Error_Open_File_Failed			 _EC(43)			//���ļ�ʧ��
#define     SDK_Error_No_SS_To_Use				 _EC(44)			//û��SS
#define		SDK_Error_Playback_Limit			 _EC(45)			//�ﵽ�ط�����
#define		SDK_Error_OtherUser_Playback		 _EC(46)			//�����û����ڻط�
#define     SDK_Error_SSPlayback_Failed			 _EC(47)			//�ط�ʧ��
#define		SDK_Error_Rtsp_Over_Capability		 _EC(48)			//ת������������·��
#define		SDK_Error_Rtsp_Be_Kicked			 _EC(49)			//����Ȩ���û�ռ��


//¼���ѯ���ؽ��----------------------------------------------------------------
#define		SDK_Error_Search_No_Right					 _EC(50)	//
#define		SDK_Error_Search_Dev_Offline				 _EC(51)


//��̨��������----------------------------
#define     SDK_Error_Ptz_No_Right						_EC(60)
#define     SDK_Error_Ptz_CruiseExist					_EC(61)	//��Ѳ����·�е�Ԥ�õ�����2ʱ����
#define     SDK_Error_Ptz_CruiseNoExist					_EC(62)	//Ѳ����·������
#define     SDK_Error_Ptz_CruiseRuning					_EC(63) //Ѳ����·�������У�������ɾ��


//�Խ���������-----------------------
#define		SDK_Error_TalkOpened						 _EC(70) //�Խ��Ѵ�
#define     SDK_Error_Talk_DeviceIsTalking				 _EC(71) //�����û����ڶԽ�	


//ö������-----------------------------
//�¼�֪ͨ
typedef enum
{
	EVENT_CMS_DISCONNECTED	=	0,	//cms���ߣ�
	EVENT_CMS_RECONNECT,			//����
	EVENT_RELOAD_DEVICE,			//֪ͨ�ⲿ���»�ȡ�豸
	EVENT_DEL_DEVICE,				//�豸ɾ��
	EVENT_VIDEO_EXCEPTION,			//��Ƶ����
	EVENT_PLAYBACK_STATUS,			//�ط�״̬
	EVENT_TALK_EXCEPTION,			//�Խ�����
	EVENT_MODIFY_EMAP,				//���ӵ�ͼ����

	//add xwy
	EVENT_PREVIEW_RESULT,
	EVENT_PLAYBACK_RESULT,
	EVENT_EXIT_APPLICATION			//ɾ���û�ʱ���˳�����
	//end
}EventCallbackType;

//CMS���߼���������Ͽ�����ʱ����ԭ��
typedef enum
{
	Disconnect_unknown = 0,	//δ֪���������CMS����֮��Ķ��ߣ�����ģʽ��SDK���Զ�����
	Disconnect_usererror	//�û���ɾ����������������ģʽ��SDK��������
}EnumCmsDisconnected;
//�ۿ���Ƶ�쳣ʱ�������쳣��Ϣ----------
typedef enum
{
	Exc_unknow = 0,							//δ֪
	Exc_MTS_Unregistered,				//MTS����
	Exc_Right_Canceled,					//Ȩ�ޱ�ȡ��
	Exc_Dev_Removed,					//�豸��ɾ��
	Exc_Session_Removed,				//�Ự��ȡ��
	Exc_Network_Wave					//���粨��
}EnumVideoException;					

//�ط�ʱ����Ϣ�ص�
typedef enum
{
	//�쳣��Ϣ
	Playback_SS_Unregistered,
	Playback_Right_Canceled,
	Playback_Dev_Removed,
	Playback_SessionRemoved,
	Playback_SessionAbnormal,
	//״̬֪ͨ
	Playback_Finished,
}EnumPlaybackStatus;

//�Խ��쳣����Ϣ�ص�
typedef enum
{
	Exc_Talk_Unknown,
	Exc_Talk_Mts_Unregistered,
	Exc_Talk_Dev_Removed,
}EnumTalkExcept;	

typedef enum
{
	Dev_Visiondigi = 0,
	Dev_Dahua,
	Dev_Hik,
}VSEnumDevProvider; 

//�豸����
typedef enum
{
	Dev_DVR,
	Dev_IPC,
	Dev_NVS,
	Dev_MCE,
	Dev_Decoder
}VSEnumDevType;
typedef enum
{
	Camera_Normal, //ǹ��
	Camera_SD,		//���
	Camera_HalfSD //����
}VSEnumCameraType;

//��̨����
typedef enum
{
	PTZ_INVALIADE,                    //��Ч����
	//��̨��������
	PTZ_UP,                              //��
	PTZ_DOWN,							 //��
	PTZ_LEFT,                            //��
	PTZ_RIGHT,                           //��
	PTZ_LEFTUP,                          //����
	PTZ_LEFTDOWN,                        //����
	PTZ_RIGHTUP,						 //����
	PTZ_RIGHTDOWN=8,                       //����

	//�䱶�佹������
	PTZ_ZOOMADD=15,							//�䱶+        //Camera operation ��ö��˳����ģ���ϵ��������
	PTZ_FOCUSADD,							//�佹+
	PTZ_APERTUREADD,	                    //��Ȧ+
	PTZ_ZOOMREDUCE,			                //�䱶-
	PTZ_FOCUSREDUCE,                        //�佹-
	PTZ_APERTUREREDUCE,                     //��Ȧ-

	PTZ_GoPrepoint,							//ת��Ԥ�õ�
	PTZ_StartCruise,						//����Ѳ��
	PTZ_StopCruise,							//ֹͣѲ��

	PTZ_LINESCAN_SETLEFT,					//��ɨ������߽�
	PTZ_LINESCAN_SETRIGHT,					//�����ұ߽�
	PTZ_LINESCAN_STARTSCAN,					//��ʼ��ɨ
	PTZ_LINESCAN_STOPSCAN,					//ֹͣ��ɨ
	PTZ_TRACE_SETMODESTART,					//Ѳ������ģʽ��ʼ
	PTZ_TRACE_SETMODESTOP,					//����ģʽ����
	PTZ_TRACE_RUNMODE,						//��ʼ����ģʽ
	PTZ_TRACE_STOPMODE						//ֹͣ����ģʽ
}VSEnumPtzCommand;

//�Խ�����
typedef enum
{
	Talk_Null = 0, //û�жԽ�
	Talk_G711a,
	Talk_Pcm,
}VSEnumTalkType;

//¼����Դ
typedef enum
{
	Record_Source_Device =2,			//�豸¼��
	Record_Source_SS =3,				//ƽ̨¼��
	Record_Source_CORE =7,		//���Ľڵ�
	Record_Source_REGN,		//����ڵ�
	Record_Source_JOIN		//����ڵ�
}VSEnumRecordSource;

//1ȫ����2����¼��3�ƶ���⣬4�ֶ�¼��5����¼��6��ʱ¼��
typedef enum
{
	Record_all = 1,					//����¼��	
	Record_Alarm_External,			//�ⲿ����
	Record_Alarm_MotionDetect,		//�ƶ����
	Record_Manual,					//�ֶ�¼��
	Record_Card =5,					//����¼��
	Record_Timer,					//��ʱ¼��
	Record_Alarm_VideoLost,			//��Ƶ��ʧ����ƽ̨��������	
	Record_Alarm_VideoShelter,		//��Ƶ�ڵ�,��ƽ̨��������
	Record_Time_SubStream = 30,		//��������ʱ¼��
}VSEnumRecordType;
//----------------

typedef enum
{
	PlayCtrl_Pause,//��ͣ�ָ�
	PlayCtrl_Speed //�ӷ�����
}VSEnumPlayCtrlType;

typedef struct  VSGroup_Info
{
	int				nID;	//ID
	int				nParentID;
	char			sztitle[SDK_MAX_NAME_LEN];	//�豸����
}VSGroup_Info_t;
typedef struct  VSDevice_Info
{
	char				szID[MAX_ID_LENGTH];	//�豸ID
	int					nGoupID;
	VSEnumDevProvider	nProvider;				//�豸�ṩ��
	VSEnumDevType		nType;					//�豸����
	int					nTotalChnlNum;			//��ͨ����
	int					nAlarmIn;
	int					nAlarmOut;
	char				sztitle[SDK_MAX_NAME_LEN];	//�豸����
	char				szChannelIDs[MAX_CHANNEL_DEVICE][MAX_ID_LENGTH];//���豸�����е�ͨ��ID
	char				szIP[16];
	int					nPort;
	char				szUser[64];
	char				szPassword[64];
	char				szLocation[256];
}VSDevice_Info_t;

typedef struct VSChannel_Info
{
	char szID[MAX_ID_LENGTH];
	VSEnumCameraType	 nType;
	int	 nChannelNo;
	char szDevID[MAX_ID_LENGTH];
	char szTitle[SDK_MAX_NAME_LEN];
}VSChannel_Info_t;

//¼���ѯ�ṹ��
typedef struct VSQuery_Record_Info
{
	char				szCameraId[MAX_ID_LENGTH];		// ����ͷID
	int					nChannelNo;						// ͨ����
	VSEnumRecordSource	nSource;						// ��Դ
	VSEnumRecordType	nRecordType;					// ����
	uint64				nStartTime;						// ��ʼʱ��
	uint64				nEndTime;						// ����ʱ��
} VSQuery_Record_Info_t;

typedef struct VSRecord_Info
{
	unsigned char	source;							//��Դ���ͣ�1δ֪��2�豸��3���ģ�7���Ľڵ㣬8����ڵ㣬9����ڵ�
	unsigned char	recordType;						//¼�����͡�1ȫ����2����¼��3�ƶ���⣬4�ֶ�¼��5����¼��6��ʱ¼��,VSEnumRecordType
	bool			bDownloadByTime;				//�Ƿ�ʱ������
	uint32			startTime;							//��ʼʱ��
	uint32			endTime;							//����ʱ��
	char			name[MAX_RECORD_NAME_LEN];			//¼�����֣���ͬ���Ҷ��ļ��ı�ʶ��ͬ��
	uint32			length;							//�ļ�����

	//����������¼������Ҫ����Ϣ
	uint32			planId;							//¼��ƻ�ID
	uint32			ssId;							//�洢����ID
	char			diskId[MAX_ID_LENGTH];			//����ID
	uint32			fileHandle;						//�ļ����

	char			devId[MAX_ID_LENGTH];			//֧�ֱ���¼��طţ���Ϊ����Դ��¼���豸�Ƿֿ���
	int				channelNo;

	char			szCamerID[MAX_ID_LENGTH];
	bool			bIpc;
}VSRecord_Info_t,*PVSRecord_Info_t;

//��̨������ؽṹ��
typedef struct PTZCommandParam
{
	long lParam1;
	long lParam2;
	long lParam3;
}VSPTZParam;

//Ԥ�õ���Ϣ
typedef struct PTZPointInfo
{
	unsigned char	code;							//Ԥ�õ��� ��1 ��ʼ
	unsigned char	revered[3];						//revered
	char			name[POINT_NAME_LEN];			//����
}VSPtzPointInfo;

//Ѳ����·�ṹ������Ҫʹ�õ�Ԥ�õ���Ϣ
typedef struct TourPointInfo
{
	unsigned char code;	//Ԥ�õ���		//��1��ʼ
	//unsigned char id;	//Ԥ�õ���Ѳ����·�еı�ʶ,
	short	 int  nStay;//Ԥ�õ�ͣ��ʱ��
}VSTourPointInfo;

typedef struct 
{
	int nCount;
	VSPtzPointInfo struPreset[PTZ_PRESETNUM];
}VSPtzPointSchedule;

typedef struct
{
	int		nId;					//Ѳ����
	int		nPresetCount;			//Ѳ����·��Ԥ�õ���
	char	name[POINT_NAME_LEN];	//Ѳ����·����
	VSTourPointInfo  point[PTZ_TOURPOINT];	//Ԥ�õ���
}VSTourInfo;

typedef struct
{
	int nCount;
	VSTourInfo struTour[PTZ_TOURNUM];
}VSTourSchedule;

typedef struct
{
	unsigned char	channel;						//ͨ��
	char			deviceId[32];			//�豸ID��DVR+ͨ����
	unsigned char	type;							//���͡�1��Ƶ��ʧ��2�ⲿ������3�ƶ����
	unsigned char	status;							//״̬��1������2��ʧ
	int				alarmLevel;						//��������
	uint32			alarmTime;						//��������ʱ��,����Ϊ�룬ֵΪtime(NULL)ֵ
	int				deviceType;						//�豸���͡�1��������2��������
	char			message[256];		//����������Ϣ��֧��GPS�ϱ���gaowei 09-07-29
}VSAlarmInfo;

typedef enum
{
	right_monitor = 1,
	right_playback,
	right_record,
	right_ptz,
	right_alarm,
	right_view,
	right_devvisit,
	right_alarm_out,
}VSEnumRightType;

//�����¼�����-----------------------

typedef struct
{
	char ip[16];		//		ip : �Զ�ip��ַ
	int port;			//		port: �Զ˶˿�
	char username[64];	//		username: ʹ���û���
	char password[64];	//		password : ʹ������
	int chstart;		//		chstart: ��ʼͨ����
	int chnum;			//		chnum : ����ͨ����
}VSConfigInfo;

//add xwy
typedef struct
{
	unsigned long nSessionId;
	bool IsDownload;
	long type;
	char szCameraId[64];
}VSParamInfo;
//end
#endif   

//�¼�֪ͨ������������ߵ�-----------------------------------------------------
//���EventCallbackType
typedef void (CALLBACK *EventCallBack)(long login_id, long res, char*szID, int nParam1,int nParam2, void* pUser);	

//֪ͨ�ⲿ�豸״̬��Ϣ----------------------------------
typedef void (CALLBACK *DeviceStatusCallback)(long login_id, char* szDevId, int nStatus,void* pUser);

//ʵʱԤ�����ݻص�
typedef void (CALLBACK *RealPlayCallback)(int nSessionId, char* buf, int len, void* user);
typedef bool (CALLBACK *PlayBackCallback)(int nSessionId, char* buf, int len, void* user);
typedef void (CALLBACK *TalkDataCallback)(int nSessionId, char* buf, int len, void* user);//�Խ����ݻص�

typedef void (CALLBACK *PlayBackPosCallback)(int nSessionId, int pos, int total, void* user);

typedef void (CALLBACK *AlarmCallback)(long login_id, char* szDevId, VSAlarmInfo* alarmInfo, void* user);

//typedef void (__stdcall *OnMsgNotify)(Inner_Msg_t msgReq, Inner_Msg_t msg, void* pUser);
/***********************************
*��ʼ�� 
************************************/
DLL_API long SDK_CUInit();
/***********************************
*����
************************************/
DLL_API long SDK_CUClear();

/***********************************
*�����豸״̬�ص�����
************************************/
DLL_API void SDK_CUSetDevStatusCallback(DeviceStatusCallback cb,void* user);
/***********************************
*����ʵʱ�������ݵĻص�����
************************************/
DLL_API int SDK_CUSetRealPlayCallBack(long logic_id, RealPlayCallback cb,void* user);
/***********************************
*���ûط����ݵĻص�����
************************************/
DLL_API int SDK_CUSetPlaybackCallBack(long logic_id,PlayBackCallback cb,void* user);


//���ûطŽ��Ȼص�����
DLL_API int SDK_CUSetPlaybackPos(long logic_id, PlayBackPosCallback cb, void* user);

/***********************************
*���öԽ����ݵĻص�����
************************************/
DLL_API int SDK_CUSetTalkDataCallback(long logic_id, TalkDataCallback cb,void* user);
/***********************************
*�����¼��ص�����
************************************/
DLL_API void SDK_CUSetEventCallback(EventCallBack ecb, void* pUser);

/***********************************
*���ñ����ص�����
************************************/
DLL_API void SDK_CUSetAlarmCallback(AlarmCallback alarmCB, void* pUser);
/***********************************
*SDK��¼��ֻ��һ���û��ܵ�¼
//!�����û���½id
bMediaTcp:1Ϊtcpת�䣬����Ϊudp
************************************/
DLL_API long SDK_CULogin(char* serverIp, long port, char* username, char* password, long* pLogin_id, int bMediaTcp);
/***********************************
*�ǳ�������
************************************/
DLL_API long SDK_CULogout(long logic_id);

/***********************************
*��ȡ����֯��Ŀ
*������֯����Ŀ�����㹻��С�Ŀռ䣬Ȼ���ٵ���SDK_CUGetGroupListȥ��ȡ���е���֯��Ϣ
************************************/
DLL_API long SDK_CUGetGroupCount(long login_id, int& nCount);
/***********************************
*��ȡ��֯��Ϣ
************************************/
DLL_API long SDK_CUGetGroupList(long login_id, void* groupinfoList, int& groupCount);

//!��ȡ��֯��Ϣ
DLL_API long SDK_CUGetGroupInfo(long login_id, VSGroup_Info_t* groupinfoList);

/***********************************
*��ȡ���豸��Ŀ
*�����豸����Ŀ�����㹻��С�Ŀռ䣬Ȼ���ٵ���SDK_GetDeviceListȥ��ȡ���е��豸��Ϣ
************************************/
DLL_API long SDK_CUGetDeviceCount(long login_id, int& nCount);
/***********************************
*��ȡ�豸��Ϣ
************************************/
DLL_API long SDK_CUGetDeviceList(long login_id, void* deviceinfoList, int& deviceCount);

//!��ȡ�豸��Ϣ
DLL_API long SDK_CUGetDeviceInfo(long login_id, VSDevice_Info_t* deviceinfoList);

/***********************************
*��ȡ�豸״̬
************************************/
DLL_API long SDK_CUGetDeviceStatus(long login_id, char* devId, int& nstatus);
/***********************************
*��ȡͨ����Ϣ
************************************/
DLL_API long SDK_CUGetChannelInfo(long login_id,char* cameraId,VSChannel_Info_t& channelInfo);
/***********************************
*��Ƶ����ͬ���ӿ�
*����ΪsessionID,��Ƶ���ݻص���ͨ��sessionId������
************************************/
DLL_API long SDK_CUVideoRequest(long login_id, char* camerId, int type, int substream,int nWaiteTime, char* destIP, int destPort);
/***********************************
*ֹͣʵʱ����
************************************/
DLL_API long SDK_CUStopVideoRequest(long login_id, char* camerId);
/***********************************
*��ѯ¼��ӿ�
*PVSRecord_Info_t ��ѯ������
*ncountΪָʾ��ǰ�ж��ٸ�¼��
*nMaxCountΪ���¼����Ŀ
*nWaitTime Ϊ�ȴ�ʱ��
************************************/

DLL_API long SDK_CUQueryRecord(long login_id, VSQuery_Record_Info_t queryInfo,PVSRecord_Info_t recordInfo,int& nCount, int nMaxCount,int nWaitTime);
/***********************************
*���ļ��طţ�ͬ���ӿ�
*���ز��ž��
*����ֵ����0�����������������ش���ֵ��SDK_Error_Rtsp_Connect_Failed��SDK_Error_OtherUser_Playback
************************************/

DLL_API long SDK_CUPlaybackByFile(long login_id, PVSRecord_Info_t recordInfo,int nWaiteTime, char* destIP, int destPort);

DLL_API long SDK_CUDownloadByFile(long login_id, PVSRecord_Info_t recordInfo, char* filename, int nWaiteTime);

/***********************************
*��ʱ��ط�
*���ز��ž��
************************************/
//DLL_API long SDK_CUPlaybackByTime(char *szCamerID, uint64 lStartTime, uint64 lEndTime,VSEnumRecordSource nSource,int nWaiteTime);
/***********************************
*ֹͣ�طţ�ͨ������رջط�
*nType 0�����ļ��طţ� 1��ʾ��ʱ��ط�
************************************/
DLL_API long SDK_CUStopPlayback(long login_id, long lPlayID);	

/***********************************
*��λ������ȡλ��-------
*timepose Ϊ��ʱ�䶨λʱ��ʱ��,����ʱ��
************************************/
DLL_API long SDK_CUSeekPlayPose(long login_id, long lPlayID,uint64 timePose);

/***********************************
*���ſ���-------
*playTypeΪPlayCtrl_Pauseʱ�� paramΪ1����ͣ�� 0��ָ�
ΪPlayCtrl_Speedʱ��paramΪ���ű���*1000
************************************/
DLL_API long SDK_CUPlayCtrl(long login_id, long lPlayID, VSEnumPlayCtrlType playType, int param);

/***********************************
*��̨����
*������ƣ��䱶�佹�ȣ�������ɨ֮���Ҳ������---
************************************/
DLL_API long SDK_CUPtzControl(long login_id, char* szCamerId, VSEnumPtzCommand ncommand,VSPTZParam param);

/***********************************
*��̨����
*��ȡԤ�õ�,�Ľӿ�Ϊͬ���ӿ�
*�����ṩ�ӿڻ�ȡ���
************************************/
DLL_API long SDK_CUPtzGetPrePoint(long login_id, char* szCanerId,VSPtzPointSchedule& schpoint,int nWaitTime);

/***********************************
*��̨����
*Ԥ�õ���ز��� ��Ҫ��������ӣ�ɾ��
*nType: 1Ϊ��� 2��Ϊɾ��
************************************/
DLL_API long SDK_CUPtzPrePointOper(long login_id, char* szCanerId,VSPtzPointInfo pointInfo, int nType);

/***********************************
*��̨����
*��ȡѲ����·,�ýӿ�Ϊͬ���ӿ�
************************************/
DLL_API long SDK_CUPtzGetTourInfo(long login_id, char* szCanerId,VSTourSchedule& schTour,int nWaitTime);
/***********************************
*��̨����
*nType 0:��ʾ���ӣ� 1��ʾɾ����2����ʾ�޸�
************************************/
DLL_API long SDK_CUPtzOperateTourInfo(long login_id, char* szCamerId,int nType,VSTourInfo schTour);

/***********************************
*�����Խ�--
*szDevId:�豸ID
*nTalk���Խ�����
*nWaitTime����ʱ�ȴ�ʱ��,
*����ֵ >0ʱΪ �Խ����ؾ��
************************************/
DLL_API long SDK_CUStartTalk(long login_id, char* szDevId,VSEnumTalkType nTalk,int nWaitTime); 
/***********************************
*�����Խ�--
*lHandle SDK_CUStartTalk ����ֵ
*------------------------------------*/
DLL_API long SDK_CUSendTalkData(long login_id, long lHandle,char* pdata,int nDataLen);
/****************************************
*�رնԽ�
**************************************/
DLL_API long SDK_CUStopTalk(long login_id, long lHandle);


//��ȡ�豸����Ȩ��
DLL_API long SDK_CUGetRight(long login_id, const char* szID, bool bIpc, VSEnumRightType type);

//��ȡ���ӵ�ͼ
DLL_API long SDK_CUGetEmap(long login_id,char* mappath);

//������
DLL_API long SDK_CUSetDecoder(long login_id,char *decoderid,int decoderchannelId,char *cameraid,bool bipc, int streamtype,
							  int nXBlock, int nYBlock, int nX, int nY);

DLL_API long SDK_CUGetDecoderCount(long login_id,int& nCount);

DLL_API long SDK_CUGetDecoderList(long login_id,void* deviceinfoList, int& deviceCount);

DLL_API long SDK_CUStopDecoderVideo(long login_id,char *decoderid,int decoderchannelId);

DLL_API long SDK_CUSetDecoderSplit(long login_id,char *decoderid,int split);

//28181�ϼ�ƽ̨�����¼�ƽ̨��¼����ͣ
DLL_API long SDK_CUStartCenterRecord(long login_id,char* camerId, int substream, int nWaitTime);
DLL_API long SDK_CUStopCenterRecord(long login_id,char* camerId);


//!�����Ӳ�������
//!param 1 : input: �ַ������������, VSConfigInfo
//!param 2 	char *querystr;	//		querystr: �����ַ���
//!param 3  :config_output: �ַ����������������ȡ�������ý��, json��ʽ,�������þ��������й�
//!����ֵ: 0--��ʾ�ɹ�
DLL_API long SDK_CUGetConfig(const VSConfigInfo* input, const char *querystr, char* config_output, const int size);

//!param 1 : input: �ַ������������, VSConfigInfo, ��ʽ��SDK_CUGetConfig input����
//!param 2 : config: �ַ������������, json��ʽ, �����ʽ�;������������й�
DLL_API long SDK_CUSetConfig(const VSConfigInfo* input, const char *querystr, const char* config);

//!param 2 : ����͸��
//!param 3 : deviceid
//!param 5 : ͸�����
DLL_API long SDK_CUGetTranInfo(long login_id, char* inbody, char* szDevId, int nWaitTime, char* outbody);
