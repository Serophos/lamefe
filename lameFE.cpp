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
#include "Ini.h"

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
	m_hCDRipDll = NULL;
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
	
	HANDLE hEvent; 

	hEvent = ::CreateEvent(NULL, FALSE, TRUE, AfxGetAppName()); 

	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{ 
		
		AfxMessageBox(IDS_APPALREADYRUNNING, MB_OK+MB_ICONINFORMATION); 

		return FALSE; 
	} 

	TCHAR	szBuffer[_MAX_PATH]; 
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
	
	CString wd = szBuffer;
	wd = wd.Left(wd.ReverseFind('\\'));

	CIni cfg;
	cfg.SetIniFileName(wd + "\\LameFE.ini");

	if(cfg.GetValue("LameFE", "ShowSplash", TRUE)){

		CLameFESplash * pSplashWnd = new CLameFESplash(IDB_SPLASH,2500);
		pSplashWnd->Create();
	}

	m_bRipperOK = InitCDRipper();
	SetAutoPlay(!cfg.GetValue("CD-ROM", "DisableAutoPlay", TRUE));
	//SetRegistryKey(_T("LameFE"));

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

	CMenu* pMenu = m_pMainWnd->GetMenu();
	if (pMenu)pMenu->DestroyMenu();
	HMENU hMenu = ((CMainFrame*) m_pMainWnd)->NewMenu();
	pMenu = CMenu::FromHandle( hMenu );
	m_pMainWnd->SetMenu(pMenu);
	((CMainFrame*)m_pMainWnd)->m_hMenuDefault = hMenu;

	// Das einzige Fenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg-Dialog für Info über Anwendung
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogdaten
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strVersion;
	//}}AFX_DATA

	// Überladungen für virtuelle Funktionen, die vom Anwendungs-Assistenten erzeugt wurden
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	//CBrush m_brush;
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_DISPLAY_STATIC, m_strVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//************************************************************************
//	 InitDialog
//
//	 	Setup the display rect and start the timer.
//************************************************************************
BOOL CAboutDlg::OnInitDialog()
{

	CDialog::OnInitDialog();
	
	m_strVersion.Format("%s", STR_VERSION_DLG);
	UpdateData(FALSE);
	return TRUE;
}

// Anwendungsbefehl zum Ausführen des Dialogfelds
void CLameFEApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CLameFEApp-Nachrichtenbehandlungsroutinen

/*LONG RegQueryValueEx(
  HKEY hKey,           // handle to key to query
  LPTSTR lpValueName,  // address of name of value to query
  LPDWORD lpReserved,  // reserved
  LPDWORD lpType,      // address of buffer for value type
  LPBYTE lpData,       // address of data buffer
  LPDWORD lpcbData     // address of data buffer size
);

DWORD disposition;
LONG result = ::RegCreateKeyEx(mRootArea, mRootSection, 0, NULL, 0, accessMask, NULL, &mActiveSectionKey, &disposition);
if (result != ERROR_SUCCESS)
{
	mRootArea = NULL;
	mRootSection = _T( "" );
	mActiveSectionKey = NULL;
	mActiveSection = _T( "" );
	mAccess = 0;
}
*/
void CLameFEApp::SetAutoPlay(BOOL bEnable)
{

/*	BOOL		bIsWindowsNT	= FALSE;
	HINSTANCE	hKernel32		= NULL;
	DWORD		value			= 0;
	DWORD		len				= 0;
	DWORD		type			= 0;
	HKEY		hkResult;
	DWORD		dwDisposition	= 0;

	hKernel32 = LoadLibrary("kernel32.dll");

	if(hKernel32 != NULL){

		bIsWindowsNT = TRUE;
	}
	FreeLibrary(hKernel32);

	if(!bEnable){

		if (bIsWindowsNT)
		{
			len = sizeof(value);
			// "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\Cdrom\\AutoRun
			LONG lResult = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, 
				"SYSTEM\\CurrentControlSet\\Services\\Cdrom", 
				0, NULL, 0, 
				KEY_ALL_ACCESS, 
				NULL, 
				&hkResult, 
				&dwDisposition
			);
			if(lResult != ERROR_SUCCESS){

				TRACE("1__BUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
			}
			lResult = ::RegQueryValueEx(hkResult, 
				  "AutoRun",
				  NULL,
				  &type,
				  (unsigned char*)value,
				  &len
			);

			if(lResult != ERROR_SUCCESS){

				TRACE("2__BUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
			}

			::RegCloseKey(hkResult);
		}
		else
		{
			// "HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\AudioCD\\shell" 
		}
	}*/
}

BOOL CLameFEApp::InitCDRipper()
{

	USES_CONVERSION;

	TCHAR	szBuffer[_MAX_PATH]; 
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
	
	CString wd = szBuffer;
	wd = wd.Left(wd.ReverseFind('\\'));

	CDEX_ERR cResult = CDEX_OK;

	if(m_hCDRipDll){

		CR_DeInit();

		if(::FreeLibrary(m_hCDRipDll) == 0){

			TRACE("::FreeLibrary() == 0\n");
		}

		m_hCDRipDll = NULL;
	}

	m_hCDRipDll = LoadLibrary(wd + "\\CDRip.dll");

	if(!m_hCDRipDll){

		cResult = CDEX_ERROR;
	}

	cResult = CR_Init(wd + "\\lameFE.ini");

	if(cResult != CDEX_OK){  //Error initialisng CD-Ripper

		TRACE("CR_Init failed\n");
		switch ( cResult )
		{
			case CDEX_NATIVEEASPINOTSUPPORTED:
				AfxMessageBox(IDS_SCSINOTSUPPORTED, MB_OK+MB_ICONSTOP);
			break;
			case CDEX_FAILEDTOLOADASPIDRIVERS:
				AfxMessageBox(IDS_FAILEDLOADINGASPI, MB_OK+MB_ICONSTOP);
			break;
			case CDEX_FAILEDTOGETASPISTATUS:
				AfxMessageBox(IDS_FAILEDGETASPISTATUS, MB_OK+MB_ICONSTOP);
			break;
			case CDEX_NATIVEEASPISUPPORTEDNOTSELECTED:
				/*
				AfxMessageBox(IDS_FAILEDASPINOTSELECTED, MB_OK+MB_ICONSTOP);
				*/
				if(AfxMessageBox(IDS_FAILEDGETASPISTATUS,  MB_YESNO) ==  IDYES)
				{
					// set native SCSI libaray option
					CR_SetTransportLayer( TRANSPLAYER_NTSCSI );

					// save settings
					CR_SaveSettings();

					cResult = CR_Init(wd + "\\lameFE.ini");				
					if (cResult == CDEX_OK)
					{
						//g_bRipperPresent = TRUE;
					}
				}

			break;
			case CDEX_ERROR:
				AfxMessageBox("Error loading library", MB_OK+MB_ICONSTOP);
				break;
			case CDEX_OK:
				TRACE("why are we here? hääää\n");
			break;
			default:
				ASSERT( FALSE );
		}
	}
	return (cResult == CDEX_OK ? TRUE : FALSE);
}

BOOL CLameFEApp::GetRipperStatus()
{

	return m_bRipperOK;
}

int CLameFEApp::ExitInstance() 
{

	if(m_hCDRipDll != NULL){
		
		CR_LockCD(FALSE); //Just to be sure :)
		CR_DeInit();
		FreeLibrary(m_hCDRipDll);
	}
	
	return CWinApp::ExitInstance();
}

