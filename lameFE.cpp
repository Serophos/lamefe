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
#include "I18n.h"

#pragma comment(linker, "/delayload:CDRip.dll")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSettings g_sSettings;
CI18n g_iLang;
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
	m_hCDRipDll		 = NULL;
	m_bAutoPlayState = FALSE;
	m_bRipperOK		 = FALSE;

}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CLameFEApp-Objekt

CLameFEApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLameFEApp Initialisierung

BOOL CLameFEApp::InitInstance()
{


	if(!g_sSettings.Init()){

		AfxMessageBox("LameFE was unable to read the configuration file. Please check your installation.\nLameFE will now terminate.", MB_OK+MB_ICONSTOP);
		return FALSE;
	}

	g_sSettings.Load();

#if defined _DUMPTABLE && defined _DEBUG
	DumpStringTable();
#endif

	g_iLang.LoadLanguage(g_sSettings.GetActiveLang());

	HANDLE hEvent; 

	hEvent = ::CreateEvent(NULL, FALSE, TRUE, AfxGetAppName()); 

	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{ 
		
		AfxMessageBox(g_iLang.GetString(IDS_APPALREADYRUNNING), MB_OK+MB_ICONINFORMATION); 

		return FALSE; 
	} 

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

	if (!AfxOleInit())
	{
		AfxMessageBox("Unable to initialize OLE.\nTerminating application!");
		return FALSE;
	}

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
/*
	CMenu* pMenu = m_pMainWnd->GetMenu();
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
	
	g_iLang.TranslateDialog(this, IDD_ABOUTBOX);
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
	DWORD		dg_strWorkingDirisposition	= 0;

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
				&dg_strWorkingDirisposition
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

	CDEX_ERR cResult = CDEX_OK;

	if(m_hCDRipDll){

		CR_DeInit();

		if(::FreeLibrary(m_hCDRipDll) == 0){

			TRACE("::FreeLibrary() == 0\n");
		}

		m_hCDRipDll = NULL;
	}

	m_hCDRipDll = LoadLibrary(g_sSettings. GetAppDir() + "\\CDRip.dll");

	if(!m_hCDRipDll){

		cResult = CDEX_ERROR;
	}

	cResult = CR_Init(g_sSettings.GetIniFilename());

	if(cResult != CDEX_OK){  //Error initialisng CD-Ripper

		TRACE("CR_Init failed\n");
		switch ( cResult )
		{
			case CDEX_NATIVEEASPINOTSUPPORTED:
				AfxMessageBox(g_iLang.GetString(IDS_SCSINOTSUPPORTED), MB_OK+MB_ICONSTOP);
			break;
			case CDEX_FAILEDTOLOADASPIDRIVERS:
				AfxMessageBox(g_iLang.GetString(IDS_FAILEDLOADINGASPI), MB_OK+MB_ICONSTOP);
			break;
			case CDEX_FAILEDTOGETASPISTATUS:
				AfxMessageBox(g_iLang.GetString(IDS_FAILEDGETASPISTATUS), MB_OK+MB_ICONSTOP);
			break;
			case CDEX_NATIVEEASPISUPPORTEDNOTSELECTED:

				if(AfxMessageBox(g_iLang.GetString(IDS_FAILEDGETASPISTATUS),  MB_YESNO) ==  IDYES)
				{
					// set native SCSI libaray option
					CR_SetTransportLayer(TRANSPLAYER_NTSCSI);

					// save settings
					CR_SaveSettings();

					cResult = CR_Init(g_sSettings.GetIniFilename());				
				}

			break;
			case CDEX_NOCDROMDEVICES:

				if (((const int)CR_GetTransportLayer()!= TRANSPLAYER_NTSCSI) && Utils::IsWindowsNT())
				{

					if(AfxMessageBox(g_iLang.GetString(IDS_FAILEDGETASPISTATUS), MB_YESNO) == IDYES)
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

int CLameFEApp::ExitInstance() 
{

	if(m_hCDRipDll != NULL){
		
		CR_LockCD(FALSE); //Just to be sure :)
		CR_DeInit();
		FreeLibrary(m_hCDRipDll);
	}
	
	return CWinApp::ExitInstance();
}


#ifdef _DUMPTABLE

void CLameFEApp::DumpStringTable()
{

	
	TRACE("Dumping table\n");

	CStdioFile fLog;
	CString    strLog, strTmp;
	CFileException e;
	TRY{

		fLog.Open(g_sSettings.GetLangDir() + "\\str_table_dump.txt", CFile::modeWrite|CFile::shareExclusive|CFile::typeText|CFile::modeCreate, &e);
		//fLog.SeekToEnd();
		
		//Sleep(50);
		fLog.WriteString("// Section 1\n");
		for(int i = 32771; i <= 40258; i++){

			strTmp.LoadString(i);
			if(strTmp.IsEmpty()) continue;
			strLog.Format("#%X#  \"%s\"", i, strTmp);
			fLog.WriteString(ConvertString(strLog, FALSE) + "\n");
		}

		fLog.WriteString("// Section 2\n");
		for(i = 61204; i <= 61221; i++){

			strTmp.LoadString(i);
			if(strTmp.IsEmpty()) continue;
			strLog.Format("#%X#  \"%s\"", i, strTmp);
			fLog.WriteString(ConvertString(strLog, FALSE) + "\n");
		}

		fLog.Flush();
		fLog.Close();
	}
	CATCH( CFileException, e )
	{
	   #ifdef _DEBUG
		  afxDump << "File could not be opened " << e->m_cause << "\n";
	   #endif
	}
	END_CATCH

}

CString CLameFEApp::ConvertString(CString strText, BOOL bBack)
{

	if(bBack){

		strText.Replace("\\n", "\n");
		strText.Replace("\\t", "\t");
	}
	else{

		strText.Replace("\n", "\\n");
		strText.Replace("\t", "\\t");
	}

	return strText;
}

#endif