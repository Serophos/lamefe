// MainFrm.cpp : Implementierung der Klasse CMainFrame
//

#include "stdafx.h"
#include "lameFE.h"
#include "cfgFile.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,           // status line indicator
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Konstruktion/Zerstörung

CMainFrame::CMainFrame()
{
	// ZU ERLEDIGEN: Hier Code zur Member-Initialisierung einfügen
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;



	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Symbolleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}
	
	/*if (!m_wndAlbumInfoBar.Create(this, IDD_ALBUMINFOBAR, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Dialogleiste konnte nicht erstellt werden\n");
		return -1;		// Fehler bei Erstellung
	}*/
	cfgFile cfg;
	// set high color icons if possible
	if ((::GetDeviceCaps(GetDC()->m_hDC,BITSPIXEL) > 8) && cfg.GetValue("usehighcoloricons")) 
	{
		//m_bmToolbarHi.LoadMappedBitmap(IDB_TOOLBAR_HI);
		//m_wndToolBar.SetBitmap( (HBITMAP)m_bmToolbarHi );

	
		CImageList	imageList;
		CBitmap		bitmap;

		// Set up hot bar image lists.
		// Create and set the normal toolbar image list.
		bitmap.LoadMappedBitmap( IDB_TOOLBAR_HI );
		imageList.Create(32, 32, ILC_COLORDDB|ILC_MASK, 13, 1);
		imageList.Add(&bitmap, RGB(255,0,255));
		m_wndToolBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
		imageList.Detach();
		bitmap.Detach();

		// Create and set the disabled toolbar image list.
		bitmap.LoadMappedBitmap( IDB_TOOLBAR_HI_DEACT );
		imageList.Create( 32, 32, ILC_COLORDDB|ILC_MASK, 13, 1);
		imageList.Add(&bitmap, RGB(255,0,255));
		m_wndToolBar.SendMessage( TB_SETDISABLEDIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
		imageList.Detach();
		bitmap.Detach();
	
	}
	
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar)
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

	// ZU ERLEDIGEN: Entfernen, wenn Sie keine QuickInfos wünschen
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndStatusBar.SetPaneInfo(1,ID_SEPARATOR,SBPS_NORMAL,250);
	m_wndStatusBar.SetPaneInfo(0,ID_SEPARATOR,SBPS_STRETCH,100);
	
	//OnViewAlbumTagEditor();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.
	
	cfgFile cfg;
	
	if(cfg.GetValue("savewinpos")){

		cs.x = cfg.GetValue("window.x");
		cs.y = cfg.GetValue("window.y");
		cs.cx = cfg.GetValue("window.cx");
		cs.cy = cfg.GetValue("window.cy");
	}
	else{

		//cs.x = 30;
		//cs.y = 30;
		cs.cx = 660;
		cs.cy = 500;
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



