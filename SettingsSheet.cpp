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
#include "SettingsSheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsSheet

IMPLEMENT_DYNAMIC(CSettingsSheet, CPropertySheet)

/*CSettingsSheet::CSettingsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}*/

CSettingsSheet::CSettingsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet("lameFE Stettings", pParentWnd, iSelectPage)
{

	m_psh.dwFlags |= PSH_NOAPPLYNOW;	// Construct pages
	
	AddPage(&m_MainPage);
	AddPage(&m_CDROMPage);
	AddPage(&m_PluginPage);
	AddPage(&m_EncoderPage);
	AddPage(&m_cddbPage);

	SetActivePage(iSelectPage);
	
}

CSettingsSheet::~CSettingsSheet()
{
}


BEGIN_MESSAGE_MAP(CSettingsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSettingsSheet)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsSheet 


void CSettingsSheet::init(CString wdir)
{

	wd = wdir;
	m_EncoderPage.init(wd);
	m_cddbPage.init(wd);
	m_CDROMPage.init(wd);
	m_MainPage.init(wd);

}

