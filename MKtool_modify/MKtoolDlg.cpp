// MKtoolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MKtool.h"
#include "MKtoolDlg.h"
#include "protocol_info.h"
#include "tool_common.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define CONFIG_FILE_NAME	(".\\config.ini")
#define CONFIG_STR_LEN 256

#define SAVE_RECORD_FILE "\\RecordTxt\\"

MRS_FRAME_STRU RxFrameDataMeter;
int m_nComRecvLen;
BYTE m_pComRecvBuff[MAX_COM_RECV_SIZE];

int m_nComRecvDecLen;
BYTE m_pComRecvDecBuff[MAX_COM_RECV_SIZE];
int m_nComRecvEnLen;
BYTE m_pComRecvEnBuff[MAX_COM_RECV_SIZE];
//static UINT ThreadFun(LPVOID pParam);

protocol protocolFrm;

const CString strProtocol[8]=
{
	"645协议-2007",
	"645协议-1997",
	"188协议",
	"698协议",
};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMKtoolDlg dialo///////////////////////////////////////////////////////////////
// CMKtoolDlg dialog

CMKtoolDlg::CMKtoolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMKtoolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMKtoolDlg)
	m_RecvDataCount = 0;
	m_RecvFrameCount = 0;
	m_SendDataCount = 0;
	m_SendFrameCount = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMKtoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMKtoolDlg)
	DDX_Control(pDX, IDC_EDIT_Send, m_SendData);
	DDX_Control(pDX, IDC_SERIAL_CHECK, m_serial_check);
	DDX_Control(pDX, IDC_SERIAL_BAUDRATE, m_serial_baudrate);
	DDX_Control(pDX, IDC_SERAIL_STOP, m_serial_stop);
	DDX_Control(pDX, IDC_SERAIL_DATA, m_serial_data);
	DDX_Control(pDX, IDC_PORT, m_comPort);
	DDX_Control(pDX, IDC_MSCOMM1, m_ctrlCOMM);
	DDX_Control(pDX, IDC_COMM_RXDATA, m_RxData);
	DDX_Text(pDX, IDC_E_recvDataLen, m_RecvDataCount);
	DDX_Text(pDX, IDC_E_recvFeame, m_RecvFrameCount);
	DDX_Text(pDX, IDC_E_SendDataLen, m_SendDataCount);
	DDX_Text(pDX, IDC_E_sendFeame, m_SendFrameCount);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMKtoolDlg, CDialog)
	//{{AFX_MSG_MAP(CMKtoolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SWITCH_SERIAL, OnSwitchSerial)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_B_CLEAR, OnBClear)
	ON_BN_CLICKED(IDC_B_PARAMETER, OnBTest)
	ON_CBN_SELCHANGE(IDC_C_EVENT_TYPE, OnSelchangeCEventType)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_autoback, OnCHECKautoback)
	ON_BN_CLICKED(IDC_CHECK_645P, OnCheck645p)
	ON_BN_CLICKED(IDC_CHECK_64597, OnCheck645_97p)
	ON_BN_CLICKED(IDC_CHECK_188P, OnCheck188p)
	ON_BN_CLICKED(IDC_CHECK_698P, OnCheck698p)
	ON_BN_CLICKED(IDC_CHECK_StopShow, OnCHECKStopShow)
	ON_BN_CLICKED(IDC_BUTTON_addAddr, OnBUTTONaddAddr)
	ON_BN_CLICKED(IDC_BUTTON_showMeterInfo, OnBUTTONshowMeterInfo)
	ON_BN_CLICKED(IDC_BUTTON_AllDelAddr, OnBUTTONAllDelAddr)
	ON_BN_CLICKED(IDC_BUTTON_DelAddr, OnBUTTONDelAddr)
	ON_BN_CLICKED(IDC_CHECK_saveFile, OnCHECKsaveFile)
	ON_CBN_EDITCHANGE(IDC_C_eventAddr, OnEditchangeCeventAddr)
	ON_BN_CLICKED(IDC_BUTTON_setPacketTime, OnBUTTONsetPacketTime)
	ON_BN_CLICKED(IDC_BUTTON_setMeterInfo, OnBUTTONsetMeterInfo)
	ON_BN_CLICKED(IDC_BUTTON_setDataIn, OnBUTTONsetDataIn)
	ON_BN_CLICKED(IDC_BUTTON_send, OnBUTTONsend)
	ON_BN_CLICKED(IDC_CHECK_Hex, OnCHECKHex)
	ON_BN_CLICKED(IDC_BUTTON_setEvent, OnBUTTONsetEvent)
	ON_BN_CLICKED(IDC_BUTTON_DelEvent, OnBUTTONDelEvent)
	ON_WM_SIZE()
	ON_MESSAGE(WM_COUNT_MSG,CMKtoolDlg::OnCountMsg)    // OnCountMsg是自定义的消息处理函数，可以在这个函数里面进行自定义的消息处理代码
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMKtoolDlg message handlers
LRESULT CMKtoolDlg::OnCountMsg(WPARAM wParam,LPARAM lParam)
{
	CString strRecvTemp,strSendTemp;
	
	if (RxFrameDataMeter.nProType == SUPPORT_PROTOCOL_WRIELESS)
	{
		strRecvTemp="接收:无线串口协议:";
		strSendTemp="发送:无线串口协议:";
	}
	else if (RxFrameDataMeter.nProType == SUPPORT_PROTOCOL_188)
	{
		strRecvTemp="接收:CJT188协议:";
		strSendTemp="发送:CJT188协议:";
	}
	else if (RxFrameDataMeter.nProType == SUPPORT_PROTOCOL_698)
	{
		strRecvTemp="接收698.45协议:";
		strSendTemp="发送698.45协议:";
	}else
	{
		strRecvTemp="接收:";
		strSendTemp="发送:";
	}
	

	m_RecvFrameCount ++;
	m_RecvDataCount += m_nComRecvDecLen;
	strRecvTemp +=byteToHexStr(m_pComRecvDecBuff,m_nComRecvDecLen);
	
	OutputLog(strRecvTemp,&(m_RxData),isStopShow==FALSE);
	if (m_nComRecvEnLen>0)
	{
		strSendTemp+= byteToHexStr(m_pComRecvEnBuff,m_nComRecvEnLen+2);
		strSendTemp +="\r\n";
		OutputLog(strSendTemp,&(m_RxData),isStopShow==FALSE);
	}
	
	memset(&RxFrameDataMeter,0,sizeof(MRS_FRAME_STRU));
	
	m_nComRecvEnLen = 0;
	UpdateData(FALSE);
	return 0;
}

