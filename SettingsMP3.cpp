// SettingsMP3.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lamefe.h"
#include "SettingsMP3.h"
#include "Ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



DWORD MPEGBitrates[3][3][15] = 
{
	{
		{0,32,64,96,128,160,192,224,256,288,320,352,384,416,448},
		{0,32,48,56,64 ,80 ,96 ,112,128,160,192,224,256,320,384},
		{0,32,40,48,56 ,64 ,80 ,96 ,112,128,160,192,224,256,320}
	},
	{
		{0,32,48,56,64,80,96,112,128,144,160,176,192,224,256},
		{0, 8,16,24,32,40,48,56 , 64, 80, 96,112,128,144,160},
		{0, 8,16,24,32,40,48,56 , 64, 80, 96,112,128,144,160}
	},
	{
		{0, 8,16,24,32,40,48,56 , 64, 80, 96,112,128,144,160},
		{0, 8,16,24,32,40,48,56 , 64, 80, 96,112,128,144,160},
		{0, 8,16,24,32,40,48,56 , 64, 80, 96,112,128,144,160},
	}
};


DWORD MP3SampleRates[3][3] =
{
	{ 48000, 44100, 32000 },
	{ 24000, 22050, 16000 },
	{  8000, 11025, 11025 } 
};

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsMP3 


CSettingsMP3::CSettingsMP3(CWnd* pParent /*=NULL*/)
	: CMySettingsPage(CSettingsMP3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsMP3)
	m_encoderVersion = _T("");
	m_abr = 0;
	m_strPresetName = _T("");
	m_strDescription = _T("");
	m_nMode = -1;
	m_nVersion = -1;
	//}}AFX_DATA_INIT
	m_nLayer=2;
}


