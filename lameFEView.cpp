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

#include "lameFEDoc.h"
#include "lameFEView.h"

#include "CDRip/CDRip.h"
#include "CDPlayerIni.h"
#include "SettingsSheet.h"
#include "cfgFile.h"
#include "CDTrack.h"
#include "CDdbQueryDlg.h"
#include "mfccddb.h"
#include "CPlugin.h"
#include "MyFileDialog.h"


#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const UINT UWM_ALBUMINFO_UPDATED = ::RegisterWindowMessage(_T("UWM_RESET_VIEW--{4E7F6EC0-6ADC-11d3-BC36-006067709674}"));
static const UINT UWM_LISTCTRL_KEYUP = ::RegisterWindowMessage(_T("UWM_LISTCTRL_KEYUP--{4E7F6EC0-6ADC-11d3-BC36-006067709674}"));

#define TIMERID2 25011

/////////////////////////////////////////////////////////////////////////////
// CLameFEView

IMPLEMENT_DYNCREATE(CLameFEView, CFormView)

BEGIN_MESSAGE_MAP(CLameFEView, CFormView)
	//{{AFX_MSG_MAP(CLameFEView)
	ON_COMMAND(IDC_CONFIG_IN, OnConfigureIn)
	ON_COMMAND(IDC_CONFIG_OUT, OnConfigureOut)
	ON_CBN_SELCHANGE(IDC_INPUT_DEVICE, OnSelchangeDevices)
	ON_COMMAND(ID_ID3TAGS_READCDPLAYERINI, OnId3tagsReadcdplayerini)
	ON_COMMAND(ID_ID3TAGS_READCDTEXT, OnId3tagsReadcdtext)
	ON_COMMAND(ID_ID3TAGS_READFREEDBSERVER, OnId3tagsReadfreedbserver)
	ON_COMMAND(ID_ID3TAGS_SAVETOCDPLAYERINI, OnId3tagsSavetocdplayerini)
	ON_WM_TIMER()
	ON_COMMAND(ID_SETTINGS_PREFERENCES, OnSettingsPreferences)
	ON_COMMAND(ID_SETTINGS_PLUGINS, OnSettingsPlugins)
	ON_COMMAND(ID_SETTINGS_LAMEENCODER, OnSettingsLameencoder)
	ON_COMMAND(ID_SETTINGS_ID3TAGSFILENAMES, OnSettingsId3tagsfilenames)
	ON_COMMAND(ID_SETTINGS_CDREADER, OnSettingsCdreader)
	ON_COMMAND(ID_SETTINGS_FREEDBSETUP, OnSettingsFreedbsetup)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_REMOVE_FILE, OnRemoveFile)
	ON_COMMAND(ID_FILE_STARTENCODING, OnFileStartencoding)
	ON_COMMAND(ID_BATCH_ALBUM, OnFileStartBatchAlbum)
	ON_COMMAND(ID_START_TO_SINGLE_FILE, OnFileStartAlbum)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_VIEW_RELOADCD, OnViewReloadcd)
	ON_COMMAND(ID_VIEW_SELECTALLTRACKSFILES, OnViewSelectalltracksfiles)
	ON_COMMAND(ID_ID3TAGS_SAVECUESHEET, OnId3tagsSavecuesheet)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateToolBar)
	ON_UPDATE_COMMAND_UI(ID_ID3TAGS_READFREEDBSERVER, OnUpdateCDControls)
	ON_COMMAND(ID_SETTINGS_LOGGING, OnSettingsLogging)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_FILES_TRACKS, OnClickFilesTracks)
	ON_WM_SIZE()
	ON_COMMAND(ID_ID3TAGS_ID3TAGEDITOR, OnId3tagsId3tageditor)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_FILE, OnUpdateToolBar)
	ON_UPDATE_COMMAND_UI(ID_ID3TAGS_READCDPLAYERINI, OnUpdateCDControls)
	ON_UPDATE_COMMAND_UI(ID_ID3TAGS_READCDTEXT, OnUpdateCDControls)
	ON_UPDATE_COMMAND_UI(ID_ID3TAGS_SAVECUESHEET, OnUpdateCDControls)
	ON_UPDATE_COMMAND_UI(ID_ID3TAGS_SAVETOCDPLAYERINI, OnUpdateCDControls)
	ON_UPDATE_COMMAND_UI(ID_START_TO_SINGLE_FILE, OnUpdateCDControls)
	ON_UPDATE_COMMAND_UI(ID_BATCH_ALBUM, OnUpdateCDControls)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RELOADCD, OnUpdateCDControls)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(UWM_ALBUMINFO_UPDATED, OnAlbumInfoUpdated)
	ON_REGISTERED_MESSAGE(UWM_LISTCTRL_KEYUP, OnClickFilesTracks)
	//ON_NOTIFY(LVN_KEYDOWN, IDC_FILES_TRACKS, OnClickFilesTracks)
END_MESSAGE_MAP()


