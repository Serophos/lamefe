/*
** Copyright (C) 2002 Thees Winkler
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

#include "MainFrm.h"
#include "lameFEDoc.h"
#include "lameFEView.h"
#include "lameFESplash.h"
#include "cdrip\cdrip.h"
#include "cfgFile.h"

#pragma comment(linker, "/delayload:CDRip.dll")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLameFEApp

BEGIN_MESSAGE_MAP(CLameFEApp, CWinApp)
	//{{AFX_MSG_MAP(CLameFEApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG_MAP
	// Dateibasierte Standard-Dokumentbefehle
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLameFEApp Konstruktion

CLameFEApp::CLameFEApp()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance platzieren
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CLameFEApp-Objekt

CLameFEApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLameFEApp Initialisierung

BOOL CLameFEApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standardinitialisierung
	// Wenn Sie diese Funktionen nicht nutzen und die Größe Ihrer fertigen 
	//  ausführbaren Datei reduzieren wollen, sollten Sie die nachfolgenden
	//  spezifischen Initialisierungsroutinen, die Sie nicht benötigen, entfernen.

#ifdef _AFXDLL
	Enable3dControls();			// Diese Funktion bei Verwendung von MFC in gemeinsam genutzten DLLs aufrufen
#else
	Enable3dControlsStatic();	// Diese Funktion bei statischen MFC-Anbindungen aufrufen
#endif

	AfxEnableControlContainer();
	InitCommonControls();

	if (!AfxOleInit())
	{
		AfxMessageBox("Unable to initialize OLE.\nTerminating application!");
		return FALSE;
	}
	
	cfgFile cfg;

	if(cfg.GetValue("showsplashscr")){

		CLameFESplash * pSplashWnd = new CLameFESplash(IDB_SPLASH,2500);
		pSplashWnd->Create();
	}

	SetRegistryKey(_T("LameFE"));

	LoadStdProfileSettings(0);  // Standard INI-Dateioptionen laden (einschließlich MRU)

	// Dokumentvorlagen der Anwendung registrieren. Dokumentvorlagen
	//  dienen als Verbindung zwischen Dokumenten, Rahmenfenstern und Ansichten.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CLameFEDoc),
		RUNTIME_CLASS(CMainFrame),       // Haupt-SDI-Rahmenfenster
		RUNTIME_CLASS(CLameFEView));
	AddDocTemplate(pDocTemplate);

	// Befehlszeile parsen, um zu prüfen auf Standard-Umgebungsbefehle DDE, Datei offen
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Verteilung der in der Befehlszeile angegebenen Befehle
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Das einzige Fenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg-Dialog für Info über Anwendung

#define SCROLLAMOUNT				-1
#define DISPLAY_SLOW				70
#define DISPLAY_MEDIUM				40
#define DISPLAY_FAST				10
#define DISPLAY_SPEED				DISPLAY_MEDIUM

#define RED						RGB(255,0,0)
#define GREEN					RGB(0,255,0)
#define WHITE   				RGB(255,255,255)
#define YELLOW  				RGB(255,255,0)
#define BLACK       			RGB(0,0,0)

#define BACKGROUND_COLOR        BLACK
#define TOP_LEVEL_TITLE_COLOR	RED
#define TOP_LEVEL_GROUP_COLOR   YELLOW
#define GROUP_TITLE_COLOR       GREEN
#define NORMAL_TEXT_COLOR		WHITE

#define TOP_LEVEL_TITLE_HEIGHT	21		
#define TOP_LEVEL_GROUP_HEIGHT  19     
#define GROUP_TITLE_HEIGHT    	17     
#define	NORMAL_TEXT_HEIGHT		15

#define TOP_LEVEL_TITLE			'\n'
#define TOP_LEVEL_GROUP         '\r'
#define GROUP_TITLE           	'\t'
#define NORMAL_TEXT				'\f' 
#define DISPLAY_BITMAP			'\b'

#define		ARRAYCOUNT		56

char *pArrCredit[] = { "LAMEFE\b",
						"",
						 STR_VERSION_DLG,
						 "",
						 "Copyright (c) 2002 \f",
						 "Thees Christian Winkler \f",
						 "theesw@users.sourceforge.net \f",
						 "--- \f",
						 "",
						 "",
						 "Project Lead \n",
						 "",
						 "Thees Christian Winkler \f",
						 "",
						 "Engineering \n",
						 "",
						 "lameFE \r",
						 "Thees Ch. Winkler \f",
						 "",
						 "CDRip.dll \r",
						 "Albert L. Faber \f",
						 "",
						 "Additional stuff \r",
						 "Mark Findlay , P.J. Naughter \f", 
						 "",
						 "Website \r",
						 "Piotr Krêglicki \f",
						 "",
						 "",
						 "Documentation \n",
						 "",
						 "Thees Ch. Winkler \f",
						 "",
						 "",
						 "Beta Testing \n",
						 "",
						 "Lorenzo Prince, Jason Faber, Simone Singh \f",
						 "William F. Pearson, Jann Peterecki \f",
						 "",
						 "",
						 "Special thanks to: \n",
						 "",
						 "Jan Ch. Winkler for the initial idea and the programs name \f",
						 "",
						 "",
						 "Provided by \n",
						 "",
						 "Sourceforge.net \f",
						 "http://www.sourceforge.net \f",
						 "http://lamefe.sourceforge.net \f",
						 "",
						 "",
						 "Distributed under the Terms of the GNU \f",
						 "General Public License version 2 or later \f",
						 "",
						 ""
};

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	#define     DISPLAY_TIMER_ID		150		// timer id

	RECT        m_ScrollRect,r;		   // rect of Static Text frame
	int         nArrIndex,nCounter;		   // work ints
	CString     m_szWork;			   // holds display line
	BOOL        m_bFirstTime;
	BOOL        m_bDrawText;
	int         nClip;
	int         nCurrentFontHeight;

	CWnd*       m_pDisplayFrame;

	CBitmap     m_bmpWork;                  // bitmap holder
	CBitmap* 	pBmpOld;                    // other bitmap work members
	CBitmap*    m_bmpCurrent;
	HBITMAP 	m_hBmpOld;

	CSize 		m_size;                     // drawing helpers
	CPoint 		m_pt;
	BITMAP 		m_bmpInfo;
	CDC 		m_dcMem;
	BOOL 		m_bProcessingBitmap;
// Dialogdaten
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// Überladungen für virtuelle Funktionen, die vom Anwendungs-Assistenten erzeugt wurden
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	CBrush m_brush;
	//{{AFX_MSG(CAboutDlg)
	virtual void OnOK();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAboutDlg::OnOK() 
{
	KillTimer(DISPLAY_TIMER_ID);
	CDialog::OnOK();
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
/*
** No need to do this!
**
** HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
*/

