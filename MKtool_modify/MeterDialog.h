#if !defined(AFX_METERDIALOG_H__47658053_ED40_4BF6_8F1F_FFA634CDCCE2__INCLUDED_)
#define AFX_METERDIALOG_H__47658053_ED40_4BF6_8F1F_FFA634CDCCE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeterDialog.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CMeterDialog dialog

class CMeterDialog : public CDialog
{
// Construction
public:
	CMeterDialog(CWnd* pParent = NULL);   // standard constructor
	void showMeterInfo(void);
// Dialog Data
	//{{AFX_DATA(CMeterDialog)
	enum { IDD = IDD_METER_DIALOG };
	CListCtrl	m_listCtrlMeter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeterDialog)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMeterDialog)
	afx_msg void OnOKSaveMeter();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONaddMeter();
	afx_msg void OnSelectdel();
	afx_msg void OnALLdel();
	afx_msg void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_METERDIALOG_H__47658053_ED40_4BF6_8F1F_FFA634CDCCE2__INCLUDED_)
