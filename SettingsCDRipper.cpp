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
#include "lamefe.h"
#include "SettingsCDRipper.h"
#include "CDRip/CDRip.h"
#include "Settings.h"
#include "I18n.h"
#include "Utils.h"
#include "Autodetect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings g_sSettings;
extern CI18n	 g_iLang;

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsCDRipper 


CSettingsCDRipper::CSettingsCDRipper(CWnd* pParent /*=NULL*/)
	: CMySettingsPage(CSettingsCDRipper::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsCDRipper)
	m_cdRipInfo = _T("");
	m_cdSpeed = 0;
	m_spinUpTime = 0;
	m_nativeSCSI = FALSE;
	m_nBlockCompare = 0;
	m_nEndOffset = 0;
	m_nReadOverlap = 0;
	m_nReadSectors = 0;
	m_nStartOffset = 0;
	m_bJitter = FALSE;
	m_bLockDuringRead = FALSE;
	m_bSwapChannels = FALSE;
	m_nRetries = 0;
	m_bUse2Phase = FALSE;
	//}}AFX_DATA_INIT
	m_bRipperPresent = FALSE;
}


void CSettingsCDRipper::DoDataExchange(CDataExchange* pDX)
{
	CMySettingsPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsCDRipper)
	DDX_Control(pDX, IDC_TWO_PHASE_EXTRACTION, m_cUse2Phase);
	DDX_Control(pDX, IDC_CHECKFORNEWCD, c_checkNewCD);
	DDX_Control(pDX, IDC_CD_SPIN_UP, c_spinUpTime);
	DDX_Control(pDX, IDC_NATIVE, c_nativeSCSI);
	DDX_Control(pDX, IDC_CD_SPEED, c_cdSpeed);
	DDX_Control(pDX, IDC_SELECT_ALL, c_select);
	DDX_Control(pDX, IDC_SWAP, c_swapChannels);
	DDX_Control(pDX, IDC_RIPPING_METHOD, c_rippingMethod);
	DDX_Control(pDX, IDC_LOCK, c_lockDrive);
	DDX_Control(pDX, IDC_EJECT, c_ejectWhenFinished);
	DDX_Control(pDX, IDC_CDROM_TYPE, c_cdromType);
	DDX_Control(pDX, IDC_CD_DRIVE, c_cdDrive);
	DDX_Text(pDX, IDC_CD_SPEED, m_cdSpeed);
	DDV_MinMaxInt(pDX, m_cdSpeed, 0, 60);
	DDX_Text(pDX, IDC_CD_SPIN_UP, m_spinUpTime);
	DDV_MinMaxInt(pDX, m_spinUpTime, 0, 120);
	DDX_Check(pDX, IDC_NATIVE, m_nativeSCSI);
	DDX_Control(pDX, IDC_CUE, c_cue);
	DDX_Text(pDX, IDC_BLOCK_COMPARE, m_nBlockCompare);
	DDX_Text(pDX, IDC_END_OFFSET, m_nEndOffset);
	DDX_Text(pDX, IDC_READ_OVERLAP, m_nReadOverlap);
	DDX_Text(pDX, IDC_READ_SECTORS, m_nReadSectors);
	DDX_Text(pDX, IDC_START_OFFSET, m_nStartOffset);
	DDX_Check(pDX, IDC_ENABLEJITTERCORRECTION, m_bJitter);
	DDX_Check(pDX, IDC_LOCK, m_bLockDuringRead);
	DDX_Check(pDX, IDC_SWAP, m_bSwapChannels);
	DDX_Text(pDX, IDC_NUMBER_RETRIES, m_nRetries);
	DDX_Check(pDX, IDC_TWO_PHASE_EXTRACTION, m_bUse2Phase);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsCDRipper, CMySettingsPage)
	//{{AFX_MSG_MAP(CSettingsCDRipper)
	ON_BN_CLICKED(IDC_NATIVE, OnNative)
	ON_CBN_SELCHANGE(IDC_CD_DRIVE, OnSelchangeCdDrive)
	ON_BN_CLICKED(IDC_AUTODETECT, OnAutodetect)
	ON_CBN_SELCHANGE(IDC_RIPPING_METHOD, OnSelchangeRippingMethod)
	ON_BN_CLICKED(IDC_ENABLEJITTERCORRECTION, OnEnablejittercorrection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsCDRipper 

void CSettingsCDRipper::Init(CString strWd)
{

	CMySettingsPage::Init(strWd);

	g_iLang.TranslateDialog(this, IDD_SETTINGS_CDRIPPER);

	m_bRipperPresent = ((CLameFEApp*)AfxGetApp())->GetRipperStatus();

	if(Utils::IsWindowsNT())
	{
		c_nativeSCSI.EnableWindow(TRUE );
	}
	else
	{
		m_nativeSCSI = FALSE;
		c_nativeSCSI.EnableWindow(FALSE);
	}

	if(m_pToolTip != NULL){

		m_pToolTip->AddTool(&c_cdDrive, g_iLang.GetString(IDS_TOOL_DEFAULTDRIVE));
		m_pToolTip->AddTool(&c_cdromType, g_iLang.GetString(IDS_TOOL_CDROMTYPE));
		m_pToolTip->AddTool(&c_ejectWhenFinished, g_iLang.GetString(IDS_TOOL_EJECTCD));
		m_pToolTip->AddTool(&c_lockDrive, g_iLang.GetString(IDS_TOOL_LOCKDRIVE));
		m_pToolTip->AddTool(&c_rippingMethod, g_iLang.GetString(IDS_TOOL_RMETHOD));
		m_pToolTip->AddTool(&c_select, g_iLang.GetString(IDS_TOOL_SELECT));
		m_pToolTip->AddTool(&c_swapChannels, g_iLang.GetString(IDS_TOOL_SWAPCHNL));
		m_pToolTip->AddTool(&c_spinUpTime, g_iLang.GetString(IDS_TOOL_SPINUP));
		m_pToolTip->AddTool(&c_nativeSCSI, g_iLang.GetString(IDS_TOOL_NTSCSI));
		m_pToolTip->AddTool(&c_cdSpeed, g_iLang.GetString(IDS_TOOL_CDSPEED));
		m_pToolTip->AddTool(&c_cue, g_iLang.GetString(IDS_TOOL_WRITECUE));
		m_pToolTip->AddTool(&c_checkNewCD, g_iLang.GetString(IDS_TOOL_CHECKNEWCD));
		m_pToolTip->AddTool(&m_cUse2Phase, g_iLang.GetString(IDS_TOOL_USE2PHASE));
		m_pToolTip->Activate(TRUE);
	}
	

	CDROMPARAMS cdParams;

	//nSelCD = -1;

	if(m_bRipperPresent){

		int nSelCD = CR_GetActiveCDROM();


		for (int i = 0; i < CR_GetNumCDROM(); i++)
		{

			CR_SetActiveCDROM(i);
			CR_GetCDROMParameters(&cdParams);
			c_cdDrive.AddString(cdParams.lpszCDROMID);
		}
		
		CR_SetActiveCDROM(nSelCD);

		c_cdDrive.SetCurSel(nSelCD);
	}

	InitControls();
}

BOOL CSettingsCDRipper::Save()
{

	if(UpdateData(TRUE) == NULL){

		return FALSE;
	}

	// Construct CDROMPARAMS
	CDROMPARAMS cdParams;


	// Get all values that have not been changed yet
	CR_GetCDROMParameters( &cdParams );

	cdParams.nNumCompareSectors =	m_nBlockCompare;
	cdParams.nNumOverlapSectors =	m_nReadOverlap;
	cdParams.nNumReadSectors =		m_nReadSectors;
	cdParams.nSpeed =				m_cdSpeed;
	cdParams.nSpinUpTime =			m_spinUpTime;
	cdParams.bSwapLefRightChannel =	c_swapChannels.GetCheck();
	cdParams.bJitterCorrection =	m_bJitter;
	cdParams.nOffsetStart =			m_nStartOffset;
	cdParams.nOffsetEnd =			m_nEndOffset;
	cdParams.bAspiPosting =			m_bAspiPosting;
	cdParams.nAspiRetries =			m_nRetries;
	cdParams.bMultiReadFirstOnly =	m_bFirstBlockOnly;
	cdParams.bEnableMultiRead =		m_bMultipleRead;
	cdParams.nMultiReadCount =		m_nMultipleReadCount;
	cdParams.bLockDuringRead =		m_bLockDuringRead;
	cdParams.nRippingMode =			( c_rippingMethod.GetCurSel() > 0 ) ? 1 : 0 ;
	cdParams.nParanoiaMode =		c_rippingMethod.GetCurSel() - cdParams.nRippingMode ;

	CR_SetTransportLayer(m_nativeSCSI);

	// Get cdParames of current selected CD-ROM
	CR_SetCDROMParameters(&cdParams);

	// Set Drive Type (has to be after SetCDROm Parameters
	CR_SelectCDROMType(DRIVETYPE(c_cdromType.GetCurSel()));

	// Save it to the ini file
	CR_SaveSettings();


	g_sSettings.SetLock(c_lockDrive.GetCheck());
	g_sSettings.SetEject(c_ejectWhenFinished.GetCheck());
	g_sSettings.SetSelect(c_select.GetCheck());
	g_sSettings.SetWriteCue(c_cue.GetCheck());
	g_sSettings.SetCheckForNewCD(c_checkNewCD.GetCheck());
//	g_sSettings.SetNumReadBuffers(m_numBuffers);
	g_sSettings.SetUse2Phase(m_cUse2Phase.GetCheck());

	g_sSettings.Save();
	return TRUE;
}

void CSettingsCDRipper::OnNative() 
{

	UpdateData(TRUE);

	CR_SetTransportLayer(m_nativeSCSI);

	// save settings
	CR_SaveSettings();

	((CLameFEApp*)AfxGetApp())->InitCDRipper();

	m_nativeSCSI = CR_GetTransportLayer();

	InitControls();

	UpdateData(FALSE);
}

void CSettingsCDRipper::OnSelchangeCdDrive() 
{

	UpdateData(TRUE);
	
	if(m_bRipperPresent){

		Save();
		CR_SetActiveCDROM(c_cdDrive.GetCurSel());
		InitControls();
	}
}

void CSettingsCDRipper::InitControls()
{

	if(m_bRipperPresent){

/*		CDROMPARAMS cdParams;

		CR_GetCDROMParameters(&cdParams);
		
		m_cdSpeed = cdParams.nSpeed;
		m_spinUpTime = cdParams.nSpinUpTime;
		c_swapChannels.SetCheck(cdParams.bSwapLefRightChannel);

		c_cdromType.SetCurSel(CR_GetCDROMType());
		c_rippingMethod.SetCurSel(cdParams.nRippingMode + cdParams.nParanoiaMode); 
		m_nativeSCSI = CR_GetTransportLayer();*/
		// Construct CDROMPARAMS
		CDROMPARAMS cdParams;

		// Get cdParames of current selected CD-ROM
		CR_GetCDROMParameters(&cdParams);

		m_nBlockCompare	= cdParams.nNumCompareSectors;
		m_nReadOverlap	= cdParams.nNumOverlapSectors;
		m_nReadSectors	= cdParams.nNumReadSectors;
		m_cdSpeed		= cdParams.nSpeed;
		m_spinUpTime	= cdParams.nSpinUpTime;
		m_bSwapChannels = cdParams.bSwapLefRightChannel;
		m_bJitter		= cdParams.bJitterCorrection;
		m_nStartOffset	= cdParams.nOffsetStart;
		m_nEndOffset	= cdParams.nOffsetEnd;
		m_bAspiPosting	= cdParams.bAspiPosting;

		c_rippingMethod.SetCurSel( cdParams.nRippingMode + cdParams.nParanoiaMode );

		m_nRetries				= cdParams.nAspiRetries;
		m_nativeSCSI			= (CR_GetTransportLayer() == TRANSPLAYER_NTSCSI);
		m_bFirstBlockOnly		= cdParams.bMultiReadFirstOnly;
		m_bMultipleRead			= cdParams.bEnableMultiRead;
		m_nMultipleReadCount	= cdParams.nMultiReadCount;
		m_bLockDuringRead		= cdParams.bLockDuringRead;
		m_bUseCDText			= cdParams.bUseCDText;
		c_cdromType.SetCurSel((int)CR_GetCDROMType());

	}

	m_bUse2Phase	=  g_sSettings.GetUse2Phase();
	c_ejectWhenFinished.SetCheck(g_sSettings.GetEject());
	//c_lockDrive.SetCheck(g_sSettings.GetLock());
	c_select.SetCheck(g_sSettings.GetSelect());
	c_cue.SetCheck(g_sSettings.GetWriteCue());
	c_checkNewCD.SetCheck(g_sSettings.GetCheckForNewCD());
	//m_numBuffers = g_sSettings.GetNumReadBuffers();
	
	SetControls();
	UpdateData(FALSE);
}

void CSettingsCDRipper::OnAutodetect() 
{

	if(AfxMessageBox(g_iLang.GetString(IDS_AUTODEFECTWARNING), MB_YESNO) == IDYES){

		CAutoDetect dlg;
		if (dlg.DoModal()==IDOK){

			c_cdromType.SetCurSel(CUSTOMDRIVE);

			UpdateData(FALSE);
		}
	}
}

void CSettingsCDRipper::SetControls()
{

	if ( c_rippingMethod.GetCurSel() > 0 ){

		GetDlgItem(IDC_ENABLEJITTERCORRECTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_NUMBER_RETRIES)->EnableWindow(FALSE);
	}
	else{

		GetDlgItem(IDC_ENABLEJITTERCORRECTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_NUMBER_RETRIES)->EnableWindow(TRUE);
	}


	// Set the controls
	if (m_bJitter &&  (c_rippingMethod.GetCurSel() == 0)){

		GetDlgItem(IDC_READ_OVERLAP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BLOCK_COMPARE)->EnableWindow(TRUE);
	}
	else{

		GetDlgItem(IDC_READ_OVERLAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BLOCK_COMPARE)->EnableWindow(FALSE);
	}
}

void CSettingsCDRipper::OnSelchangeRippingMethod() 
{

	SetControls();
}

void CSettingsCDRipper::OnEnablejittercorrection() 
{

	SetControls();
}
