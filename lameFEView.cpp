/*
** Copyright (C) 2002-2003 Thees Winkler
**  
** Parts of this code (c) by Albert L. Faber
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
#include "Settings.h"
#include "CDTrack.h"
#include "CDdbQueryDlg.h"
#include "mfccddb.h"
#include "CPlugin.h"
#include "MyFileDialog.h"
#include "SettingsDlg.h"
#include "CheckNewVersion.h"
#include "PresetSaveDlg.h"
#include "MainFrm.h"
#include "Utils.h"
#include <direct.h>
#include "I18n.h"
#include "EncoderPlugin.h"
#include "EncoderLame.h"
#include "MusicBrainz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const UINT UWM_ALBUMINFO_UPDATED = ::RegisterWindowMessage(_T("UWM_RESET_VIEW--{4E7F6EC0-6ADC-11d3-BC36-006067709674}"));
static const UINT UWM_LISTCTRL_KEYUP	= ::RegisterWindowMessage(_T("UWM_LISTCTRL_KEYUP--{4E7F6EC0-6ADC-11d3-BC36-006067709674}"));
static const UINT UWM_PRESETSELCHANGED  = ::RegisterWindowMessage(_T("UWM_PRESETSELCHANGED--{4E7F6EC-6ADC-11d3-BC36-006067709674}"));
static const UINT UWM_TAG_COMMAND = ::RegisterWindowMessage(_T("UWM_TAG_COMMAND--{4E7F6EC0-6ADC-11d3-BC36-006067709674}"));

extern CSettings g_sSettings;
extern CI18n g_iLang;

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
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_REMOVE_FILE, OnRemoveFile)
	ON_COMMAND(ID_FILE_STARTENCODING, OnFileStartencoding)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_VIEW_RELOADCD, OnViewReloadcd)
	ON_COMMAND(ID_VIEW_SELECTALLTRACKSFILES, OnViewSelectalltracksfiles)
	ON_COMMAND(ID_ID3TAGS_SAVECUESHEET, OnId3tagsSavecuesheet)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateToolBar)
	ON_UPDATE_COMMAND_UI(ID_ID3TAGS_READFREEDBSERVER, OnUpdateCDControls)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_FILES_TRACKS, OnClickFilesTracks)
	ON_WM_SIZE()
	ON_COMMAND(ID_ID3TAGS_ID3TAGEDITOR, OnId3tagsId3tageditor)
	ON_COMMAND(ID_CHECKFORNEWVESION, OnCheckfornewvesion)
	ON_COMMAND(ID_SETTINGS, OnSettings)
	ON_COMMAND(ID_HELP_REPORTABUG, OnHelpReportabug)
	ON_CBN_SELCHANGE(IDC_OUTPUT_DEVICE, OnSelchangeOutputDevice)
	ON_COMMAND(ID_SAVEPRESET, OnSavepreset)
	ON_COMMAND(ID_DELETEPRESET, OnDeletepreset)
	ON_NOTIFY(NM_RCLICK, IDC_FILES_TRACKS, OnRclickFilesTracks)
	ON_COMMAND(ID_AUTO_RIP, OnAutoRip)
	ON_UPDATE_COMMAND_UI(ID_AUTO_RIP, OnUpdateAutoRip)
	ON_COMMAND(ID_INVERT_SEL, OnInvertSel)
	ON_COMMAND(ID_RIP_CD, OnRipCd)
	ON_COMMAND(ID_CD_ABLUM_RIP, OnCdAblumRip)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_FILE, OnUpdateToolBar)
	ON_UPDATE_COMMAND_UI(ID_FILE_STARTENCODING, OnUpdateToolBar)
	ON_UPDATE_COMMAND_UI(ID_ID3TAGS_READCDPLAYERINI, OnUpdateCDControls)
	ON_UPDATE_COMMAND_UI(ID_ID3TAGS_READCDTEXT, OnUpdateCDControls)
	ON_UPDATE_COMMAND_UI(ID_ID3TAGS_SAVECUESHEET, OnUpdateCDControls)
	ON_UPDATE_COMMAND_UI(ID_ID3TAGS_SAVETOCDPLAYERINI, OnUpdateCDControls)
	ON_UPDATE_COMMAND_UI(ID_RIP_CD, OnUpdateCDControls)
	ON_UPDATE_COMMAND_UI(ID_CD_ABLUM_RIP, OnUpdateCDControls)
	ON_UPDATE_COMMAND_UI(ID_BATCH_SINGLETRACKS, OnUpdateCDControls)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RELOADCD, OnUpdateCDControls)
	ON_COMMAND(ID_ALBUMTAGS_QUERYMUSICBRAINZDATABASE, OnQuerymusicbrainzdatabase)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(UWM_ALBUMINFO_UPDATED, OnAlbumInfoUpdated)
	ON_REGISTERED_MESSAGE(UWM_TAG_COMMAND, OnAlbumTagCommand)
	ON_REGISTERED_MESSAGE(UWM_LISTCTRL_KEYUP, OnClickFilesTracks)
	ON_REGISTERED_MESSAGE(UWM_PRESETSELCHANGED, OnPresetSelChanged)
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

//	wd = g_sSettings.GetWorkingDir();

	m_strPresetPath = g_sSettings.GetPresetPath();
	
	m_bPaused			= FALSE;
	m_bIsEjected		= FALSE;
	m_pStatus			= NULL;
	m_pToolBar			= NULL;
	m_pToolTip			= NULL;
	m_wndPresetBar		= NULL;	
	m_nSelItem			= NULL;
	m_bCheckCD			= FALSE;
	m_bTagEditorVisible	= FALSE;
	m_bAutoRipMode      = FALSE;
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
	// ZU ERLEDIGEN: Hier Code zum Zeichnen der ursprünglichen Daten hinzufügen
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

CLameFEDoc* CLameFEView::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
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

		AfxMessageBox(g_iLang.GetString(IDS_ID3_CHOOSE_FILE), MB_OK+MB_ICONSTOP);
		return;
	}
	
	int *pIndex = new int[nSelCnt];
	m_ctrlList.GetSelItems(nSelCnt, pIndex);

	if(IsPluginMode()){

		m_mmFiles[m_nSelItem]->m_id3Info.Copy(m_aAlbumInfoCtrl.GetTag());
	}
	else{

		m_cCD.GetCDTrack(m_nSelItem)->m_id3Info.Copy(m_aAlbumInfoCtrl.GetTag());
	}

	for(int i = 0; i < nSelCnt; i++){
		
		if(IsPluginMode())
			m_mmFiles[pIndex[i]]->m_id3Info.Copy(m_aAlbumInfoCtrl.GetTag(), FALSE);
		else
			m_cCD.GetCDTrack(pIndex[i])->m_id3Info.Copy(m_aAlbumInfoCtrl.GetTag(), FALSE);
	}

	if(!IsPluginMode()){

		CString strTmp;

		for(i = 0; i < m_ctrlList.GetItemCount(); i++){

			if(!m_cCD.GetCDTrack(i)->IsAudioTrack()) continue;

			strTmp.Format("%s - %s", m_cCD.GetCDTrack(i)->m_id3Info.GetArtist(), m_cCD.GetCDTrack(i)->m_id3Info.GetSong());
			m_ctrlList.SetItemText(i, 1, strTmp);
		}
	}
	delete pIndex;
	TRACE("Leaving CLameFEView::OnAlbumInfoUpdated()\n");
}

void CLameFEView::OnAlbumTagCommand(WPARAM wParam, LPARAM lParam)
{

/*
#define AIC_SWAP_ARTISTALBUM	0
#define AIC_SWAP_TITLEARTIST	1
#define AIC_CORRECT_CASE		2
#define AIC_CLEAR_TAG			3
#define AIC_COPY_V1_V2			4
#define AIC_COPY_V2_V1			5
#define AIC_FIX_VARIOUS			6
#define AIC_CLEANUP				7
*/
	int nSelCnt = m_ctrlList.GetSelCount();
	int i = 0;

	if(nSelCnt == 0){

		AfxMessageBox(g_iLang.GetString(IDS_ID3_CHOOSE_FILE), MB_OK+MB_ICONSTOP);
		return;
	}
	
	int *pIndex = new int[nSelCnt];
	m_ctrlList.GetSelItems(nSelCnt, pIndex);

	switch(wParam){

	case AIC_SWAP_ARTISTALBUM:

		for(i = 0; i < nSelCnt; i++){
			
			if(IsPluginMode()){

				m_mmFiles[pIndex[i]]->m_id3Info.SwapAlbumInterpret((ID3T_Tag)lParam);
			}
			else{

				m_cCD.GetCDTrack(pIndex[i])->m_id3Info.SwapAlbumInterpret((ID3T_Tag)lParam);
			}
		}
		break;

	case AIC_SWAP_TITLEARTIST:

		for(i = 0; i < nSelCnt; i++){
			
			if(IsPluginMode()){

				m_mmFiles[pIndex[i]]->m_id3Info.SwapTitleInterpret((ID3T_Tag)lParam);
			}
			else{

				m_cCD.GetCDTrack(pIndex[i])->m_id3Info.SwapTitleInterpret((ID3T_Tag)lParam);
			}
		}
		break;

	case AIC_CORRECT_CASE:

		for(i = 0; i < nSelCnt; i++){
			
			if(IsPluginMode()){

				m_mmFiles[pIndex[i]]->m_id3Info.Case((ID3T_Tag)lParam);
			}
			else{

				m_cCD.GetCDTrack(pIndex[i])->m_id3Info.Case((ID3T_Tag)lParam);
			}
		}
		break;

	case AIC_CLEAR_TAG:

		for(i = 0; i < nSelCnt; i++){
			
			if(IsPluginMode()){

				m_mmFiles[pIndex[i]]->m_id3Info.Empty((ID3T_Tag)lParam);
			}
			else{

				m_cCD.GetCDTrack(pIndex[i])->m_id3Info.Empty((ID3T_Tag)lParam);
			}
		}
		break;

	case AIC_COPY_V1_V2:
	case AIC_COPY_V2_V1:

		for(i = 0; i < nSelCnt; i++){
			
			if(IsPluginMode()){

				m_mmFiles[pIndex[i]]->m_id3Info.Sync((ID3T_Tag)lParam);
			}
			else{

				m_cCD.GetCDTrack(pIndex[i])->m_id3Info.Sync((ID3T_Tag)lParam);
			}
		}
		break;

	case AIC_FIX_VARIOUS:

		for(i = 0; i < nSelCnt; i++){
			
			if(IsPluginMode()){

				m_mmFiles[pIndex[i]]->m_id3Info.FixVariousArtistCD((ID3T_Tag)lParam);
			}
			else{

				m_cCD.GetCDTrack(pIndex[i])->m_id3Info.FixVariousArtistCD((ID3T_Tag)lParam);
			}
		}
	case AIC_FIX_NAME:

		for(i = 0; i < nSelCnt; i++){
			
			if(IsPluginMode()){

				m_mmFiles[pIndex[i]]->m_id3Info.FixArtistName((ID3T_Tag)lParam);
			}
			else{

				m_cCD.GetCDTrack(pIndex[i])->m_id3Info.FixArtistName((ID3T_Tag)lParam);
			}
		}
		break;
	default:
		ASSERT(FALSE);
		return;
	}
	if(!IsPluginMode()){

		CString strTmp;

		for(i = 0; i < m_ctrlList.GetItemCount(); i++){

			if(!m_cCD.GetCDTrack(i)->IsAudioTrack()) continue;
			strTmp.Format("%s - %s", m_cCD.GetCDTrack(i)->m_id3Info.GetArtist(), m_cCD.GetCDTrack(i)->m_id3Info.GetSong());
			m_ctrlList.SetItemText(i, 1, strTmp);
		}
	}
	delete pIndex;
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
	
	if(!m_cCD.GetNumAudioTracks()){
		
		AfxMessageBox(g_iLang.GetString(IDS_MAIN_NOAUDIOCD), MB_OK+MB_ICONSTOP);
		return;
	}

	int iCnt = m_ctrlList.GetSelCount();
	int *pIndex = new int[iCnt];
	m_ctrlList.GetSelItems(iCnt, pIndex);

	CCDPlayerIni cdPlayerIni(&m_cCD);
	if(!cdPlayerIni.Read()){

		AfxMessageBox(g_iLang.GetString(IDS_CDPINI_NOENTRY), MB_OK+MB_ICONINFORMATION);
		return;
	}
	
	RefreshTrackList();
	m_ctrlList.SetSelItems(iCnt, pIndex);
	SetAlbumInfo();
	delete pIndex;
}

