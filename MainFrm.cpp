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

#include "stdafx.h"
#include "lameFE.h"
#include "Settings.h"
#include "MainFrm.h"
#include "Utils.h"
#include "I18n.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "version")

extern CSettings g_sSettings;
extern CI18n	g_iLang;

#ifndef _countof
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_HELP_HELPINDEX, OnHelpHelpindex)
	ON_COMMAND(ID_HELP_LICENSE, OnHelpLicense)
	ON_COMMAND(ID_VIEW_SHOWTOOLBAR, OnViewShowtoolbar)
	ON_COMMAND(ID_VIEW_SHOWSTATUSLINE, OnViewShowstatusline)
	ON_COMMAND(ID_VIEW_SHOWPRESETBAR, OnViewShowPresets)
	ON_WM_MEASUREITEM()
	ON_WM_MENUCHAR()
	ON_WM_INITMENUPOPUP()
	//}}AFX_MSG_MAP

	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnI18nToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnI18nToolTipText)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,           // status line indicator
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Konstruktion/Zerstörung

CMainFrame::CMainFrame()
{

	m_pLangMenu = NULL;
}

CMainFrame::~CMainFrame()
{

	if(m_pLangMenu != NULL){

		delete m_pLangMenu;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{

		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndCtrlBar.CreateEx(this, TBSTYLE_FLAT) ||
		!m_wndCtrlBar.LoadToolBar(IDR_CONTROLBAR))
	{

		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	/*	if (!m_wndToolBar.Create(this,WS_CHILD|WS_VISIBLE|CBRS_TOP,AFX_IDW_TOOLBAR) || 
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{

		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndCtrlBar.Create(this,WS_CHILD|WS_VISIBLE|CBRS_TOP,AFX_IDW_TOOLBAR) || 
		!m_wndCtrlBar.LoadToolBar(IDR_CONTROLBAR))
	{

		TRACE0("Failed to create IDR_CONTROLBAR\n");
		return -1;      // fail to create
	}*/

	if(!m_wndPresetBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | TBSTYLE_FLAT, ID_PRESETBAR))
	{

		TRACE0("Couldn't create presetbar\n");
	}


	// set high color icons if possible
	if ((::GetDeviceCaps(GetDC()->m_hDC,BITSPIXEL) > 8) && g_sSettings.GetUseHighColBar()) 
	{
	
		CImageList	imageList;
		CBitmap		bitmap;

		// Set up toolbar image lists.
		// Create and set the normal toolbar image list.
		bitmap.LoadMappedBitmap( IDB_TOOLBAR_HI );
		imageList.Create(48, 48, ILC_COLORDDB|ILC_MASK, 4, 1);
		imageList.Add(&bitmap, RGB(3,3,2));
		m_wndToolBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
		imageList.Detach();
		bitmap.Detach();

		// Create and set the disabled toolbar image list.
		bitmap.LoadMappedBitmap( IDB_TOOLBAR_HI_DEACT );
		imageList.Create(48, 48, ILC_COLORDDB|ILC_MASK, 4, 1);
		imageList.Add(&bitmap, RGB(0,0,0));
		m_wndToolBar.SendMessage( TB_SETDISABLEDIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
		imageList.Detach();
		bitmap.Detach();

		// Set up toolbar image lists.
		// Create and set the normal toolbar image list.
		bitmap.LoadMappedBitmap( IDB_TOOLBAR_2_HI );
		imageList.Create(32, 16, ILC_COLORDDB|ILC_MASK, 4, 1);
		imageList.Add(&bitmap, RGB(255,0,255));
		m_wndCtrlBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
		imageList.Detach();
		bitmap.Detach();

		// Create and set the disabled toolbar image list.
		bitmap.LoadMappedBitmap( IDB_TOOLBAR_2_HI);
		imageList.Create(32, 16, ILC_COLORDDB|ILC_MASK, 4, 1);
		imageList.Add(&bitmap, RGB(255,0,255));
		m_wndCtrlBar.SendMessage( TB_SETDISABLEDIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
		imageList.Detach();
		bitmap.Detach();
	}
	
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	m_wndCtrlBar.SetBarStyle(m_wndCtrlBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndPresetBar.SetBarStyle(m_wndPresetBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// Allow docking everywhere
	EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar, AFX_IDW_DOCKBAR_LEFT);


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Statusleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}

	m_wndStatusBar.SetPaneInfo(0,ID_SEPARATOR,SBPS_STRETCH,190);
    m_wndStatusBar.SetPaneInfo(1,ID_SEPARATOR,SBPS_NORMAL,140);
	m_wndStatusBar.SetPaneInfo(2,ID_SEPARATOR,SBPS_NOBORDERS ,50);
	m_wndStatusBar.SetPaneInfo(3,ID_SEPARATOR,SBPS_NORMAL,45);
	
	InitLanguages();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	if(g_sSettings.GetSaveWinPos()){

		cs.x	= g_sSettings.GetWindowX();
		cs.y	= g_sSettings.GetWindowY();
		cs.cx	= g_sSettings.GetWindowCX();
		cs.cy	= g_sSettings.GetWindowCY();
	}
	else{

		cs.cx = 665;
		cs.cy = 520;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Diagnose

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Nachrichten-Handler



void CMainFrame::OnHelp() 
{
	
	HtmlHelp(
		0,
        "lameFE.chm",
        HH_DISPLAY_TOC,
        NULL) ;
}

void CMainFrame::OnHelpHelpindex() 
{

	HtmlHelp(
		0,
        "lameFE.chm",
        HH_DISPLAY_INDEX,
        NULL) ;
}

void CMainFrame::OnHelpLicense() 
{

	ShellExecute(GetSafeHwnd(), 
					"open", 
					"license.txt",
					NULL,
					NULL,
					SW_SHOW
			);
}

void CMainFrame::OnViewShowtoolbar() 
{

	ShowControlBar(&m_wndToolBar, (m_wndToolBar.GetStyle() & WS_VISIBLE) == 0,FALSE);
}


void CMainFrame::OnViewShowstatusline() 
{

	ShowControlBar(&m_wndStatusBar, (m_wndStatusBar.GetStyle() & WS_VISIBLE) == 0,FALSE);
}

//DEL void CMainFrame::OnShowPlayer() 
//DEL {
//DEL 
//DEL 	ShowControlBar(&m_wndPlayerBar, (m_wndPlayerBar.GetStyle() & WS_VISIBLE) == 0,FALSE);
//DEL }

void CMainFrame::OnViewShowPresets()
{

//	ShowControlBar(&m_wndDlgBar, (m_wndDlgBar.GetStyle() & WS_VISIBLE) == 0, FALSE);
	ShowControlBar(&m_wndPresetBar, (m_wndPresetBar.GetStyle() & WS_VISIBLE) == 0, FALSE);
}

/*
HMENU CMainFrame::NewMenu()
{
	// Load the menu from the resources
	// ****replace IDR_MAINFRAME with your menu ID****
	m_menu.LoadMenu(IDR_MAINFRAME);  


	CImageList	menuImageList;
	CBitmap		bitmap;
	bitmap.LoadMappedBitmap( IDB_MENUITEMS );
	menuImageList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 7, 1);
	menuImageList.Add(&bitmap, RGB(255,0,255));
	
	m_menu.SetBitmapBackground(RGB(255,0,255));
	//BOOL BCMenu::ModifyODMenuW(wchar_t *lpstrText,UINT nID,CImageList *il,int xoffset)
	//
	m_menu.ModifyODMenu(NULL,ID_FILE_OPEN, &menuImageList, 0);
	m_menu.ModifyODMenu(NULL,ID_REMOVE_FILE, &menuImageList, 1);
	m_menu.ModifyODMenu(NULL,ID_ID3TAGS_READFREEDBSERVER, &menuImageList, 2);
	m_menu.ModifyODMenu(NULL,ID_ID3TAGS_SAVETOCDPLAYERINI, &menuImageList, 3);
	m_menu.ModifyODMenu(NULL,ID_CHECKFORNEWVESION, &menuImageList, 4);
	m_menu.ModifyODMenu(NULL,ID_SETTINGS, &menuImageList, 5);
	m_menu.ModifyODMenu(NULL,ID_HELP, &menuImageList, 6);

	menuImageList.Detach();
	bitmap.Detach();

	return(m_menu.Detach());
}


void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{

	BOOL setflag=FALSE;
	if(lpMeasureItemStruct->CtlType==ODT_MENU){

		if(IsMenu((HMENU)lpMeasureItemStruct->itemID)){
			CMenu* cmenu = 
			CMenu::FromHandle((HMENU)lpMeasureItemStruct->itemID);

			if(m_menu.IsMenu(cmenu)){
				m_menu.MeasureItem(lpMeasureItemStruct);
				setflag=TRUE;
			}
		}
	}

	if(!setflag){

		CFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
	}
}

LRESULT CMainFrame::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{

	LRESULT lresult;
	if(m_menu.IsMenu(pMenu))
		lresult=BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	else
		lresult=CFrameWnd::OnMenuChar(nChar, nFlags, pMenu);

	return lresult ;
}

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	if(!bSysMenu){

		if(m_menu.IsMenu(pPopupMenu)){

			BCMenu::UpdateMenu(pPopupMenu);
		}
	}

}

*/
void CMainFrame::InitLanguages()
{

	g_iLang.TranslateMenu(GetMenu(), IDR_MAINFRAME, TRUE);
//	ActivateFrame( SW_HIDE );
//	ActivateFrame( SW_SHOW );

}

BOOL CMainFrame::OnI18nToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;

	UINT nID = pNMHDR->idFrom;

	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ((UINT)(WORD)::GetDlgCtrlID( (HWND)nID ) );
	}

	if (nID != 0) // will be zero on a separator
	{
		// don't handle the message if no string resource found
		CString strLangTipText = g_iLang.GetString(nID);

		if(strLangTipText.IsEmpty())
		{
			return FALSE;
		}	

		// this is the command id, not the button index
		AfxExtractSubString(strTipText, strLangTipText, 1, '\n');
	}

#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText) );
	else
		_mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText) );
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#endif
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);

	return TRUE;    // message was handled
}

void CMainFrame::GetMessageString(UINT nID, CString& rMessage) const
{
	// load appropriate string
	CString strLang = g_iLang.GetString(nID);

	// extract first portion of string
	AfxExtractSubString(rMessage, strLang, 0, '\n');

	if ( rMessage.IsEmpty() )
	{
		// not found
		TRACE1("Warning: no message line prompt for ID 0x%04X.\n", nID);
	}
}
