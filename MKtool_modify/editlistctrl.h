#if !defined(AFX_EDITLISTCTRL_H__2DB82817_9A6F_432C_BD76_6A1810C355DA__INCLUDED_)
#define AFX_EDITLISTCTRL_H__2DB82817_9A6F_432C_BD76_6A1810C355DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditListCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CItemEdit window
#define WM_USER_EDIT_END  WM_USER+1001

class CItemEdit : public CEdit
{
	// Construction
public:
	CItemEdit();
	
	// Attributes
public:
	DWORD GetCtrlData();
	void SetCtrlData(DWORD dwData);
	// Operations
public:
	BOOL m_bExchange;//是否进行数据交换
	 DWORD m_dwData;//待编辑区域行列号信息
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CItemEdit();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CItemEdit)
	afx_msg void OnSetfocus();
	afx_msg void OnKillfocus();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CEditListCtrl window

class CEditListCtrl : public CListCtrl
{
// Construction
public:
	CEditListCtrl();
	 void ShowEdit(BOOL bShow,int nItem,int nIndex,CRect rc = CRect(0,0,0,0));
// Attributes
public:
	CItemEdit m_edit;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditListCtrl();
	afx_msg LRESULT OnEditEnd(WPARAM wParam,LPARAM lParam = FALSE);
	// Generated message map functions
protected:
	//{{AFX_MSG(CEditListCtrl)
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITLISTCTRL_H__2DB82817_9A6F_432C_BD76_6A1810C355DA__INCLUDED_)