BOOL CMKtoolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	MeterArray *PmeterArray=getMeterArray();
	memset(PmeterArray,0,sizeof(MeterArray));

	ProDIData *nProDIData=getProDIData();
	memset(nProDIData,0,sizeof(ProDIData));
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	SetWindowText("虚拟电表测试工具V1.80");
	
	CRect rect;  
	::GetWindowRect(m_hWnd,rect);//这里m_hWnd为窗口句柄，如果不存在此变量则在该行代码前加一句：HWND h_Wnd=GetSafeHwnd( );  
	ScreenToClient(rect);  
	LONG m_nDlgWidth = rect.right - rect.left;  
	LONG m_nDlgHeight = rect.bottom - rect.top;  
	//Calc 分辨率  
	LONG m_nWidth = GetSystemMetrics(SM_CXSCREEN);  
	LONG m_nHeight = GetSystemMetrics(SM_CYSCREEN);  
	//计算放大倍数(要用float值，否则误差很大)  
	m_Multiple_width = float(m_nWidth)/float(m_nDlgWidth);  
	m_Multiple_height = float(m_nHeight)/float(m_nDlgHeight); 
	// 增大的数量
    lWidth = -m_nDlgWidth + m_nWidth;
	lHeight = -m_nDlgHeight + m_nHeight;

	change_flag = TRUE;//用来判断OnSize执行时，OninitDialg是否已经执行了  

	pMeterDlg = NULL;
	pDataInDlg = NULL;

	isAutoBack = TRUE; // 是否支持自动回复

	isStopShow = 0; // 默认显示
	nPacketTime = 30; // 设置为30ms
	CString strtime;
	strtime.Format("%d",nPacketTime);
	((CEdit*)GetDlgItem(IDC_E_breakTime))->SetWindowText(strtime);

	set64507SmDI(0x37333734);
	set64597SmDI(0x3733);
	((CEdit*)GetDlgItem(IDC_E_SM_DI))->SetWindowText("37333734");
	

	isSaveDateToFile = 0;

	//设置串口参数
	m_serial_baudrate.SetCurSel(2);
	m_serial_check.SetCurSel(2);
	m_serial_data.SetCurSel(3);
	m_serial_stop.SetCurSel(0);

	m_ctrlCOMM.SetInBufferSize(1024);	// 设置输入缓冲区的大小，Bytes
	m_ctrlCOMM.SetOutBufferSize(1024);	// 设置输出缓冲区的大小，Bytes
	m_ctrlCOMM.SetInputMode(1);			// 设置输入方式为二进制方式
	m_ctrlCOMM.SetRThreshold(1);			// 为1表示有一个字符引发一个事件
	//	m_ctrlCom.SetDTREnable(TRUE);
	//m_ctrlCOMM.SetRTSEnable(TRUE);
	m_ctrlCOMM.SetInputLen(0);


	m_nComRecvLen = 0;
	bRxCompletionFlag = TRUE;

	m_RecvFrameCount =0;
	m_SendFrameCount =0;
	m_RecvDataCount = 0;
	m_SendDataCount = 0;

	isHex = 1;

	// 读取配置文件
	char chTemp[CONFIG_STR_LEN];
	
	int nMeterCount, i,nIndex,len;
	int ret;
	CString strTemp,strlog,strAllTemp;
	char strBufTemp[MAX_LEN];

	// 支持的的协议
	ret = GetPrivateProfileString("SupportProtocol", "Protocol", "1", chTemp, 256, CONFIG_FILE_NAME);
	nSupportProtocol = atoi(chTemp);

	ret = GetPrivateProfileString("MeterCount", "count", "", chTemp, 256, CONFIG_FILE_NAME);
	nMeterCount = atoi(chTemp);
	strlog = "";
	for (i=0; i<nMeterCount; i++)
	{
		
		strTemp.Format("meter%d", i+1);
		GetPrivateProfileString("Meter", strTemp, "", chTemp, 256, CONFIG_FILE_NAME);
		strAllTemp = chTemp;
		
		PmeterArray->sMeter[i].nValid =1; // 默认所有的有效

		nIndex = strAllTemp.Find("#");
		len = strAllTemp.Left(nIndex).GetLength();
		memset(strBufTemp,0,MAX_LEN);
		memcpy(strBufTemp,strAllTemp.Left(nIndex),len);
		len = strToHex(strBufTemp,PmeterArray->sMeter[i].address); // 地址
		PmeterArray->sMeter[i].nAddrLen = len; // 地址长度

		strAllTemp.Delete(0, nIndex+1);
		nIndex = strAllTemp.Find("#");
		PmeterArray->sMeter[i].protocol = atoi(strAllTemp.Left(nIndex)); //协议

		strAllTemp.Delete(0, nIndex+1);
		nIndex = strAllTemp.Find("#");
		PmeterArray->sMeter[i].IsSupportEventState = atoi(strAllTemp.Left(nIndex)); //协议


		//strRev(strBufTemp);
		//strlog.Format("第%d 表地址:%s,协议:0x%02x:%s\r\n",i+1,strBufTemp,PmeterArray->sMeter[i].protocol,strProtocol[OSUnMapTbl[PmeterArray->sMeter[i].protocol]]);
		//OutputLog(strlog,&m_RxData,TRUE);
	}
	PmeterArray->numMeter = nMeterCount;
	showMeterInfo(PmeterArray);

	ret = GetPrivateProfileString("DICount", "count", "", chTemp, 256, CONFIG_FILE_NAME);
	nProDIData->nCount = atoi(chTemp);
	BYTE bufHex[8];
	int j;
	for (i=0; i<nProDIData->nCount; i++)
	{
		
		strTemp.Format("DI%d", i+1);
		GetPrivateProfileString("DI", strTemp, "", chTemp, 256, CONFIG_FILE_NAME);
		strAllTemp = chTemp;
		
		nIndex = strAllTemp.Find("#");
		nProDIData->DI[i].isValid = atoi(strAllTemp.Left(nIndex));
		strAllTemp.Delete(0, nIndex+1);

		nIndex = strAllTemp.Find("#");
		len = strAllTemp.Left(nIndex).GetLength();
		memset(strBufTemp,0,MAX_LEN);
		memcpy(strBufTemp,strAllTemp.Left(nIndex),len);
		len = strToHex(strBufTemp,bufHex);
		for (j=0;j<len;j++)
		{
			nProDIData->DI[i].nDI <<=8;
			nProDIData->DI[i].nDI |=bufHex[j];
		}
		strAllTemp.Delete(0, nIndex+1);

		nIndex = strAllTemp.Find("#");
		nProDIData->DI[i].len = atoi(strAllTemp.Left(nIndex));
		strAllTemp.Delete(0, nIndex+1);

		nIndex = strAllTemp.Find("#");
		len = strAllTemp.Left(nIndex).GetLength();
		memset(strBufTemp,0,MAX_LEN);
		memcpy(strBufTemp,strAllTemp.Left(nIndex),len);
		len = strToHex(strBufTemp,nProDIData->DI[i].DIData);
		strAllTemp.Delete(0, nIndex+1);

		nIndex = strAllTemp.Find("#");
		nProDIData->DI[i].nPro = atoi(strAllTemp.Left(nIndex));
		strAllTemp.Delete(0, nIndex+1);

		nIndex = strAllTemp.Find("#");
		nProDIData->DI[i].nFunID = atoi(strAllTemp.Left(nIndex));
	}

	// 初始化设置
	((CButton *)GetDlgItem(IDC_CHECK_autoback))->SetCheck(isAutoBack);//自动回复选上
	((CButton *)GetDlgItem(IDC_CHECK_645P))->SetCheck(nSupportProtocol&SUPPORT_PROTOCOL_645_07);//645选上
	((CButton *)GetDlgItem(IDC_CHECK_64597))->SetCheck(nSupportProtocol&SUPPORT_PROTOCOL_645_97);//645选上
	((CButton *)GetDlgItem(IDC_CHECK_188P))->SetCheck(nSupportProtocol&SUPPORT_PROTOCOL_188);//188选上
	((CButton *)GetDlgItem(IDC_CHECK_698P))->SetCheck(nSupportProtocol&SUPPORT_PROTOCOL_698);//698选上
	((CButton *)GetDlgItem(IDC_CHECK_Hex))->SetCheck(TRUE);
	
	((CComboBox*)GetDlgItem(IDC_C_PROTOCOL_TYPE))->SetCurSel(0);
	((CEdit*)GetDlgItem(IDC_E_ADDR))->SetWindowText("010000000001");

	((CComboBox*)GetDlgItem(IDC_C_EVENT_TYPE))->SetCurSel(0);
	RefreshEventAddr(PmeterArray);

	char strPath[256];
	CString strFileName;
	GetCurrentDirectory(256,strPath);
	sPath.Format("%s",strPath);
	mkdir(sPath+"\\RecordTxt");
	/*
	m_strTime.Format("%s", COleDateTime::GetCurrentTime().Format("%Y-%m-%d"));
	strFileName = sPath;
	strFileName += SAVE_RECORD_FILE+m_strTime+".txt";
	RecordFile.Open(strFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate);
	RecordFile.Seek(0,CFile::end);
	*/

	// 翻转各地址
	/*
	for (i =0 ;i<nMeterCount;i++)
	{
		reverse(PmeterArray->sMeter[i].address,P645_ADDRESS_LEN);
	}
	*/

	// 注册协议
	protocolFrm.regProtocol(SUPPORT_PROTOCOL_645_07|SUPPORT_PROTOCOL_645_97,MRS_Proto645Dec,MRS_Proto645Enc);
	protocolFrm.regProtocol(SUPPORT_PROTOCOL_188,MRS_Proto188Dec,MRS_Proto188Enc);
	protocolFrm.regProtocol(SUPPORT_PROTOCOL_698,MRS_Proto698Dec,MRS_Proto698Enc);
	protocolFrm.regProtocol(SUPPORT_PROTOCOL_188,MRS_ProtoWirelessDec,MRS_ProtoWirelessEnc);
	
	hWakeupEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if (hWakeupEvent == NULL)
	{
		MessageBox("初始化错误，不能正常使用");
	}
