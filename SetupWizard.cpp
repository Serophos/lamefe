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
#include "SetupWizard.h"
#include "CDRip/CDRip.h"
#include "cfgFile.h"

#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSetupWizard 


CSetupWizard::CSetupWizard(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupWizard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupWizard)
	m_DefaultPlayer = _T("");
	m_Path2DefaultPlayer = _T("");
	//}}AFX_DATA_INIT
}


void CSetupWizard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupWizard)
	DDX_Control(pDX, IDC_DEFAULT_CD, m_cDefaultCD);
	DDX_Text(pDX, IDC_DEFAULTPLAYER, m_DefaultPlayer);
	DDX_Text(pDX, IDC_PATH2PLAYER, m_Path2DefaultPlayer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupWizard, CDialog)
	//{{AFX_MSG_MAP(CSetupWizard)
	ON_BN_CLICKED(IDC_CHANGE_PLAYER, OnChangePlayer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSetupWizard::OnInitDialog()
{
	
	CDialog::OnInitDialog();
	
	CFileStatus cfs;
	if(CFile::GetStatus("C:\\Programme\\Winamp\\Plugins", cfs)){
		
		m_Path2DefaultPlayer = "C:\\Programme\\Winamp\\Plugins";
	}

	if(CFile::GetStatus("D:\\Programme\\Winamp\\Plugins", cfs)){
		
		m_Path2DefaultPlayer = "D:\\Programme\\Winamp\\Plugins";
	}

	if(CFile::GetStatus("D:\\Program Files\\Winamp\\Plugins", cfs)){
		
		m_Path2DefaultPlayer = "D:\\Program Files\\Winamp\\Plugins";
	}

	if(CFile::GetStatus("D:\\Program Files\\Winamp\\Plugins", cfs)){
		
		m_Path2DefaultPlayer = "D:\\Program Files\\Winamp\\Plugins";
	}
	
	if(m_Path2DefaultPlayer.IsEmpty()){

		m_DefaultPlayer.LoadString(IDS_SETUP_NOWINAMP);
	}
	else{

		m_DefaultPlayer.LoadString(IDS_SETUP_FOUNDWINAMP);
	}

	char buffer[1024];
	_getcwd(buffer,1024);
	wd = buffer;

	CR_Init(wd + "\\lameFE.ini");

	CDROMPARAMS cdParams;

	for (int i = 0; i < CR_GetNumCDROM(); i++)
	{

		CR_SetActiveCDROM(i);
		CR_GetCDROMParameters(&cdParams);
		m_cDefaultCD.AddString(cdParams.lpszCDROMID);
	}
	
	m_cDefaultCD.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSetupWizard 

void CSetupWizard::OnChangePlayer() 
{
	
}

void CSetupWizard::OnOK() 
{
	
	CR_SetActiveCDROM(m_cDefaultCD.GetCurSel());
	CR_SaveSettings();
	CR_DeInit();

	UpdateData(TRUE);
	cfgFile cfg(wd);
	cfg.SetStringValue("pluginpath", m_Path2DefaultPlayer);

	CDialog::OnOK();
}

