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
#include "Ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

	CIni cfg;
	cfg.SetIniFileName(m_strWd + "\\LameFE.ini");

	c_batchAllDrives.SetCheck(cfg.GetValue("CD-ROM", "BatchUseAllDrives", FALSE));
	c_appendDiscID.SetCheck(cfg.GetValue("CD-ROM", "BatchAppendDiscID", TRUE));
	c_batchBeep.SetCheck(cfg.GetValue("CD-ROM", "BatchBeepOnFinishCD", TRUE));
	c_batchFreeDB.SetCheck(cfg.GetValue("CD-ROM", "BatchFreeDB", TRUE));
	c_batchTimeOut.SetCheck(cfg.GetValue("CD-ROM", "BatchTimesOut", TRUE));
	m_nTimeOut = cfg.GetValue("CD-ROM", "BatchTimesOutAfterMin", 30);
	UpdateData(FALSE);
}

BOOL CSettingsBatchMode::Save()
{

	if(UpdateData(TRUE) == NULL){

		return FALSE;
	}

	CIni cfg;
	cfg.SetIniFileName(m_strWd + "\\LameFE.ini");

	cfg.SetValue("CD-ROM", "BatchUseAllDrives", c_batchAllDrives.GetCheck());
	cfg.SetValue("CD-ROM", "BatchBeepOnFinishCD", c_batchBeep.GetCheck());
	cfg.SetValue("CD-ROM", "BatchAppendDiscID", c_appendDiscID.GetCheck());
	cfg.SetValue("CD-ROM", "BatchFreeDB", c_batchFreeDB.GetCheck());
	cfg.SetValue("CD-ROM", "BatchTimesOut", c_batchTimeOut.GetCheck());
	cfg.SetValue("CD-ROM", "BatchTimesOutAfterMin", m_nTimeOut);

	return TRUE;
}

void CSettingsBatchMode::OnBatchtimeout() 
{

	c_nBatchTimeOut.EnableWindow(c_batchTimeOut.GetCheck());
}
