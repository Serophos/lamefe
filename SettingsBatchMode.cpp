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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings g_sSettings;

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsBatchMode 


CSettingsBatchMode::CSettingsBatchMode(CWnd* pParent /*=NULL*/)
	: CMySettingsPage(CSettingsBatchMode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsBatchMode)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	m_nTimeOut = 0;
	//}}AFX_DATA_INIT
}


void CSettingsBatchMode::DoDataExchange(CDataExchange* pDX)
{
	CMySettingsPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsBatchMode)
		// HINWEIS: Der Klassen-Assistent fügt hier DDX- und DDV-Aufrufe ein
	DDX_Control(pDX, IDC_APPENDDISCID, c_appendDiscID);
	DDX_Control(pDX, IDC_BATCH_TIMEOUTEDIT, c_nBatchTimeOut);
	DDX_Control(pDX, IDC_BATCH_BEEP, c_batchBeep);
	DDX_Control(pDX, IDC_BATCHTIMEOUT, c_batchTimeOut);
	DDX_Control(pDX, IDC_BATCHALLDRIVES, c_batchAllDrives);
	DDX_Control(pDX, IDC_BATCH_FREEDB, c_batchFreeDB);
	DDX_Text(pDX, IDC_BATCH_TIMEOUTEDIT, m_nTimeOut);
	DDV_MinMaxInt(pDX, m_nTimeOut, 1, 360);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsBatchMode, CMySettingsPage)
	//{{AFX_MSG_MAP(CSettingsBatchMode)
		// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	ON_BN_CLICKED(IDC_BATCHTIMEOUT, OnBatchtimeout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsBatchMode 

void CSettingsBatchMode::Init(CString strWd)
{

	CMySettingsPage::Init(strWd);

	c_batchAllDrives.SetCheck(g_sSettings.GetBatchUseAllDrives());
	c_appendDiscID.SetCheck(g_sSettings.GetBatchAppendDiscID());
	c_batchBeep.SetCheck(g_sSettings.GetBatchBeepOnFinished());
	c_batchFreeDB.SetCheck(g_sSettings.GetBatchFreeDB());
	c_batchTimeOut.SetCheck(g_sSettings.GetBatchTimesOut());
	m_nTimeOut = g_sSettings.GetBatchTimesOutAfterMin();

	if(m_pToolTip != NULL){

		m_pToolTip->AddTool(&c_batchAllDrives, IDS_TOOL_BATCHALLCDS);
		m_pToolTip->AddTool(&c_batchBeep, IDS_TOOL_BATCHBEEP);
		m_pToolTip->AddTool(&c_batchFreeDB, IDS_TOOL_BATCHFREEDB);
		m_pToolTip->AddTool(&c_batchTimeOut, IDS_TOOL_BATCHTIMESOUT);
		m_pToolTip->AddTool(&c_nBatchTimeOut, IDS_TOOL_BATCHTIME);
		m_pToolTip->AddTool(&c_appendDiscID, IDS_TOOL_BATCHAPPENDDISCID);
		m_pToolTip->Activate(TRUE);
	}
	UpdateData(FALSE);
}

BOOL CSettingsBatchMode::Save()
{

	if(UpdateData(TRUE) == NULL){

		return FALSE;
	}

	g_sSettings.SetBatchUseAllDrives(c_batchAllDrives.GetCheck());
	g_sSettings.SetBatchBeepOnFinished(c_batchBeep.GetCheck());
	g_sSettings.SetBatchAppendDiscID(c_appendDiscID.GetCheck());
	g_sSettings.SetBatchFreeDB(c_batchFreeDB.GetCheck());
	g_sSettings.SetBatchTimesOut(c_batchTimeOut.GetCheck());
	g_sSettings.SetBatchTimesOutAfterMin(m_nTimeOut);
	g_sSettings.Save();

	return TRUE;
}

void CSettingsBatchMode::OnBatchtimeout() 
{

	c_nBatchTimeOut.EnableWindow(c_batchTimeOut.GetCheck());
}
