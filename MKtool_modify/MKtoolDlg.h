// MKtoolDlg.h : header file
//
//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES

#if !defined(AFX_MKTOOLDLG_H__8740C7E1_277B_45B5_A6A2_E13F3BEEEBCE__INCLUDED_)
#define AFX_MKTOOLDLG_H__8740C7E1_277B_45B5_A6A2_E13F3BEEEBCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "protocol_frm.h"
#include "protocol_info.h"
#include "MeterDialog.h"
#include "DialogDataIn.h"
/////////////////////////////////////////////////////////////////////////////
// CMKtoolDlg dialog
#define MAX_COM_RECV_SIZE 2048
#include "resource.h"
#define WM_COUNT_MSG WM_USER+100

class CMKtoolDlg : public CDialog
{
// Construction
public:
	CMKtoolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMKtoolDlg)
	enum { IDD = IDD_MKTOOL_DIALOG };
	CEdit	m_SendData;
	CComboBox	m_serial_check;
	CComboBox	m_serial_baudrate;
	CComboBox	m_serial_stop;
	CComboBox	m_serial_data;
	CComboBox	m_comPort;
	CMSComm	m_ctrlCOMM;
	CEdit	m_RxData;
	int		m_RecvDataCount;
	int		m_RecvFrameCount;
	int		m_SendDataCount;
	int		m_SendFrameCount;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMKtoolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	public:
		HANDLE hWakeupEvent;

		BOOL isAutoBack; // 是否支持自动回复
		int  nSupportProtocol; // 支持的的协议
		BOOL isSaveDateToFile; // 是否保存协议
		BOOL isStopShow;

		BOOL isHex;

		int nPacketTime; // 设置分包时间
		
		CString sPath; // 保存路径
		CString m_strTime;

		CFile RecordFile;

		BYTE bRxCompletionFlag; // 是否接收完成一包数据
		BYTE nSendBuf[256];
		
		CMeterDialog *pMeterDlg;
		CDialogDataIn *pDataInDlg;
		CWinThread* pThread;

		  
		BOOL change_flag;  
		float m_Multiple_height;  
		float m_Multiple_width;

		LONG lWidth;
		LONG lHeight;
		
		void RefreshEventAddr(MeterArray *PmeterArray);

		void RefreshComPortList();
		void WriteLogToFile(CString strLog);
		CString byteToHexStr(BYTE *pData, int nRecvLen);
		int CStrToHex(CString str, BYTE byte[]);
		void OutputLog(LPCSTR strLog,CEdit *pEdit,BOOL isOut);
		int CommSend(LPCSTR strSend);
		int CommSendHex(BYTE buf[],int nlen);
		BYTE charToHex(char ch);
		BYTE strToHex(char str[],BYTE byte[]);
		int CstringToHex(CString cstr,BYTE byte[]);
		CString RxParaData(BYTE buf[],int nlen);
		int IntToBcd(LONG64 data,BYTE bcd[],BYTE *len);
		void CreateThread();
		static UINT ThreadFun(LPVOID pParam);
		void showMeterInfo(MeterArray *PmeterArray);
		void OperateMeterInfo(MeterArray *PmeterArray,int op); // 1 增加，2删除

		void ReSizeMultiple(int nID);
		void ReSizeOnlyXY(int nID,LONG leftw,LONG lefth,LONG rightw,LONG righth) ;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMKtoolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOnCommMscomm();
	afx_msg void OnSwitchSerial();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBClear();
	afx_msg void OnBTest();
	afx_msg void OnSelchangeCEventType();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnCHECKautoback();
	afx_msg void OnCheck645p();
	afx_msg void OnCheck645_97p();
	afx_msg void OnCheck188p();
	afx_msg void OnCheck698p();
	afx_msg void OnCHECKStopShow();
	afx_msg void OnBUTTONaddAddr();
	afx_msg void OnBUTTONshowMeterInfo();
	afx_msg void OnBUTTONAllDelAddr();
	afx_msg void OnBUTTONDelAddr();
	afx_msg void OnCHECKsaveFile();
	afx_msg void OnEditchangeCeventAddr();
	afx_msg void OnBUTTONsetPacketTime();
	afx_msg void OnBUTTONsetMeterInfo();
	afx_msg void OnBUTTONsetDataIn();
	afx_msg void OnBUTTONsend();
	afx_msg void OnCHECKHex();
	afx_msg void OnBUTTONsetEvent();
	afx_msg void OnBUTTONDelEvent();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnCountMsg(WPARAM,LPARAM);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MKTOOLDLG_H__8740C7E1_277B_45B5_A6A2_E13F3BEEEBCE__INCLUDED_)
