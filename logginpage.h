#if !defined(AFX_LOGGINPAGE_H__7FBCF66F_A6B9_4E17_BE11_0143A2F30CAE__INCLUDED_)
#define AFX_LOGGINPAGE_H__7FBCF66F_A6B9_4E17_BE11_0143A2F30CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// logginpage.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CLogginPage 

#include "resource.h"

class CLogginPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLogginPage)

// Konstruktion
public:
	virtual BOOL OnInitDialog();
	void Init(CString wd);
	CLogginPage();
	~CLogginPage();

// Dialogfelddaten
	//{{AFX_DATA(CLogginPage)
	enum { IDD = IDD_LOGGING_PAGE };
	CEdit	m_startSession;
	CEdit	m_logEntry;
	CEdit	m_endSession;
	CString	m_strEndSession;
	CString	m_strLogEntry;
	CString	m_strStartSession;
	CButton m_oldLogStyle;
	CButton m_newLogStyle;
	//}}AFX_DATA


// Überschreibungen
	// Der Klassen-Assistent generiert virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CLogginPage)
	public:
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CLogginPage)
	afx_msg void OnRealTimeLog();
	afx_msg void OnReportStyle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CToolTipCtrl* m_pToolTip;

private:
	CString m_strWd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_LOGGINPAGE_H__7FBCF66F_A6B9_4E17_BE11_0143A2F30CAE__INCLUDED_
