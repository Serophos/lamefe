#if !defined(AFX_ALBUMINFOCTRL_H__9EE24ABE_A30E_4F31_AE38_2FFA3E6F5E71__INCLUDED_)
#define AFX_ALBUMINFOCTRL_H__9EE24ABE_A30E_4F31_AE38_2FFA3E6F5E71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlbumInfoCtrl.h : Header-Datei
//

#include "ID3Info.h"
//#include "MessageIDs.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CAlbumInfoCtrl 


class CAlbumInfoCtrl : public CDialog
{
// Konstruktion
public:
	void Clear();
	void EnableControls(BOOL bEnable);
	void SetInfo(CID3Info* info);
	CAlbumInfoCtrl(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CAlbumInfoCtrl)
	enum { IDD = IDD_ALBUMINFOCTRL };
	CComboBox	m_ctrlGenre;
	CString	m_strAlbum;
	CString	m_strComment;
	CString	m_strGenre;
	CString	m_strInterpret;
	CString	m_strSong;
	int		m_nTrack;
	int		m_nYear;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CAlbumInfoCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	virtual BOOL OnInitDialog();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CAlbumInfoCtrl)
	afx_msg void OnChangeAlbumname();
	afx_msg void OnChangeAlbumInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL	m_bDataChanged;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_ALBUMINFOCTRL_H__9EE24ABE_A30E_4F31_AE38_2FFA3E6F5E71__INCLUDED_
