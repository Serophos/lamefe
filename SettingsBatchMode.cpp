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
#include "SettingsBatchMode.h"
#include "Settings.h"
#include "I18n.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings g_sSettings;
extern CI18n	 g_iLang;

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsBatchMode 


CSettingsBatchMode::CSettingsBatchMode(CWnd* pParent /*=NULL*/)
	: CMySettingsPage(CSettingsBatchMode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsBatchMode)
	m_nTimeOut = 0;
	m_strActive = _T("");
	m_strAvailable = _T("");
	//}}AFX_DATA_INIT
}


void CSettingsBatchMode::DoDataExchange(CDataExchange* pDX)
{
	CMySettingsPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsBatchMode)
	DDX_Control(pDX, IDC_AVAILABLE_COMMANDS, m_lAvailable);
	DDX_Control(pDX, IDC_ACTIVE_COMMANDS, m_lActive);
	DDX_Control(pDX, IDC_APPENDDISCID, c_appendDiscID);
	DDX_Control(pDX, IDC_BATCH_TIMEOUTEDIT, c_nBatchTimeOut);
	DDX_Control(pDX, IDC_BATCHTIMEOUT, c_batchTimeOut);
	DDX_Control(pDX, IDC_BATCHALLDRIVES, c_batchAllDrives);
	DDX_Text(pDX, IDC_BATCH_TIMEOUTEDIT, m_nTimeOut);
	DDV_MinMaxInt(pDX, m_nTimeOut, 1, 360);
	DDX_LBString(pDX, IDC_ACTIVE_COMMANDS, m_strActive);
	DDX_LBString(pDX, IDC_AVAILABLE_COMMANDS, m_strAvailable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsBatchMode, CMySettingsPage)
	//{{AFX_MSG_MAP(CSettingsBatchMode)
	ON_BN_CLICKED(IDC_BATCHTIMEOUT, OnBatchtimeout)
	ON_BN_CLICKED(IDC_ADD_COMMAND, OnAddCommand)
	ON_BN_CLICKED(IDC_REMOVE_COMMAND, OnRemoveCommand)
	ON_BN_CLICKED(IDC_COMMAND_UP, OnCommandUp)
	ON_BN_CLICKED(IDC_COMMAND_DOWN, OnCommandDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsBatchMode 

void CSettingsBatchMode::Init(CString strWd)
{

	CMySettingsPage::Init(strWd);

	g_iLang.TranslateDialog(this, IDD_SETTINGS_BATCHMODE);

	c_batchAllDrives.SetCheck(g_sSettings.GetBatchUseAllDrives());
	c_appendDiscID.SetCheck(g_sSettings.GetBatchAppendDiscID());
	c_batchTimeOut.SetCheck(g_sSettings.GetBatchTimesOut());
	m_nTimeOut = g_sSettings.GetBatchTimesOutAfterMin();

	if(m_pToolTip != NULL){

		m_pToolTip->AddTool(&c_batchAllDrives, g_iLang.GetString(IDS_TOOL_BATCHALLCDS));
		m_pToolTip->AddTool(&c_batchTimeOut, g_iLang.GetString(IDS_TOOL_BATCHTIMESOUT));
		m_pToolTip->AddTool(&c_nBatchTimeOut, g_iLang.GetString(IDS_TOOL_BATCHTIME));
		m_pToolTip->AddTool(&c_appendDiscID, g_iLang.GetString(IDS_TOOL_BATCHAPPENDDISCID));
		m_pToolTip->Activate(TRUE);
	}

	CString strTmp;

	for(int nID = IDS_NBATCH_BEEP; nID <= IDS_NBATCH_FINISH; nID++){

		strTmp = g_iLang.GetString(nID);
		m_lAvailable.AddString(strTmp);
	}

	strTmp = g_sSettings.GetBatchScript();
	nID = 0;
	
	while((nID = strTmp.Find("|", 0)) > 0){

		m_lActive.AddString(strTmp.Left(nID));
		strTmp = strTmp.Right(strTmp.GetLength() - nID - 1);
	}
	UpdateData(FALSE);
}

BOOL CSettingsBatchMode::Save()
{

	if(UpdateData(TRUE) == NULL){

		return FALSE;
	}

	CString strTmp, strScript;

	for(int i = 0; i < m_lActive.GetCount(); i++){

		m_lActive.GetText(i, strTmp);
		strScript += strTmp + "|";
	}
	g_sSettings.SetBatchScript(strScript);
	g_sSettings.SetBatchUseAllDrives(c_batchAllDrives.GetCheck());
	g_sSettings.SetBatchAppendDiscID(c_appendDiscID.GetCheck());
	g_sSettings.SetBatchTimesOut(c_batchTimeOut.GetCheck());
	g_sSettings.SetBatchTimesOutAfterMin(m_nTimeOut);
	g_sSettings.Save();

	return TRUE;
}

void CSettingsBatchMode::OnBatchtimeout() 
{

	c_nBatchTimeOut.EnableWindow(c_batchTimeOut.GetCheck());
}

void CSettingsBatchMode::OnAddCommand() 
{

	if(m_lAvailable.GetCurSel() >= 0){

		CString strTmp;
		m_lAvailable.GetText(m_lAvailable.GetCurSel(), strTmp);
		m_lActive.AddString(strTmp);
	}
}

void CSettingsBatchMode::OnRemoveCommand() 
{

	if(m_lActive.GetCurSel() >= 0){

		m_lActive.DeleteString(m_lActive.GetCurSel());
	}
}

void CSettingsBatchMode::OnCommandUp() 
{

	if(m_lActive.GetCurSel() > 0){


		CString strTmp;
		int nPos = m_lActive.GetCurSel();

		m_lActive.GetText(nPos, strTmp);
		m_lActive.DeleteString(nPos);
		m_lActive.InsertString(nPos-1, strTmp);
		m_lActive.SetCurSel(nPos-1);
	}
}

void CSettingsBatchMode::OnCommandDown() 
{

	
	if((m_lActive.GetCurSel() >= 0) && (m_lActive.GetCurSel() < m_lActive.GetCount())){


		CString strTmp;
		int nPos = m_lActive.GetCurSel();

		m_lActive.GetText(nPos, strTmp);
		m_lActive.DeleteString(nPos);
		m_lActive.InsertString(nPos+1, strTmp);
		m_lActive.SetCurSel(nPos+1);
	}
}
