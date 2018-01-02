// MeterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MKtool.h"
#include "MeterDialog.h"
#include "protocol_info.h"
#include "tool_common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeterDialog dialog


CMeterDialog::CMeterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMeterDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeterDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMeterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeterDialog)
	DDX_Control(pDX, IDC_LIST_METER, m_listCtrlMeter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeterDialog, CDialog)
	//{{AFX_MSG_MAP(CMeterDialog)
	ON_BN_CLICKED(IDOK, OnOKSaveMeter)
	ON_BN_CLICKED(IDC_BUTTON_add, OnBUTTONaddMeter)
	ON_BN_CLICKED(IDC_Selectdel, OnSelectdel)
	ON_BN_CLICKED(IDC_ALLdel, OnALLdel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL,OnCancel)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeterDialog message handlers

void CMeterDialog::OnOKSaveMeter() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}
const CString strProtocol[8]=
{
	"645-07协议",
	"645-97协议",
	"188协议",
	"698协议",
};
BOOL CMeterDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwStyle = m_listCtrlMeter.GetExtendedStyle();                    //添加列表框的网格线！！！
	
    dwStyle |=LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_UNDERLINEHOT|LVS_EX_CHECKBOXES;               
    m_listCtrlMeter.SetExtendedStyle(dwStyle);
	
    m_listCtrlMeter.InsertColumn(0,"序号",LVCFMT_LEFT,60);              //添加列标题！！！！  这里的80,40,90用以设置列的宽度。！！！LVCFMT_LEFT用来设置对齐方式！！！
    m_listCtrlMeter.InsertColumn(1,"电表表号",LVCFMT_LEFT,160);
    m_listCtrlMeter.InsertColumn(2,"电表协议",LVCFMT_LEFT,180);
    m_listCtrlMeter.InsertColumn(3,"备注",LVCFMT_LEFT,200);


	((CButton *)GetDlgItem(IDC_CHECK_645_07))->SetCheck(TRUE);//645-07选上

	((CEdit*)GetDlgItem(IDC_EDIT_AddMeterAddr))->SetWindowText("010000000001");
	((CEdit*)GetDlgItem(IDC_EDIT_AddMeterNum))->SetWindowText("1");
	((CEdit*)GetDlgItem(IDC_EDIT_AddStep))->SetWindowText("1");

	MeterArray *PmeterArray=getMeterArray();
	int i,j;
	CString strTemp,strAddr,strPro="";
	for (i=0;i<PmeterArray->numMeter;i++)
	{
		m_listCtrlMeter.InsertItem(i, ""); 
		strTemp.Format("%d",i+1);
		m_listCtrlMeter.SetItemText(i,0,strTemp);

		strAddr = "";
		for (j=PmeterArray->sMeter[i].nAddrLen-1;j>=0;j--)
		{
			strTemp.Format("%02x",PmeterArray->sMeter[i].address[j]);
			strAddr += strTemp;
		}
		m_listCtrlMeter.SetItemText(i,1,strAddr);
		
		for (j = 0;j<4;j++)
		{
			if (PmeterArray->sMeter[i].protocol&(0x01<<j))
			{
				strTemp.Format("%s ",strProtocol[j]);
				strPro += strTemp;
			}
		}
		m_listCtrlMeter.SetItemText(i,2,strPro);
		strTemp.Format("状态:%d 事件1-4:%d,%d,%d,%d",PmeterArray->sMeter[i].IsSupportEventState,PmeterArray->sMeter[i].nEvent[0],PmeterArray->sMeter[i].nEvent[1],
			PmeterArray->sMeter[i].nEvent[2],PmeterArray->sMeter[i].nEvent[3]);
		m_listCtrlMeter.SetItemText(i,3,strTemp);
	}

	m_listCtrlMeter.Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#define MAX_LEN_ADD_METER 32
void CMeterDialog::OnBUTTONaddMeter() 
{
	// TODO: Add your control notification handler code here
	int nAddProtocol =0;
	char strTemp[MAX_LEN_ADD_METER];
	char strAddr[MAX_LEN_ADD_METER];
	CString strlog,CstrTemp;
	int nCount =0 ;
	int nStep = 0;
	int i,j,k,isCommAddr=0;
	BYTE nSupportEventState = 0;
	Meter MeterTemp;
	memset(&MeterTemp,0,sizeof(Meter));

	if (((CButton *)GetDlgItem(IDC_CHECK_645_07))->GetCheck())
	{
		nAddProtocol |= SUPPORT_PROTOCOL_645_07;
	}
	if (((CButton *)GetDlgItem(IDC_CHECK_645_97))->GetCheck())
	{
		nAddProtocol |= SUPPORT_PROTOCOL_645_97;
	}
	if (((CButton *)GetDlgItem(IDC_CHECK_188))->GetCheck())
	{
		nAddProtocol |= SUPPORT_PROTOCOL_188;
	}
	if (((CButton *)GetDlgItem(IDC_CHECK_698))->GetCheck())
	{
		nAddProtocol |= SUPPORT_PROTOCOL_698;
	}
	if (((CButton *)GetDlgItem(IDC_CHECK_eventState))->GetCheck())
	{
		nSupportEventState = 1;
	}

	((CEdit*)GetDlgItem(IDC_EDIT_AddMeterAddr))->GetWindowText(strAddr,MAX_LEN_ADD_METER);

	((CEdit*)GetDlgItem(IDC_EDIT_AddMeterNum))->GetWindowText(strTemp,MAX_LEN_ADD_METER);
	nCount = atoi(strTemp);

	((CEdit*)GetDlgItem(IDC_EDIT_AddStep))->GetWindowText(strTemp,MAX_LEN_ADD_METER);
	nStep = atoi(strTemp);

	MeterArray *PmeterArray=getMeterArray();

	if (strlen(strAddr)%2)
	{
		strlog.Format("地址长度请填偶数位");
		MessageBox(strlog);
		return ;
	}

	MeterTemp.nAddrLen = strToHex(strAddr,MeterTemp.address);
	MeterTemp.protocol = nAddProtocol;
	reverse(MeterTemp.address,MeterTemp.nAddrLen);
	int naddr;
	for (k=0;k<nCount;k++)
	{
		if (PmeterArray->numMeter >MAX_METER_COUNT)
		{
			break;
		}
		naddr =1000*((MeterTemp.address[1])/16)+100*(MeterTemp.address[1]%16)+10*((MeterTemp.address[0])/16)+MeterTemp.address[0]%16;
		
		if (k!=0)
		{
			naddr +=nStep;
		}

		if (!(((CButton *)GetDlgItem(IDC_CHECK_BCD))->GetCheck()))
		{
			IntToBcd(naddr,MeterTemp.address,2);
		}
		
		
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
				// 相同表地址
				isCommAddr = 1;
				break;
			}
		}
		if (isCommAddr==1)
		{
			isCommAddr = 0;
			continue;
		}

		MeterTemp.nValid =1;
		MeterTemp.IsSupportEventState = nSupportEventState;
		memcpy(&(PmeterArray->sMeter[PmeterArray->numMeter]),&MeterTemp,sizeof(Meter));