void CLameFEView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLameFEDlg)
	DDX_Control(pDX, IDC_FILES_TRACKS, m_ctrlList);
	DDX_Control(pDX, IDC_INPUT_DEVICE, c_inputDevice);
	DDX_Text(pDX, IDC_INPUT_DEVICE, m_strInputDevice);
	DDX_Text(pDX, IDC_OUTPUT_DEVICE, m_strOutputDevice);
	DDX_Control(pDX, IDC_OUTPUT_DEVICE, c_outputDevice);
	DDX_Control(pDX, IDC_CONFIG_OUT, c_configureOut);
	DDX_Control(pDX, IDC_CONFIG_IN, c_configureIn);

	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CLameFEView Konstruktion/Destruktion

CLameFEView::CLameFEView()
:CFormView(CLameFEView::IDD)
{

	m_pToolTip = NULL;
	m_bTagEditorVisible = FALSE;
	m_hCDRipDll = NULL;
}

CLameFEView::~CLameFEView()
{

	delete m_pToolTip;
}

BOOL CLameFEView::PreCreateWindow(CREATESTRUCT& cs)
{
	
	//CenterWindow();
	return CFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLameFEView Zeichnen

void CLameFEView::OnDraw(CDC* pDC)
{
	CLameFEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// ZU ERLEDIGEN: Hier Code zum Zeichnen der urspr¸nglichen Daten hinzuf¸gen
}

/////////////////////////////////////////////////////////////////////////////
// CLameFEView Diagnose

#ifdef _DEBUG
void CLameFEView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLameFEView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CLameFEDoc* CLameFEView::GetDocument() // Die endg¸ltige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLameFEDoc)));
	return (CLameFEDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLameFEView Nachrichten-Handler


void CLameFEView::OnAlbumInfoUpdated(WPARAM wParam,LPARAM lParam)
{

	TRACE("Entering CLameFEView::OnAlbumInfoUpdated()\n");
	if(!m_bTagEditorVisible){

		return;
	}
	int nSelCnt = m_ctrlList.GetSelCount();

	if(nSelCnt == 0){

		AfxMessageBox(IDS_ID3_CHOOSE_FILE, MB_OK+MB_ICONSTOP);
		return;
	}
	
	int *pIndex = new int[nSelCnt];
	m_ctrlList.GetSelItems(nSelCnt, pIndex);

	if(IsPluginMode()){

		m_mmFiles[m_nSelItem]->m_id3Info.SetSong(m_aAlbumInfoCtrl.m_strSong);
	}
	else{

		m_compactDisc.GetCDTrack(m_nSelItem)->m_id3Info.SetSong(m_aAlbumInfoCtrl.m_strSong);
	}

	for(int i = 0; i < nSelCnt; i++){
		
		CID3Info *tmp = (IsPluginMode() ? &m_mmFiles[pIndex[i]]->m_id3Info : &m_compactDisc.GetCDTrack(pIndex[i])->m_id3Info);

		tmp->SetAlbum(m_aAlbumInfoCtrl.m_strAlbum);
		tmp->SetArtist(m_aAlbumInfoCtrl.m_strInterpret);
		tmp->SetComment(m_aAlbumInfoCtrl.m_strComment);
		tmp->SetGenre(m_aAlbumInfoCtrl.m_strGenre);
		tmp->SetTrack(m_aAlbumInfoCtrl.m_nTrack);
		tmp->SetYear(m_aAlbumInfoCtrl.m_nYear);
	}

	if(!IsPluginMode()){

		CString strTmp;

		for(i = 0; i < m_ctrlList.GetItemCount(); i++){

			strTmp.Format("%s - %s", m_compactDisc.GetCDTrack(i)->m_id3Info.GetArtist(), m_compactDisc.GetCDTrack(i)->m_id3Info.GetSong());
			m_ctrlList.SetItemText(i, 1, strTmp);
		}
	}
	delete pIndex;
	TRACE("Leaving CLameFEView::OnAlbumInfoUpdated()\n");
}

void CLameFEView::OnId3tagsId3tageditor() 
{

	if(m_bTagEditorVisible){

		m_aAlbumInfoCtrl.ShowWindow(SW_HIDE);
		m_bTagEditorVisible = FALSE;
		ResizeControls();
	}
	else{

		m_bTagEditorVisible = TRUE;
		ResizeControls();
		CRect rcSheet;
		GetDlgItem(IDC_FILES_TRACKS)->GetWindowRect(&rcSheet);

		UINT nFlags = SWP_NOSIZE|SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER ;
		m_aAlbumInfoCtrl.ShowWindow(SW_SHOW);

		m_aAlbumInfoCtrl.SetWindowPos(NULL, rcSheet.left, rcSheet.bottom, 0, 0, nFlags);
	}

	CMenu *pMenu = AfxGetApp()->m_pMainWnd->GetMenu();
	if(pMenu){

		pMenu->CheckMenuItem(ID_ID3TAGS_ID3TAGEDITOR, (m_bTagEditorVisible ? MF_CHECKED : MF_UNCHECKED));
	}
}

void CLameFEView::OnId3tagsReadcdplayerini() 
{
	
	if(!m_compactDisc.GetNumAudioTracks()){
		
		AfxMessageBox(IDS_MAIN_NOAUDIOCD, MB_OK+MB_ICONSTOP);
		return;
	}

	int iCnt = m_ctrlList.GetSelCount();
	int *pIndex = new int[iCnt];
	m_ctrlList.GetSelItems(iCnt, pIndex);

	CCDPlayerIni cdPlayerIni(&m_compactDisc);
	if(!cdPlayerIni.Read()){

		AfxMessageBox(IDS_CDPINI_NOENTRY, MB_OK+MB_ICONINFORMATION);
		return;
	}
	
	RefreshTrackList();
	m_ctrlList.SetSelItems(iCnt, pIndex);
	SetAlbumInfo();
	delete pIndex;
}

void CLameFEView::OnId3tagsReadcdtext() 
{

	if(!m_compactDisc.ReadCDText()){

		AfxMessageBox(IDS_CDTEXT_ERROR, MB_OK+MB_ICONINFORMATION);
		return;
	}

	int iCnt = m_ctrlList.GetSelCount();
	int *pIndex = new int[iCnt];
	m_ctrlList.GetSelItems(iCnt, pIndex);


	RefreshTrackList();
	m_ctrlList.SetSelItems(iCnt, pIndex);
	SetAlbumInfo();
	delete pIndex;
}

void CLameFEView::OnId3tagsReadfreedbserver() 
{
	
	
	//save current selection
	int iCnt = m_ctrlList.GetSelCount();
	int *pIndex = new int[iCnt];
	m_ctrlList.GetSelItems(iCnt, pIndex);


	if(!m_compactDisc.GetNumAudioTracks()){
		
		AfxMessageBox(IDS_MAIN_NOAUDIOCD, MB_OK+MB_ICONSTOP);
		delete pIndex;
		return;
	}
	CWaitCursor wc;

	CCDdbQueryDlg cddbDlg(this, &m_compactDisc, CR_GetActiveCDROM(), wd);
	int nResult = cddbDlg.DoModal();

	RefreshTrackList();
	m_ctrlList.SetSelItems(iCnt, pIndex);
	delete pIndex;

	//save to cdplayer.ini if config says so
	cfgFile cfg(wd);
	if(cfg.GetValue("savecdplayerini") && nResult == 2){

		CCDPlayerIni cdini(&m_compactDisc);
		cdini.Write();
	}
	SetAlbumInfo();
}

void CLameFEView::OnId3tagsSavecuesheet() 
{

	if(!m_compactDisc.GetNumAudioTracks()){
		
		AfxMessageBox(IDS_MAIN_NOAUDIOCD, MB_OK+MB_ICONSTOP);
		return;
	}

	if(!m_compactDisc.WriteCueSheet(wd, ".mp3")){

		AfxMessageBox(IDS_CUESHEETFAILED, MB_OK + MB_ICONSTOP);
	}
	else{
		
		CString strTmp;
		strTmp.Format(IDS_CUESHEETSAVED, m_compactDisc.GetAlbumString(wd, ".cue"));
		AfxMessageBox(strTmp, MB_OK + MB_ICONINFORMATION);
	}
}

void CLameFEView::OnId3tagsSavetocdplayerini() 
{
	
	if(!m_compactDisc.GetNumAudioTracks()){
		
		AfxMessageBox(IDS_MAIN_NOAUDIOCD, MB_OK+MB_ICONSTOP);
		return;
	}

	CCDPlayerIni cdini(&m_compactDisc);
	int nResult = cdini.Write();
	if(nResult == -1){

		AfxMessageBox(IDS_CDPINI_ERRORWRITE, MB_OK+MB_ICONEXCLAMATION);
	}
	else if(nResult == 2){

		AfxMessageBox(IDS_CDPINI_DOUBLEENTRY, MB_OK+MB_ICONINFORMATION);
	}
	else{

		AfxMessageBox(IDS_CDPINI_WRITE_OK, MB_OK+MB_ICONINFORMATION);
	}
}



void CLameFEView::OnInitialUpdate() 
{
	
	SetTimer(TIMERID2, 5000, NULL);

	CFormView::OnInitialUpdate();
	m_ctrlList.InsertColumn(0, "Track", LVCFMT_RIGHT, 80);
	m_ctrlList.InsertColumn(1, "Title", LVCFMT_LEFT, 300);
	
	m_listBkImage.LoadBitmap(IDB_LIST_BK); //"D:\\cpp\\lameFE2\\res\\list_bk.bmp"
	m_ctrlList.SetBkImage((HBITMAP)m_listBkImage, FALSE, 90, 90);
	m_ctrlList.SetTextBkColor(CLR_NONE);
	m_ctrlList.Init();
	// Create imaglist and attach it to the tracklist
	// Create 256 color image lists
	HIMAGELIST hList = ImageList_Create(16,16, ILC_COLOR8 |ILC_MASK , 3, 1);
	m_cImageList.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_IMAGES);
	m_cImageList.Add(&cBmp, RGB(255,0, 255));
	cBmp.DeleteObject();

	m_ctrlList.SetImageList(&m_cImageList, LVSIL_SMALL);

	m_aAlbumInfoCtrl.Create(IDD_ALBUMINFOCTRL, this);

	TCHAR	szBuffer[_MAX_PATH]; 
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
	
	wd = szBuffer;
	wd = wd.Left(wd.ReverseFind('\\'));

	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
	   TRACE("Unable To create ToolTip\n");
	}
	else{

		m_pToolTip->AddTool(&c_configureIn, "Configure Input Device");
		m_pToolTip->AddTool(&c_configureOut, "Configure Output Device");
		m_pToolTip->AddTool(&c_inputDevice, "Select Input Source / Device");
		m_pToolTip->AddTool(&c_outputDevice, "Select Target Encoder");
	}

	// Set Icons to the configure buttons

	int ICON = IDI_CONFIG_OUT;
	HANDLE hIcon = LoadImage(
			AfxGetInstanceHandle(),
			reinterpret_cast<LPCSTR>(ICON),
			IMAGE_ICON,
			16,
			16,
			LR_DEFAULTCOLOR
			);
	c_configureOut.SetIcon(static_cast<HICON>(hIcon));

	ICON = IDI_CONFIG_IN;
	hIcon = LoadImage(
			AfxGetInstanceHandle(),
			reinterpret_cast<LPCSTR>(ICON),
			IMAGE_ICON,
			16,
			16,
			LR_DEFAULTCOLOR
			);
	c_configureIn.SetIcon(static_cast<HICON>(hIcon));


	m_pStatus	 = (CStatusBar *)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	m_pStatus->SetPaneText(1,"LameFE");

	m_pToolBar	 = (CToolBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_TOOLBAR);

	GetInputDevices();
	GetOutputDevices();

	cfgFile cfg(wd);

	if(cfg.GetValue("showtagedit")){
		
		OnId3tagsId3tageditor();
	}

	m_bCheckCD = cfg.GetValue("CheckForNewCD");

	ResizeControls();
	ReadCDContents();
}