/*
** Return the white brush.
*/
	return m_brush;
}

//************************************************************************
//	 InitDialog
//
//	 	Setup the display rect and start the timer.
//************************************************************************
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_brush.CreateSolidBrush(RGB(0, 0, 0)); 
	BOOL bRet;
    	UINT nRet;
	
	nCurrentFontHeight = NORMAL_TEXT_HEIGHT;
	
	CClientDC dc(this);
	bRet = m_dcMem.CreateCompatibleDC(&dc);
	
	
	m_bProcessingBitmap=FALSE;
	
	nArrIndex=0;
	nCounter=1;
	nClip=0;
	m_bFirstTime=TRUE;
	m_bDrawText=FALSE;
	m_hBmpOld = 0;
	
	m_pDisplayFrame=(CWnd*)GetDlgItem(IDC_DISPLAY_STATIC);	

	// If you assert here, you did not assign your static display control
	// the IDC_ value that was used in the GetDlgItem(...). This is the
    // control that will display the credits.
	_ASSERTE(m_pDisplayFrame);
				 
	m_pDisplayFrame->GetClientRect(&m_ScrollRect);


	nRet = SetTimer(DISPLAY_TIMER_ID,DISPLAY_SPEED,NULL);
    _ASSERTE(nRet != 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//************************************************************************
//	 OnTimer
//
//	 	On each of the display timers, scroll the window 1 unit. Each 20
//      units, fetch the next array element and load into work string. Call
//      Invalidate and UpdateWindow to invoke the OnPaint which will paint 
//      the contents of the newly updated work string.
//************************************************************************
void CAboutDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent != DISPLAY_TIMER_ID)
		{
		CDialog::OnTimer(nIDEvent);
		return;
		}

	if (!m_bProcessingBitmap)
	if (nCounter++ % nCurrentFontHeight == 0)	 // every x timer events, show new line
		{
		nCounter=1;
		m_szWork = pArrCredit[nArrIndex++];
		
		if (nArrIndex > ARRAYCOUNT-1)
			nArrIndex=0;
		nClip = 0;
		m_bDrawText=TRUE;
		}
	
	m_pDisplayFrame->ScrollWindow(0,SCROLLAMOUNT,&m_ScrollRect,&m_ScrollRect);
	nClip = nClip + abs(SCROLLAMOUNT);	
	
    CRect r;
    CWnd* pWnd = GetDlgItem(IDC_DISPLAY_STATIC);
    ASSERT_VALID(pWnd);
    pWnd->GetClientRect(&r);
    pWnd->ClientToScreen(r);
    ScreenToClient(&r);
    InvalidateRect(r,FALSE); // FALSE does not erase background

	CDialog::OnTimer(nIDEvent);
}


