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
#include "MySettingsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CMySettingsPage 


CMySettingsPage::CMySettingsPage(UINT nIDTemplate, CWnd* pParentWnd /*= NULL */)
	: CDialog(CMySettingsPage::IDD, pParentWnd)
{
	//{{AFX_DATA_INIT(CMySettingsPage)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
	m_pToolTip = NULL;
}

CMySettingsPage::~CMySettingsPage()
{

	delete m_pToolTip;
}

void CMySettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySettingsPage)
		// HINWEIS: Der Klassen-Assistent fügt hier DDX- und DDV-Aufrufe ein
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMySettingsPage, CDialog)
	//{{AFX_MSG_MAP(CMySettingsPage)
		// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CMySettingsPage 

void CMySettingsPage::Init(CString strWd)
{

	m_strWd = strWd;

	//Set up the tooltip
	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
	   TRACE("Unable To create ToolTip\n");
	}
}

BOOL CMySettingsPage::Save()
{

	return TRUE;
}

void CMySettingsPage::OnOK()
{
	
}

void CMySettingsPage::OnCancel()
{

}

BOOL CMySettingsPage::PreTranslateMessage(MSG* pMsg) 
{

	if (NULL != m_pToolTip){
		
		m_pToolTip->RelayEvent(pMsg);
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