void CLameFEView::OnSelchangeDevices(){

	OnSelchangeDevices(TRUE);
}

void CLameFEView::OnSelchangeDevices(BOOL bReset){


	if(IsPluginMode()){
		
		if(bReset){

			m_ctrlList.DeleteAllItems();
			ResetFileList();
		}
	}
	else{

		if(nNumCDDrives == 0){

			return;
		}
		m_ctrlList.DeleteAllItems();
		ResetFileList();
		CR_SetActiveCDROM(c_inputDevice.GetCurSel());
		CR_SaveSettings();
		ReadCDContents();
	}

}

BOOL CLameFEView::GetOutputDevices()
{

	TRACE("Entering CLameFEView::GetOutputDevices()\n");
	// We have always the lame encoder as output device:
	c_outputDevice.AddString("MPEG I/II Layer 3 (lame_enc.dll)");
	
	// check for output plugins

	CFileFind finder;
	CString	  strTmp;
	BOOL bResult = finder.FindFile(wd + "\\Plugins\\*_out.dll");
	while(bResult){

		bResult = finder.FindNextFile();
		
		TRACE("Added plugin %s.\n", finder.GetFileName());
		COutPlugin	tmp(finder.GetFilePath(), wd + "\\lameFE.ini");
		tmp.Load();
		strTmp.Format("%s (%s)", tmp.GetInfoString(), finder.GetFileName());
		c_outputDevice.AddString(strTmp);
		tmp.Unload();
	}

	c_outputDevice.SetCurSel(0);

	TRACE("Entering CLameFEView::GetOutputDevices()\n");
	return TRUE;
}