//************************************************************************
//	 OnPaint
//
//	 	Send the newly updated work string to the display rect.
//************************************************************************
void CAboutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	PAINTSTRUCT ps;
	CDC* pDc = m_pDisplayFrame->BeginPaint(&ps);
	
	pDc->SetBkMode(TRANSPARENT);


	//*********************************************************************
	//	FONT SELECTION
    	CFont m_fntArial;
	CFont* pOldFont;
	BOOL bSuccess;
	
	BOOL bUnderline;
	BOOL bItalic;


	if (!m_szWork.IsEmpty())
	switch (m_szWork[m_szWork.GetLength()-1] )
		{
		case NORMAL_TEXT:
		default:
			bItalic = FALSE;
			bUnderline = FALSE;
			nCurrentFontHeight = NORMAL_TEXT_HEIGHT;
   			bSuccess = m_fntArial.CreateFont(NORMAL_TEXT_HEIGHT, 0, 0, 0, 
   								FW_THIN, bItalic, bUnderline, 0, 
   								ANSI_CHARSET,
                               	OUT_DEFAULT_PRECIS,
                               	CLIP_DEFAULT_PRECIS,
                               	PROOF_QUALITY,
                               	VARIABLE_PITCH | 0x04 | FF_DONTCARE,
                               	(LPSTR)"Arial");
			pDc->SetTextColor(NORMAL_TEXT_COLOR);
			pOldFont  = pDc->SelectObject(&m_fntArial);
			break;

		case TOP_LEVEL_GROUP:
			bItalic = FALSE;
			bUnderline = FALSE;
			nCurrentFontHeight = TOP_LEVEL_GROUP_HEIGHT;
   			bSuccess = m_fntArial.CreateFont(TOP_LEVEL_GROUP_HEIGHT, 0, 0, 0, 
   								FW_BOLD, bItalic, bUnderline, 0, 
   								ANSI_CHARSET,
                               	OUT_DEFAULT_PRECIS,
                               	CLIP_DEFAULT_PRECIS,
                               	PROOF_QUALITY,
                               	VARIABLE_PITCH | 0x04 | FF_DONTCARE,
                               	(LPSTR)"Arial");
			pDc->SetTextColor(TOP_LEVEL_GROUP_COLOR);
			pOldFont  = pDc->SelectObject(&m_fntArial);
			break;
		
		
		
		case GROUP_TITLE:
			bItalic = FALSE;
			bUnderline = FALSE;
			nCurrentFontHeight = GROUP_TITLE_HEIGHT;
   			bSuccess = m_fntArial.CreateFont(GROUP_TITLE_HEIGHT, 0, 0, 0, 
   								FW_BOLD, bItalic, bUnderline, 0, 
   								ANSI_CHARSET,
                               	OUT_DEFAULT_PRECIS,
                               	CLIP_DEFAULT_PRECIS,
                               	PROOF_QUALITY,
                               	VARIABLE_PITCH | 0x04 | FF_DONTCARE,
                               	(LPSTR)"Arial");
			pDc->SetTextColor(GROUP_TITLE_COLOR);
			pOldFont  = pDc->SelectObject(&m_fntArial);
			break;
		
		
		case TOP_LEVEL_TITLE:
			bItalic = FALSE;
			bUnderline = TRUE;
			nCurrentFontHeight = TOP_LEVEL_TITLE_HEIGHT;
			bSuccess = m_fntArial.CreateFont(TOP_LEVEL_TITLE_HEIGHT, 0, 0, 0, 
								FW_BOLD, bItalic, bUnderline, 0, 
								ANSI_CHARSET,
	                           	OUT_DEFAULT_PRECIS,
	                           	CLIP_DEFAULT_PRECIS,
	                           	PROOF_QUALITY,
	                           	VARIABLE_PITCH | 0x04 | FF_DONTCARE,
	                           	(LPSTR)"Arial");
			pDc->SetTextColor(TOP_LEVEL_TITLE_COLOR);
			pOldFont  = pDc->SelectObject(&m_fntArial);
			break;
		
		case DISPLAY_BITMAP:
			if (!m_bProcessingBitmap)
				{
				CString szBitmap = m_szWork.Left(m_szWork.GetLength()-1);
	   			if (!m_bmpWork.LoadBitmap((const char *)szBitmap))
					{
					CString str; 
					str.Format("Could not find bitmap resource \"%s\". "
                               "Be sure to assign the bitmap a QUOTED resource name", szBitmap); 
					KillTimer(DISPLAY_TIMER_ID); 
					MessageBox(str); 
					return; 
					}
				m_bmpCurrent = &m_bmpWork;
	   			m_bmpCurrent->GetObject(sizeof(BITMAP), &m_bmpInfo);
			
				m_size.cx = m_bmpInfo.bmWidth;	// width  of dest rect
				RECT workRect;
				m_pDisplayFrame->GetClientRect(&workRect);
				m_pDisplayFrame->ClientToScreen(&workRect);
				ScreenToClient(&workRect);
				// upper left point of dest
				m_pt.x = (workRect.right - 
							((workRect.right-workRect.left)/2) - (m_bmpInfo.bmWidth/2));
				m_pt.y = workRect.bottom;
				
				
				pBmpOld = m_dcMem.SelectObject(m_bmpCurrent);
				if (m_hBmpOld == 0)
					m_hBmpOld = (HBITMAP) pBmpOld->GetSafeHandle();
				m_bProcessingBitmap = TRUE;
				}
			break;

		}
	
	
	
	
	CBrush bBrush(BLACK);
	CBrush* pOldBrush;
	pOldBrush  = pDc->SelectObject(&bBrush);
	// Only fill rect comprised of gap left by bottom of scrolling window
	r=m_ScrollRect;
	r.top = r.bottom-abs(SCROLLAMOUNT); 
	pDc->DPtoLP(&r);
	
	if (m_bFirstTime)
		{
		m_bFirstTime=FALSE;
		pDc->FillRect(&m_ScrollRect,&bBrush);
		}
	else
		pDc->FillRect(&r,&bBrush);
	
	r=m_ScrollRect;
	r.top = r.bottom-nClip;
	
	
	if (!m_bProcessingBitmap)
		{
		int x = pDc->DrawText((const char *)m_szWork,m_szWork.GetLength()-1,&r,DT_TOP|DT_CENTER|
					DT_NOPREFIX | DT_SINGLELINE);	
		m_bDrawText=FALSE;
		}
	else
		{
    	dc.StretchBlt( m_pt.x, m_pt.y-nClip, m_size.cx, nClip, 
                   		&m_dcMem, 0, 0, m_bmpInfo.bmWidth-1, nClip,
                   		SRCCOPY );
		if (nClip > m_bmpInfo.bmHeight)
			{
			m_bmpWork.DeleteObject();
			m_bProcessingBitmap = FALSE;
			nClip=0;
			m_szWork.Empty();
			nCounter=1;
			}
		pDc->SelectObject(pOldBrush);
		bBrush.DeleteObject();
		m_pDisplayFrame->EndPaint(&ps);
		return;
		}
	
	
	pDc->SelectObject(pOldBrush);
	bBrush.DeleteObject();
	
	if (!m_szWork.IsEmpty())
		{
		pDc->SelectObject(pOldFont);
		m_fntArial.DeleteObject();
		}

	m_pDisplayFrame->EndPaint(&ps);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CAboutDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
    m_dcMem.SelectObject(CBitmap::FromHandle(m_hBmpOld));
    m_bmpWork.DeleteObject();
	
}