//	hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	CreateThread();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

UINT CMKtoolDlg::ThreadFun(LPVOID pParam) 
{
	CString strRecvTemp,strSendTemp;
	long tstart,tend;
	CMKtoolDlg *pdlg=((CMKtoolDlg *)(AfxGetApp()->GetMainWnd()));
	while (TRUE)
	{
		
		WaitForSingleObject(pdlg->hWakeupEvent, INFINITE);
		ResetEvent(pdlg->hWakeupEvent);
	
		tstart=GetTickCount();
		m_pComRecvEnBuff[0] = 0xFE;
		m_pComRecvEnBuff[1] = 0xFE;
		protocolFrm.protocolFmt(m_pComRecvDecBuff+2,m_nComRecvDecLen,m_pComRecvEnBuff+2,&m_nComRecvEnLen,&RxFrameDataMeter,pdlg->nSupportProtocol);
		
		if(RxFrameDataMeter.st645Ctrl.ucDir)
		{
			continue;
		}

		if (pdlg->isAutoBack && m_nComRecvEnLen>0)
		{
			pdlg->CommSendHex(m_pComRecvEnBuff,m_nComRecvEnLen+2);
		}
		tend=GetTickCount();
		if (tend - tstart> pdlg->nPacketTime*2)
		{
			strSendTemp.Format("警告:处理时间过长,可能造成丢帧，耗时:%d ms\r\n",tend - tstart);
			pdlg->OutputLog(strSendTemp,&(pdlg->m_RxData),TRUE);
		}

		pdlg->SendMessage(WM_COUNT_MSG,(WPARAM)(RxFrameDataMeter.nProType),0);
	}
    return  0 ; 
} 
void CMKtoolDlg::CreateThread(void)
{
	pThread = AfxBeginThread(ThreadFun,NULL);
}
void CMKtoolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CMKtoolDlg::RefreshComPortList()
{
	char chName[256];
	char chPortName[256];
	DWORD dwIndex;
	DWORD dwName;
	DWORD dwSizeofPortName;
	CString strPort;
	int nItem, nPort;
	if (m_ctrlCOMM.GetPortOpen())
	{
		return ;
	}
	// delete all the com port
	m_comPort.ResetContent();
	
	// open register key
	HKEY hKey; 
	LPCTSTR data_Set = "HARDWARE\\DEVICEMAP\\SERIALCOMM\\"; 
	long ret0 = (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey)); 
	if (ret0 != ERROR_SUCCESS) 
	{ 
		return; 
	} 
	
	dwIndex = 0;
	
	while (TRUE)
	{
		dwName = sizeof(chName);
		dwSizeofPortName = sizeof(chPortName);
		
		if (RegEnumValue(hKey, dwIndex++, chName, &dwName, NULL, NULL, 
			(PUCHAR)chPortName, &dwSizeofPortName) == ERROR_NO_MORE_ITEMS)
		{
			break;
		}
		
		strPort = chPortName;
		strPort.MakeUpper();
		strPort.TrimLeft("COM");
		nPort = atoi(strPort);
		if (nPort > 0)
		{
			nItem = m_comPort.AddString((char*)chPortName);
			m_comPort.SetItemData(nItem, nPort);
		}
	}
	
	RegCloseKey(hKey);
	
	m_comPort.SetCurSel(0);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMKtoolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	RefreshComPortList();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMKtoolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BEGIN_EVENTSINK_MAP(CMKtoolDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMKtoolDlg)
	ON_EVENT(CMKtoolDlg, IDC_MSCOMM1, 1 /* OnComm */, OnOnCommMscomm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMKtoolDlg::OnOnCommMscomm() 
{
	// TODO: Add your control notification handler code here
	TCHAR szCause[255];
	CString strtemp;
	VARIANT vResponse;
	COleSafeArray safearray_inp;
	long i;
	int nLen;
	BYTE ch;
	
	nLen = 0;
	vResponse.vt = VT_ARRAY;

	switch (m_ctrlCOMM.GetCommEvent())
	{
	case 0:
		break;

	case 1:
		break;

	case 2:
		try
		{
			vResponse = m_ctrlCOMM.GetInput();
			safearray_inp = vResponse;
			nLen = safearray_inp.GetOneDimSize();
			
			if (nLen>0)
			{
				for(i=0; i<nLen; i++)	//转换为BYTE型数组
				{
					safearray_inp.GetElement(&i,&ch);
					if (m_nComRecvLen < MAX_COM_RECV_SIZE)
					{
						m_pComRecvBuff[m_nComRecvLen++] = ch;
					}
					else
					{
						TRACE("COM receive buffer overflow!\r\n");
						m_nComRecvLen = 0;
					}
				}
				SetTimer(1,nPacketTime,NULL);
				bRxCompletionFlag = FALSE;
			}

		}
		catch (CException* e)
		{
			e->GetErrorMessage(szCause, 255);
			strtemp.Format("串口异常: %s", szCause);
			MessageBox(strtemp, "串口异常");
			e->Delete();
		}
		break;

	default:
		break;
	}
}

void CMKtoolDlg::OnSwitchSerial() 
{
	// TODO: Add your control notification handler code here
	int nPortNum;
	CString str1,str2,str3;
	TCHAR szCause[255];
	str1 = "";
	try
	{
		if (m_ctrlCOMM.GetPortOpen())
		{
			m_ctrlCOMM.SetPortOpen(FALSE);
			GetDlgItem(IDC_SWITCH_SERIAL)->SetWindowText("打开串口");
			m_comPort.EnableWindow(TRUE);
			m_serial_stop.EnableWindow(TRUE);
			m_serial_data.EnableWindow(TRUE);
			m_serial_check.EnableWindow(TRUE);
			m_serial_baudrate.EnableWindow(TRUE);
		}
		else
		{
			nPortNum = m_comPort.GetItemData(m_comPort.GetCurSel());

			m_serial_baudrate.GetLBText(m_serial_baudrate.GetCurSel(), str2);      //从combox读取操作码
			str1 +=str2+",";

			m_serial_check.GetLBText(m_serial_check.GetCurSel(), str2);      //从combox读取操作码
			str3.Format("%c",str2.GetAt(0));
			str1 += str3+",";

			m_serial_data.GetLBText(m_serial_data.GetCurSel(), str2);      //从combox读取操作码
			str1 +=str2+",";

			m_serial_stop.GetLBText(m_serial_stop.GetCurSel(), str2);      //从combox读取操作码
			str1 +=str2;

			m_ctrlCOMM.SetCommPort(nPortNum);
			m_ctrlCOMM.SetSettings(str1);
			m_ctrlCOMM.SetPortOpen(TRUE);
			
				m_comPort.EnableWindow(FALSE);
				m_serial_stop.EnableWindow(FALSE);
				m_serial_data.EnableWindow(FALSE);
				m_serial_check.EnableWindow(FALSE);
				m_serial_baudrate.EnableWindow(FALSE);
				GetDlgItem(IDC_SWITCH_SERIAL)->SetWindowText("关闭串口");
			
			//	GetDlgItem(IDC_ID)->SetFocus();
		}
	}
	catch (CException* e)
	{
		e->GetErrorMessage(szCause, 255);
		str1.Format("串口异常: %s", szCause);
		MessageBox(str1, "串口异常");
		
		e->Delete();
	}
}

// 写日志
void CMKtoolDlg::WriteLogToFile(CString strLog)
{
	int nLen;
	CString strCurrentTime;
	CString strRecordFileName;
	if (!isSaveDateToFile)
	{
		return;
	}
	nLen = 0;
	strCurrentTime.Format("%s", COleDateTime::GetCurrentTime().Format("%Y-%m-%d"));
	
	if (strCurrentTime != m_strTime)
	{	
		RecordFile.Close();
		strRecordFileName = sPath;
		strRecordFileName += SAVE_RECORD_FILE + strCurrentTime + ".txt";
		RecordFile.Open(strRecordFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
		m_strTime = strCurrentTime;
	}
	nLen = strLog.GetLength();
	RecordFile.Write(strLog, nLen);
}
CString CMKtoolDlg:: byteToHexStr(BYTE *pData, int nRecvLen)
{
	int i;
	int nvalue;
	int nValue1;
	int nVlaue2;
	CString strRecvData;
	CString strVlaue1;
	CString strValue2;
	
	for (i=0;i<nRecvLen;i++)
	{
		nvalue=pData[i];
		nValue1=nvalue/16;
		nVlaue2=nvalue % 16;
		
		if (nValue1 >=0 && nValue1 <= 9)
		{
			strVlaue1 = nValue1 + 48;
		}
		else
		{
			strVlaue1 = nValue1 + 55;
		}
		
		if (nVlaue2 >=0 && nVlaue2 <= 9)
		{
			strValue2 = nVlaue2 + 48;
		}
		else
		{
			strValue2 = nVlaue2 + 55;
		}
		
		strRecvData += strVlaue1;
		strRecvData += strValue2;
	}
	nRecvLen = nRecvLen * 2;
	return strRecvData;
}

void CMKtoolDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		if (bRxCompletionFlag)
		{ 
			if (m_nComRecvLen>0)
			{
				do 
				{
					memcpy(m_pComRecvDecBuff,m_pComRecvBuff,m_nComRecvLen);
					m_nComRecvDecLen = m_nComRecvLen;
					m_nComRecvLen = 0;
					SetEvent(hWakeupEvent);
				} while (0);
				
			}
			
			KillTimer(1);
		}
		bRxCompletionFlag = TRUE;
	}
	else if (nIDEvent == 2)
	{
		
	}
	CDialog::OnTimer(nIDEvent);
}