BOOL CLameFEView::GetInputDevices()
{

	TRACE("Entering CLameFEView::GetInputDevices()\n");

	// We have always CD-Ripping as input device:
	// So Init CD-Ripper and iterate through all available drives.
	
	USES_CONVERSION;

	CDEX_ERR cResult = CDEX_OK;

	nNumCDDrives = 0;

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
				AfxMessageBox(IDS_FAILEDASPINOTSELECTED, MB_OK+MB_ICONSTOP);
			break;
			case CDEX_ERROR:
				AfxMessageBox("Error loading library", MB_OK+MB_ICONSTOP);
				break;
			case CDEX_OK:
				TRACE("why are we here? h‰‰‰‰\n");
			break;
			default:
				ASSERT( FALSE );
		}
	}
	else{  // Ripper status is OK enumerate drives and load settings...

		CDROMPARAMS cdParams;
		nNumCDDrives = 0;


		int nSelCD = CR_GetActiveCDROM();
		CString tmp;

		for (int i = 0; i < CR_GetNumCDROM(); i++)
		{

			CR_SetActiveCDROM(i);
			CR_GetCDROMParameters(&cdParams);
			tmp.Format("CD: %s", cdParams.lpszCDROMID);
			c_inputDevice.AddString(tmp);
			nNumCDDrives++;
		}
		
		CR_SetActiveCDROM(nSelCD);

		c_inputDevice.SetCurSel(nSelCD);
	}

	//Check if there are any inputplugins
	CFileFind finder;
	BOOL bResult = finder.FindFile(wd + "\\Plugins\\*_in.dll");
	while(bResult){

		bResult = finder.FindNextFile();
		
		TRACE("Added plugin %s.\n", finder.GetFileName());
		CLameFEPlugin	tmp(finder.GetFilePath());
		tmp.Load();
		tmp.Unload();
		m_paPlugins.Add(tmp);
	}

	if(m_paPlugins.GetSize() > 0){
		
		CString strDevice;
		strDevice.LoadString(IDS_AUDIOFILEPLUGIN);
		c_inputDevice.AddString(strDevice);
	}

	TRACE("Leaving CLameFEView::GetInputDevices()\n");
	return TRUE;
}