// Anwendungsbefehl zum Ausführen des Dialogfelds
void CLameFEApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CLameFEApp-Nachrichtenbehandlungsroutinen

BOOL CLameFEApp::InitCDRipDLL()
{

/*	USES_CONVERSION;

	LTRACE( _T( "Entering CCDexApp::LoadCDRipLibrary()" ) );

	CDEX_ERR bReturn = CDEX_OK;

	g_bRipperPresent = FALSE;

	if ( m_hCDRipDll )
	{
		g_config.Save();

		CR_DeInit();

		if ( 0 == ::FreeLibrary( m_hCDRipDll ) )
		{
			LTRACE( GetLastErrorString() );

		}
		m_hCDRipDll = NULL;
	}

	if ( FALSE == LoadDLL(  _T( "CDRip.dll" ),
							_T( "\\CDRip" ),
							m_hCDRipDll,
							TRUE,
							FALSE ) )
	{
		bReturn = CDEX_ERROR;
	}
	else
	{
		CString strMsg;

		bReturn = CR_Init( W2A( g_config.GetIniFileName() ) );

		// Initialize the CD-ROM ripper
		if ( CDEX_OK == bReturn )
		{
			g_bRipperPresent = TRUE;
		}

		switch ( bReturn )
		{
			case CDEX_NATIVEEASPINOTSUPPORTED:
				AfxMessageBox( g_language.GetString( IDS_NATIVEASPINOTSUPPORTED ) );
			break;
			case CDEX_FAILEDTOLOADASPIDRIVERS:
				AfxMessageBox( g_language.GetString( IDS_FAILEDTOLOADASPIDRIVERS ) );
			break;
			case CDEX_FAILEDTOGETASPISTATUS:
				AfxMessageBox( g_language.GetString( IDS_FAILEDTOGETASPISTATUS ) );
			break;
			case CDEX_NATIVEEASPISUPPORTEDNOTSELECTED:
				AfxMessageBox( g_language.GetString( IDS_ASPIFAILEDSUGGESTNATIVENTSCSI ) );
			break;

			case CDEX_OK:
			break;
			default:
				ASSERT( FALSE );

		}
	}

	LTRACE( _T( "Leaving CCDexApp::LoadCDRipLibrary(), return value %d" ), bReturn );
*/
	return FALSE;
}
