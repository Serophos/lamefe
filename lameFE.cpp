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

#include "MainFrm.h"
#include "lameFEDoc.h"
#include "lameFEView.h"
#include "lameFESplash.h"
#include "cdrip\cdrip.h"
#include "Settings.h"
#include "Utils.h"

#pragma comment(linker, "/delayload:CDRip.dll")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSettings g_sSettings;


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

//	m_mtInitializing.Lock();
	
	HANDLE hEvent; 

	hEvent = ::CreateEvent(NULL, FALSE, TRUE, AfxGetAppName()); 

	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{ 
		
		AfxMessageBox(IDS_APPALREADYRUNNING, MB_OK+MB_ICONINFORMATION); 

		return FALSE; 
	} 

	g_sSettings.Init();
	g_sSettings.Load();

	m_bRipperOK = InitCDRipper();

	if(g_sSettings.GetShowSplash()){

		CLameFESplash * pSplashWnd = new CLameFESplash(IDB_SPLASH,2500);
		pSplashWnd->Create();
	}

	AfxEnableControlContainer();


#ifdef _AFXDLL
	Enable3dControls();			// Diese Funktion bei Verwendung von MFC in gemeinsam genutzten DLLs aufrufen
#else
	Enable3dControlsStatic();	// Diese Funktion bei statischen MFC-Anbindungen aufrufen
#endif

	AfxEnableControlContainer();
	
	InitCommonControls();

	if(!Utils::CheckCOMTL32Dll()){  // Version of Common Controls library is too old

		//cfg.SetValue("LameFE", "UseHighColBar", FALSE);
		g_sSettings.SetUseHighColBar(FALSE);
		TRACE("Deactivated highcoloricons as Common Controls library is too old\n");
	}


	//SetAutoPlay(!cfg.GetValue("CD-ROM", "DisableAutoPlay", TRUE));
	free((void*)m_pszProfileName);
	m_pszProfileName = _tcsdup(g_sSettings.GetIniFilename());

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CLameFEDoc),
		RUNTIME_CLASS(CMainFrame),       // Haupt-SDI-Rahmenfenster
		RUNTIME_CLASS(CLameFEView));
	AddDocTemplate(pDocTemplate);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Verteilung der in der Befehlszeile angegebenen Befehle
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

/*	CMenu* pMenu = m_pMainWnd->GetMenu();
	if (pMenu)pMenu->DestroyMenu();
	HMENU hMenu = ((CMainFrame*) m_pMainWnd)->NewMenu();
	pMenu = CMenu::FromHandle( hMenu );
	m_pMainWnd->SetMenu(pMenu);
	((CMainFrame*)m_pMainWnd)->m_hMenuDefault = hMenu;
*/
	// Das einzige Fenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();


	return TRUE;
}