void CLameFEView::OnViewReloadcd() 
{

	if(IsPluginMode()){

		return;
	}

	ReadCDContents();

}

void CLameFEView::OnViewSelectalltracksfiles() 
{

	m_ctrlList.SelectAll();
}

void CLameFEView::ReadCDContents()
{

	if(nNumCDDrives == 0){

		return;
	}

	CWaitCursor wCursor;
	m_pStatus->SetPaneText(1,"Reading out CD Contents");
	
	m_ctrlList.DeleteAllItems();
	
	CDEX_ERR	bErr;
	//Sleep(400);
	
	CString  track;
	if (CR_IsUnitReady() == 0x01)
	{
		
		bErr = CR_LoadSettings();

		if(bErr != CDEX_OK){

			track.Format(IDS_DRIVENOTREADY, bErr);
			AfxMessageBox(track, MB_OK+MB_ICONSTOP);
			return;
		}
		
		m_compactDisc.Init();

		cfgFile cfg(wd);

		if(cfg.GetValue("readcdtext")){

			m_compactDisc.ReadCDText();
		}

		if(cfg.GetValue("readcdplayerini")){

			CCDPlayerIni cdini(&m_compactDisc);
			cdini.Read();
		}

		RefreshTrackList();

		if(cfg.GetValue("select", TRUE)){

			m_ctrlList.SelectAll();
			SetAlbumInfo();
		}

		UpdateData(FALSE);
	}
	else{

		m_ctrlList.DeleteAllItems();
		m_compactDisc.Eject();
	}
	m_pStatus->SetPaneText(1,"");
}

void CLameFEView::OnTimer(UINT nIDEvent) 
{

	
	(AfxGetMainWnd())->SetWindowText(STR_VERSION);

	if((nNumCDDrives == 0) || (IsPluginMode()) || ! IsWindowVisible() || !m_bCheckCD){

		return;
	}

	
	if(CR_IsUnitReady() == 0x01){


		m_mtLock.Lock();
		CCompactDisc cdTmp;
		cdTmp.Init();

		if(cdTmp.GetDiscID() != m_compactDisc.GetDiscID()){
			
			m_ctrlList.DeleteAllItems();
			ReadCDContents();
		}
		m_mtLock.Unlock();
	}
	else{

		m_compactDisc.Eject();
		m_ctrlList.DeleteAllItems();
	}

	CFormView::OnTimer(nIDEvent);
}

void CLameFEView::ShowSettingsDialog(int nTab)
{

	int nActiveCD = c_inputDevice.GetCurSel();
	CSettingsSheet settings("lameFE Settings", this, nTab);
	settings.init(wd, (nNumCDDrives > 0 ? TRUE : FALSE));
	settings.DoModal();
	c_inputDevice.SetCurSel(nActiveCD);

	CR_SetActiveCDROM(nActiveCD);
	CR_SaveSettings();
	cfgFile cfg(wd);
	m_bCheckCD = cfg.GetValue("CheckForNewCD");
}

void CLameFEView::OnSettingsPreferences() 
{
	ShowSettingsDialog(0);
}

void CLameFEView::OnSettingsPlugins() 
{

	ShowSettingsDialog(3);
}

void CLameFEView::OnSettingsLameencoder() 
{

	ShowSettingsDialog(4);
}

void CLameFEView::OnSettingsId3tagsfilenames() 
{

	ShowSettingsDialog(1);
}

void CLameFEView::OnSettingsCdreader() 
{

	ShowSettingsDialog(2);
}

void CLameFEView::OnSettingsLogging() 
{
	
	ShowSettingsDialog(5);
}

void CLameFEView::OnSettingsFreedbsetup() 
{

	ShowSettingsDialog(6);
}
void CLameFEView::OnConfigureIn()
{

	if(!IsPluginMode()){

		OnSettingsCdreader();
	}
	else{

		OnSettingsPlugins();
	}
}

void CLameFEView::OnConfigureOut()
{

	UpdateData(TRUE);
	
	CString dllName = m_strOutputDevice.Mid(m_strOutputDevice.ReverseFind('(') +1, m_strOutputDevice.GetLength() - m_strOutputDevice.ReverseFind('(') - 2);
	
	if(dllName == "lame_enc.dll"){

		OnSettingsLameencoder();
	}
	else{

		COutPlugin tmp(wd + "\\Plugins\\" + dllName, wd + "\\lameFE.ini");
		
		if(!tmp.Load()){

			CString strTmp;
			strTmp.Format(IDS_ENC_PLUG_LOADFAILED, dllName);
			AfxMessageBox(strTmp, MB_OK+MB_ICONEXCLAMATION);
			return;
		}
		tmp.Configure();
		tmp.Unload();
	}
	
}

