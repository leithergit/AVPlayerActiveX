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
// 	{// 处理按钮压下消息
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
// 	{// 处理按钮弹起
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
	// 以下操作皆存在绝对和相对两种模式
	_T("Ptz_Pan"),	// 水平转动
	_T("Ptz_Tilt"),				// 垂直转动
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
				// nPtzValue1 X坐标，nPtzValue2 Y坐标
				// 将摄像机的中心移至客户端视频码流的像素坐标x，y。
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,nPtzValue1,nPtzValue2,nPtzOption);
				break;
			}
		case Ptz_Areazoom:
			{
				// nPtzValue1 X坐标，nPtzValue2 Y坐标，nPtzValue3 Z值 为放大倍数
				// 移动至坐标 x , y （类似于 center 命令）并按比例 z/100 进行缩放。如果
				//	z 大于100，则图像放大（例如：z= 300 时，表示将视野缩小为当前的1/3）。
				// 如果 z 小于 100，则图像缩小（例如：z = 50 时，视野放大为当前的2倍）。
				//	注：在一些安讯士产品中，通过校准镜片偏移补偿参数，可以较大地改进
				//	areazoom 的精确度。
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,MAKELONG(nPtzValue1,nPtzValue2),nPtzValue3,nPtzOption);
				break;
			}
		case Ptz_ImageWidth:/*当显示的图像宽度与默认大小（该默
							认值由产品决定）不同时，要求该参
							数与 center 或者 areazoom 联合使
							用*/
		case Ptz_ImageHeight:/*当显示的图像高度与默认大小（该默
							 认值由产品决定）不同时，要求该参
							 数与 center 或者 areazoom 联合使
							 用*/
			{
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,0,0,0);
				break;
			}
		case Ptz_Move:/*
					  绝对移动：将图像按指定的方向移动
					  至图像宽度的 25%。
					  相对移动：将设备按指定的方向移动
					  大约 50-90 度2 。
					  (0)home	 = 将图像移动至 Home 位置。
					  (1)up		 = 向上移动图像。
					  (2)down	 = 向下移动图像。
					  (3)left	 = 向左移动图像。
					  (4)right	 = 向右移动图像。
					  (5)upleft	 = 沿对角线向左上方移动图像。
					  (6)upright = 沿对角线向右上方移动图像。
					  (7)downleft = 沿对角线向左下方移动图像。
					  (8)downright = 沿对角线向右下方移动图像。
					  (9)stop = 停止水平转动/垂直转动
					  */
			{
				Ptz_MoveOption nMoveOpt = (Ptz_MoveOption)nPtzValue1;
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,(long)nMoveOpt,0,0);
				break;
			}
		// 以下操作皆存在绝对和相对两种模式
		case Ptz_Pan:	// 水平转动设备到一个指定的坐标位置,绝移动取值-180…180，相对移动取值 -360…360
		case Ptz_Tilt:	// 垂直转动设备至指定的坐标位置，绝移动取值-180…180，相对移动取值 -360…360
		case Ptz_Zoom:	// 缩放设备n步至一个指定位置。较大的值表示放大，较小的值表示缩小,绝对移动取值1…9999,相对取值-9999…9999
		case Ptz_Focus:	// 调节对焦n步至一个指定的置。较大的值表示长焦，较小的值表示短焦,绝对移动取值1…9999,相对取值-9999…9999
		case Ptz_Iris:	// 调节光圈n步至一个指定的绝对位置。较大的值表示打开光圈，较小的值表示关闭光圈。绝对移动取值1…9999,相对取值-9999…9999
		case Ptz_Brightness:// 调节亮度n步至一个指定的绝对位置。较大的值表示较明亮的图像，较小的值表示较暗的图像。绝对移动取值1…9999,相对取值-9999…9999
			{
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,nPtzValue1,0,nPtzOption);
				break;
			}
		case Ptz_AutoFocus:	// 启用/禁用自动对焦,nPtzValue1值为时启用，为0禁用
			
		case Ptz_AutoIris:	// 启用/禁用自动对焦,nPtzValue1值为时启用，为0禁用
			{
				
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,nPtzValue1,0,0);
				break;
			}
		case Ptz_ContinuousPanTiltMove: // 连续水平转动/垂直转动。正值表示向右（水平转动）及向上（垂直转动）移动，
										// 负值表示向左（水平转动）及向下（垂直转动）移动。 0,0 表示停止移动。
										// nPtzValue1为水平转动速度，nPtzValue2为垂直转动速度,取值范围-100…100
			{
				m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,nPtzValue1,nPtzValue2,0);
				break;
			}
		case Ptz_ContinuousFocusMove:	// 连续变焦调节。正值表示图像放大，负值表示图像缩小。 0 表示停止缩放。
										// nPtzValue1取值范围-100…100
		case Ptz_ContinuousZoomMove:	// 连续对焦调节。正值表示缩小焦距，	负值表示放大焦距。 0 表示停止调节焦距。
		case Ptz_ContinuousIrisMove:	// 连续光圈调节。正值表示打开光圈，负值表示关闭光圈。 0 表示停止调节光圈。
		case Ptz_ContinuousBrightnessMove:// 连续亮度调节。正值表示增加图像亮度，负值表示降低图像亮度。 0 表示停止调节亮度。
		case Ptz_GogoServerPresetNo:	// 移动至与指定预置位编号相关联的位置，取值1-6
		case Ptz_Speed:					// 设置水平转动及垂直转动的速度。取值1-100
		case Ptz_IrcutFilter:			// 控制红外滤光片。	
										// nPtzValue1 = 1时为on = 应用红外滤光片，以便阻隔红外光。
										// nPtzValue1 = 0时为off = 移除滤光片，以便使红外光可到达图像的传感器。
										// nPtzValue1 > 1时为auto = 取决于光线的情况，自动在	on 及 off 之间转换。
		case Ptz_BackLight:				// 控制背光补偿。
										// nPtzValue1 = 1时为on = 明亮模式。
										// nPtzValue1 = 0时off = 正常模式
			m_pAVPlayer->SendPtzCommand(szDeviceID,nCode,nPtzValue1,0,0);
		default:
			break;
	}
	
}
