#if !defined(AFX_DIALOGDATAIN_H__BF730391_FE0E_4E53_96A7_218FA37D54FE__INCLUDED_)
#define AFX_DIALOGDATAIN_H__BF730391_FE0E_4E53_96A7_218FA37D54FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogDataIn.h : header file
//
#include "EditListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogDataIn dialog
#include "resource.h"
class CDialogDataIn : public CDialog
{
// Construction
public:
	CDialogDataIn(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogDataIn)
	enum { IDD = IDD_DIALOG_DI };
	CEditListCtrl	m_listDi;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogDataIn)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogDataIn)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListDi(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListDi(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnBUTTONdelALL();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGDATAIN_H__BF730391_FE0E_4E53_96A7_218FA37D54FE__INCLUDED_)