CString CMKtoolDlg::RxParaData(BYTE buf[],int nlen)
{
	int i;
	CString strPara,strTemp;
	BYTE nVerify = 0;
	BYTE nCmd;
	BYTE nDataLen;

	for (i=0; i<4;i++)
	{
		if (buf[i] != 0xFF)
		{
			strPara.Format("%s\n\r","接收的数据前导字节不是0xFF 错误");
			return strPara;
		}
	}

	if (buf[4]!=0x68)
	{
		strPara.Format("%s\n\r","接收的数据起始数据不是0x68 错误");
		return strPara;
	}

	for (i=4;i<nlen-2;i++)
	{
		nVerify += buf[i];
	}

	if (nVerify!=buf[nlen-2])
	{
		strPara.Format("%s 计算值:%x,接收值:%x\n\r","接收的数据校验错误",nVerify,buf[nlen-2]);
		return strPara;
	}

	if (buf[nlen-1]!=0x16)
	{
		strPara.Format("%s\n\r","接收的数据结束符不是0x16 错误");
		return strPara;
	}

	nCmd = buf[10];
	nDataLen = buf[11];
// 解析
	return strPara;
}
void CMKtoolDlg::OutputLog(LPCSTR strLog,CEdit *pEdit,BOOL isOut)
{
	// 最多600行
	int nEnd;
	int temp;
	CString str;
	SYSTEMTIME st;
	
	GetLocalTime(&st);
	//str.Format("%s: %s\r\n", COleDateTime::GetCurrentTime().Format("%H:%M:%S"), strLog);
	str.Format("[%02d:%02d:%02d:%04d]: %s\r\n", st.wHour,st.wMinute,st.wSecond,st.wMilliseconds, strLog);
	
	if (isOut)
	{
		if (pEdit->GetLineCount() >= 600)
		{
			// 删除前面的300行
			nEnd = pEdit->LineIndex(300);
			pEdit->SetSel(0, nEnd);
			pEdit->Clear();
			pEdit->LineScroll(pEdit->GetLineCount(),0);
		}
		temp = pEdit->GetWindowTextLength();
		//	m_RxData.SetSel(-1, -1);
		pEdit->SetSel(temp,temp);
		pEdit->ReplaceSel(str);
		::SendMessage(pEdit->GetSafeHwnd(), EM_SCROLLCARET, 0, 0); //滚动到插入点
	}

	WriteLogToFile(str);
}
int CMKtoolDlg::CommSend(LPCSTR strSend)
{
	int i, nSendLen;
	CByteArray array;
	TCHAR szCause[255];
	
	nSendLen = strlen(strSend);
	array.RemoveAll();
	array.SetSize(nSendLen);
	for(i=0; i<nSendLen; i++)
	{
		array.SetAt(i, strSend[i]);
	}
	
	try
	{
		m_ctrlCOMM.SetOutput(COleVariant(array)); // 发送数据
	}
	catch (CException* e)
	{
		e->GetErrorMessage(szCause, 255);
		TRACE("数据发送异常: %s\r\n", szCause);
		e->Delete();
		
		return -1;
	}
	m_SendFrameCount++;
	m_SendDataCount += nSendLen;
	return 0;
}
int CMKtoolDlg::CommSendHex(BYTE buf[],int nlen)
{
	int i;
	CByteArray array;
	TCHAR szCause[255];

	array.RemoveAll();
	array.SetSize(nlen);
	for(i=0; i<nlen; i++)
	{
		array.SetAt(i, buf[i]);
	}
	
	try
	{
		this->m_ctrlCOMM.SetOutput(COleVariant(array)); // 发送数据
	}
	catch (CException* e)
	{
		e->GetErrorMessage(szCause, 255);
		TRACE("数据发送异常: %s\r\n", szCause);
		e->Delete();
		
		return -1;
	}
	m_SendDataCount += nlen;
	m_SendFrameCount++;
	return 0;
}