BOOL CLameFEApp::InitCDRipper()
{

	USES_CONVERSION;

	// Windows 98 is shipped with a fucked up ASPI driver
	// so check and warn if it is so
	if(!Utils::IsWindowsNT()){

		CString strVersionWNASPI32, strVersionWINASPI, strVersionAPIX, strVersionASPIENUM;

		if(CheckVersion("WNASPI32.DLL", strVersionWNASPI32) &&
			CheckVersion("WINASPI.DLL", strVersionWINASPI) &&
			CheckVersion("IOSUBSYS\\APIX.VXD", strVersionAPIX) &&
			CheckVersion("ASPIENUM.VXD", strVersionASPIENUM)){

			if((strVersionWNASPI32 != strVersionWINASPI) || (strVersionWNASPI32 != strVersionAPIX) ||
				(strVersionWNASPI32 != strVersionASPIENUM)){

				if(AfxMessageBox(IDS_FAILEDLOADINGASPI, MB_YESNO+MB_ICONSTOP) == IDNO){

					return FALSE;
				}
				else{

					WinExec(g_sSettings.GetWorkingDir() + "\\LASPI.exe", SW_SHOW);
					exit(EXIT_FAILURE);
				}

			}
		}
		else{
			
				if(AfxMessageBox(IDS_FAILEDLOADINGASPI, MB_YESNO+MB_ICONSTOP) == IDNO){

					return FALSE;
				}
				else{

					WinExec(g_sSettings.GetWorkingDir() + "\\LASPI.exe", SW_SHOW);
					exit(EXIT_FAILURE);
				}
		}
	}

	
	CDEX_ERR cResult = CDEX_OK;

	if(m_hCDRipDll){

		CR_DeInit();

		if(::FreeLibrary(m_hCDRipDll) == 0){

			TRACE("::FreeLibrary() == 0\n");
		}

		m_hCDRipDll = NULL;
	}

	m_hCDRipDll = LoadLibrary(g_sSettings.GetWorkingDir() + "\\CDRip.dll");

	if(!m_hCDRipDll){

		cResult = CDEX_ERROR;
	}

	cResult = CR_Init(g_sSettings.GetIniFilename());

	if(cResult != CDEX_OK){  //Error initialisng CD-Ripper

		TRACE("CR_Init failed\n");
		switch ( cResult )
		{
			case CDEX_NATIVEEASPINOTSUPPORTED:
				AfxMessageBox(IDS_SCSINOTSUPPORTED, MB_OK+MB_ICONSTOP);
			break;
			case CDEX_FAILEDTOLOADASPIDRIVERS:
				//AfxMessageBox(IDS_FAILEDLOADINGASPI, MB_OK+MB_ICONSTOP);
				if(AfxMessageBox(IDS_FAILEDLOADINGASPI, MB_YESNO+MB_ICONSTOP) == IDYES){

					WinExec(g_sSettings.GetWorkingDir(), SW_SHOW);
					exit(EXIT_FAILURE);
				}

			break;
			case CDEX_FAILEDTOGETASPISTATUS:
				AfxMessageBox(IDS_FAILEDGETASPISTATUS, MB_OK+MB_ICONSTOP);
			break;
			case CDEX_NATIVEEASPISUPPORTEDNOTSELECTED:

				if(AfxMessageBox(IDS_FAILEDGETASPISTATUS,  MB_YESNO) ==  IDYES)
				{
					// set native SCSI libaray option
					CR_SetTransportLayer(TRANSPLAYER_NTSCSI);

					// save settings
					CR_SaveSettings();

					cResult = CR_Init(g_sSettings.GetIniFilename() + "\\LASPI.exe");				
				}

			break;
			case CDEX_NOCDROMDEVICES:

				if (((const int)CR_GetTransportLayer()!= TRANSPLAYER_NTSCSI) && Utils::IsWindowsNT())
				{

					if(AfxMessageBox( IDS_FAILEDGETASPISTATUS, MB_YESNO) == IDYES)
					{
						// set native SCSI libaray option
						CR_SetTransportLayer( TRANSPLAYER_NTSCSI );

						// save settings
						CR_SaveSettings();

						cResult = CR_Init(g_sSettings.GetIniFilename());				
					}
				}
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




int CLameFEApp::ExitInstance() 
{

	if(m_hCDRipDll != NULL){
		
		CR_LockCD(FALSE); //Just to be sure :)
		CR_DeInit();
		FreeLibrary(m_hCDRipDll);
	}
	
	return CWinApp::ExitInstance();
}


BOOL CLameFEApp::CheckVersion(CString strDll, CString &strVersion)
{

	BOOL bReturn = FALSE;
    LPBYTE  lpVersionData; 
    DWORD   dwLangCharset; 
	
	strDll.Insert(0, "\\");

 	TCHAR lpszModuleName[MAX_PATH + 1] = { '\0',};
	

	GetSystemDirectory(lpszModuleName,	MAX_PATH);
	
	strcat(lpszModuleName, strDll);

	// Get File size
	CFileStatus rStatus;

	if(!CFile::GetStatus(lpszModuleName, rStatus)){

		return FALSE;
	}

	DWORD dwHandle;     
    DWORD dwDataSize = ::GetFileVersionInfoSize(lpszModuleName, &dwHandle); 
    if ( dwDataSize == 0 ){

        return FALSE;
	}

    lpVersionData = new BYTE[dwDataSize]; 
    if(!::GetFileVersionInfo((LPTSTR)lpszModuleName, dwHandle, dwDataSize, (void**)lpVersionData)){

		delete[] lpVersionData; 
		lpVersionData = NULL;
		dwLangCharset = 0;

        return FALSE;
    }

    UINT nQuerySize;
    DWORD* pTransTable;
    if (!::VerQueryValue(lpVersionData, "\\VarFileInfo\\Translation",
                         (void **)&pTransTable, &nQuerySize)){

		delete[] lpVersionData; 
		lpVersionData = NULL;
		dwLangCharset = 0;

        return FALSE;
    }

    // Swap the words to have lang-charset in the correct format
    dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));

    // Query version information value
    LPVOID lpData;
    CString  strBlockName;

    strBlockName.Format(_T("\\StringFileInfo\\%08lx\\%s"), dwLangCharset, "FileVersion");

    if(::VerQueryValue((void **)lpVersionData, strBlockName.GetBuffer(0), &lpData, &nQuerySize)){

        strVersion = (LPCTSTR)lpData;
	}

    strBlockName.ReleaseBuffer();

	//fVersion = 0.0f;
	//_stscanf(strVersion, "%f", & fVersion);
	


	bReturn = TRUE;

	delete[] lpVersionData; 
	lpVersionData = NULL;
	dwLangCharset = 0;

	return bReturn;
}
