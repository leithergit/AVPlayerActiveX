2019.06.14 �汾���� 1.5.0.0
1.���Ӳ����������Կ����ڴ�������������

2017.10.25 ��������ѡ�ʹ�������ļ�Configuration.xml�������ú͹رո����������ȡ��ʾλ����Ϣ�Ĺ��ܡ�
  
       �����ļ���ʽ����EnableΪfalseʱ����رչ��ù��ܣ���������Ĭ�Ϲر�
				<?xml version="1.0" encoding="utf-8"?>
				<Configuration >
					<CameraPostion Enable ="true"/>		
				</Configuration>
				

2017.09.21 
1.����SendPtzCommand�ӿڣ�
LONG SendPtzCommand(LPCTSTR strDeviceID, LONG nPtzCommand,LONG nPtzValue1,LONG nPtzValue2,LONG nOpType);
���У�
    strDeviceID	�豸ID
    nPtzCommand	������룬��������ȡֵ�μ�SampleCPP��ʾ���롣
    nPtzValue1��nPtzValue1������������Ӧ������ֵ����ת���Ƕȣ��佹ֵ��
    nOpType		�������ͣ�0Ϊ��Զ�����1Ϊ���Զ�����Ĭ��Ϊ��Զ���

2017.8.17  �����������ӿ�
1��SetPlayComboBorder   ������û���õ�
2��SetPlayComboBorderEx ���ݸ߶���������ͼ���ƴ�ӷ�Χ   
    BSTR strDevice1���豸ID
    BSTR strDevice2���豸ID
    DOUBLE fSpreadHei�����߸߶�    

2017.07.03 1.0.0.9
1.����PlayComboStream������һ��bug,���bug��Ӱ��playstream�������������У�����ʹ��PlayComboStream�����������ʹ��playstream����������ͬ������ʱ�����ܻ᲻��ͼ��

2017.07.03 1.0.0.7
1.�޸�PlayComboStream����������ͼ�����з�ʽ���������з�ʽΪ0ʱ��ͼ���������У�������Ϊ1ʱ��ͼ���������У�

2017.07.03 1.0.0.7
1.����layComboStream�������ɰ���·�����ͼ���ȡһ���Ϊһ��ͼ��
��������:
C++��ʽ��
LONG PlayComboStream(LPCTSTR strDevice1, LPCTSTR strDevice2, LONG hWnd, LONG nEnableHWAccel);

  
C#��ʽ��
  int PlayComboStream(string strDevice1, string strDevice2, int hWnd, int nEnableHWAccel);
���У�
  strDevice1  ����ͼ��
  strDevice2  �Ұ��ͼ��
  hWnd        ͼ����ʾ����
  nEnableHWAccel  �Ƿ�����Ӳ������

2017.04.13 1.0.0.5
1.ȡ��RecvTimeout�¼�,���������ɿؼ��ڲ�ʵ��.

2017.01.02 1.0.0.5
1.����GetDeviceWindow���������ڻ�ȡ����ָ���豸ʹ�ô����б�
2.����GetWindowDevice���������ڻ�ȡʹ��ָ�����ڽ��в��ŵ��豸ID
3.������������:
              AvError_DeviceNotInPlaying  (�豸���ڲ�����)
              AVError_BufferOverflow      (�������,�ṩ���ڴ�ռ䲻�������������������)
              AvError_WindowNotPlaying    (������δ�����κ��豸)
              AvError_ExternalError       (�ڲ�����)


      

                        