void CMKtoolDlg::RefreshEventAddr(MeterArray *PmeterArray)
{
	CString strlog,strTemp,strAddr;
	int i,j;
	
	((CComboBox*)GetDlgItem(IDC_C_eventAddr))->ResetContent();//消除现有所有内容
	
	for (i=0;i<PmeterArray->numMeter;i++)
	{
		strAddr = "";
		for (j=PmeterArray->sMeter[i].nAddrLen-1;j>=0;j--)
		{
			strTemp.Format("%02x",PmeterArray->sMeter[i].address[j]);
			strAddr += strTemp;
		}
		((CComboBox*)GetDlgItem(IDC_C_eventAddr))->AddString(strAddr);
	}
	((CComboBox*)GetDlgItem(IDC_C_eventAddr))->SetCurSel(0);
}

void CMKtoolDlg::OnBClear() 
{
	// TODO: Add your control notification handler code here

	m_RxData.SetWindowText(_T(""));
	m_RecvDataCount = 0;
	m_RecvFrameCount = 0;
	m_SendDataCount = 0;
	m_SendFrameCount = 0;
	UpdateData(FALSE);
}

void CMKtoolDlg::OnBTest() 
{
	// TODO: Add your control notification handler code here
	int nLen=0;
	char s1[MAX_LEN];
	
	((CEdit*)GetDlgItem(IDC_EDIT_Send))->GetWindowText(s1,MAX_LEN);
	nLen = strToHex(s1,m_pComRecvDecBuff);
	
	if (nLen == 0)
	{
		return ;
	}
	m_nComRecvDecLen = nLen;
	
	SetEvent(hWakeupEvent);
/*
	strLog += byteToHexStr(m_pComRecvDecBuff,m_nComRecvLen);

	OutputLog(strLog,&m_RxData);
	m_nComRecvLen=0;
	protocolFrm.protocolFmt(m_pComRecvDecBuff,m_nComRecvDecLen,m_pComRecvEnBuff,&m_nComRecvEnLen,&RxFrameDataMeter);
	
	strtemp += byteToHexStr(m_pComRecvEnBuff,m_nComRecvEnLen);
	CommSendHex(m_pComRecvEnBuff,m_nComRecvEnLen);
	memset(&RxFrameDataMeter,0,sizeof(MRS_FRAME_STRU));
	m_nComRecvEnLen = 0;
	OutputLog(strtemp,&m_RxData);
	*/
}
BYTE CMKtoolDlg::charToHex(char ch)
{
	BYTE temp;
	temp = ch;
	if (ch>=48 && ch <58)
	{
		temp = ch-48;
	}
	else
	{
		if (ch>='a' && ch<='f')
		{
			temp = ch -'a'+10;
		}
		else if (ch>='A' && ch<='F')
		{
			temp = ch -'A'+10;
		}
		else
		{
			TRACE("input paratemer error!");
		}
	}
	return temp;
}

BYTE CMKtoolDlg::strToHex(char str[],BYTE byte[])
{
	int len,i,ntemp;
	BYTE nHexLen = 0;
	
	if (str==NULL || byte ==NULL)
	{
		return 0;
	}
	len = strlen(str);
	
	for (i=0; i<len; )
	{
		if (str[i]==' ')
		{
			i++;
			continue;
		}
		ntemp = charToHex(str[i])<<4;
		i++;

		if (i<len)
		{
			ntemp += charToHex(str[i]);
			i++;
		}
		
		byte[nHexLen++] = ntemp;
	}
	return nHexLen;
}

int CMKtoolDlg::CstringToHex(CString cstr,BYTE byte[])
{
	int len,i,ntemp;
	BYTE nHexLen = 0;
	
	if (byte ==NULL)
	{
		return 0;
	}
	len = cstr.GetLength();
	
	for (i=0; i<len; )
	{
		ntemp = charToHex(cstr.GetAt(i))<<4;
		i++;
		if (i<len)
		{
			ntemp += charToHex(cstr.GetAt(i));
			i++;
		}
		byte[nHexLen++] = ntemp;
	}
	return nHexLen;
}
/*将整数转化成定长BCD码,并逆序*/
int CMKtoolDlg:: IntToBcd(LONG64 data,BYTE bcd[],BYTE *len)
{
    //int i = 0;
    //LONG64 remain = data;
    
    return 0;
}
int CMKtoolDlg::CStrToHex(CString str,BYTE byte[])
{
	/*int len,i,ntemp,n;
	int nHexLen = 0;
	
	if (str == "" || byte ==NULL)
	{
		return 0;
	}
	len = str.GetLength();

	while (!str.IsEmpty())
	{
		ntemp = str.Left(0)-0x30;
		str.Delete(0, 1);
		if (!str.IsEmpty())
		{
			ntemp += (str.Left(0)-0x30)*10;
			str.Delete(0, 1);
		}
		byte[nHexLen++] = ntemp;
	}
	*/
	return 0;
}

void CMKtoolDlg::OnSelchangeCEventType() 
{
	// TODO: Add your control notification handler code here
	/*
	CString strOrderCmd;
	CComboBox *pCombox;
	SYSTEMTIME st ;
	CString strOrderData;
	
	pCombox = ((CComboBox*)GetDlgItem(IDC_C_EVENT_TYPE));
	pCombox->GetLBText(pCombox->GetCurSel(), strOrderCmd);      //从combox读取操作码
	
	nOrderCmd = charToHex(strOrderCmd.GetAt(2));
	nOrderCmd <<= 4;
	nOrderCmd +=charToHex(strOrderCmd.GetAt(3));

	if (nOrderCmd==0x25)
	{
		GetLocalTime(&st);    //获得当前时间，即电脑上的当前时间
		strOrderData.Format("%04d%02d%02d%02d%02d%02d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
		//	COleDateTime::GetCurrentTime().Format("%H:%M:%S")
		m_Parameter = strOrderData;
	}
	UpdateData(FALSE);
	*/
}

