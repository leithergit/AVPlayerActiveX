// DialogPTZ.cpp : implementation file
//

#include "stdafx.h"
#include "SampleCPP.h"
#include "DialogPTZ.h"

#include "Utility.h"
#include <WindowsX.h>
#include "avplayerctrl.h"
// CDialogPTZ dialog

IMPLEMENT_DYNAMIC(CDialogPTZ, CDialog)

CDialogPTZ::CDialogPTZ(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPTZ::IDD, pParent)
{

}

CDialogPTZ::~CDialogPTZ()
{
}

void CDialogPTZ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogPTZ, CDialog)
	ON_COMMAND_RANGE(IDC_BUTTON_UP,IDC_BUTTON_ZOOMOUT, &CDialogPTZ::OnComandPTZ)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND, &CDialogPTZ::OnBnClickedButtonCommand)
END_MESSAGE_MAP()


// CDialogPTZ message handlers

void CDialogPTZ::OnComandPTZ(UINT nID)
{
	
}

BOOL CDialogPTZ::PreTranslateMessage(MSG* pMsg)
{
// 	if(pMsg->message == WM_LBUTTONDOWN)
// 	{// ����ťѹ����Ϣ
// 		int nID = WindowFromPoint(pMsg->pt)->GetDlgCtrlID();
// 		bool bPtzOption = (bool)IsDlgButtonChecked(IDC_CHECK_ABSTRACTOP);
// 		bool bPtzValue = (bool)IsDlgButtonChecked(IDC_CHECK_PTZValue);
// 		int nPtzValue = GetDlgItemInt(IDC_EDIT_PTZVALUE);
// 		TCHAR szDeviceID[32] = {0};
// 		GetDlgItemText(IDC_EDIT_CAMERAID,szDeviceID,32);
// 		PtzOption nPtzOption = bPtzOption== true?Opt_Abstract:Opt_Relative;
// 
// 		switch(nID)
// 		{
// 		case IDC_BUTTON_UP:
// 			{
// 				
// 				TraceMsgA("IDC_BUTTON_UP\n");
// 			}
// 			break;
// 		case IDC_BUTTON_RESET:
// 			{
// 				TraceMsgA("IDC_BUTTON_RESET\n");
// 			}
// 			break;
// 		case IDC_BUTTON_DOWN:
// 			{
// 				TraceMsgA("IDC_BUTTON_DOWN\n");
// 			}
// 			break;
// 		case IDC_BUTTON_LEFTUP:
// 			{
// 				TraceMsgA("IDC_BUTTON_LEFTUP\n");
// 			}
// 			break;
// 		case IDC_BUTTON_LEFT:
// 			{
// 				TraceMsgA("IDC_BUTTON_LEFT\n");
// 			}
// 			break;
// 		case IDC_BUTTON_LEFTDOWN:
// 			{
// 				TraceMsgA("IDC_BUTTON_LEFTDOWN\n");
// 			}
// 			break;
// 		case IDC_BUTTON_RIGHTUP:
// 			{
// 				TraceMsgA("IDC_BUTTON_RIGHTUP\n");
// 			}
// 			break;
// 		case IDC_BUTTON_RIGHT:
// 			{
// 				TraceMsgA("IDC_BUTTON_RIGHT\n");
// 			}
// 			break;
// 		case IDC_BUTTON_RIGHTDOWN:
// 			{
// 				TraceMsgA("IDC_BUTTON_RIGHTDOWN\n");
// 			}
// 			break;
// 		case IDC_BUTTON_ZOOMIN:
// 			{
// 				TraceMsgA("IDC_BUTTON_ZOOMIN\n");
// 			}
// 			break;
// 		case IDC_BUTTON_ZOOMOUT:
// 			{
// 				TraceMsgA("IDC_BUTTON_ZOOMOUT\n");
// 			}
// 			break;
// 		}
// 	}
// 	if (pMsg->message == WM_LBUTTONUP)
// 	{// ����ť����
// 		int nID = WindowFromPoint(pMsg->pt)->GetDlgCtrlID();
// 		switch(nID)
// 		{
// 		case IDC_BUTTON_UP:
// 			{
// 				TraceMsgA("IDC_BUTTON_UP\n");
// 			}
// 			break;
// 		case IDC_BUTTON_RESET:
// 			{
// 				TraceMsgA("IDC_BUTTON_RESET\n");
// 			}
// 			break;
// 		case IDC_BUTTON_DOWN:
// 			{
// 				TraceMsgA("IDC_BUTTON_DOWN\n");
// 			}
// 			break;
// 		case IDC_BUTTON_LEFTUP:
// 			{
// 				TraceMsgA("IDC_BUTTON_LEFTUP\n");
// 			}
// 			break;
// 		case IDC_BUTTON_LEFT:
// 			{
// 				TraceMsgA("IDC_BUTTON_LEFT\n");
// 			}
// 			break;
// 		case IDC_BUTTON_LEFTDOWN:
// 			{
// 				TraceMsgA("IDC_BUTTON_LEFTDOWN\n");
// 			}
// 			break;
// 		case IDC_BUTTON_RIGHTUP:
// 			{
// 				TraceMsgA("IDC_BUTTON_RIGHTUP\n");
// 			}
// 			break;
// 		case IDC_BUTTON_RIGHT:
// 			{
// 				TraceMsgA("IDC_BUTTON_RIGHT\n");
// 			}
// 			break;
// 		case IDC_BUTTON_RIGHTDOWN:
// 			{
// 				TraceMsgA("IDC_BUTTON_RIGHTDOWN\n");
// 			}
// 			break;
// 		case IDC_BUTTON_ZOOMIN:
// 			{
// 				TraceMsgA("IDC_BUTTON_ZOOMIN\n");
// 			}
// 			break;
// 		case IDC_BUTTON_ZOOMOUT:
// 			{
// 				TraceMsgA("IDC_BUTTON_ZOOMOUT\n");
// 			}
// 			break;
// 		}
// 
// 	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDialogPTZ::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_oleTarget.Register(this);
	TCHAR *szPtzArray[] = {
	_T("Ptz_Center"),
	_T("Ptz_Areazoom"),
	_T("Ptz_ImageWidth"),
	_T("Ptz_ImageHeight"),
	_T("Ptz_Move"),
	// ���²����Դ��ھ��Ժ��������ģʽ
	_T("Ptz_Pan"),	// ˮƽת��
	_T("Ptz_Tilt"),				// ��ֱת��
	_T("Ptz_Zoom"),
	_T("Ptz_Focus"),
	_T("Ptz_Iris"),
	_T("Ptz_Brightness"),
	_T("Ptz_AutoFocus"),
	_T("Ptz_AutoIris"),
	_T("Ptz_ContinuousPanTiltMove"),
	_T("Ptz_ContinuousFocusMove"),
	_T("Ptz_ContinuousZoomMove"),
	_T("Ptz_ContinuousIrisMove"),
	_T("Ptz_ContinuousBrightnessMove"),
	_T("Ptz_GogoServerPresetNo"),
	_T("Ptz_Speed"),
	_T("Ptz_IrcutFilter"),
	_T("Ptz_BackLight")
	};
	HWND hComboxPtz = ::GetDlgItem(m_hWnd,IDC_COMBO_PTZ);
	for (int i = 0;i < sizeof(szPtzArray)/sizeof(TCHAR*);i ++)
	{
		ComboBox_AddString(hComboxPtz,szPtzArray[i]);
	}
	ComboBox_SetCurSel(hComboxPtz,0);
	SetDlgItemText(IDC_EDIT_PTZVALUE1,_T("50"));
	SetDlgItemText(IDC_EDIT_PTZVALUE2,_T("50"));
	CheckDlgButton(IDC_RADIO_RELATVIE,BST_CHECKED);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogPTZ::OnBnClickedButtonCommand()
{
	HWND hComboxPtz = ::GetDlgItem(m_hWnd,IDC_COMBO_PTZ);
	TCHAR szDeviceID[32] = {0};
	GetDlgItemText(IDC_EDIT_CAMERAID,szDeviceID,32);
	PtzCode nCode = (PtzCode)ComboBox_GetCurSel(hComboxPtz);
	int nPtzValue1 = GetDlgItemInt(IDC_EDIT_PTZVALUE1);
	int nPtzValue2 = GetDlgItemInt(IDC_EDIT_PTZVALUE2);
	int nPtzValue3 = GetDlgItemInt(IDC_EDIT_PTZVALUE3);
	PtzOption nPtzOption = IsDlgButtonChecked(IDC_RADIO_RELATVIE)?Opt_Relative:Opt_Abstract;
	switch(nCode)
	{
		case Ptz_Center:
			{
				// nPtzValue1 X���꣬nPtzValue2 Y����
				// ������������������ͻ�����Ƶ��������������x��y��
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,nPtzValue1,nPtzValue2,nPtzOption);
				break;
			}
		case Ptz_Areazoom:
			{
				// nPtzValue1 X���꣬nPtzValue2 Y���꣬nPtzValue3 Zֵ Ϊ�Ŵ���
				// �ƶ������� x , y �������� center ����������� z/100 �������š����
				//	z ����100����ͼ��Ŵ����磺z= 300 ʱ����ʾ����Ұ��СΪ��ǰ��1/3����
				// ��� z С�� 100����ͼ����С�����磺z = 50 ʱ����Ұ�Ŵ�Ϊ��ǰ��2������
				//	ע����һЩ��Ѷʿ��Ʒ�У�ͨ��У׼��Ƭƫ�Ʋ������������Խϴ�ظĽ�
				//	areazoom �ľ�ȷ�ȡ�
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,MAKELONG(nPtzValue1,nPtzValue2),nPtzValue3,nPtzOption);
				break;
			}
		case Ptz_ImageWidth:/*����ʾ��ͼ������Ĭ�ϴ�С����Ĭ
							��ֵ�ɲ�Ʒ��������ͬʱ��Ҫ��ò�
							���� center ���� areazoom ����ʹ
							��*/
		case Ptz_ImageHeight:/*����ʾ��ͼ��߶���Ĭ�ϴ�С����Ĭ
							 ��ֵ�ɲ�Ʒ��������ͬʱ��Ҫ��ò�
							 ���� center ���� areazoom ����ʹ
							 ��*/
			{
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,0,0,0);
				break;
			}
		case Ptz_Move:/*
					  �����ƶ�����ͼ��ָ���ķ����ƶ�
					  ��ͼ���ȵ� 25%��
					  ����ƶ������豸��ָ���ķ����ƶ�
					  ��Լ 50-90 ��2 ��
					  (0)home	 = ��ͼ���ƶ��� Home λ�á�
					  (1)up		 = �����ƶ�ͼ��
					  (2)down	 = �����ƶ�ͼ��
					  (3)left	 = �����ƶ�ͼ��
					  (4)right	 = �����ƶ�ͼ��
					  (5)upleft	 = �ضԽ��������Ϸ��ƶ�ͼ��
					  (6)upright = �ضԽ��������Ϸ��ƶ�ͼ��
					  (7)downleft = �ضԽ��������·��ƶ�ͼ��
					  (8)downright = �ضԽ��������·��ƶ�ͼ��
					  (9)stop = ֹͣˮƽת��/��ֱת��
					  */
			{
				Ptz_MoveOption nMoveOpt = (Ptz_MoveOption)nPtzValue1;
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,(long)nMoveOpt,0,0);
				break;
			}
		// ���²����Դ��ھ��Ժ��������ģʽ
		case Ptz_Pan:	// ˮƽת���豸��һ��ָ��������λ��,���ƶ�ȡֵ-180��180������ƶ�ȡֵ -360��360
		case Ptz_Tilt:	// ��ֱת���豸��ָ��������λ�ã����ƶ�ȡֵ-180��180������ƶ�ȡֵ -360��360
		case Ptz_Zoom:	// �����豸n����һ��ָ��λ�á��ϴ��ֵ��ʾ�Ŵ󣬽�С��ֵ��ʾ��С,�����ƶ�ȡֵ1��9999,���ȡֵ-9999��9999
		case Ptz_Focus:	// ���ڶԽ�n����һ��ָ�����á��ϴ��ֵ��ʾ��������С��ֵ��ʾ�̽�,�����ƶ�ȡֵ1��9999,���ȡֵ-9999��9999
		case Ptz_Iris:	// ���ڹ�Ȧn����һ��ָ���ľ���λ�á��ϴ��ֵ��ʾ�򿪹�Ȧ����С��ֵ��ʾ�رչ�Ȧ�������ƶ�ȡֵ1��9999,���ȡֵ-9999��9999
		case Ptz_Brightness:// ��������n����һ��ָ���ľ���λ�á��ϴ��ֵ��ʾ��������ͼ�񣬽�С��ֵ��ʾ�ϰ���ͼ�񡣾����ƶ�ȡֵ1��9999,���ȡֵ-9999��9999
			{
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,nPtzValue1,0,nPtzOption);
				break;
			}
		case Ptz_AutoFocus:	// ����/�����Զ��Խ�,nPtzValue1ֵΪʱ���ã�Ϊ0����
			
		case Ptz_AutoIris:	// ����/�����Զ��Խ�,nPtzValue1ֵΪʱ���ã�Ϊ0����
			{
				
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,nPtzValue1,0,0);
				break;
			}
		case Ptz_ContinuousPanTiltMove: // ����ˮƽת��/��ֱת������ֵ��ʾ���ң�ˮƽת���������ϣ���ֱת�����ƶ���
										// ��ֵ��ʾ����ˮƽת���������£���ֱת�����ƶ��� 0,0 ��ʾֹͣ�ƶ���
										// nPtzValue1Ϊˮƽת���ٶȣ�nPtzValue2Ϊ��ֱת���ٶ�,ȡֵ��Χ-100��100
			{
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,nPtzValue1,nPtzValue2,0);
				break;
			}
		case Ptz_ContinuousFocusMove:	// �����佹���ڡ���ֵ��ʾͼ��Ŵ󣬸�ֵ��ʾͼ����С�� 0 ��ʾֹͣ���š�
										// nPtzValue1ȡֵ��Χ-100��100
		case Ptz_ContinuousZoomMove:	// �����Խ����ڡ���ֵ��ʾ��С���࣬	��ֵ��ʾ�Ŵ󽹾ࡣ 0 ��ʾֹͣ���ڽ��ࡣ
		case Ptz_ContinuousIrisMove:	// ������Ȧ���ڡ���ֵ��ʾ�򿪹�Ȧ����ֵ��ʾ�رչ�Ȧ�� 0 ��ʾֹͣ���ڹ�Ȧ��
		case Ptz_ContinuousBrightnessMove:// �������ȵ��ڡ���ֵ��ʾ����ͼ�����ȣ���ֵ��ʾ����ͼ�����ȡ� 0 ��ʾֹͣ�������ȡ�
		case Ptz_GogoServerPresetNo:	// �ƶ�����ָ��Ԥ��λ����������λ�ã�ȡֵ1-6
		case Ptz_Speed:					// ����ˮƽת������ֱת�����ٶȡ�ȡֵ1-100
		case Ptz_IrcutFilter:			// ���ƺ����˹�Ƭ��	
										// nPtzValue1 = 1ʱΪon = Ӧ�ú����˹�Ƭ���Ա��������⡣
										// nPtzValue1 = 0ʱΪoff = �Ƴ��˹�Ƭ���Ա�ʹ�����ɵ���ͼ��Ĵ�������
										// nPtzValue1 > 1ʱΪauto = ȡ���ڹ��ߵ�������Զ���	on �� off ֮��ת����
		case Ptz_BackLight:				// ���Ʊ��ⲹ����
										// nPtzValue1 = 1ʱΪon = ����ģʽ��
										// nPtzValue1 = 0ʱoff = ����ģʽ
			m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,nPtzValue1,0,0);
		default:
			break;
	}
	
}