void CLameFEView::OnFileOpen() 
{

	TRACE("Entering CLameFEDlg::OnAddFile()\n");
	
	cfgFile lcfg(wd);
	
	// Get all supported filetypes of all input plugins
	CString tmpSzFilter, szFilter, strAllExt;
	
	for(int i = 0; i < m_paPlugins.GetSize(); i++){

		tmpSzFilter  += m_paPlugins[i].GetSzFilter();
		strAllExt += m_paPlugins[i].GetExtension() + ";";
	}
	strAllExt.Replace("*.;", "");

	//All Files (*.abc;*.cde)|*.abc; *.cde
	szFilter.Format("All supported Audio Files (%s)|%s|", strAllExt, strAllExt);
	szFilter += tmpSzFilter + "All Files (*.*)|*.*|";

	CMyFileDialog FileOpen(TRUE, 0, 0, OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT  , szFilter, 0);
	
	FileOpen.m_ofn.lpstrInitialDir = (const char*)lcfg.GetStringValue("output");

	CString strFileName;
	int maxChar = 1000;
	FileOpen.m_ofn.lpstrFile = strFileName.GetBuffer(maxChar);
	FileOpen.m_ofn.nMaxFile = maxChar;


	int nResponse = FileOpen.DoModal();
	strFileName.ReleaseBuffer();
	if (nResponse == IDOK){
	
		POSITION pos = FileOpen.GetStartPosition();
		CString path;
		
		while (pos != NULL){
		
			TRACE("Processing files (pos=%d)\n", pos);
			path = FileOpen.GetNextPathName(pos);
			
			int		nItem = m_ctrlList.GetItemCount();
			CString strTmp;
			strTmp.Format("%02d", nItem + 1);
			m_ctrlList.InsertItem(nItem, strTmp, 2);
			m_ctrlList.SetItemText(nItem, 1, path);
			CMultimediaFile *tmp = new CMultimediaFile(path);
			m_mmFiles.Add(tmp);			
		}
	}

	UpdateData(FALSE);

	TRACE("Leaving CLameFEDlg::OnAddFile\n");
}

void CLameFEView::OnRemoveFile() 
{

	if(!IsPluginMode()){

		AfxMessageBox(IDS_MAIN_ADDFILECD, MB_OK+MB_ICONEXCLAMATION);
		return;
	}
	
	if (m_ctrlList.GetItemCount() != 0 ){
	
		UpdateData(TRUE);
		int iCnt,i;
		iCnt = m_ctrlList.GetSelCount();
		
		if (iCnt != 0){
		
			int *pIndex;
			pIndex = new int[iCnt];
			m_ctrlList.GetSelItems(iCnt, pIndex);
			for(i = iCnt-1; i >= 0; i--){

				m_ctrlList.DeleteItem(pIndex[i]);
				CMultimediaFile *tmp = m_mmFiles.GetAt(pIndex[i]);
				m_mmFiles.RemoveAt(pIndex[i]);
				delete tmp;
			}
			
			delete pIndex;
		}
	}
	else{
		
		AfxMessageBox(IDS_EMPTYLIST,MB_OK+MB_ICONINFORMATION,0);
	}
}

void CLameFEView::ResetFileList()
{

	CMultimediaFile *tmp;

	for(int i = m_mmFiles.GetSize() - 1; i >= 0; i--){

		tmp = m_mmFiles.GetAt(i);
		m_mmFiles.RemoveAt(i);
		delete tmp;
	}

	m_mmFiles.FreeExtra();
	m_mmFiles.SetSize(0);
}



void CLameFEView::OnDestroy() 
{
	
	KillTimer(TIMERID2);

	OnId3tagsId3tageditor();
	m_ctrlList.DeleteAllItems();
	ResetFileList();

	if(m_hCDRipDll != NULL){
		
		CR_LockCD(FALSE); //Just to be sure :)
		CR_DeInit();
		FreeLibrary(m_hCDRipDll);
	}

	cfgFile cfg;
	RECT rc;
	AfxGetApp()->m_pMainWnd->GetWindowRect(&rc);
	cfg.SetValue("window.x", rc.left);
	cfg.SetValue("window.y", rc.top);
	cfg.SetValue("window.cx", rc.right-rc.left);
	cfg.SetValue("window.cy", rc.bottom-rc.top);

	CFormView::OnDestroy();

}

void CLameFEView::RefreshTrackList()
{

	m_pStatus->SetPaneText(1,"Updating tracklist");
	m_ctrlList.DeleteAllItems();
	CString tmp;

	for(int i = 0; i < m_compactDisc.GetNumTracks(); i++){

		tmp.Format("%02d", i+1);
		if(m_compactDisc.GetCDTrack(i)->IsAudioTrack()){

			m_ctrlList.InsertItem(i, tmp, 0);
			tmp.Format("%s - %s", m_compactDisc.GetCDTrack(i)->m_id3Info.GetArtist(), m_compactDisc.GetCDTrack(i)->m_id3Info.GetSong());
		}
		else{
			
			m_ctrlList.InsertItem(i, tmp, 1);
			tmp.Format("%s", m_compactDisc.GetCDTrack(i)->GetTrackname());
		}

		m_ctrlList.SetItemText(i ,1, tmp);
	}
	m_pStatus->SetPaneText(1,"");
}