void CSettingsMP3::DoDataExchange(CDataExchange* pDX)
{
	CMySettingsPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsMP3)
	DDX_Control(pDX, IDC_OUTSAMPLERATE, c_OutSampleRate);
	DDX_Control(pDX, IDC_PRESETDESCRIPTION, m_cPresetDsc);
	DDX_Control(pDX, IDC_PRESETMP3DLG, m_cPresetName);
	DDX_Control(pDX, IDC_THREADPRIORITY, m_cThreadPriority);
	DDX_Control(pDX, IDC_ABR, c_abr);
	DDX_Control(pDX, IDC_BITSPERSAMPLE_MAX, c_maxBitrate);
	DDX_Control(pDX, IDC_QUALITY, m_qualityPreset);
	DDX_Control(pDX, IDC_VBR_QUALITY, c_vbrQuality);
	DDX_Control(pDX, IDC_VBR_METHOD, c_vbrMethod);
	DDX_Control(pDX, IDC_MPEG, c_MPEG);
	DDX_Control(pDX, IDC_PRIVATE, c_private);
	DDX_Control(pDX, IDC_ORIGINAL, c_original);
	DDX_Control(pDX, IDC_ID3V1, c_writeId3v1);
	DDX_Control(pDX, IDC_ID3_2, c_writeId3v2);
	DDX_Control(pDX, IDC_COPYRIGHT, c_copyright);
	DDX_Control(pDX, IDC_CHECKSUM, c_checkSum);
	DDX_Control(pDX, IDC_CHANNELS, c_channels);
	DDX_Control(pDX, IDC_BITSPERSAMPLE, c_bitsPerSample);
	DDX_Text(pDX, IDC_ENCODER_VERSION, m_encoderVersion);
	DDX_Text(pDX, IDC_ABR, m_abr);
	DDX_CBString(pDX, IDC_PRESETMP3DLG, m_strPresetName);
	DDX_Text(pDX, IDC_PRESETDESCRIPTION, m_strDescription);
	DDV_MaxChars(pDX, m_strDescription, 100);
	DDX_CBIndex(pDX, IDC_CHANNELS, m_nMode);
	DDX_CBIndex(pDX, IDC_MPEG, m_nVersion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsMP3, CMySettingsPage)
	//{{AFX_MSG_MAP(CSettingsMP3)
	ON_CBN_SELCHANGE(IDC_VBR_METHOD, OnSelchangeVbrMethod)
	ON_CBN_SELCHANGE(IDC_QUALITY, OnSelchangeQuality)
	ON_CBN_SETFOCUS(IDC_THREADPRIORITY, OnSetfocusThreadpriority)
	ON_BN_CLICKED(IDC_SAVEPRESET, OnSavepreset)
	ON_CBN_SELCHANGE(IDC_PRESETMP3DLG, OnSelchangePresetmp3dlg)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_CBN_SELENDOK(IDC_MPEG, OnSelendokMpeg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsMP3 

void CSettingsMP3::Init(CString strWd)
{

	CMySettingsPage::Init(strWd);

	HINSTANCE	lameDLL			= NULL;
	BE_VERSION	Version			= {0,};
	
	lameDLL=LoadLibrary(m_strWd + "\\lame_enc.dll");

	if(lameDLL==NULL)
	{
		m_encoderVersion = "Using LAME-Encoder.\nhttp://www.mp3devel.org";
	}
	else{	
		// Get Interface function beVersion
		beVersion		= (BEVERSION) GetProcAddress(lameDLL, TEXT_BEVERSION);
		if(!beVersion)
		{
			m_encoderVersion = "Using LAME-Encoder.\nhttp://www.mp3devel.org";
		}
		else{
			beVersion(&Version);
			m_encoderVersion.Format("lame_enc.dll version %u.%02u (%u/%u/%u)\n"
					"lame_enc Engine %u.%02u\n"
					"%s",	
					Version.byDLLMajorVersion, Version.byDLLMinorVersion,
					Version.byDay, Version.byMonth, Version.wYear,
					Version.byMajorVersion, Version.byMinorVersion,
					Version.zHomepage);
		}
	}

	UpdateData(FALSE);
	m_cPresetName.EnableAutoCompletion(FALSE);
	//Setup Thread warning

	m_mToolTip.Create(this);
	m_mToolTip.AddControlInfo(IDC_THREADPRIORITY, IDS_THREADWARNING);

	if(m_pToolTip != NULL){

		m_pToolTip->AddTool(&c_copyright, IDS_TOOL_CP);
		m_pToolTip->AddTool(&c_checkSum, IDS_TOOL_CRC);
		m_pToolTip->AddTool(&c_original, IDS_TOOL_ORIG);
		m_pToolTip->AddTool(&c_private, IDS_TOOL_PRIVATE);
		m_pToolTip->AddTool(&c_channels, IDS_TOOL_CHANNELS);
		m_pToolTip->AddTool(&c_bitsPerSample, IDS_TOOL_MINBITRATE);
		m_pToolTip->AddTool(&c_abr, IDS_TOOL_ABR);
		m_pToolTip->AddTool(&c_vbrMethod, IDS_TOOL_VBRMETHOD);
		m_pToolTip->AddTool(&c_vbrQuality, IDS_TOOL_VBRQUALITY);
		m_pToolTip->AddTool(&c_maxBitrate, IDS_TOOL_MAXBITRATE);
		m_pToolTip->AddTool(&c_MPEG, IDS_TOOL_MPEGVER);
		m_pToolTip->AddTool(&c_writeId3v2, IDS_TOOL_WRITEID3);
		m_pToolTip->AddTool(&m_qualityPreset, IDS_TOOL_QUALPRESET);
		m_pToolTip->AddTool(&m_cThreadPriority, IDS_TOOL_ENCTHREADP);

		m_pToolTip->Activate(TRUE);
	}

	InitPresets();

	CIni cfg;
	cfg.SetIniFileName(m_strWd + "\\LameFE.ini");
	
	CString strPreset = cfg.GetValue("L.A.M.E.", "PresetName", "default");

	LoadPreset(strPreset);

	int nIndex = m_cPresetName.FindStringExact(0, strPreset);

	m_cPresetName.SetCurSel(nIndex);
	m_cPresetName.GetLBText(nIndex, m_strPresetName);

	OnSelchangeVbrMethod();
	OnSelchangeQuality();
	UpdateData(FALSE);
}

BOOL CSettingsMP3::Save()
{

	if(UpdateData(TRUE) == NULL){

		return FALSE;
	}
	
	CString strTmp = m_strPresetName;
	strTmp.MakeLower();

	if((m_strPresetName.GetLength() == 0) || (strTmp == "lamefe")){

		AfxMessageBox(IDS_PRESET_BLANKNAME, MB_OK+MB_ICONINFORMATION);
		return FALSE;
	}

	CIni cfg;
	cfg.SetIniFileName(m_strWd + "\\LameFE.ini");
	cfg.SetValue("L.A.M.E.", "PresetName", m_strPresetName);


	cfg.SetIniFileName(cfg.GetValue("LameFE", "PresetPath", m_strWd) + "\\" + m_strPresetName + ".ini");
	
	cfg.SetValue("L.A.M.E.", "OutSampleRate", c_OutSampleRate.GetCurSel());
	cfg.SetValue("L.A.M.E.", "Description", m_strDescription);
	cfg.SetValue("L.A.M.E.", "Bitrate", c_bitsPerSample.GetCurSel());
	cfg.SetValue("L.A.M.E.", "Channels", c_channels.GetCurSel());
	cfg.SetValue("L.A.M.E.", "Crc", c_checkSum.GetCheck());
	cfg.SetValue("L.A.M.E.", "Copyright", c_copyright.GetCheck());
	cfg.SetValue("L.A.M.E.", "Original", c_original.GetCheck());
	cfg.SetValue("L.A.M.E.", "Private", c_private.GetCheck());
	cfg.SetValue("L.A.M.E.", "Id3v2", c_writeId3v2.GetCheck());
	cfg.SetValue("L.A.M.E.", "MaxBitrate", c_maxBitrate.GetCurSel());
	cfg.SetValue("L.A.M.E.", "QualityPreset", m_qualityPreset.GetCurSel());
	cfg.SetValue("L.A.M.E.", "VbrMethod", c_vbrMethod.GetCurSel());
	cfg.SetValue("L.A.M.E.", "VbrQuality", c_vbrQuality.GetCurSel());
	cfg.SetValue("L.A.M.E.", "MpegVer", c_MPEG.GetCurSel());
	cfg.SetValue("L.A.M.E.", "Abr", m_abr);
	cfg.SetValue("L.A.M.E.", "Id3v1", c_writeId3v1.GetCheck());
	cfg.SetValue("L.A.M.E.", "ThreadPriority", m_cThreadPriority.GetCurSel());
	
	if(!m_cPresetName.FindString(0, m_strPresetName)){
		
		m_cPresetName.AddString(m_strPresetName);
	}

	return TRUE;
}

void CSettingsMP3::OnSelchangeVbrMethod() 
{

	if(c_vbrMethod.GetCurSel() != 0){

		c_maxBitrate.EnableWindow(TRUE);
		c_vbrQuality.EnableWindow(TRUE);
	}
	else{

		c_maxBitrate.EnableWindow(FALSE);
		c_vbrQuality.EnableWindow(FALSE);
	}
	if(c_vbrMethod.GetCurSel() == 5){

		c_abr.EnableWindow(TRUE);
	}
	else{

		c_abr.EnableWindow(FALSE);
	}
}

void CSettingsMP3::SetControls()
{
}


void CSettingsMP3::OnSelchangeQuality() 
{

	ASSERT((m_qualityPreset.GetCurSel() >= 0) && (m_qualityPreset.GetCurSel() < 12));

	switch(m_qualityPreset.GetCurSel()){
/*
	LQP_NORMAL_QUALITY		= 0,
	LQP_LOW_QUALITY			= 1,
	LQP_HIGH_QUALITY		= 2,
	LQP_VOICE_QUALITY		= 3,
	LQP_R3MIX				= 4,
	LQP_VERYHIGH_QUALITY	= 5,
	LQP_STANDARD			= 6,
	LQP_FAST_STANDARD		= 7,
	LQP_EXTREME				= 8,
	LQP_FAST_EXTREME		= 9,
	LQP_INSANE				= 10,
	LQP_ABR					= 11,
	LQP_CBR					= 12,

    BE_MP3_MODE_STEREO		= 0
    BE_MP3_MODE_JSTEREO		= 1
    BE_MP3_MODE_DUALCHANNEL	 = 2
    BE_MP3_MODE_MONO		= 3
32 bps
40 bps
48 bps
56 bps
64 bps
80 bps
96  bps
112 bps
128 bps
144 bps
160 bps
192 bps (default) 
224 bps
256 bps
320 bps
*/

	case LQP_R3MIX: // check for R3MIX setting 
		c_vbrMethod.SetCurSel(3);	// set to VBR MTRH method
		SetMinBitrate(96); // set minimum bit rate
		m_nMode = 1;				// set joint stereo
		c_MPEG.SetCurSel(0);		// set MPEG 1
		c_vbrQuality.SetCurSel(1);	// set VBR Quality to 1
		GetDlgItem(IDC_MPEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHANNELS)->EnableWindow(FALSE);
		GetDlgItem(IDC_VBR_METHOD)->EnableWindow(FALSE);
	break;

	case LQP_STANDARD: // check for alt-preset standard 
		c_vbrMethod.SetCurSel(2);	// set to VBR RH method
		SetMinBitrate(128);			// set minimum bit rate
		m_nMode = 1;				// set joint stereo
		c_MPEG.SetCurSel(0);		// set MPEG 1
		c_vbrQuality.SetCurSel(4);	// set VBR Quality to 4
		GetDlgItem(IDC_MPEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHANNELS)->EnableWindow(FALSE);
		GetDlgItem(IDC_VBR_METHOD)->EnableWindow(FALSE);
	break;
	case LQP_FAST_STANDARD:			// check for alt-preset fast standard
		c_vbrMethod.SetCurSel(3);	// set to VBR MTRH method
		SetMinBitrate(128);			// set minimum bit rate
		m_nMode = 1;				// set joint stereo
		c_MPEG.SetCurSel(0);		// set MPEG 1
		c_vbrQuality.SetCurSel(4);// set VBR Quality to 4
		GetDlgItem(IDC_MPEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHANNELS)->EnableWindow(FALSE);
		GetDlgItem(IDC_VBR_METHOD)->EnableWindow(FALSE);
	break;

	case LQP_EXTREME: // check for alt-preset extreme
		c_vbrMethod.SetCurSel(2);	// set to VBR RH method
		SetMinBitrate(128);			// set minimum bit rate
		m_nMode = 1;				// set joint stereo
		c_MPEG.SetCurSel(0);		// set MPEG 1
		c_vbrQuality.SetCurSel(4);	// set VBR Quality to 4
		GetDlgItem(IDC_CHANNELS)->EnableWindow(FALSE);
		GetDlgItem(IDC_MPEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_VBR_METHOD)->EnableWindow(FALSE);
	break;

	case LQP_FAST_EXTREME: // check for alt-preset fast extreme
		c_vbrMethod.SetCurSel(3);	// set to VBR MTRH method
		SetMinBitrate(128);			// set minimum bit rate
		m_nMode = 1;				// set joint stereo
		c_MPEG.SetCurSel(0);		// set MPEG 1
		c_vbrQuality.SetCurSel(4);		// set VBR Quality to 4
		GetDlgItem(IDC_MPEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHANNELS)->EnableWindow(FALSE);
		GetDlgItem(IDC_VBR_METHOD)->EnableWindow(FALSE);
	break;
	case LQP_INSANE: // check for alt-preset fast insane
		c_vbrMethod.SetCurSel(0);	// set to none
		SetMinBitrate(320);			// set minimum bit rate
		m_nMode = 1;				// set joint stereo
		c_MPEG.SetCurSel(0);		// set MPEG 1
		GetDlgItem(IDC_MPEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHANNELS)->EnableWindow(FALSE);
		GetDlgItem(IDC_VBR_METHOD)->EnableWindow(FALSE);
	break;
	case LQP_ABR: // check for alt-preset fast insane
		c_vbrQuality.SetCurSel(4);// set VBR Quality to 4
		c_vbrMethod.SetCurSel(VBR_METHOD_ABR + 1);	// set to VBR ABR
		SetMinBitrate(64);		// set minimum bit rate
		m_nMode = 1;				// set joint stereo
		c_MPEG.SetCurSel(0);		// set MPEG 1
		GetDlgItem(IDC_MPEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHANNELS)->EnableWindow(FALSE);
		GetDlgItem(IDC_VBR_METHOD)->EnableWindow(FALSE);
	break;
	case LQP_CBR: // check for alt-preset fast insane
		c_vbrMethod.SetCurSel(0); // set to none
		m_nMode = 1;				// set joint stereo
		c_MPEG.SetCurSel(0);		// set MPEG 1
		GetDlgItem(IDC_MPEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHANNELS)->EnableWindow(FALSE);
		GetDlgItem(IDC_VBR_METHOD)->EnableWindow(FALSE);
	break;
	default:
		GetDlgItem(IDC_MPEG)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHANNELS)->EnableWindow(TRUE);
		GetDlgItem(IDC_VBR_METHOD)->EnableWindow(TRUE);
		
	}		

	UpdateData(FALSE);
}

