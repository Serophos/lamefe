#if !defined(AFX_FILENAMEPAGE_H__26ED3C6F_E975_42AB_B0F8_1828B930E177__INCLUDED_)
#define AFX_FILENAMEPAGE_H__26ED3C6F_E975_42AB_B0F8_1828B930E177__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilenamePage.h : Header-Datei
//

#include "MyEditCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CFilenamePage 

class CFilenamePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFilenamePage)

// Konstruktion
public:
	virtual BOOL OnInitDialog();
	void CreatePreview(CString & in, CString & out);
	void Init(CString strWd);
	CFilenamePage();
	~CFilenamePage();

// Dialogfelddaten
	//{{AFX_DATA(CFilenamePage)
	enum { IDD = IDD_FILENAMES };
	CMyEditCtrl	m_ctrlAlbum;
	CMyEditCtrl	m_ctrlPlaylist;
	CMyEditCtrl	m_ctrlFilename;
	CStatic m_ctrlPath;
	CString	m_strAlbumMode;
	CString	m_strAlbumPrev;
	CString	m_strFilenamePrev;
	CString	m_strFilename;
	CString	m_strPlaylist;
	CString	m_strPlaylistPrev;
	CString m_strOutputPath;
	BOOL	m_bRename;
	//}}AFX_DATA


// Überschreibungen
	// Der Klassen-Assistent generiert virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CFilenamePage)
	public:
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	void OnPath();
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CFilenamePage)
	afx_msg void OnRename();
	afx_msg void OnChangeAlbumModeFilename();
	afx_msg void OnChangeFormat();
	afx_msg void OnChangePlaylistFilename();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_strWd;
	CToolTipCtrl* m_pToolTip;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_FILENAMEPAGE_H__26ED3C6F_E975_42AB_B0F8_1828B930E177__INCLUDED_