/* 显示
		m_listCtrlMeter.InsertItem(PmeterArray->numMeter, ""); 
		CstrTemp.Format("%d",PmeterArray->numMeter+1);
		m_listCtrlMeter.SetItemText(PmeterArray->numMeter,0,CstrTemp);

		strlog = "";
		for (i=MeterTemp.nAddrLen-1;i>=0;i--)
		{
			CstrTemp.Format("%02x",MeterTemp.address[i]);
			strlog += CstrTemp;
		}

		m_listCtrlMeter.SetItemText(PmeterArray->numMeter,1,strlog);

		strlog = "";
		for (j = 0;j<4;j++)
		{
			if (PmeterArray->sMeter[i].protocol&(0x01<<j))
			{
				CstrTemp.Format("%s ",strProtocol[j]);
				strlog += CstrTemp;
			}
		}
		m_listCtrlMeter.SetItemText(PmeterArray->numMeter,2,strlog);

		CstrTemp.Format("状态:%d 事件1-4:%d,%d,%d,%d",PmeterArray->sMeter[PmeterArray->numMeter].IsSupportEventState,PmeterArray->sMeter[PmeterArray->numMeter].nEvent[0],PmeterArray->sMeter[PmeterArray->numMeter].nEvent[1],
			PmeterArray->sMeter[PmeterArray->numMeter].nEvent[2],PmeterArray->sMeter[PmeterArray->numMeter].nEvent[3]);
		m_listCtrlMeter.SetItemText(i,3,CstrTemp);
*/
		PmeterArray->numMeter++;
	}
	showMeterInfo();
}



void CMeterDialog::OnSelectdel() 
{
	int i;
	MeterArray *PmeterArray=getMeterArray();
	for (i =0;i<PmeterArray->numMeter;i++)
	{
		if (m_listCtrlMeter.GetCheck(i))
		{
			PmeterArray->sMeter[i].nValid = 0;
		}
	}
	RefreshMeter();
	showMeterInfo();
}
void CMeterDialog::showMeterInfo(void)
{
	CString strlog,strTemp,strAddr;
	int i,j;
	MeterArray *PmeterArray=getMeterArray();

	m_listCtrlMeter.DeleteAllItems();
	for (i=0;i<PmeterArray->numMeter;i++)
	{
		strAddr = "";
		m_listCtrlMeter.InsertItem(PmeterArray->numMeter, ""); 
		for (j=PmeterArray->sMeter[i].nAddrLen-1;j>=0;j--)
		{
			strTemp.Format("%02x",PmeterArray->sMeter[i].address[j]);
			strAddr += strTemp;
		}
		m_listCtrlMeter.SetItemText(i,1,strAddr);

		strTemp.Format("%d",i+1);
		m_listCtrlMeter.SetItemText(i,0,strTemp);

		strlog = "";
		for (j = 0;j<4;j++)
		{
			if (PmeterArray->sMeter[i].protocol&(0x01<<j))
			{
				strTemp.Format("%s ",strProtocol[j]);
				strlog += strTemp;
			}
		}
		
		m_listCtrlMeter.SetItemText(i,2,strlog);

		strTemp.Format("%s,事件1-4:%d,%d,%d,%d",PmeterArray->sMeter[i].IsSupportEventState>0?"支持":"不支持",PmeterArray->sMeter[i].nEvent[0],PmeterArray->sMeter[i].nEvent[1],
			PmeterArray->sMeter[i].nEvent[2],PmeterArray->sMeter[i].nEvent[3]);
		m_listCtrlMeter.SetItemText(i,3,strTemp);
	}
	m_listCtrlMeter.Invalidate();
}

void CMeterDialog::OnALLdel() 
{
	MeterArray *PmeterArray=getMeterArray();
	PmeterArray->numMeter = 0;
	showMeterInfo();
}

void CMeterDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnOK();
}



void CMeterDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CMeterDialog::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMeterDialog::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nID)
    {
    case SC_CLOSE:
        CDialog::OnOK();
        break;
    }
	CDialog::OnSysCommand(nID, lParam);
}