void CLameFEView::OnFileStartAlbum() 
{

	OnViewSelectalltracksfiles();
	StartEncoding(ALBUMMODE);
}

void CLameFEView::OnFileStartBatchAlbum()
{

	OnViewSelectalltracksfiles();
	StartEncoding(BATCHALBUMMODE);
}

void CLameFEView::OnFileStartencoding() 
{

	StartEncoding(NORMALMODE);
}

void CLameFEView::StartEncoding(modes mEMode)
{

	int     nInputDevice    = c_inputDevice.GetCurSel();
	cfgFile cfg(wd);

	if((!m_compactDisc.GetNumTracks() && nInputDevice  < nNumCDDrives) ||
		(!m_mmFiles.GetSize() && nInputDevice  >= nNumCDDrives)){

		AfxMessageBox(IDS_EMPTYLIST, MB_OK+MB_ICONEXCLAMATION);
		return;
	}

	// Get DLL name of output Device...
	int     nOutputDevice   = c_outputDevice.GetCurSel();
	CString strOutputDevice, dllName;
	c_outputDevice.GetLBText(nOutputDevice, strOutputDevice);
	dllName = strOutputDevice.Mid(strOutputDevice.ReverseFind('(') +1, strOutputDevice.GetLength() - strOutputDevice.ReverseFind('(') - 2);

	
	CEncodingStatusDlg esdlg(this, wd);
	
	if(nInputDevice < nNumCDDrives){  //input cd

		UpdateData(TRUE);
		int iCnt; 

		iCnt = m_ctrlList.GetSelCount();

		if(!iCnt){
			
			AfxMessageBox(IDS_NOTRACKSSELECTED, MB_OK+MB_ICONINFORMATION);
			return;
		}

		int iAudioCnt = 0, j = 0;
		int *pIndex, *pAudioVerified;

		pIndex = new int[iCnt];

		m_ctrlList.GetSelItems(iCnt, pIndex);

		//verify that all selected tracks are audio tracks
		for(int i = 0; i < iCnt; i++){
			
			if(m_compactDisc.GetCDTrack(pIndex[i])->IsAudioTrack()){

				iAudioCnt++;
			}
			else{

				CString strTmp;
				strTmp.Format(IDS_DATATRACK, pIndex[i] + 1);
				AfxMessageBox(strTmp, MB_OK+MB_ICONINFORMATION);
			}
		}
		//Fill Audiotracks selection array 
		pAudioVerified = new int[iAudioCnt];
		for(i = 0; i < iCnt; i++){

			if(m_compactDisc.GetCDTrack(pIndex[i])->IsAudioTrack()){

				pAudioVerified[j++] = pIndex[i];
			}
		}
		m_compactDisc.SetSelItems(iAudioCnt, pAudioVerified);
		
		delete pIndex;
		delete pAudioVerified;

		if(!m_compactDisc.GetNumSelTracks()){

			AfxMessageBox(IDS_NOTRACKSSELECTED, MB_OK+MB_ICONINFORMATION);
			return;
		}


		esdlg.SetJob(RIP_TO_ENCODER, dllName, mEMode); 
		esdlg.SetCDROM(&m_compactDisc);
	}
	else{  //input from file
		
		esdlg.SetJob(ANY_TO_ENCODER, dllName); 
		esdlg.SetFiles(&m_mmFiles);
	}
	
	if(cfg.GetValue("hideduringenc")){

		GetTopLevelParent()->ShowWindow(SW_MINIMIZE);
		GetTopLevelParent()->ShowWindow(SW_HIDE);
	}
	m_bCheckCD = FALSE;

	int nResult = esdlg.DoModal();
	
	GetTopLevelParent()->ShowWindow(SW_SHOWNORMAL);
	m_bCheckCD = cfg.GetValue("CheckForNewCD");
	
	if(nResult == IDCANCEL){

		TRACE("Dont`exit lameFE or shutdown windows. Encoding cancelled\n");
		return;
	}

	//cfgFile cfg(wd);
	if(cfg.GetValue("quit")){

		//AfxGetApp()->ExitInstance();
		OnAppExit();
	}

	if(cfg.GetValue("shutdown")){

		if(!ShutDown()){

			AfxMessageBox(IDS_ERR_SHUTDOWN, MB_OK+MB_ICONSTOP);
		}
	}
}


BOOL CLameFEView::ShutDown()
{

	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tp, tpPrevious;
	DWORD cbPrevious = sizeof( TOKEN_PRIVILEGES );
	BOOL bSuccess = FALSE;

	if ( ! LookupPrivilegeValue( NULL, static_cast<CString>( SE_SHUTDOWN_NAME), &luid ) )
		return FALSE;

	if( ! OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken ) )
		return FALSE;

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = 0;

	AdjustTokenPrivileges( hToken, FALSE, &tp, sizeof( TOKEN_PRIVILEGES ), &tpPrevious, &cbPrevious );

	if ( GetLastError() == ERROR_SUCCESS )
	{
		tpPrevious.PrivilegeCount = 1;
		tpPrevious.Privileges[0].Luid = luid;


		tpPrevious.Privileges[0].Attributes |= ( SE_PRIVILEGE_ENABLED );

		AdjustTokenPrivileges( hToken, FALSE, &tpPrevious, cbPrevious, NULL, NULL );

		if ( GetLastError() == ERROR_SUCCESS )
			bSuccess=TRUE;
	}

	CloseHandle( hToken );

	InitiateSystemShutdown(NULL, "System shutdown", 10, FALSE, FALSE);
	return TRUE;
}

