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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings g_sSettings;

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsCDRipper 


CSettingsCDRipper::CSettingsCDRipper(CWnd* pParent /*=NULL*/)
	: CMySettingsPage(CSettingsCDRipper::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsCDRipper)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	m_cdRipInfo = _T("");
	m_cdSpeed = 0;
	m_spinUpTime = 0;
	m_nativeSCSI = FALSE;
	m_numBuffers = -1;
	//}}AFX_DATA_INIT
	m_bRipperPresent = FALSE;
}


void CSettingsCDRipper::DoDataExchange(CDataExchange* pDX)
{
	CMySettingsPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsCDRipper)
		// HINWEIS: Der Klassen-Assistent fügt hier DDX- und DDV-Aufrufe ein
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
	DDX_CBIndex(pDX, IDC_NUMBUFFERS, m_numBuffers);
	DDX_Control(pDX, IDC_NUMBUFFERS, c_numBuffers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsCDRipper, CMySettingsPage)
	//{{AFX_MSG_MAP(CSettingsCDRipper)
		// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	ON_BN_CLICKED(IDC_NATIVE, OnNative)
	ON_CBN_SELCHANGE(IDC_CD_DRIVE, OnSelchangeCdDrive)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsCDRipper 

void CSettingsCDRipper::Init(CString strWd)
{

	CMySettingsPage::Init(strWd);

	m_bRipperPresent = ((CLameFEApp*)AfxGetApp())->GetRipperStatus();

	if(m_pToolTip != NULL){

		m_pToolTip->AddTool(&c_cdDrive, IDS_TOOL_DEFAULTDRIVE);
		m_pToolTip->AddTool(&c_cdromType, IDS_TOOL_CDROMTYPE);
		m_pToolTip->AddTool(&c_ejectWhenFinished, IDS_TOOL_EJECTCD);
		m_pToolTip->AddTool(&c_lockDrive, IDS_TOOL_LOCKDRIVE);
		m_pToolTip->AddTool(&c_rippingMethod, IDS_TOOL_RMETHOD);
		m_pToolTip->AddTool(&c_select, IDS_TOOL_SELECT);
		m_pToolTip->AddTool(&c_swapChannels, IDS_TOOL_SWAPCHNL);
		m_pToolTip->AddTool(&c_spinUpTime, IDS_TOOL_SPINUP);
		m_pToolTip->AddTool(&c_nativeSCSI, IDS_TOOL_NTSCSI);
		m_pToolTip->AddTool(&c_cdSpeed, IDS_TOOL_CDSPEED);
		m_pToolTip->AddTool(&c_cue, IDS_TOOL_WRITECUE);
		m_pToolTip->AddTool(&c_checkNewCD, IDS_TOOL_CHECKNEWCD);
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

	cdParams.nNumCompareSectors =	1;
	cdParams.nNumOverlapSectors =	7;
	cdParams.nNumReadSectors =		26;
	cdParams.nSpeed =				m_cdSpeed;
	cdParams.nSpinUpTime =			m_spinUpTime;
	cdParams.bSwapLefRightChannel =	c_swapChannels.GetCheck();
	cdParams.bJitterCorrection =	TRUE;
	cdParams.nOffsetStart =			0;
	cdParams.nOffsetEnd =			0;
	cdParams.bAspiPosting =			3000;
	cdParams.nAspiRetries =			0;
	cdParams.bMultiReadFirstOnly =	0;
	cdParams.bEnableMultiRead =		0;
	cdParams.nMultiReadCount =		0;
	cdParams.bLockDuringRead =		c_lockDrive.GetCheck();
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
	g_sSettings.SetNumReadBuffers(m_numBuffers);
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

		CDROMPARAMS cdParams;

		CR_GetCDROMParameters(&cdParams);
		
		m_cdSpeed = cdParams.nSpeed;
		m_spinUpTime = cdParams.nSpinUpTime;
		c_swapChannels.SetCheck(cdParams.bSwapLefRightChannel);

		c_cdromType.SetCurSel(CR_GetCDROMType());
		c_rippingMethod.SetCurSel(cdParams.nRippingMode + cdParams.nParanoiaMode); 
		m_nativeSCSI = CR_GetTransportLayer();
	}
	
	c_ejectWhenFinished.SetCheck(g_sSettings.GetEject());
	c_lockDrive.SetCheck(g_sSettings.GetLock());
	c_select.SetCheck(g_sSettings.GetSelect());
	c_cue.SetCheck(g_sSettings.GetWriteCue());
	c_checkNewCD.SetCheck(g_sSettings.GetCheckForNewCD());
	m_numBuffers = g_sSettings.GetNumReadBuffers();
	
	UpdateData(FALSE);
}
