// MainFrm.cpp : Implementierung der Klasse CMainFrame
//

#include "stdafx.h"
#include "lameFE.h"
#include "cfgFile.h"
#include "MainFrm.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "version")

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



	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT ) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Symbolleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}
	
	cfgFile cfg;

	if(!Utils::CheckCOMTL32Dll()){  // Version of Common Controls library is too old

		cfg.SetValue("usehighcoloricons", FALSE);
		TRACE("Deactivated highcoloricons as Common Controls library is too old\n");
	}

	// set high color icons if possible
	if ((::GetDeviceCaps(GetDC()->m_hDC,BITSPIXEL) > 8) && cfg.GetValue("usehighcoloricons")) 
	{
	
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
	
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

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


//DEL BOOL CMainFrame::CheckCOMCTL32DLL()
//DEL {
//DEL 
//DEL 	BOOL bReturn = FALSE;
//DEL     LPBYTE  lpVersionData; 
//DEL     DWORD   dwLangCharset; 
//DEL 
//DEL  	TCHAR lpszModuleName[ MAX_PATH + 1 ] = { '\0',};
//DEL 	
//DEL 	// Get Comctl32.dll product version
//DEL 	GetSystemDirectory( lpszModuleName,	MAX_PATH );
//DEL 	
//DEL 	strcat(lpszModuleName, "\\COMCTL32.DLL");
//DEL 
//DEL 	DWORD dwHandle;     
//DEL     DWORD dwDataSize = ::GetFileVersionInfoSize(lpszModuleName, &dwHandle); 
//DEL     if ( dwDataSize == 0 ){
//DEL 
//DEL         return FALSE;
//DEL 	}
//DEL 
//DEL     lpVersionData = new BYTE[dwDataSize]; 
//DEL     if(!::GetFileVersionInfo((LPTSTR)lpszModuleName, dwHandle, dwDataSize, (void**)lpVersionData)){
//DEL 
//DEL 		delete[] lpVersionData; 
//DEL 		lpVersionData = NULL;
//DEL 		dwLangCharset = 0;
//DEL 
//DEL         return FALSE;
//DEL     }
//DEL 
//DEL     UINT nQuerySize;
//DEL     DWORD* pTransTable;
//DEL     if (!::VerQueryValue(lpVersionData, "\\VarFileInfo\\Translation",
//DEL                          (void **)&pTransTable, &nQuerySize)){
//DEL 
//DEL 		delete[] lpVersionData; 
//DEL 		lpVersionData = NULL;
//DEL 		dwLangCharset = 0;
//DEL 
//DEL         return FALSE;
//DEL     }
//DEL 
//DEL     // Swap the words to have lang-charset in the correct format
//DEL     dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
//DEL 
//DEL     // Query version information value
//DEL     LPVOID lpData;
//DEL     CString strVersion, strBlockName;
//DEL 
//DEL     strBlockName.Format(_T("\\StringFileInfo\\%08lx\\%s"), dwLangCharset, "FileVersion");
//DEL 
//DEL     if(::VerQueryValue((void **)lpVersionData, strBlockName.GetBuffer(0), &lpData, &nQuerySize)){
//DEL 
//DEL         strVersion = (LPCTSTR)lpData;
//DEL 	}
//DEL 
//DEL     strBlockName.ReleaseBuffer();
//DEL 
//DEL 	float fVersion = 0.0f;
//DEL 	_stscanf(strVersion, "%f", & fVersion);
//DEL 
//DEL 	bReturn = (fVersion >= 4.70);
//DEL 
//DEL 	delete[] lpVersionData; 
//DEL 	lpVersionData = NULL;
//DEL 	dwLangCharset = 0;
//DEL 
//DEL 	return bReturn;
//DEL }
