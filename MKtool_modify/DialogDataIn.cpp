// DialogDataIn.cpp : implementation file
//

#include "stdafx.h"
#include "MKtool.h"
#include "DialogDataIn.h"
#include "protocol_info.h"
#include "tool_common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDataIn dialog


CDialogDataIn::CDialogDataIn(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDataIn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDataIn)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogDataIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDataIn)
	DDX_Control(pDX, IDC_LIST_DI, m_listDi);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDataIn, CDialog)
	//{{AFX_MSG_MAP(CDialogDataIn)
	ON_BN_CLICKED(IDC_BUTTON_delALL, OnBUTTONdelALL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDataIn message handlers

BOOL CDialogDataIn::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int i,j;
	CString strlog;

	strlog.Format("%x",get64507SmDI());
	GetDlgItem(IDC_EDIT_64507_SM_DI)->SetWindowText(strlog);
	strlog.Format("%x",get64597SmDI());
	GetDlgItem(IDC_EDIT_64597_SM_DI)->SetWindowText(strlog);

	DWORD dwStyle = m_listDi.GetExtendedStyle();                    //添加列表框的网格线！！！
	
	dwStyle |=LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_UNDERLINEHOT;
    m_listDi.SetExtendedStyle(dwStyle);

	m_listDi.InsertColumn(0,"有效性",LVCFMT_LEFT,60);              //添加列标题LVCFMT_LEFT用来设置对齐方式
	m_listDi.InsertColumn(1,"数据标识",LVCFMT_LEFT,100);
    m_listDi.InsertColumn(2,"长度",LVCFMT_LEFT,60);
    m_listDi.InsertColumn(3,"回复数据",LVCFMT_LEFT,300);
	m_listDi.InsertColumn(4,"协议",LVCFMT_LEFT,60);
	m_listDi.InsertColumn(5,"命令",LVCFMT_LEFT,100);

	for (i=0;i<16;i++)
	{
		m_listDi.InsertItem(i,"");

	}

	ProDIData *nProDIData=getProDIData();
	CString strTemp,str1;
	for (i=0;i<nProDIData->nCount;i++)
	{
		strTemp.Format("%d",nProDIData->DI[i].isValid);
		m_listDi.SetItemText(i,0,strTemp);

		strTemp.Format("%x",nProDIData->DI[i].nDI);
		m_listDi.SetItemText(i,1,strTemp);

		strTemp.Format("%d",nProDIData->DI[i].len);
		m_listDi.SetItemText(i,2,strTemp);

		strTemp = "";
		for (j=0;j<nProDIData->DI[i].len;j++)
		{
			str1.Format("%02x",nProDIData->DI[i].DIData[j]);
			strTemp += str1;
		}
		m_listDi.SetItemText(i,3,strTemp);

		strTemp.Format("%d",nProDIData->DI[i].nPro);
		m_listDi.SetItemText(i,4,strTemp);

		strTemp.Format("%d",nProDIData->DI[i].nFunID);
		m_listDi.SetItemText(i,5,strTemp);
	}
	 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogDataIn::OnOK() 
{
	// TODO: Add extra validation here
	char buf[MAX_LEN];
	BYTE bufHex[8];
	int i=0,di;
	int len ;//= m_listDi.GetItemText(i,0,buf,MAX_LEN);
	ProDIData *nProDIData=getProDIData();
	nProDIData->nCount = 0;
	int index;
//	int nTemp;

	while (len = m_listDi.GetItemText(i,1,buf,MAX_LEN))
	{
		memset(bufHex,0,8);
		strToHex(buf,bufHex);
		di = 0;
		for (index = 0; index < 4; index++)
		{
			di <<= 8;
			di |= bufHex[index];
		}
		nProDIData->DI[i].nDI = di;

		m_listDi.GetItemText(i,0,buf,MAX_LEN);
		nProDIData->DI[i].isValid = atoi(buf);

		m_listDi.GetItemText(i,2,buf,MAX_LEN);
		nProDIData->DI[i].len= atoi(buf);

		m_listDi.GetItemText(i,3,buf,MAX_LEN);
		strToHex(buf,nProDIData->DI[i].DIData);

		m_listDi.GetItemText(i,4,buf,MAX_LEN);
		nProDIData->DI[i].nPro= atoi(buf);

		m_listDi.GetItemText(i,5,buf,MAX_LEN);
		nProDIData->DI[i].nFunID = atoi(buf);
		
		nProDIData->nCount++;
		if (nProDIData->nCount >PROTOCOL_DI_COUNT)
		{
			break;
		}
		i++;
	}
	GetDlgItem(IDC_EDIT_64507_SM_DI)->GetWindowText(buf,MAX_LEN);
	memset(bufHex,0,8);
	//ByteToHex(bufHex,strToHex(buf,bufHex));
	len = strToHex(buf,bufHex);
	set64507SmDI(ByteToHex(bufHex,len));


	GetDlgItem(IDC_EDIT_64597_SM_DI)->GetWindowText(buf,MAX_LEN);
	memset(bufHex,0,8);
	len = strToHex(buf,bufHex);
	set64597SmDI(ByteToHex(bufHex,len));

	CDialog::OnOK();
}

void CDialogDataIn::OnBUTTONdelALL() 
{
	m_listDi.DeleteAllItems();
}
