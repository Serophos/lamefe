// MainFrm.cpp : Implementierung der Klasse CMainFrame
//

#include "stdafx.h"
#include "lameFE.h"
#include "Ini.h"
#include "MainFrm.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "version")

extern CString		g_strIniFile;

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
	ON_COMMAND(IDS_SHOW_PLAYER, OnShowPlayer)
	ON_COMMAND(ID_VIEW_SHOWPRESETBAR, OnViewShowPresets)
	ON_WM_MEASUREITEM()
	ON_WM_MENUCHAR()
	ON_WM_INITMENUPOPUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,           // status line indicator
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Konstruktion/Zerst�rung

CMainFrame::CMainFrame()
{

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;



	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT ) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Symbolleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}
	
	if(!m_wndPlayerBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndPlayerBar.LoadToolBar(IDR_INT_PLAYER)){

		TRACE0("Couldn't create playerbar\n");
	}

	if(!m_wndPresetBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP, ID_PRESETBAR))
	{

		TRACE0("Couldn't create presetbar\n");
	}

/*	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Dialogleiste konnte nicht erstellt werden\n");
		return -1;		// Fehler bei Erstellung
	}
*/
	TCHAR	szBuffer[_MAX_PATH]; 
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
	
	CString wd = szBuffer;
	wd = wd.Left(wd.ReverseFind('\\'));

	CIni cfg;
	cfg.SetIniFileName(g_strIniFile);

	// set high color icons if possible
	if ((::GetDeviceCaps(GetDC()->m_hDC,BITSPIXEL) > 8) && cfg.GetValue("LameFE", "UseHighColBar", TRUE)) 
	{
	
		CImageList	imageList;
		CBitmap		bitmap;

		// Set up hot bar image lists.
		// Create and set the normal toolbar image list.
		bitmap.LoadMappedBitmap( IDB_TOOLBAR_HI );
		imageList.Create(32, 32, ILC_COLORDDB|ILC_MASK, 9, 1);
		imageList.Add(&bitmap, RGB(254,2,254));
		m_wndToolBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
		imageList.Detach();
		bitmap.Detach();

		// Create and set the disabled toolbar image list.
		bitmap.LoadMappedBitmap( IDB_TOOLBAR_HI_DEACT );
		imageList.Create( 32, 32, ILC_COLORDDB|ILC_MASK, 9, 1);
		imageList.Add(&bitmap, RGB(255,0,255));
		m_wndToolBar.SendMessage( TB_SETDISABLEDIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
		imageList.Detach();
		bitmap.Detach();
	
	}
	
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndPlayerBar.SetBarStyle(m_wndPlayerBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndPresetBar.SetBarStyle(m_wndPresetBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndPlayerBar) ||
		!m_wndReBar.AddBar(&m_wndPresetBar)
		)
	{
		TRACE0("Infoleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Statusleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}

    m_wndStatusBar.SetPaneInfo(1,ID_SEPARATOR,SBPS_NORMAL,250);
	m_wndStatusBar.SetPaneInfo(0,ID_SEPARATOR,SBPS_STRETCH,100);
	
	OnShowPlayer();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// ZU ERLEDIGEN: �ndern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.
	
	TCHAR	szBuffer[_MAX_PATH]; 
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
	
	CString wd = szBuffer;
	wd = wd.Left(wd.ReverseFind('\\'));

	CIni cfg;
	cfg.SetIniFileName(g_strIniFile);
	
	if(cfg.GetValue("LameFE", "SaveWinPos", TRUE)){

		cs.x	= cfg.GetValue("LameFE", "Window.x", 60);
		cs.y	= cfg.GetValue("LameFE", "Window.y", 60);
		cs.cx	= cfg.GetValue("LameFE", "Window.cx", 665);
		cs.cy	= cfg.GetValue("LameFE", "Window.cy", 520);
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

void CMainFrame::OnShowPlayer() 
{

	ShowControlBar(&m_wndPlayerBar, (m_wndPlayerBar.GetStyle() & WS_VISIBLE) == 0,FALSE);
}

void CMainFrame::OnViewShowPresets()
{

//	ShowControlBar(&m_wndDlgBar, (m_wndDlgBar.GetStyle() & WS_VISIBLE) == 0, FALSE);
	ShowControlBar(&m_wndPresetBar, (m_wndPresetBar.GetStyle() & WS_VISIBLE) == 0, FALSE);
}

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