void CLameFEView::OnAppExit() 
{
	
	OnDestroy();
	AfxGetApp()->HideApplication();
	AfxGetApp()->CloseAllDocuments(FALSE);
	AfxGetApp()->ExitInstance();
}

void CLameFEView::OnUpdateToolBar(CCmdUI* pCmdUI) 
{

	pCmdUI->Enable(IsPluginMode());
}

void CLameFEView::OnUpdateCDControls(CCmdUI* pCmdUI) 
{

	pCmdUI->Enable(!IsPluginMode());
}


BOOL CLameFEView::PreTranslateMessage(MSG* pMsg) 
{

	if (NULL != m_pToolTip){
		
		m_pToolTip->RelayEvent(pMsg);
	}
	
	return CFormView::PreTranslateMessage(pMsg);
}



void CLameFEView::OnDropFiles(HDROP hDropInfo) 
{

	TRACE("Entering CLameFEView::OnDropFiles(...)\n");

	if(!IsPluginMode()){

		AfxMessageBox(IDS_CDMODENODROP, MB_OK+MB_ICONSTOP);
		return;
	}

	int nTotalFiles = ::DragQueryFile(hDropInfo, (UINT)-1,NULL, 0);
	
	// Get all supported filetypes of all input plugins
	CString strAllExt;
	
	for(int i = 0; i < m_paPlugins.GetSize(); i++){

		strAllExt += m_paPlugins[i].GetExtension() + ";";
	}
	strAllExt.Replace("*.;", "");
	strAllExt.MakeLower();

	int nPos = 0;

	for(i = 0; i < nTotalFiles; i++){

		TCHAR	 lpszFileName[1000];
		CString  strFileName;
		CString  strFileExt;
		// Get drop file name
		::DragQueryFile( hDropInfo, i, lpszFileName, sizeof( lpszFileName ) );

		strFileName = lpszFileName;
		strFileExt  = strFileName.Right(3);
		strFileExt.MakeLower();

		if(strAllExt.Find(strFileExt, 0) == -1){
			
			CString strTmp;
			strTmp.Format(IDS_UNSUPPORTEDFILE, strFileName);

			AfxMessageBox(strTmp, MB_OK+MB_ICONINFORMATION);
		}
		else{
			
			CString strTmp;
			strTmp.Format("%02d", nPos + 1);
			m_ctrlList.InsertItem(nPos, strTmp, 2);
			m_ctrlList.SetItemText(nPos, 1, strFileName);
			CMultimediaFile *tmp = new CMultimediaFile(strFileName);
			m_mmFiles.Add(tmp);
			nPos++;
		}
	}
}


void CLameFEView::OnClickFilesTracks(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	SetAlbumInfo();
	//*pResult = 0;
}

void CLameFEView::ResizeControls()
{

	if (!m_ctrlList.m_hWnd){

		return;
	}

	CRect	rcList;
	CRect	rcParent;

	GetClientRect(rcParent);

	// Get rect of track-list
	m_ctrlList.GetWindowRect( rcList );

	// Aha, we need to convert the coordinates (client!!) otherwise there might happen funny stuff :-)
	ScreenToClient( rcList );
	
	// margin...
	rcList.right = rcParent.right - 10;
	// if the tag-editor is visible we need some space at under the tracklist for it
	rcList.bottom = rcParent.bottom - (m_bTagEditorVisible ? 100 : 0);
	// adjust the window size
	m_ctrlList.MoveWindow(rcList);

	if(m_bTagEditorVisible){

		// move the tag-editor window also if visible
		m_aAlbumInfoCtrl.SetWindowPos(NULL, rcList.left, rcList.bottom, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	}
}

void CLameFEView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	ResizeControls();
		
}


void CLameFEView::SetAlbumInfo()
{

	TRACE("Entering CLameFEView::SetAlbumInfo()\n");

	m_nSelItem = m_ctrlList.GetCurSel();

	if((m_ctrlList.GetSelCount()) > 0 && (m_nSelItem >= 0)){
			
		m_aAlbumInfoCtrl.EnableControls(TRUE);
		if(IsPluginMode()){

			m_aAlbumInfoCtrl.SetInfo(&m_mmFiles[m_nSelItem]->m_id3Info);
		}
		else{
				
			m_aAlbumInfoCtrl.SetInfo(&m_compactDisc.GetCDTrack(m_nSelItem)->m_id3Info);
			
			if(!m_compactDisc.GetCDTrack(m_nSelItem)->IsAudioTrack()){
				
				m_aAlbumInfoCtrl.EnableControls(FALSE);
			}
		}
	}
	else{
		
		m_aAlbumInfoCtrl.Clear();
		m_aAlbumInfoCtrl.EnableControls(FALSE);
	}
}


BOOL CLameFEView::IsPluginMode()
{

	return c_inputDevice.GetCurSel() >= nNumCDDrives;
}

