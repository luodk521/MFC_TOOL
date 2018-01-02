// MKtool.h : main header file for the MKTOOL application
//

#if !defined(AFX_MKTOOL_H__12DDAAEA_7554_4DB0_A925_B0B52F4F4CE9__INCLUDED_)
#define AFX_MKTOOL_H__12DDAAEA_7554_4DB0_A925_B0B52F4F4CE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMKtoolApp:
// See MKtool.cpp for the implementation of this class
//

class CMKtoolApp : public CWinApp
{
public:
	CMKtoolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMKtoolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMKtoolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MKTOOL_H__12DDAAEA_7554_4DB0_A925_B0B52F4F4CE9__INCLUDED_)
