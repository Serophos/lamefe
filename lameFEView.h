/*
** Copyright (C) 2002-2003 Thees Winkler
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#if !defined(AFX_LAMEFEVIEW_H__BC39AFAD_87BC_42DD_8658_4B3170582855__INCLUDED_)
#define AFX_LAMEFEVIEW_H__BC39AFAD_87BC_42DD_8658_4B3170582855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include "ETSLayout.h"
//using namespace ETSLayout;

#include "MultimediaFile.h"
#include "CompactDisk.h"
#include "LameFEPlugin.h"
#include "WinampPlugin.h"
#include "OutPlugin.h"
#include "ExtListCtrl.h"
#include "AlbumInfoCtrl.h"
#include "MyComboBox.h"
#include "EncodingStatusDlg.h"
#include "NewPresetBar.h"

typedef CArray<CPlugin, CPlugin>  CPluginArray;
typedef CArray<CMultimediaFile*, CMultimediaFile*>	CMMFArray;

class CLameFEView : public CFormView
{
protected: // Nur aus Serialisierung erzeugen
	CLameFEView();
	DECLARE_DYNCREATE(CLameFEView)

// Attribute
public:
	CLameFEDoc* GetDocument();

// Operationen
public:

	CAlbumInfoCtrl m_aAlbumInfoCtrl;
	CExtListCtrl	m_ctrlList;
	CMyComboBox	c_inputDevice;
	CMyComboBox	c_outputDevice;
	CButton		c_configureOut;
	CButton		c_configureIn;
	CString		m_strInputDevice;
	CString		m_strOutputDevice;
	CImageList	m_cImageList;
	CString		m_strPresetName;
	enum { IDD = IDD_TRACKLIST };
	// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CLameFEView)
	public:
	virtual void OnDraw(CDC* pDC);  // überladen zum Zeichnen dieser Ansicht
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementierung
public:
	void SaveSettings();
	void OnFileStartBatchSingle();
	int m_nSelItem;
	BOOL IsPluginMode();
	void SetAlbumInfo();
	void ResizeControls();
	BOOL ShutDown();
	void ResetFileList();
	BOOL GetInputDevices();
	BOOL GetOutputDevices();
	virtual ~CLameFEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CString m_strPresetPath;
	void LoadSettings();
	void InitProfileBar();
	void OnSelchangeDevices(BOOL bReset = TRUE);
	BOOL			  isEjected;
	CString			  wd;
	HICON			  m_hIcon;



// Generierte Message-Map-Funktionen
protected:
	BOOL AddDirectory(CString strPath, CString strExt);
	afx_msg void OnAlbumInfoUpdated(WPARAM wParam,LPARAM lParam);
	afx_msg void OnPresetSelChanged(WPARAM nSelection,LPARAM lParam);
	//{{AFX_MSG(CLameFEView)
	afx_msg void OnConfigureIn();
	afx_msg void OnConfigureOut();
	afx_msg void OnSelchangeDevices();
	afx_msg void OnId3tagsReadcdplayerini();
	afx_msg void OnId3tagsReadcdtext();
	afx_msg void OnId3tagsReadfreedbserver();
	afx_msg void OnId3tagsSavetocdplayerini();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFileOpen();
	afx_msg void OnRemoveFile();
	afx_msg void OnFileStartencoding();
	afx_msg void OnAppExit();
	afx_msg void OnFileStartAlbum();
	afx_msg void OnViewReloadcd();
	afx_msg void OnViewSelectalltracksfiles();
	afx_msg void OnId3tagsSavecuesheet();
	afx_msg void OnUpdateToolBar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCDControls(CCmdUI* pCmdUI);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
	afx_msg void OnClickFilesTracks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnId3tagsId3tageditor();
	afx_msg void OnCheckfornewvesion();
	afx_msg void OnSettings();
	afx_msg void OnHelpReportabug();
	afx_msg void OnSelchangeOutputDevice();
	afx_msg void OnSavepreset();
	afx_msg void OnDeletepreset();
	afx_msg void OnDateiVerzeichnishinzufgen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	BOOL m_bCheckCD;
	BOOL m_bTagEditorVisible;
	void ShowSettingsDialog(int nTab);
	void OnFileStartBatchAlbum();
	void StartEncoding(modes m_mEMode);
	void RefreshTrackList();
	void ReadCDContents();
	int nNumCDDrives;

	CMutex			m_mtLock;
	BOOL			m_bPaused;
	CMMFArray		m_mmFiles;
	CCompactDisc	m_compactDisc;
	CPluginArray	m_paPlugins;
	CStatusBar      *m_pStatus;
	CToolBar		*m_pToolBar;
	CBitmap			m_listBkImage;
	CToolTipCtrl*   m_pToolTip;
	CNewPresetBar*		m_wndPresetBar;	

};

#ifndef _DEBUG  // Testversion in lameFEView.cpp
inline CLameFEDoc* CLameFEView::GetDocument()
   { return (CLameFEDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_LAMEFEVIEW_H__BC39AFAD_87BC_42DD_8658_4B3170582855__INCLUDED_)