BOOL CSettingsMP3::PreTranslateMessage(MSG *pMsg)
{

	if(pMsg->message == WM_MOUSEMOVE)
	{
		POINT pt = pMsg->pt;    
		ScreenToClient(&pt);

		// this is the only function to call
		m_mToolTip.ShowToolTip((CPoint)pt);
	}

	return CMySettingsPage::PreTranslateMessage(pMsg);
}

void CSettingsMP3::OnSetfocusThreadpriority() 
{

	m_mToolTip.ShowToolTip(IDC_THREADPRIORITY);
}

void CSettingsMP3::OnSavepreset() 
{

	Save();
}

void CSettingsMP3::OnDelete() 
{

	if(UpdateData(TRUE) != NULL){
	
		if(m_strPresetName == "default"){

			AfxMessageBox(IDS_DEFAULTPRESETNOTREMOVABLE, MB_OK+MB_ICONINFORMATION);
			return;
		}
		TRY{


			CIni cfg;
			cfg.SetIniFileName(m_strWd + "\\LameFE.ini");
			CFile::Remove(cfg.GetValue("LameFE", "PresetPath", m_strWd) + "\\" + m_strPresetName + ".ini");
			m_cPresetName.DeleteString(m_cPresetName.FindStringExact(0, m_strPresetName));
			m_cPresetName.SetCurSel(m_cPresetName.FindStringExact(0, "default"));
		}
		CATCH(CFileException, e)
		{
			
			AfxMessageBox(IDS_COULDNOTREMOVEPRESET, MB_OK+MB_ICONEXCLAMATION);
		}
		END_CATCH
	}
}

