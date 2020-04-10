2019.06.14 版本升级 1.5.0.0
1.增加播放容器，以控制内存无限增长问题

2017.10.25 增加配置选项，使用配置文件Configuration.xml可以启用和关闭根据摄像机读取显示位置信息的功能。
  
       配置文件格式，当Enable为false时，则关闭功该功能，否则开启，默认关闭
				<?xml version="1.0" encoding="utf-8"?>
				<Configuration >
					<CameraPostion Enable ="true"/>		
				</Configuration>
				

2017.09.21 
1.新增SendPtzCommand接口：
LONG SendPtzCommand(LPCTSTR strDeviceID, LONG nPtzCommand,LONG nPtzValue1,LONG nPtzValue2,LONG nOpType);
其中：
    strDeviceID	设备ID
    nPtzCommand	命令代码，命令代码的取值参见SampleCPP演示代码。
    nPtzValue1，nPtzValue1，与命令代码对应的命令值，如转动角度，变焦值等
    nOpType		操作类型，0为相对动作，1为绝对动作，默认为相对动作

2017.8.17  增加了两个接口
1、SetPlayComboBorder   废弃，没有用到
2、SetPlayComboBorderEx 根据高度设置两个图像的拼接范围   
    BSTR strDevice1：设备ID
    BSTR strDevice2：设备ID
    DOUBLE fSpreadHei：吊具高度    

2017.07.03 1.0.0.9
1.修正PlayComboStream函数的一个bug,这个bug会影响playstream函数的正常运行，当先使用PlayComboStream播放摄像机再使用playstream函数播放相同的摄像时，可能会不出图像；

2017.07.03 1.0.0.7
1.修改PlayComboStream函数，增加图像排列方式参数，排列方式为0时，图像左右排列，排列试为1时，图像上下排列；

2017.07.03 1.0.0.7
1.增加layComboStream函数，可把两路摄像机图像各取一半合为一个图像；
定义如下:
C++形式：
LONG PlayComboStream(LPCTSTR strDevice1, LPCTSTR strDevice2, LONG hWnd, LONG nEnableHWAccel);

  
C#形式：
  int PlayComboStream(string strDevice1, string strDevice2, int hWnd, int nEnableHWAccel);
其中：
  strDevice1  左半侧图像
  strDevice2  右半侧图像
  hWnd        图像显示窗口
  nEnableHWAccel  是否启用硬件加速

2017.04.13 1.0.0.5
1.取消RecvTimeout事件,重连操作由控件内部实现.

2017.01.02 1.0.0.5
1.新增GetDeviceWindow函数，用于获取播放指定设备使用窗口列表
2.新增GetWindowDevice函数，用于获取使用指定窗口进行播放的设备ID
3.新增错误类型:
              AvError_DeviceNotInPlaying  (设备不在播放中)
              AVError_BufferOverflow      (缓存溢出,提供的内存空间不足以容纳所请求的数据)
              AvError_WindowNotPlaying    (窗口尚未播放任何设备)
              AvError_ExternalError       (内部错误)


      

                        


