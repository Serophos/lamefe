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
#include "stdafx.h"
#include "PresetBar.h"
#include "resource.h"
#include "Ini.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

extern CString		g_strIniFile;

CPresetBar::CPresetBar()
{

}

CPresetBar::~CPresetBar()
{

}

BEGIN_MESSAGE_MAP(CPresetBar, CMyDialogBar)
	//{{AFX_MSG_MAP(CMyDialogBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_SELECTPRESET, OnSelchangePreset)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyDialogBar message handlers

void CPresetBar::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Add your specialized code here and/or call the base class
	DDX_Control(pDX, IDC_SELECTPRESET ,m_mcPreset) ;
	DDX_Text(pDX, IDC_SELECTPRESET ,m_strPreset) ;
	CMyDialogBar::DoDataExchange(pDX);
}

void CPresetBar::OnInitDialogBar()
{

	TCHAR	szBuffer[_MAX_PATH]; 
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
	
	m_strWd = szBuffer;
	m_strWd = m_strWd.Left(m_strWd.ReverseFind('\\'));

	CFileFind finder;
	CString   strPreset;
	
	CIni cfg;
	cfg.SetIniFileName(g_strIniFile);

	strPreset = cfg.GetValue("L.A.M.E.", "PresetName", "default");

	m_mcPreset.ResetContent();

	BOOL bResult = finder.FindFile(cfg.GetValue("LameFE", "PresetPath", m_strWd) + "\\*.ini");

	while(bResult){

		bResult = finder.FindNextFile();
		
		strPreset = finder.GetFileName();
		strPreset.MakeLower();
		if(strPreset == "lamefe.ini"){

			continue;
		}

		strPreset = finder.GetFileName();
		strPreset = strPreset.Left(strPreset.GetLength() - 4);
		m_mcPreset.AddString(strPreset);
	}


	cfg.SetIniFileName(g_strIniFile);

	strPreset = cfg.GetValue("L.A.M.E.", "PresetName", "default");

	int nIndex = m_mcPreset.FindStringExact(0, strPreset);

	if(nIndex == CB_ERR && (m_mcPreset.GetCount() != 0)){

		m_mcPreset.SetCurSel(0);
		m_mcPreset.GetLBText(0, m_strPreset);
	}
	else{

		m_mcPreset.SetCurSel(nIndex);
		m_mcPreset.GetLBText(nIndex, m_strPreset);
	}

	m_pStatus	 = (CStatusBar *)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	
	cfg.SetIniFileName(cfg.GetValue("LameFE", "PresetPath", m_strWd) + "\\" + m_strPreset + ".ini");
	m_pStatus->SetPaneText(1,cfg.GetValue("L.A.M.E.", "Description", ""));
}

void CPresetBar::OnSelchangePreset()
{

	UpdateData(TRUE);
	CIni cfg;
	cfg.SetIniFileName(g_strIniFile);
	cfg.SetValue("L.A.M.E.", "PresetName", m_strPreset);

	cfg.SetIniFileName(cfg.GetValue("LameFE", "PresetPath", m_strWd) + "\\" + m_strPreset + ".ini");
	m_pStatus->SetPaneText(1,cfg.GetValue("L.A.M.E.", "Description", ""));
}


void CPresetBar::Enable(BOOL bEnable)
{

	m_mcPreset.EnableWindow(bEnable);
}