void CSettingsMP3::InitPresets()
{

	CFileFind finder;
	CString   strPreset;

	CIni cfg;
	cfg.SetIniFileName(m_strWd + "\\LameFE.ini");

	BOOL bResult = finder.FindFile(cfg.GetValue("LameFE", "PresetPath", m_strWd) + "\\*.ini");

	while(bResult){

		bResult = finder.FindNextFile();
		
		strPreset = finder.GetFileName();
		strPreset.MakeLower();
		if(strPreset == "lamefe.ini"){

			continue;
		}
		strPreset = strPreset.Left(strPreset.GetLength() - 4);
		m_cPresetName.AddString(strPreset);
	}
	
	int nIndex = m_cPresetName.FindStringExact(0, "default");

	if(nIndex == CB_ERR){

		m_cPresetName.SetCurSel(0);
		m_cPresetName.GetLBText(0, m_strPresetName);
	}
	else{

		m_cPresetName.SetCurSel(nIndex);
		m_cPresetName.GetLBText(nIndex, m_strPresetName);
	}
	

}

void CSettingsMP3::LoadPreset(CString strPreset)
{

	UpdateData(TRUE);
	CIni cfg;
	cfg.SetIniFileName(m_strWd + "\\LameFE.ini");
	CString strTmp = cfg.GetValue("LameFE", "PresetPath", m_strWd) + "\\" + strPreset + ".ini";
	cfg.SetIniFileName(strTmp);

	c_MPEG.SetCurSel(cfg.GetValue("L.A.M.E.", "MpegVer", 0));
	OnSelendokMpeg() ;

	m_qualityPreset.SetCurSel(cfg.GetValue("L.A.M.E.", "QualityPreset", 0));
	OnSelchangeQuality();

	c_OutSampleRate.SetCurSel(cfg.GetValue("L.A.M.E.", "OutSampleRate", 0));
	c_bitsPerSample.SetCurSel(cfg.GetValue("L.A.M.E.", "Bitrate", 14));
	c_maxBitrate.SetCurSel(cfg.GetValue("L.A.M.E.", "MaxBitrate", 16));
	c_checkSum.SetCheck(cfg.GetValue("L.A.M.E.", "Crc", FALSE));
	c_copyright.SetCheck(cfg.GetValue("L.A.M.E.", "Copyright", FALSE));
	c_original.SetCheck(cfg.GetValue("L.A.M.E.", "Original", FALSE));
	c_private.SetCheck(cfg.GetValue("L.A.M.E.", "Private", FALSE));
	c_writeId3v2.SetCheck(cfg.GetValue("L.A.M.E.", "Id3v2", TRUE));
	c_vbrMethod.SetCurSel(cfg.GetValue("L.A.M.E.", "VbrMethod", 0));
	m_abr = cfg.GetValue("L.A.M.E.", "Abr", 15);
	c_writeId3v1.SetCheck(cfg.GetValue("L.A.M.E.", "Id3v1", FALSE));
	m_cThreadPriority.SetCurSel(cfg.GetValue("L.A.M.E.", "ThreadPriority", 3));
	m_nMode = cfg.GetValue("L.A.M.E.", "Channels", 0);
	c_vbrQuality.SetCurSel(cfg.GetValue("L.A.M.E.", "VbrQuality", 5));
	m_strDescription = cfg.GetValue("L.A.M.E.", "Description", "");

	UpdateData(FALSE);
}