void CMKtoolDlg::OnDestroy() 
{
	CString str,strMeter,str1,strTemp;
	int n,i,j,nLen;
	if (hWakeupEvent!=NULL)
	{
		CloseHandle(hWakeupEvent);
		hWakeupEvent = NULL;
	}
	MeterArray *PmeterArray=getMeterArray();
	ProDIData *nProDIData=getProDIData();

	WritePrivateProfileString("配置文件说明", "MeterCount", "count里面填写增加表数量",  CONFIG_FILE_NAME);
	WritePrivateProfileString("配置文件说明", "Meter", "meter数字=表地址#协议#",  CONFIG_FILE_NAME);
	WritePrivateProfileString("配置文件说明", "表地址", "12位字符",  CONFIG_FILE_NAME);
	WritePrivateProfileString("配置文件说明", "协议对应关系", "1<<<0:645协议-07 ;1<<<1:645协议-97 ;1<<2:188协议 ; 1<<3:698协议",  CONFIG_FILE_NAME);

	//保存支持的协议
	str.Format("%d",nSupportProtocol);
	WritePrivateProfileString("SupportProtocol", "Protocol", str,  CONFIG_FILE_NAME);

	// 保存电表数据
	n = PmeterArray->numMeter;
	str.Format("%d",n);
	WritePrivateProfileString("MeterCount", "count", str,  CONFIG_FILE_NAME);
	for (i=0;i<n;i++)
	{
		str.Format("%s%d","meter",i+1);
		nLen = PmeterArray->sMeter[i].nAddrLen;
		str1 = "";
		for (j=0;j<nLen;j++)
		{
			strTemp.Format("%02x",PmeterArray->sMeter[i].address[j]);
			str1 +=strTemp;
		}
		strMeter.Format("%s#%d#%d#",str1,PmeterArray->sMeter[i].protocol,PmeterArray->sMeter[i].IsSupportEventState);
		WritePrivateProfileString("Meter", str, strMeter,  CONFIG_FILE_NAME);
	}
	str.Format("%d",isAutoBack);
	WritePrivateProfileString("设置参数", "自动回复", str,  CONFIG_FILE_NAME);
	str.Format("%d",nSupportProtocol);
	WritePrivateProfileString("设置参数", "支持协议", str,  CONFIG_FILE_NAME);

	str.Format("%d",nProDIData->nCount);
	WritePrivateProfileString("DICount", "count", str,  CONFIG_FILE_NAME);
	for (i=0;i<nProDIData->nCount;i++)
	{
		str.Format("%s%d","DI",i+1);
		nLen = nProDIData->DI[i].len;
		str1 = "";
		for (j=0;j<nLen;j++)
		{
			strTemp.Format("%02x",nProDIData->DI[i].DIData[j]);
			str1 +=strTemp;
		}
		strMeter.Format("%d#%x#%d#%s#%d#%d#",nProDIData->DI[i].isValid,nProDIData->DI[i].nDI,nProDIData->DI[i].len,
			str1,nProDIData->DI[i].nPro,nProDIData->DI[i].nFunID);
		WritePrivateProfileString("DI", str, strMeter,  CONFIG_FILE_NAME);
	}

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	

}

void CMKtoolDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
//	CString str = "address#protocol";
//	WritePrivateProfileString("MeterTest", "name", str,  CONFIG_FILE_NAME);
	CDialog::OnClose();
}

void CMKtoolDlg::OnCHECKautoback() 
{
	// TODO: Add your control notification handler code here
	isAutoBack = ((CButton *)GetDlgItem(IDC_CHECK_autoback))->GetCheck();//选上
}

void CMKtoolDlg::OnCheck645p() 
{
	// TODO: Add your control notification handler code here
	if (((CButton *)GetDlgItem(IDC_CHECK_645P))->GetCheck())
	{
		nSupportProtocol |= SUPPORT_PROTOCOL_645_07;//645选上
	}
	else
	{
		nSupportProtocol &=~SUPPORT_PROTOCOL_645_07 ;
	}
}
void CMKtoolDlg::OnCheck645_97p() 
{
	// TODO: Add your control notification handler code here
	if (((CButton *)GetDlgItem(IDC_CHECK_64597))->GetCheck())
	{
		nSupportProtocol |= SUPPORT_PROTOCOL_645_97;//645选上
	}
	else
	{
		nSupportProtocol &=~SUPPORT_PROTOCOL_645_97 ;
	}
}

void CMKtoolDlg::OnCheck188p() 
{
	// TODO: Add your control notification handler code here
	if (((CButton *)GetDlgItem(IDC_CHECK_188P))->GetCheck())
	{
		nSupportProtocol |= SUPPORT_PROTOCOL_188;//645选上
	}
	else
	{
		nSupportProtocol &=~SUPPORT_PROTOCOL_188 ;
	}
}

void CMKtoolDlg::OnCheck698p() 
{
	// TODO: Add your control notification handler code here
	if (((CButton *)GetDlgItem(IDC_CHECK_698P))->GetCheck())
	{
		nSupportProtocol |= SUPPORT_PROTOCOL_698;
	}
	else
	{
		nSupportProtocol &=~SUPPORT_PROTOCOL_698;
	}	
}


void CMKtoolDlg::OnCHECKStopShow() 
{
	// TODO: Add your control notification handler code here
	if (((CButton *)GetDlgItem(IDC_CHECK_StopShow))->GetCheck())
	{
		isStopShow = TRUE;
	}
	else
	{
		isStopShow = FALSE;
	}
	
}
void CMKtoolDlg::showMeterInfo(MeterArray *PmeterArray)
{
	CString strlog,strTemp,strAddr,strPro;
	int i,j;
	if (PmeterArray->numMeter==0)
	{
		strlog.Format("没有电表信息\r\n");
		OutputLog(strlog,&m_RxData,TRUE);
		return;
	}
	for (i=0;i<PmeterArray->numMeter;i++)
	{
		strAddr = "";
		for (j=PmeterArray->sMeter[i].nAddrLen-1;j>=0;j--)
		{
			strTemp.Format("%02x",PmeterArray->sMeter[i].address[j]);
			strAddr += strTemp;
		}
		strPro = "";
		for (j = 0;j<4;j++)
		{
			if (PmeterArray->sMeter[i].protocol&(0x01<<j))
			{
				strTemp.Format("%s ",strProtocol[j]);
				strPro += strTemp;
			}
		}
		strlog.Format("第%d 表地址:%s,协议:0x%02x:%s\r\n",i+1,strAddr,PmeterArray->sMeter[i].protocol,strPro);
		OutputLog(strlog,&m_RxData,TRUE);
	}
}

void CMKtoolDlg::OnBUTTONaddAddr() 
{
	// TODO: Add your control notification handler code here
	MeterArray *PmeterArray=getMeterArray();
	OperateMeterInfo(PmeterArray,1); // 增加电表信息
	RefreshEventAddr(PmeterArray);
	UpdateData(TRUE);
}

void CMKtoolDlg::OnBUTTONshowMeterInfo() 
{
	// TODO: Add your control notification handler code here
	MeterArray *PmeterArray=getMeterArray();
	showMeterInfo(PmeterArray);
	RefreshEventAddr(PmeterArray);
}