void CLameFEView::OnId3tagsReadcdtext() 
{

	if(!m_cCD.ReadCDText()){

		AfxMessageBox(g_iLang.GetString(IDS_CDTEXT_ERROR), MB_OK+MB_ICONINFORMATION);
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


	if(!m_cCD.GetNumAudioTracks()){
		
		AfxMessageBox(g_iLang.GetString(IDS_MAIN_NOAUDIOCD), MB_OK+MB_ICONSTOP);
		delete pIndex;
		return;
	}
	CWaitCursor wc;

	CCDdbQueryDlg cddbDlg(this, &m_cCD, CR_GetActiveCDROM());
	int nResult = cddbDlg.DoModal();

	RefreshTrackList();
	m_ctrlList.SetSelItems(iCnt, pIndex);
	delete pIndex;

	//save to cdplayer.ini if config says so

	if(g_sSettings.GetWriteCDPlayerIni() && (nResult == IDOK)){

		CCDPlayerIni cdini(&m_cCD);
		cdini.Write();
	}
	SetAlbumInfo();
}


void CLameFEView::OnQuerymusicbrainzdatabase() 
{

	//save current selection
	int iCnt = m_ctrlList.GetSelCount();
	int *pIndex = new int[iCnt];
	m_ctrlList.GetSelItems(iCnt, pIndex);


	if(!m_cCD.GetNumAudioTracks()){
		
		AfxMessageBox(g_iLang.GetString(IDS_MAIN_NOAUDIOCD), MB_OK+MB_ICONSTOP);
		delete pIndex;
		return;
	}
	CWaitCursor wc;

	CMusicBrainz mb(&m_cCD);
	if(mb.QueryDB()){

		RefreshTrackList();
	}

	m_ctrlList.SetSelItems(iCnt, pIndex);
	delete pIndex;
}

void CLameFEView::OnId3tagsSavecuesheet() 
{

	if(!m_cCD.GetNumAudioTracks()){
		
		AfxMessageBox(g_iLang.GetString(IDS_MAIN_NOAUDIOCD), MB_OK+MB_ICONSTOP);
		return;
	}

	if(!m_cCD.WriteCueSheet(g_sSettings.GetWorkingDir(), ".mp3")){

		AfxMessageBox(g_iLang.GetString(IDS_CUESHEETFAILED), MB_OK + MB_ICONSTOP);
	}
	else{
		
		CString strTmp;
		strTmp.Format(g_iLang.GetString(IDS_CUESHEETSAVED), m_cCD.GetAlbumString(g_sSettings.GetWorkingDir(), ".cue"));
		AfxMessageBox(strTmp, MB_OK + MB_ICONINFORMATION);
	}
}

void CLameFEView::OnId3tagsSavetocdplayerini() 
{
	
	if(!m_cCD.GetNumAudioTracks()){
		
		AfxMessageBox(g_iLang.GetString(IDS_MAIN_NOAUDIOCD), MB_OK+MB_ICONSTOP);
		return;
	}

	CCDPlayerIni cdini(&m_cCD);
	int nResult = cdini.Write();
	if(nResult == -1){

		AfxMessageBox(g_iLang.GetString(IDS_CDPINI_ERRORWRITE), MB_OK+MB_ICONEXCLAMATION);
	}
	else if(nResult == 2){

		AfxMessageBox(g_iLang.GetString(IDS_CDPINI_DOUBLEENTRY), MB_OK+MB_ICONINFORMATION);
	}
	else{

		AfxMessageBox(g_iLang.GetString(IDS_CDPINI_WRITE_OK), MB_OK+MB_ICONINFORMATION);
	}
}



void CLameFEView::SetLanguage()
{

	g_iLang.TranslateDialog((CDialog*)this, IDD_TRACKLIST);
	g_iLang.TranslateMenu(AfxGetApp()->GetMainWnd()->GetMenu(), IDR_MAINFRAME, TRUE);
	//g_iLang.TranslateDialog(&m_aAlbumInfoCtrl, IDD_ALBUMINFOCTRL);
	//m_ctrlList
}


void CLameFEView::OnInitialUpdate() 
{
	
	CFormView::OnInitialUpdate();

	SetTimer(TIMERID2, 5000, NULL);
	OnUpdate(this, 0, NULL);

	SetLanguage();
	
	m_ctrlList.InsertColumn(0, g_iLang.GetString(IDS_TL_TRACK), LVCFMT_RIGHT, 80);
	m_ctrlList.InsertColumn(1, g_iLang.GetString(IDS_TL_TITLE), LVCFMT_LEFT, 300);
	m_ctrlList.InsertColumn(2, g_iLang.GetString(IDS_TL_TIME), LVCFMT_RIGHT, 80);
	m_ctrlList.InsertColumn(3, g_iLang.GetString(IDS_TL_INSIZE), LVCFMT_RIGHT, 80);
	m_ctrlList.InsertColumn(4, g_iLang.GetString(IDS_TL_ESTSIZE), LVCFMT_RIGHT, 80);
	
	m_listBkImage.LoadBitmap(IDB_LIST_BK); 
	m_ctrlList.SetBkImage((HBITMAP)m_listBkImage, FALSE, 90, 90);
	m_ctrlList.SetTextBkColor(CLR_NONE);
	m_ctrlList.Init(FALSE);

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

	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
	   TRACE("Unable To create ToolTip\n");
	}
	else{

		m_pToolTip->AddTool(&c_configureIn, g_iLang.GetString(IDS_TOOL_CONFIGUREIN));
		m_pToolTip->AddTool(&c_configureOut,g_iLang.GetString(IDS_TOOL_CONFIGUREOUT));
		m_pToolTip->AddTool(&c_inputDevice, g_iLang.GetString(IDS_TOOL_INPUTDEVICE));
		m_pToolTip->AddTool(&c_outputDevice,g_iLang.GetString(IDS_TOOL_OUTPUTDEVICE));
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

	CString strTmp;

	float fDiskSpace = Utils::GetMyFreeDiskSpace(g_sSettings.GetBasePath())/1024.0/1024;

	if(fDiskSpace > 1000){

		strTmp.Format("%.0f GB", fDiskSpace/1000.0);
	}
	else{

		strTmp.Format("%.0f MB", fDiskSpace);
	}
	m_pStatus->SetPaneText(1,"LameFE");
	m_pStatus->SetPaneText(3, strTmp);
	m_pToolBar	 = (CToolBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_TOOLBAR);


	GetInputDevices();
	GetOutputDevices();

	m_wndPresetBar = (CNewPresetBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_PRESETBAR) ;

	InitProfileBar();

	if(g_sSettings.GetShowAlbumEditor() && !m_bTagEditorVisible){
		
		OnId3tagsId3tageditor();
	}

	m_bCheckCD = g_sSettings.GetCheckForNewCD();

	ResizeControls();
	
	LoadSettings();

	if(g_sSettings.GetDefaultFromCD()){

		ReadCDContents();
	}

	(AfxGetMainWnd())->SetWindowText(STR_VERSION);
	OnUpdate(this, 0, NULL);
}

void CLameFEView::OnSelchangeDevices(){

	OnSelchangeDevices(TRUE);
}

void CLameFEView::OnSelchangeDevices(BOOL bReset){


	TRACE("Entering CLameFEView::OnSelchangeDevices()\n");
	if(IsPluginMode()){
		
		if(bReset){

			m_ctrlList.DeleteAllItems();
			ResetFileList();
			if(g_sSettings.GetShowAlbumEditorOnFile() && !m_bTagEditorVisible){

				OnId3tagsId3tageditor();
			}
		}
	}
	else{

		if(m_nNumCDDrives == 0){

			return;
		}
		m_ctrlList.DeleteAllItems();
		ResetFileList();
		CR_SetActiveCDROM(c_inputDevice.GetCurSel() - (m_paPlugins.GetSize() > 0 ? 1 : 0));
		CR_SaveSettings();
		ReadCDContents();
	}
	TRACE("Leaving CLameFEView::OnSelchangeDevices()\n");

}


BOOL CLameFEView::GetOutputDevices()
{

	TRACE("Entering CLameFEView::GetOutputDevices()\n");
	
	CFileFind finder;
	CString	  strTmp;

	// We have always the lame encoder as output device:
	//c_outputDevice.AddString("MPEG I/II Layer 3 (lame_enc.dll)");
//	auto_ptr <CEncoderLame> pelEncoder(new CEncoderLame());
	CEncoderLame* pelEncoder = new CEncoderLame();

	if(!pelEncoder->Init()){

		strTmp.Format(g_iLang.GetString(IDS_ENC_PLUG_LOADFAILED), "lame_enc.dll");
		AfxMessageBox(strTmp, MB_OK+MB_ICONEXCLAMATION);
	}
	else{

		c_outputDevice.AddString(pelEncoder->GetEncoderDescription());
		m_eEncoders.push_back(pelEncoder);
		//m_eEncoders.Add(elEncoder);
		
	}

	// check for output plugins

	BOOL bResult = finder.FindFile(g_sSettings.GetAppDir() + "\\Plugins\\*_out.dll");
	
	while(bResult){

		bResult = finder.FindNextFile();
		
		//TRACE("Added plugin %s.\n", finder.GetFileName());

		//auto_ptr <CEncoderPlugin> pepEncoder(new CEncoderPlugin(finder.GetFilePath()));
		CEncoderPlugin* pepEncoder = new CEncoderPlugin(finder.GetFilePath());
		if(!pepEncoder->Init()){

			strTmp.Format(g_iLang.GetString(IDS_ENC_PLUG_LOADFAILED), finder.GetFileName());
			AfxMessageBox(strTmp, MB_OK+MB_ICONEXCLAMATION);
		}
		else{

			strTmp = pepEncoder->GetEncoderDescription();
			c_outputDevice.AddString(strTmp);
			m_eEncoders.push_back(pepEncoder);
		}


	}


	int nSelect = 0;

	if(g_sSettings.GetRememberEncoder()){

		nSelect = c_outputDevice.FindStringExact(0, g_sSettings.GetLastEncoder());

		if(nSelect == CB_ERR){

			nSelect = 0;	
		}
	}

	c_outputDevice.SetCurSel(nSelect);

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

	//////////////////////////////////////////////////////////
	// Check if there are any LameFE input plugins
	//////////////////////////////////////////////////////////
/*	CFileFind finder;
	BOOL bResult = finder.FindFile(g_sSettings.GetAppDir() + "\\Plugins\\*_in.dll");
	while(bResult){

		bResult = finder.FindNextFile();
		
		TRACE("Added plugin %s.\n", finder.GetFileName());
		CLameFEPlugin	tmp(finder.GetFilePath());
		tmp.Load();  
		tmp.Unload();
		m_paPlugins.Add(tmp);
	}

	// Check if there are any Winamp input plugins
	
	bResult = finder.FindFile(g_sSettings.GetWinampPluginPath() + "\\in_*.dll");

	while(bResult){

		bResult = finder.FindNextFile();
		
		TRACE("Added plugin %s.\n", finder.GetFileName());
		CWinampPlugin	tmp(finder.GetFilePath());
		tmp.Load(NULL);  
		
		if(tmp.UsesOutput()){

			m_paPlugins.Add(tmp);
		}
		tmp.Unload();
	}

	// any plugins found?
	if(m_paPlugins.GetSize() > 0){
		
		CString strDevice;
		strDevice = g_iLang.GetString(IDS_AUDIOFILEPLUGIN);
		c_inputDevice.AddString(strDevice);
	}
*/
	//////////////////////////////////////////////////////////////
	// Check for CD drives if Ripper was initialized correctly
	//////////////////////////////////////////////////////////////

	if(!((CLameFEApp*)AfxGetApp())->GetRipperStatus()){
		
		m_nNumCDDrives = 0;
	}
	else{  // Ripper status is OK enumerate drives and load settings...

		CDROMPARAMS cdParams;
		m_nNumCDDrives = 0;


		int nSelCD = CR_GetActiveCDROM();
		CString tmp;

		for (int i = 0; i < CR_GetNumCDROM(); i++)
		{

			CR_SetActiveCDROM(i);
			CR_GetCDROMParameters(&cdParams);
			tmp.Format("CD: %s", cdParams.lpszCDROMID);
			c_inputDevice.AddString(tmp);
			m_nNumCDDrives++;
		}
		
		CR_SetActiveCDROM(nSelCD);

		c_inputDevice.SetCurSel(nSelCD + (m_paPlugins.GetSize() > 0 ? 1 : 0));
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

void CLameFEView::OnInvertSel() 
{

	m_ctrlList.InvertSelection();
}

void CLameFEView::OnViewSelectalltracksfiles() 
{

	m_ctrlList.SelectAll();
}

void CLameFEView::ReadCDContents()
{

	if(m_nNumCDDrives == 0){

		return;
	}

	CWaitCursor wCursor;
	CString strOldPaneText = m_pStatus->GetPaneText(1);
	m_pStatus->SetPaneText(1, g_iLang.GetString(IDS_READCDCONTENT));
	
	m_ctrlList.DeleteAllItems();
	
	CDEX_ERR	bErr;
	//Sleep(400);
	
	CString  track;
	if (CR_IsUnitReady() == 0x01)
	{
		
		bErr = CR_LoadSettings();

		if(bErr != CDEX_OK){

			track.Format(g_iLang.GetString(IDS_DRIVENOTREADY), bErr);
			AfxMessageBox(track, MB_OK+MB_ICONSTOP);
			return;
		}
		
		CDMEDIASTATUS cdMediaStatus = CDMEDIA_NOT_PRESENT;
		CR_IsMediaLoaded(cdMediaStatus);
		if(cdMediaStatus != CDMEDIA_PRESENT){

			return;
		}
		m_cCD.Init();

		if(g_sSettings.GetReadCDTex()){

			m_cCD.ReadCDText();
		}

		if(g_sSettings.GetReadCDPlayerIni()){

			CCDPlayerIni cdini(&m_cCD);
			cdini.Read();
		}

		RefreshTrackList();

		if(g_sSettings.GetSelect()){

			m_ctrlList.SelectAll();
			SetAlbumInfo();
		}

		UpdateData(FALSE);
	}
	else{

		m_ctrlList.DeleteAllItems();
		m_cCD.Eject();
	}
	m_pStatus->SetPaneText(1,strOldPaneText);
}

void CLameFEView::OnTimer(UINT nIDEvent) 
{

	OnUpdate(this, 0, NULL);	

	if((m_nNumCDDrives == 0) || IsPluginMode() || !IsWindowVisible() || !m_bCheckCD){

		return;
	}

	
	CDMEDIASTATUS cdMediaStatus = CDMEDIA_NOT_PRESENT;
	CR_IsMediaLoaded(cdMediaStatus);
	if(cdMediaStatus == CDMEDIA_PRESENT){


		m_mtLock.Lock();
		CCompactDisc cdTmp;
		cdTmp.Init();

		if(cdTmp.GetDiscID() != m_cCD.GetDiscID()){
			
			m_ctrlList.DeleteAllItems();
			ReadCDContents();
		}
		m_mtLock.Unlock();
	}
	else{

		m_cCD.Eject();
		m_ctrlList.DeleteAllItems();
	}

	CFormView::OnTimer(nIDEvent);
}


void CLameFEView::OnConfigureIn()
{

	BOOL bOldCDCheck = m_bCheckCD;
	m_bCheckCD = FALSE;

	CSettingsDlg dlg;
	if(!IsPluginMode()){

		dlg.ShowCDRipper();
	}
	else{

		dlg.ShowDecoders();
	}
	dlg.DoModal();

	m_bCheckCD = bOldCDCheck;
}

void CLameFEView::OnConfigureOut()
{

	UpdateData(TRUE);
	
	BOOL bOldCDCheck = m_bCheckCD;
	m_bCheckCD = FALSE;
	
	m_eEncoders.at(c_outputDevice.GetCurSel())->Configure();

	m_bCheckCD = bOldCDCheck;

	RefreshTrackList();
}

void CLameFEView::OnFileOpen() 
{

	TRACE("Entering CLameFEDlg::OnAddFile()\n");
	
	// Get all supported filetypes of all input plugins
	CString tmpSzFilter, szFilter, strAllExt;
	
	for(int i = 0; i < m_paPlugins.GetSize(); i++){

		tmpSzFilter  += m_paPlugins[i].GetSzFilter();
		strAllExt += m_paPlugins[i].GetExtension() + ";";
	}
	strAllExt.Replace("*.;", "");

	//All Files (*.abc;*.cde)|*.abc; *.cde
	szFilter.Format(g_iLang.GetString(IDS_SUPPORTEDFILES), strAllExt, strAllExt);
	szFilter += tmpSzFilter + g_iLang.GetString(IDS_ALLFILES);

	CMyFileDialog FileOpen(TRUE, 0, 0, OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT  , szFilter, 0);
	
	FileOpen.m_ofn.lpstrInitialDir = (const char*)g_sSettings.GetBasePath();

	CString strFileName;
	int maxChar = 1000;
	FileOpen.m_ofn.lpstrFile = strFileName.GetBuffer(maxChar);
	FileOpen.m_ofn.nMaxFile = maxChar;


	int nResponse = FileOpen.DoModal();
	strFileName.ReleaseBuffer();
	if (nResponse == IDOK){
	
		POSITION pos = FileOpen.GetStartPosition();
		CString path;
		
		CString strOutputDevice, dllName;
		c_outputDevice.GetLBText(c_outputDevice.GetCurSel(), strOutputDevice);
		dllName = strOutputDevice.Mid(strOutputDevice.ReverseFind('(') + 1, strOutputDevice.GetLength() - strOutputDevice.ReverseFind('(') - 2);

		while (pos != NULL){
		
			TRACE("Processing files (pos=%d)\n", pos);
			path = FileOpen.GetNextPathName(pos);
			
			int		nItem = m_ctrlList.GetItemCount();

			CMultimediaFile *tmp = new CMultimediaFile(path);
			
/*			if(dllName != "lame_enc.dll"){

				COutPlugin out(g_sSettings.GetAppDir() + "\\Plugins\\" + dllName, wd);
				if(out.Load()){

					LF_OUT *outMod;
					MMFILE_FORMAT mmf;
					MMFILE_ALBUMINFO mma;

					outMod = out.GetOutModule();

					mmf.dwSampleRate = tmp->GetWfx()->nSamplesPerSec;
					mmf.nBitsPerSample = tmp->GetWfx()->wBitsPerSample;
					mmf.nChannels = tmp->GetWfx()->nChannels;

					outMod->Open(path, &mmf, &mma);
					tmp->SetEstFileSize(outMod->GetEstFileSize());
					outMod->Close();
					out.Unload();
				}
			}*/

			m_mmFiles.Add(tmp);	

			CString strTmp;
			strTmp.Format("%02d", nItem + 1);
			m_ctrlList.InsertItem(nItem, strTmp, 2);
			m_ctrlList.SetItemText(nItem, 1, path);
			strTmp.Format("%02 MB", tmp->GetEstFileSize() / 1024);
			m_ctrlList.SetItemText(nItem, 3, strTmp);
		}
	}

	UpdateData(FALSE);

	TRACE("Leaving CLameFEDlg::OnAddFile\n");
}

void CLameFEView::OnRemoveFile() 
{

	if(!IsPluginMode()){

		AfxMessageBox(g_iLang.GetString(IDS_MAIN_ADDFILECD), MB_OK+MB_ICONEXCLAMATION);
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
		
		AfxMessageBox(g_iLang.GetString(IDS_EMPTYLIST), MB_OK+MB_ICONINFORMATION,0);
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

	TRACE("Entering CLameFEView::OnDestroy()\n");
	KillTimer(TIMERID2);

	OnId3tagsId3tageditor();
	m_ctrlList.DeleteAllItems();
	ResetFileList();
	
	// De-Init all encoders
	for(int i = m_eEncoders.size() - 1; i >= 0; i--){

		CGenericEncoder *pE = m_eEncoders.at(i);
		m_eEncoders.pop_back();
		pE->DeInit();

		delete pE;
		pE = NULL;
	}
	
	SaveSettings();
	CFormView::OnDestroy();

	TRACE("Leaving CLameFEView::OnDestroy()\n");
}

void CLameFEView::RefreshTrackList()
{

	CString strOldPaneText = m_pStatus->GetPaneText(1);
	m_pStatus->SetPaneText(1, g_iLang.GetString(IDS_UPDATETRACKLIST));
	m_ctrlList.DeleteAllItems();
	CString strTmp;

	for(int i = 0; i < m_cCD.GetNumTracks(); i++){

		strTmp.Format("%02d", i+1);
		if(m_cCD.GetCDTrack(i)->IsAudioTrack()){

			m_ctrlList.InsertItem(i, strTmp, 0);
			strTmp.Format("%s - %s", m_cCD.GetCDTrack(i)->m_id3Info.GetArtist(), m_cCD.GetCDTrack(i)->m_id3Info.GetSong());
			
		}
		else{
			
			m_ctrlList.InsertItem(i, strTmp, 1);
			strTmp.Format("%s", m_cCD.GetCDTrack(i)->GetTrackname());
		}

		m_ctrlList.SetItemText(i ,1, strTmp);
		m_ctrlList.SetItemText(i, 2, m_cCD.GetTrackDuration(i));
		
		strTmp.Format("%02.2f", (float)m_cCD.GetTrackSize(i) / 1024 / 1024);
		m_ctrlList.SetItemText(i, 3, strTmp);
		strTmp.Format("%02.2f", (float)m_eEncoders.at(c_outputDevice.GetCurSel())->GetEstimatedSize(m_cCD.GetTrackSize(i), 2, 44100,16) / 1024 / 1024);
		m_ctrlList.SetItemText(i, 4, strTmp);

	}
	m_pStatus->SetPaneText(1,strOldPaneText);
	strTmp.Format("%02d Tracks", m_cCD.GetNumTracks());
	m_pStatus->SetPaneText(2, strTmp);
}



//DEL void CLameFEView::OnFileStartAlbum() 
//DEL {
//DEL 
//DEL 	OnViewSelectalltracksfiles();
//DEL //	StartEncoding(ALBUMMODE);
//DEL }

//DEL void CLameFEView::OnFileStartBatchAlbum()
//DEL {
//DEL 
//DEL 	OnViewSelectalltracksfiles();
//DEL //	StartEncoding(BATCHALBUMMODE);
//DEL }

//DEL void CLameFEView::OnFileStartBatchSingle()
//DEL {
//DEL 
//DEL 	OnViewSelectalltracksfiles();
//DEL //	StartEncoding(BATCHSINGLETRACKMODE);
//DEL }

void CLameFEView::OnRipCd() 
{

	if(m_bAutoRipMode){

		StartEncoding((ENC_MODE)(LFE_RIP_SINGLE | LFE_AUTO_MODE));
	}
	else{

		StartEncoding(LFE_RIP_SINGLE);
	}
}

void CLameFEView::OnCdAblumRip() 
{

	if(m_bAutoRipMode){

		StartEncoding((ENC_MODE)(LFE_RIP_ALBUM | LFE_AUTO_MODE));
	}
	else{

		StartEncoding(LFE_RIP_ALBUM);
	}
}

void CLameFEView::OnFileStartencoding() 
{

	StartEncoding(LFE_FILE_MODE);
}

void CLameFEView::StartEncoding(ENC_MODE eMode)
{

	int     nInputDevice    = c_inputDevice.GetCurSel();

	if((IsPluginMode() && !m_mmFiles.GetSize()) ||
		(!IsPluginMode() && !m_cCD.GetNumTracks())
		){

		AfxMessageBox(g_iLang.GetString(IDS_EMPTYLIST), MB_OK+MB_ICONEXCLAMATION);
		return;
	}

	
	CEncodingStatusDlg esdlg(this);
	
	// If CD Input Do...
	if(!IsPluginMode()){ 

		UpdateData(TRUE);
		int iCnt; 

		iCnt = m_ctrlList.GetSelCount();

		if(!iCnt){
			
			AfxMessageBox(g_iLang.GetString(IDS_NOTRACKSSELECTED), MB_OK+MB_ICONINFORMATION);
			return;
		}

		int iAudioCnt = 0, j = 0;
		int *pIndex, *pAudioVerified;

		pIndex = new int[iCnt];

		m_ctrlList.GetSelItems(iCnt, pIndex);

		//verify that all selected tracks are audio tracks
		for(int i = 0; i < iCnt; i++){
			
			if(m_cCD.GetCDTrack(pIndex[i])->IsAudioTrack()){

				iAudioCnt++;
			}
			else{

				CString strTmp;
				strTmp.Format(g_iLang.GetString(IDS_DATATRACK), pIndex[i] + 1);
				AfxMessageBox(strTmp, MB_OK+MB_ICONINFORMATION);
			}
		}
		//Fill Audiotracks selection array 
		pAudioVerified = new int[iAudioCnt];
		for(i = 0; i < iCnt; i++){

			if(m_cCD.GetCDTrack(pIndex[i])->IsAudioTrack()){

				pAudioVerified[j++] = pIndex[i];
			}
		}
		m_cCD.SetSelItems(iAudioCnt, pAudioVerified);
		
		delete pIndex;
		delete pAudioVerified;

		if(!m_cCD.GetNumSelTracks()){

			AfxMessageBox(g_iLang.GetString(IDS_NOTRACKSSELECTED), MB_OK+MB_ICONINFORMATION);
			return;
		}


		//esdlg.SetJob(RIP_TO_ENCODER, dllName, mEMode); 
		//esdlg.SetCDROM(&m_cCD);
		esdlg.Init((m_eEncoders.at(c_outputDevice.GetCurSel())), &m_cCD, eMode);
	}
	else{  //input from file
		
		//esdlg.SetJob(ANY_TO_ENCODER, dllName); 
		//esdlg.SetFiles(&m_mmFiles);
	}
	
	if(g_sSettings.GetHideMainWnd()){

		GetTopLevelParent()->ShowWindow(SW_MINIMIZE);
		GetTopLevelParent()->ShowWindow(SW_HIDE);
	}
	m_bCheckCD = FALSE;

	int nResult = esdlg.DoModal();
	
	GetTopLevelParent()->ShowWindow(SW_SHOWNORMAL);
	m_bCheckCD = g_sSettings.GetCheckForNewCD();
	
	if(nResult == IDCANCEL){

		TRACE("Dont`exit lameFE or shutdown windows. Encoding cancelled\n");
		return;
	}

	if(g_sSettings.GetExitLameFE()){

		OnAppExit();
	}

	if(g_sSettings.GetShutdown()){

		if(!ShutDown()){

			AfxMessageBox(g_iLang.GetString(IDS_ERR_SHUTDOWN), MB_OK+MB_ICONSTOP);
		}
	}

//	AfxMessageBox("Not implemented");
}


BOOL CLameFEView::ShutDown()
{

	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tp, tpPrevious;
	DWORD cbPrevious = sizeof(TOKEN_PRIVILEGES);
	BOOL bSuccess = FALSE;

	if ( ! LookupPrivilegeValue(NULL, static_cast<CString>( SE_SHUTDOWN_NAME), &luid ))
		return FALSE;

	if( ! OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken ))
		return FALSE;

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = 0;

	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &tpPrevious, &cbPrevious);

	if (GetLastError() == ERROR_SUCCESS){

		tpPrevious.PrivilegeCount = 1;
		tpPrevious.Privileges[0].Luid = luid;


		tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);

		AdjustTokenPrivileges(hToken, FALSE, &tpPrevious, cbPrevious, NULL, NULL);

		if (GetLastError() == ERROR_SUCCESS){

			bSuccess=TRUE;
		}
	}

	CloseHandle(hToken);

	InitiateSystemShutdown(NULL, "System shutdown", 10, FALSE, FALSE);
	return TRUE;
}

void CLameFEView::OnAppExit() 
{

	TRACE("Entering CLameFEView::OnAppExit()\n");

	AfxGetApp()->HideApplication();
	AfxGetApp()->CloseAllDocuments(FALSE);
	AfxGetApp()->ExitInstance();

	TRACE("Leaving CLameFEView::OnAppExit()\n");
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

		AfxMessageBox(g_iLang.GetString(IDS_CDMODENODROP), MB_OK+MB_ICONSTOP);
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
		::DragQueryFile(hDropInfo, i, lpszFileName, sizeof(lpszFileName));

		strFileName = lpszFileName;
		strFileExt  = strFileName.Right(3);
		strFileExt.MakeLower();

		if(strAllExt.Find(strFileExt, 0) == -1){
			
			CString strTmp;
			strTmp.Format(g_iLang.GetString(IDS_UNSUPPORTEDFILE), strFileName);

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
	ScreenToClient(rcList);
	
	// margin...
	rcList.right = rcParent.right - 10;
	// if the tag-editor is visible we need some space under the tracklist for it
	rcList.bottom = rcParent.bottom - (m_bTagEditorVisible ? 130 : 0);
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
				
			m_aAlbumInfoCtrl.SetInfo(&m_cCD.GetCDTrack(m_nSelItem)->m_id3Info);
			
			if(!m_cCD.GetCDTrack(m_nSelItem)->IsAudioTrack()){
				
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

	return FALSE;
}


void CLameFEView::OnCheckfornewvesion() 
{

	CCheckNewVersion dlg;
	dlg.DoModal();
}

void CLameFEView::OnSettings() 
{

	BOOL bOldCDCheck = m_bCheckCD;
	m_bCheckCD = FALSE;

	CSettingsDlg dlg;
	dlg.DoModal();
	RefreshTrackList();
	m_bCheckCD = bOldCDCheck;
}


void CLameFEView::OnHelpReportabug() 
{


	CString strFilename = g_iLang.GetString(IDS_BUGREPORTFORUM);

	ShellExecute(GetSafeHwnd(), 
				"open", 
				strFilename,
				NULL,
				NULL,
				SW_SHOW);
}

void CLameFEView::OnSelchangeOutputDevice() 
{

	if(!IsPluginMode()){

		RefreshTrackList();
	}
}

void CLameFEView::LoadSettings()
{


	if(g_sSettings.GetShowAlbumEditor() && !m_bTagEditorVisible){
		
		OnId3tagsId3tageditor();
	}

	m_bCheckCD = g_sSettings.GetCheckForNewCD();

	ResizeControls();

	if(g_sSettings.GetRememberLastDecoder()){

		if(g_sSettings.GetLastDecoder() > c_inputDevice.GetCount() - 1){

			c_inputDevice.SetCurSel(0);
		}
		else{

			TRACE("Using last Decoder\n");
			c_inputDevice.SetCurSel(g_sSettings.GetLastDecoder());
		}
	}
	else{

		//Select correct Input Device:
		if((m_paPlugins.GetSize() > 0) && (!g_sSettings.GetDefaultFromCD())){
			
			// We have plugins and the preset is set to plugins
			c_inputDevice.SetCurSel(0);
		}
		else{

			// Select the last used CD drive as saved in preset
			int nSelCD = CR_GetActiveCDROM();
			
			CR_SetActiveCDROM(nSelCD);

			c_inputDevice.SetCurSel(nSelCD + (m_paPlugins.GetSize() > 0 ? 1 : 0));
		}

	}

	int nSelect = 0;
	if(g_sSettings.GetRememberEncoder()){

		nSelect = c_outputDevice.FindStringExact(0, g_sSettings.GetLastEncoder());

		if(nSelect == CB_ERR){

			nSelect = 0;	
		}
	}

	c_outputDevice.SetCurSel(nSelect);
	OnSelchangeDevices(TRUE);
}

void CLameFEView::SaveSettings()
{

	TRACE("Entering CLameFEView::SaveSettings()\n");
	RECT rc;
	AfxGetApp()->m_pMainWnd->GetWindowRect(&rc);

	CString strTmp;
	c_outputDevice.GetLBText(c_outputDevice.GetCurSel(), strTmp);
	
	g_sSettings.SetLastEncoder(strTmp);
	TRACE("### %d\n",c_inputDevice.GetCurSel());
	g_sSettings.SetLastDecoder(c_inputDevice.GetCurSel());
	g_sSettings.SetWindowX(rc.left);
	g_sSettings.SetWindowY(rc.top);
	g_sSettings.SetWindowCX(rc.right-rc.left);
	g_sSettings.SetWindowCY(rc.bottom-rc.top);
	g_sSettings.Save();

	TRACE("Leaving CLameFEView::SaveSettings()\n");
}

void CLameFEView::OnPresetSelChanged(WPARAM nSelection,LPARAM lParam)
{

	TRACE("Entering CLameFEView::OnPresetSelChanged()\n");
	
	BOOL bCheckCD = m_bCheckCD;
	m_bCheckCD = FALSE;

	CString strActivePreset, strPaneText; 
	
	strActivePreset = m_wndPresetBar->GetPresetSelectionString();

	// Only load preset if it isnt active.
	if(strActivePreset != g_sSettings.GetActivePreset()){

		// Preset is not active yet, copy it to LameFE.ini and call Load Settings
		TRACE("### Preset is not active yet. Loading and initializing\n");
		g_sSettings.LoadPreset(strActivePreset);

    	g_sSettings.SetActivePreset(strActivePreset);
		strPaneText = g_sSettings.GetPresetComment();
		//TRACE("%s\n", strPaneText);
		m_pStatus->SetPaneText(1, strPaneText);
		LoadSettings();
	}

	m_bCheckCD = bCheckCD;

	TRACE("Leaving CLameFEView::OnPresetSelChanged()\n");
}

void CLameFEView::OnSavepreset() 
{
	
	TRACE("Entering CLameFEView::OnSavePreset()\n");
	
	BOOL bCheckCD = m_bCheckCD;
	m_bCheckCD = FALSE;

	CPresetSaveDlg dlg;
	dlg.m_strPresetName = m_wndPresetBar->GetPresetSelectionString();
	dlg.m_strComment = m_pStatus->GetPaneText(1);

	if(dlg.DoModal() == IDOK){

	
		CString strSaveAs = m_strPresetPath +  "\\" + dlg.m_strPresetName + ".ini";
		CFileStatus rStatus;

		if(CFile::GetStatus(strSaveAs, rStatus)){

			CString strTmp;
			strTmp.Format(g_iLang.GetString(IDS_ENC_FILEEXISTS), strSaveAs);
			if(AfxMessageBox(strTmp, MB_YESNO+MB_ICONQUESTION) == IDNO){
				
				m_bCheckCD = bCheckCD;
				return;
			}
		}
		else{

            g_sSettings.SetActivePreset(dlg.m_strPresetName);
			g_sSettings.SetPresetName(dlg.m_strPresetName);
			g_sSettings.SetPresetComment(dlg.m_strComment);
			SaveSettings();
			//g_sSettings.Save();
			g_sSettings.SavePreset(dlg.m_strPresetName);
			m_wndPresetBar->AddPresetSelection(dlg.m_strPresetName);
			m_wndPresetBar->SetPresetSelection(dlg.m_strPresetName);

		}

	}

	m_bCheckCD = bCheckCD;

	TRACE("Leaving CLameFEView::OnSavePreset()\n");
}

void CLameFEView::OnDeletepreset() 
{

	TRACE("Entering CLameFEView::OnDeletepreset()\n");

	BOOL bCheckCD = m_bCheckCD;
	m_bCheckCD = FALSE;

	CString strToDelete = m_wndPresetBar->GetPresetSelectionString();
	strToDelete.MakeLower();

	if((strToDelete == "lamefe") || (strToDelete == "default")){

		AfxMessageBox(g_iLang.GetString(IDS_DEFAULTPRESETNOTREMOVABLE), MB_OK+MB_ICONINFORMATION);
	}
	else{

		strToDelete = m_wndPresetBar->GetPresetSelectionString();

		CString strFilename = m_strPresetPath + "\\" + strToDelete  + ".ini";
		
		TRY
		{

			CFile::Remove(strFilename);
			m_wndPresetBar->DeletePresetSelection(m_wndPresetBar->GetPresetSelectionString());
			m_wndPresetBar->SetPresetSelection("default");
			SendMessage(UWM_PRESETSELCHANGED, m_wndPresetBar->GetPresetSelection(), 0L);
		}
		CATCH(CFileException, e)
		{

			AfxMessageBox(g_iLang.GetString(IDS_COULDNOTREMOVEPRESET), MB_OK+MB_ICONINFORMATION);
			m_bCheckCD = bCheckCD;
		}
		END_CATCH
	}

	m_bCheckCD = bCheckCD;

	TRACE("Leaving CLameFEView::OnDeletepreset()\n");
}

void CLameFEView::InitProfileBar()
{

	CFileFind finder;
	CString   strPreset;
	CString	  strActivePreset;

	strActivePreset = g_sSettings.GetActivePreset();

	BOOL bResult = finder.FindFile(m_strPresetPath + "\\*.ini");

	while(bResult){

		bResult = finder.FindNextFile();
		
		strPreset = finder.GetFileName();
		strPreset.MakeLower();
		if(strPreset == "lamefe.ini"){

			continue;
		}
		strPreset = strPreset.Left(strPreset.GetLength() - 4);
		m_wndPresetBar->AddPresetSelection(strPreset);
	}

	m_wndPresetBar->SetPresetSelection(strActivePreset);
}


void CLameFEView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	
	if(IsWindowVisible()){

		TRACE("Entering CLameFEView::OnUpdate()\n");
		AfxGetApp()->m_pMainWnd->SetWindowText(STR_VERSION);

		TRACE("Leaving CLameFEView::OnUpdate()\n");
	}
}

void CLameFEView::OnRclickFilesTracks(NMHDR* pNMHDR, LRESULT* pResult) 
{

	CMenu mContextMenu;
	mContextMenu.LoadMenu(IDR_CONTEXTMENU_LIST);

	// translate the menu items
	g_iLang.TranslateMenu(&mContextMenu, IDR_CONTEXTMENU_LIST, FALSE);

	POINT point;
	GetCursorPos(&point);

	if(m_ctrlList.GetNextItem(-1, LVNI_FOCUSED) >= 0)
	{

		mContextMenu.GetSubMenu(0)->TrackPopupMenu(	TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
											point.x,
											point.y,
											AfxGetMainWnd() );
	}

	*pResult = 0;
}

void CLameFEView::OnAutoRip() 
{

	m_bAutoRipMode = !m_bAutoRipMode;
}

void CLameFEView::OnUpdateAutoRip(CCmdUI* pCmdUI) 
{

	pCmdUI->Enable(!IsPluginMode());
	if(m_bAutoRipMode){

		pCmdUI->SetCheck(1);
	}
	else{

		pCmdUI->SetCheck(0);
	}
	
}