void CSettingsMP3::OnSelchangePresetmp3dlg() 
{

	//UpdateData(TRUE);
	m_cPresetName.GetLBText(m_cPresetName.GetCurSel(), m_strPresetName);
	UpdateData(FALSE);
	LoadPreset(m_strPresetName);
	if(m_cPresetName.FindStringExact(0, m_strPresetName) == CB_ERR){

		m_cPresetName.AddString(m_strPresetName);
	}
	
}



void CSettingsMP3::OnSelendokMpeg() 
{

	int i = 0;

	UpdateData(TRUE);

	m_nVersion = c_MPEG.GetCurSel();

	// Fill Min bitrate table
	FillMinBitrateTable();

	// Fill Max bitrate table
	FillMaxBitrateTable();

	// Fill OutputSample rate control
	FillOutSampleRateCtrl();

	if (c_MPEG.GetCurSel() == 0)
	{
		SetMinBitrate(192);
		SetMaxBitrate(256);
	}
	else
	{
		SetMinBitrate(64);
		SetMaxBitrate(160);
	}

	
	c_maxBitrate.EnableWindow(c_vbrMethod.GetCurSel() > 0);

	/*UpdateData(TRUE);
	for (int i = 0; i < sizeof(MP3SampleRates[m_nVersion]) / sizeof(MP3SampleRates[m_nVersion][0]); i++){

		CString strAdd;
		strAdd.Format("%d",MP3SampleRates[m_nVersion][ i ]);
		c_OutSampleRate.AddString(strAdd);
	}
	c_OutSampleRate.SetCurSel(0);*/
}

