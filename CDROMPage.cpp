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
#include "CDROMPage.h"
#include "CDRip/CDRip.h"
#include "cfgFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite CCDROMPage 

IMPLEMENT_DYNCREATE(CCDROMPage, CPropertyPage)

CCDROMPage::CCDROMPage() : CPropertyPage(CCDROMPage::IDD)
{
	//{{AFX_DATA_INIT(CCDROMPage)
	m_cdRipInfo = _T("");
	m_cdripVersion = _T("");
	m_cdSpeed = 0;
	m_spinUpTime = 0;
	m_nativeSCSI = FALSE;
	m_nTimeOut = 0;
	m_numBuffers = -1;
	//}}AFX_DATA_INIT
	m_pToolTip = NULL;
}

CCDROMPage::~CCDROMPage()
{
	delete m_pToolTip;
}

void CCDROMPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCDROMPage)
	DDX_Control(pDX, IDC_CHECKFORNEWCD, c_checkNewCD);
	DDX_Control(pDX, IDC_NUMBUFFERS, c_numBuffers);
	DDX_Control(pDX, IDC_BATCH_TIMEOUTEDIT, c_nBatchTimeOut);
	DDX_Control(pDX, IDC_BATCH_BEEP, c_batchBeep);
	DDX_Control(pDX, IDC_BATCHTIMEOUT, c_batchTimeOut);
	DDX_Control(pDX, IDC_BATCHALLDRIVES, c_batchAllDrives);
	DDX_Control(pDX, IDC_BATCH_FREEDB, c_batchFreeDB);
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
	DDX_Text(pDX, IDC_Version, m_cdripVersion);
	DDX_Text(pDX, IDC_CD_SPEED, m_cdSpeed);
	DDV_MinMaxInt(pDX, m_cdSpeed, 0, 60);
	DDX_Text(pDX, IDC_CD_SPIN_UP, m_spinUpTime);
	DDV_MinMaxInt(pDX, m_spinUpTime, 0, 120);
	DDX_Check(pDX, IDC_NATIVE, m_nativeSCSI);
	DDX_Control(pDX, IDC_CUE, c_cue);
	DDX_Text(pDX, IDC_BATCH_TIMEOUTEDIT, m_nTimeOut);
	DDV_MinMaxInt(pDX, m_nTimeOut, 1, 360);
	DDX_CBIndex(pDX, IDC_NUMBUFFERS, m_numBuffers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCDROMPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCDROMPage)
	ON_BN_CLICKED(IDC_NATIVE, OnNative)
	ON_CBN_SELCHANGE(IDC_CD_DRIVE, OnSelchangeCdDrive)
	ON_BN_CLICKED(IDC_BATCHTIMEOUT, OnBatchtimeout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CCDROMPage 

void CCDROMPage::init(CString wdir, BOOL bRipperPresent)
{

	wd = wdir;
	m_bRipperPresent = bRipperPresent;
}

void CCDROMPage::OnOK()
{

	SaveSettings();
}

void CCDROMPage::OnCancel()
{

}

void CCDROMPage::SaveSettings()
{

	UpdateData(TRUE);

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


	cfgFile cfg(wd);

	cfg.SetValue("lock", c_lockDrive.GetCheck());
	cfg.SetValue("eject", c_ejectWhenFinished.GetCheck());
	cfg.SetValue("select", c_select.GetCheck());
	cfg.SetValue("writecue", c_cue.GetCheck());
	cfg.SetValue("BatchUseAllDrives", c_batchAllDrives.GetCheck());
	cfg.SetValue("BatchBeepOnFinishCD", c_batchBeep.GetCheck());
	cfg.SetValue("BatchFreeDB", c_batchFreeDB.GetCheck());
	cfg.SetValue("BatchTimesOut", c_batchTimeOut.GetCheck());
	cfg.SetValue("BatchTimesOutAfterMin", m_nTimeOut);
	cfg.SetValue("nNumReadBuffers", m_numBuffers);
	cfg.SetValue("CheckForNewCD", c_checkNewCD.GetCheck());
}

void CCDROMPage::initControls()
{

	if(m_bRipperPresent){

		CDROMPARAMS cdParams;

		CR_GetCDROMParameters(&cdParams);
		
		m_cdSpeed = cdParams.nSpeed;
		m_spinUpTime = cdParams.nSpinUpTime;
		c_swapChannels.SetCheck(cdParams.bSwapLefRightChannel);

		c_cdromType.SetCurSel(CR_GetCDROMType());
		c_rippingMethod.SetCurSel(cdParams.nRippingMode + cdParams.nParanoiaMode); 
	}
	
	cfgFile cfg(wd);
	c_ejectWhenFinished.SetCheck(cfg.GetValue("eject"));
	c_lockDrive.SetCheck(cfg.GetValue("lock"));
	c_select.SetCheck(cfg.GetValue("select"));
	c_cue.SetCheck(cfg.GetValue("writecue"));
	c_checkNewCD.SetCheck(cfg.GetValue("CheckForNewCD"));

	m_numBuffers = cfg.GetValue("nNumReadBuffers");
	c_batchAllDrives.SetCheck(cfg.GetValue("BatchUseAllDrives"));
	c_batchBeep.SetCheck(cfg.GetValue("BatchBeepOnFinishCD"));
	c_batchFreeDB.SetCheck(cfg.GetValue("BatchFreeDB"));
	c_batchTimeOut.SetCheck(cfg.GetValue("BatchTimesOut"));
	m_nTimeOut = cfg.GetValue("BatchTimesOutAfterMin");
	UpdateData(FALSE);
	OnBatchtimeout();
}
BOOL CCDROMPage::OnInitDialog()
{

	CPropertyPage::OnInitDialog();


	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
	   TRACE("Unable To create ToolTip\n");
	   return TRUE;
	}

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
	m_pToolTip->AddTool(&c_batchAllDrives, IDS_TOOL_BATCHALLCDS);
	m_pToolTip->AddTool(&c_batchBeep, IDS_TOOL_BATCHBEEP);
	m_pToolTip->AddTool(&c_batchFreeDB, IDS_TOOL_BATCHFREEDB);
	m_pToolTip->AddTool(&c_batchTimeOut, IDS_TOOL_BATCHTIMESOUT);
	m_pToolTip->AddTool(&c_nBatchTimeOut, IDS_TOOL_BATCHTIME);
	m_pToolTip->AddTool(&c_numBuffers, IDS_TOOL_READBUFFERS);
	m_pToolTip->AddTool(&c_cue, IDS_TOOL_WRITECUE);
	m_pToolTip->Activate(TRUE);
	
	m_cdripVersion.Format("CDRip.dll Version %.2f\n(c) 1998-2002 by Albert L. Faber\nhttp://www.cdex.n3.net", (float)CR_GetCDRipVersion()/100);

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

	initControls();

	return TRUE;
}


void CCDROMPage::OnNative() 
{

	UpdateData(TRUE);
	if (m_nativeSCSI)
	{
		AfxMessageBox(IDS_RESTARTLAME);
	}
}

void CCDROMPage::OnSelchangeCdDrive() 
{

	UpdateData(TRUE);
	
	if(m_bRipperPresent){

		SaveSettings();
		CR_SetActiveCDROM(c_cdDrive.GetCurSel());
		initControls();
	}
}

BOOL CCDROMPage::PreTranslateMessage(MSG* pMsg) 
{

	if (NULL != m_pToolTip){
		
		m_pToolTip->RelayEvent(pMsg);
	}
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}


void CCDROMPage::OnBatchtimeout() 
{

	c_nBatchTimeOut.EnableWindow(c_batchTimeOut.GetCheck());
}
