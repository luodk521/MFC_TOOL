// EditListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MKtool.h"
#include "EditListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditListCtrl

CEditListCtrl::CEditListCtrl()
{
	m_edit.m_hWnd = NULL;
}
void CEditListCtrl::ShowEdit(BOOL bShow, int nItem, int nIndex, CRect rc)
{
	// 如果编辑框对象尚未创建
    if(m_edit.m_hWnd == NULL)
	{
		//创建一个编辑框（大小为零）
		m_edit.Create(ES_AUTOHSCROLL|WS_CHILD|ES_LEFT|ES_WANTRETURN|WS_BORDER,CRect(0,0,0,0),this,IDC_EDIT);
		m_edit.ShowWindow(SW_HIDE);// 隐藏
		//使用默认字体
		CFont tpFont;
		tpFont.CreateStockObject(DEFAULT_GUI_FONT);
		m_edit.SetFont(&tpFont);
		tpFont.DeleteObject();
	}
	//如果bShow为true，显示编辑框
	if(bShow == TRUE)
	{
		CString strItem = CListCtrl::GetItemText(nItem,nIndex);//获取列表控件中数据项的内容
		m_edit.MoveWindow(rc);// 移动到子项所在区域
		m_edit.ShowWindow(SW_SHOW);//显示控件
		m_edit.SetWindowText(strItem);// 显示数据
		::SetFocus(m_edit.GetSafeHwnd());//设置焦点
		//::SendMessage(m_edit.GetSafeHwnd(), EM_SETSEL, 0, -1);//使数据处于选择状态
		m_edit.SetSel(-1);
		m_edit.SetCtrlData(MAKEWPARAM(nIndex,nItem));
	}
	else
		m_edit.ShowWindow(SW_HIDE);
}
CEditListCtrl::~CEditListCtrl()
{
}


BEGIN_MESSAGE_MAP(CEditListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CEditListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_USER_EDIT_END,OnEditEnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditListCtrl message handlers
/////////////////////////////////////////////////////////////////////////////
// CItemEdit

CItemEdit::CItemEdit()
{
}

CItemEdit::~CItemEdit()
{
}


BEGIN_MESSAGE_MAP(CItemEdit, CEdit)
	//{{AFX_MSG_MAP(CItemEdit)
	ON_CONTROL_REFLECT(EN_SETFOCUS, OnSetfocus)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CItemEdit::SetCtrlData(DWORD dwData)
{
	m_dwData=dwData;
}
DWORD CItemEdit::GetCtrlData()
{
	return m_dwData;
}
/////////////////////////////////////////////////////////////////////////////
// CItemEdit message handlers

void CEditListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_edit.m_hWnd != NULL)
	{
		DWORD dwStyle = m_edit.GetStyle();
		if((dwStyle&WS_VISIBLE) == WS_VISIBLE)
		{
			m_edit.ShowWindow(SW_HIDE);
		}
	} 
	*pResult = 0;
}


void CEditListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rcCtrl;        //数据项所在区域
	LVHITTESTINFO lvhti; //用于列表控件子项鼠标点击测试的数据结构
	lvhti.pt = point;  //输入鼠标位置
	int nItem = CListCtrl::SubItemHitTest(&lvhti);//调用基类的子项测试函数，返回行号
	if(nItem == -1)   //如果鼠标在控件外双击，不做任何处理
		return;
	int nSubItem = lvhti.iSubItem;//获得列号
	CListCtrl::GetSubItemRect(nItem,nSubItem,LVIR_LABEL,rcCtrl);
	//获得子项所在区域，存入rcCtrl
    ShowEdit(TRUE,nItem,nSubItem,rcCtrl); //调用自定义函数，显示编辑框
	CListCtrl::OnLButtonDblClk(nFlags, point);//调用基类鼠标鼠标双击事件的响应函数
}

void CItemEdit::OnSetfocus() 
{
	// TODO: Add your control notification handler code here
	m_bExchange = TRUE; 
}

void CItemEdit::OnKillfocus() 
{
	// TODO: Add your control notification handler code here
	CWnd* pParent = this->GetParent();
	::PostMessage(pParent->GetSafeHwnd(),WM_USER_EDIT_END,m_bExchange,0);
}

LRESULT CEditListCtrl::OnEditEnd(WPARAM wParam, LPARAM lParam)
{
	if(wParam == TRUE)
	{
		CString strText(_T(""));
		m_edit.GetWindowText(strText);
		DWORD dwData = m_edit.GetCtrlData();
		int nItem= dwData>>16;
		int nIndex = dwData&0x0000ffff;
		CListCtrl::SetItemText(nItem,nIndex,strText);
	}
	else
	{   
	}
    if(lParam == FALSE)
		m_edit.ShowWindow(SW_HIDE);
	return 0;
}

BOOL CItemEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			CWnd* pParent = this->GetParent();
			m_bExchange = TRUE;
			::PostMessage(pParent->GetSafeHwnd(),WM_USER_EDIT_END,m_bExchange,0);
			return true;
		}
		else if(pMsg->wParam == VK_ESCAPE)
		{
			CWnd* pParent = this->GetParent();
			m_bExchange = FALSE;
			::PostMessage(pParent->GetSafeHwnd(),WM_USER_EDIT_END,m_bExchange,0);
			return true;
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}