void CSettingsMP3::FillMaxBitrateTable()
{
	// Depending on the settings, fill the bit-rate tables
	c_maxBitrate.ResetContent();

	int nItems=sizeof(MPEGBitrates[m_nVersion][m_nLayer])/sizeof(MPEGBitrates[m_nVersion][m_nLayer][0]);
	
	int i;

	// DO NOT DISPLAY ITEM 0
	for (i=1;i<nItems;i++)
	{
		CString strItem;
		strItem.Format(_T("%d kbps"), MPEGBitrates[ m_nVersion ][ m_nLayer ][ i ]);
		c_maxBitrate.AddString(strItem);
	}
}

void CSettingsMP3::FillOutSampleRateCtrl()
{
	int		i = 0;
	CString	strLang;

	c_OutSampleRate.ResetContent();

	c_OutSampleRate.AddString("Auto");

	for (i=0; i < sizeof(MP3SampleRates[m_nVersion]) / sizeof(MP3SampleRates[m_nVersion][0]); i++){

		CString strAdd;
		strAdd.Format("%d", MP3SampleRates[m_nVersion][ i ]);
		c_OutSampleRate.AddString(strAdd);
	}
	c_OutSampleRate.SetCurSel(0);
}

void CSettingsMP3::FillMinBitrateTable()
{
	// Depending on the settings, fill the bit-rate tables
	c_bitsPerSample.ResetContent();

	int nItems=sizeof(MPEGBitrates[m_nVersion][m_nLayer])/sizeof(MPEGBitrates[m_nVersion][m_nLayer][0]);
	
	int i;

	// DO NOT DISPLAY ITEM 0
	for (i=1;i<nItems;i++)
	{
		CString strItem;
		strItem.Format(_T("%d kbps"), MPEGBitrates[ m_nVersion ][ m_nLayer ][ i ]);
		c_bitsPerSample.AddString(strItem);
	}
}