void CMKtoolDlg::OnBUTTONAllDelAddr() 
{
	// TODO: Add your control notification handler code here
	MeterArray *PmeterArray=getMeterArray();
	memset(PmeterArray,0,sizeof(MeterArray));
	RefreshEventAddr(PmeterArray);
	UpdateData(TRUE);
	
}

void CMKtoolDlg::OnBUTTONDelAddr() 
{
	// TODO: Add your control notification handler code here
	MeterArray *PmeterArray=getMeterArray();
	OperateMeterInfo(PmeterArray,2); // 删除电表信息
	RefreshEventAddr(PmeterArray);
	UpdateData(TRUE);

}
void CMKtoolDlg::OperateMeterInfo(MeterArray *PmeterArray,int op)
{
	Meter MeterTemp;
	int nProtocol=0,nTemp=0;
	UINT nAddrLen =0;
	char strAddr[32];
	CString strlog;
	
	nTemp=  (((CComboBox*)GetDlgItem(IDC_C_PROTOCOL_TYPE))->GetCurSel());
	nProtocol = 0x01<<nTemp;
	((CEdit*)GetDlgItem(IDC_E_ADDR))->GetWindowText(strAddr,32);
	
//	if (op==1)
	{
		if(nProtocol&SUPPORT_PROTOCOL_188)
		{
			nAddrLen = 7;	
		}
		else if (nProtocol&SUPPORT_PROTOCOL_698)
		{
		}
		else
		{
			nAddrLen = P645_ADDRESS_LEN;
		}
		
		if (strlen(strAddr)%2 || (nAddrLen!=strlen(strAddr)/2))
		{
			strlog.Format("%s 协议地址长度应该为 %d",strProtocol[nTemp],nAddrLen);
			MessageBox(strlog);
			return ;
		}
	}
	
	
	
	strToHex(strAddr,MeterTemp.address);
	MeterTemp.nAddrLen = nAddrLen;
	MeterTemp.protocol = nProtocol;
	reverse(MeterTemp.address,nAddrLen);
	int i,j,isCommAddr=0,nIndex;
	for (i=0;i<PmeterArray->numMeter;i++)
	{
		if (MeterTemp.nAddrLen !=PmeterArray->sMeter[i].nAddrLen )
		{
			continue;
		}
		for (j=0;j<MeterTemp.nAddrLen;j++)
		{
			if (MeterTemp.address[j] != PmeterArray->sMeter[i].address[j])
			{
				break;
			}
		}
		if (j>=MeterTemp.nAddrLen)
		{
			isCommAddr =1;
			nIndex = i;
			break;
		}
	}
	if (op==1)
	{
		if (isCommAddr==1)
		{
			MessageBox("无需添加相同的地址");
			return ;
		}
		PmeterArray->sMeter[PmeterArray->numMeter].nAddrLen = MeterTemp.nAddrLen;
		PmeterArray->sMeter[PmeterArray->numMeter].protocol = MeterTemp.protocol;
		memcpy(PmeterArray->sMeter[PmeterArray->numMeter].address,MeterTemp.address,MeterTemp.nAddrLen);
		PmeterArray->numMeter++;
		
		strlog.Format("第%d 表地址:%s,协议:%s\r\n",PmeterArray->numMeter,strAddr,strProtocol[OSUnMapTbl[nProtocol]]);
		OutputLog(strlog,&m_RxData,TRUE);
	}
	else if (op==2)
	{
		if (isCommAddr==0)
		{
			strlog.Format("没有找到需要删除电表\r\n");
			OutputLog(strlog,&m_RxData,TRUE);
			return ;
		}
		PmeterArray->numMeter--;
		for (i=nIndex;i<PmeterArray->numMeter;i++)
		{
			memcpy(&(PmeterArray->sMeter[i]),&(PmeterArray->sMeter[i+1]),sizeof(Meter));
		}
		strlog.Format("第%d 表地址:%s,协议:%s 已被删除\r\n",nIndex+1,strAddr,strProtocol[OSUnMapTbl[nProtocol]]);
		OutputLog(strlog,&m_RxData,TRUE);
	}
}

void CMKtoolDlg::OnCHECKsaveFile() 
{
	// TODO: Add your control notification handler code here
	if (((CButton *)GetDlgItem(IDC_CHECK_saveFile))->GetCheck())
	{
		CString strFileName;
		if(RecordFile.m_hFile == CFile::hFileNull)
		{
			m_strTime.Format("%s", COleDateTime::GetCurrentTime().Format("%Y-%m-%d"));
			strFileName = sPath;
			strFileName += SAVE_RECORD_FILE+m_strTime+".txt";
			RecordFile.Open(strFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate);
			RecordFile.Seek(0,CFile::end);
		}
		isSaveDateToFile = TRUE;
	}
	else
	{
		isSaveDateToFile = FALSE;
		if(RecordFile.m_hFile != CFile::hFileNull)
		{
			RecordFile.Close();
		}
	}
}

void CMKtoolDlg::OnEditchangeCeventAddr() 
{
	// TODO: Add your control notification handler code here
	//MeterArray *PmeterArray=getMeterArray();
	//RefreshEventAddr(PmeterArray);
}

void CMKtoolDlg::OnBUTTONsetPacketTime() 
{
	// TODO: Add your control notification handler code here
	char strtime[32];
	
	CString strlog;
	
	((CEdit*)GetDlgItem(IDC_E_breakTime))->GetWindowText(strtime,32);
	nPacketTime = atoi(strtime);
}

void CMKtoolDlg::OnBUTTONsetMeterInfo() 
{
	// TODO: Add your control notification handler code here
	if (pMeterDlg == NULL)   
	{  
		pMeterDlg = new CMeterDialog();  
		pMeterDlg->Create(IDD_METER_DIALOG,this);  
	}  
		
	if (pMeterDlg == NULL) 
	{
		MessageBox(_T("子窗口的句柄为空！"));
		return ;
	}
    pMeterDlg->ShowWindow(SW_SHOWNORMAL); 
	pMeterDlg->showMeterInfo();
}

void CMKtoolDlg::OnBUTTONsetDataIn() 
{
	// TODO: Add your control notification handler code here
	if (pDataInDlg == NULL)   
	{  
		pDataInDlg = new CDialogDataIn();  
		pDataInDlg->Create(IDD_DIALOG_DI,this);  
	}  
	
	if (pDataInDlg == NULL) 
	{
		MessageBox(_T("子窗口的句柄为空！"));
		return ;
	}
    pDataInDlg->ShowWindow(SW_SHOWNORMAL);
}

void CMKtoolDlg::OnBUTTONsend() 
{
	// TODO: Add your control notification handler code here
	BYTE *pSendBuf = NULL;
	char *pStrSend = NULL;
	CString strTemp = "发送:";
	int nLen;
	if (NULL == pSendBuf)
	{
		pSendBuf = (BYTE *)malloc(2048);
		if (pSendBuf == NULL )
		{
			return ;
		}
		pStrSend = (char *)malloc(2048);
		if (pStrSend == NULL )
		{
			free(pSendBuf);
			return ;
		}
		memset(pStrSend,0,2048);
	}
	((CEdit*)GetDlgItem(IDC_EDIT_Send))->GetWindowText(pStrSend,1024);
	strTemp += pStrSend;
	if (isHex)
	{
		nLen = strToHex(pStrSend,pSendBuf);
		CommSendHex(pSendBuf,nLen);
	}
	else
	{
		CommSend(pStrSend);
	}
	
	
	OutputLog(strTemp,&m_RxData,TRUE);

	if (NULL != pSendBuf)
	{
		free(pSendBuf);
	}
	if (NULL != pStrSend)
	{
		free(pStrSend);
	}
	UpdateData(FALSE);
}

void CMKtoolDlg::OnCHECKHex() 
{
	// TODO: Add your control notification handler code here
	if (((CButton *)GetDlgItem(IDC_CHECK_Hex))->GetCheck())
	{
		isHex =1;//Hex选上
	}
	else
	{
		isHex =0;
	}
}

void CMKtoolDlg::OnBUTTONsetEvent() 
{
	// TODO: Add your control notification handler code here
	int nEventSel = (((CComboBox*)GetDlgItem(IDC_C_EVENT_TYPE))->GetCurSel());
	int nMeterSel =(((CComboBox*)GetDlgItem(IDC_C_eventAddr))->GetCurSel());
	MeterArray *PmeterArray=getMeterArray();
	int i,j;
		
	if (((CButton *)GetDlgItem(IDC_CHECK_ALL_Meter))->GetCheck())
	{ // 所有电表
		
		if (((CButton *)GetDlgItem(IDC_CHECK_ALL_event))->GetCheck())
		{
			for (i=0;i<PmeterArray->numMeter;i++)
			{// 所有事件
				for (j =0;j<MAETER_EVENT_COUNT;j++)
				{
					PmeterArray->sMeter[i].nEvent[j]++;
				}
			}
		}
		else
		{
			for (i=0;i<PmeterArray->numMeter;i++)
			{
				PmeterArray->sMeter[i].nEvent[nEventSel%MAETER_EVENT_COUNT]++;
			}
		}
	}
	else
	{
		if (((CButton *)GetDlgItem(IDC_CHECK_ALL_event))->GetCheck())
		{
			for (j =0;j<MAETER_EVENT_COUNT;j++)
			{
				PmeterArray->sMeter[nMeterSel].nEvent[j]++;
			}
		}
		else
		{
			PmeterArray->sMeter[nMeterSel].nEvent[nEventSel%MAETER_EVENT_COUNT]++;
		}
	}
	
}
void CMKtoolDlg::OnBUTTONDelEvent() 
{
	// TODO: Add your control notification handler code here
	int nEventSel = (((CComboBox*)GetDlgItem(IDC_C_EVENT_TYPE))->GetCurSel());
	int nMeterSel =(((CComboBox*)GetDlgItem(IDC_C_eventAddr))->GetCurSel());
	MeterArray *PmeterArray=getMeterArray();
	int i,j;
	
	if (((CButton *)GetDlgItem(IDC_CHECK_ALL_Meter))->GetCheck())
	{
		
		if (((CButton *)GetDlgItem(IDC_CHECK_ALL_event))->GetCheck())
		{
			for (i=0;i<PmeterArray->numMeter;i++)
			{
				for (j =0;j<MAETER_EVENT_COUNT;j++)
				{
					PmeterArray->sMeter[i].nEvent[j] =0;
				}
			}
		}
		else
		{
			for (i=0;i<PmeterArray->numMeter;i++)
			{
				PmeterArray->sMeter[i].nEvent[nEventSel%MAETER_EVENT_COUNT]=0;
			}
		}
	}
	else
	{
		if (((CButton *)GetDlgItem(IDC_CHECK_ALL_event))->GetCheck())
		{
			for (j =0;j<MAETER_EVENT_COUNT;j++)
			{
				PmeterArray->sMeter[nMeterSel].nEvent[j] = 0;
			}
		}
		else
		{
			PmeterArray->sMeter[nMeterSel].nEvent[nEventSel%MAETER_EVENT_COUNT]=0;
		}
	}
	
}
void CMKtoolDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (change_flag)
    {
		ReSizeOnlyXY(IDC_COMM_RXDATA,0,0,lWidth,lHeight);
		ReSizeOnlyXY(IDC_EDIT_Send,0,lHeight,lWidth,lHeight);
		
		ReSizeOnlyXY(IDC_B_PARAMETER,lWidth,lHeight,lWidth,lHeight);
		ReSizeOnlyXY(IDC_BUTTON_send,lWidth,lHeight,lWidth,lHeight);
		
		ReSizeOnlyXY(IDC_STATIC_crc,0,lHeight,0,lHeight);
		ReSizeOnlyXY(IDC_STATIC_sendFile,0,lHeight,0,lHeight); 
		ReSizeOnlyXY(IDC_STATIC_sendTime,0,lHeight,0,lHeight);
		
        //恢复放大倍数，并保存 (确保还原时候能够还原到原来的大小)  
        m_Multiple_width = float(1)/m_Multiple_width;  
        m_Multiple_height = float(1)/m_Multiple_height; 
		lWidth = -lWidth;
		lHeight = -lHeight;
    }   
	
}
void CMKtoolDlg::ReSizeMultiple(int nID) 
{
	CRect Rect;   
	CWnd *pWnd = ((CWnd*)GetDlgItem(nID));
	if (pWnd == NULL)
	{
		return ;
	}
    pWnd->GetWindowRect(&Rect);   
    ScreenToClient(&Rect);   
    //计算控件左上角点   
    CPoint OldTLPoint,TLPoint;   
    OldTLPoint = Rect.TopLeft();   
	TLPoint.x = long(OldTLPoint.x *m_Multiple_width);   
    TLPoint.y = long(OldTLPoint.y * m_Multiple_height );   
  
    //计算控件右下角点   
    CPoint OldBRPoint,BRPoint; 
	OldBRPoint = Rect.BottomRight();   
    BRPoint.x = long(OldBRPoint.x *m_Multiple_width);   
    BRPoint.y = long(OldBRPoint.y *m_Multiple_width);   
    //移动控件到新矩形   
    Rect.SetRect(TLPoint,BRPoint);   
    pWnd->MoveWindow(&Rect,TRUE);  
}
void CMKtoolDlg::ReSizeOnlyXY(int nID,LONG leftw,LONG lefth,LONG rightw,LONG righth) 
{
	CRect Rect;   
	CWnd *pWnd = ((CWnd*)GetDlgItem(nID));
	if (pWnd == NULL)
	{
		return ;
	}
    pWnd->GetWindowRect(&Rect);   
    ScreenToClient(&Rect);   
    //计算控件左上角点   
    CPoint OldTLPoint,TLPoint;   
    OldTLPoint = Rect.TopLeft();   
    TLPoint.x = long(OldTLPoint.x+leftw); 
    TLPoint.y = long(OldTLPoint.y+lefth); 
    //计算控件右下角点   
    CPoint OldBRPoint,BRPoint; OldBRPoint = Rect.BottomRight();   
    BRPoint.x = long(OldBRPoint.x  +rightw);   
    BRPoint.y = long(OldBRPoint.y +righth );   
    //移动控件到新矩形   
    Rect.SetRect(TLPoint,BRPoint);   
    pWnd->MoveWindow(&Rect,TRUE);  
}