int CSettingsMP3::GetMinBitrate()
{
	int nItems=sizeof(MPEGBitrates[m_nVersion][m_nLayer])/sizeof(MPEGBitrates[m_nVersion][m_nLayer][0]);
	return MPEGBitrates[m_nVersion][m_nLayer][c_bitsPerSample.GetCurSel()+1];
}

int CSettingsMP3::GetMaxBitrate()
{
	int nItems=sizeof(MPEGBitrates[m_nVersion][m_nLayer])/sizeof(MPEGBitrates[m_nVersion][m_nLayer][0]);
	return MPEGBitrates[m_nVersion][m_nLayer][c_maxBitrate.GetCurSel()+1];
}


void CSettingsMP3::SetMaxBitrate(int nBitrate)
{
	int nItems=sizeof(MPEGBitrates[m_nVersion][m_nLayer])/sizeof(MPEGBitrates[m_nVersion][m_nLayer][0]);
	
	int i;

	for (i=1;i<nItems;i++)
	{
		if (nBitrate==MPEGBitrates[m_nVersion][m_nLayer][i])
		{
			c_maxBitrate.SetCurSel(i-1);
			return;
		}
	}

	// default return if not found
	c_maxBitrate.SetCurSel(9);
}

void CSettingsMP3::SetMinBitrate(int nBitrate)
{
	int nItems=sizeof(MPEGBitrates[m_nVersion][m_nLayer])/sizeof(MPEGBitrates[m_nVersion][m_nLayer][0]);
	
	int i;

	for (i=1;i<nItems;i++)
	{
		if (nBitrate==MPEGBitrates[m_nVersion][m_nLayer][i])
		{
			c_bitsPerSample.SetCurSel(i-1);
			return;
		}
	}

	// default return if not found
	c_bitsPerSample.SetCurSel(9);
}


