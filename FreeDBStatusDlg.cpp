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
#include "FreeDBStatusDlg.h"
#include "I18n.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CI18n	 g_iLang;

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld FreeDBStatusDlg 


FreeDBStatusDlg::FreeDBStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(FreeDBStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(FreeDBStatusDlg)
	m_strStatusMsg = _T("Stand by...");
	//}}AFX_DATA_INIT
	Create(FreeDBStatusDlg::IDD, pParent);
}


void FreeDBStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FreeDBStatusDlg)
	DDX_Text(pDX, IDC_STATUS_MSG, m_strStatusMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FreeDBStatusDlg, CDialog)
	//{{AFX_MSG_MAP(FreeDBStatusDlg)
	ON_WM_TIMER()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten FreeDBStatusDlg 

void FreeDBStatusDlg::SetMessage(CString strMsg)
{

	m_mLock.Lock();
	m_strStatusMsg = strMsg;
	UpdateData(FALSE);
	m_mLock.Unlock();
	Sleep(0);
}

void FreeDBStatusDlg::SetMessage(UINT nID)
{

	m_mLock.Lock();
	m_strStatusMsg = g_iLang.GetString(nID);
	UpdateData(FALSE);
	m_mLock.Unlock();
	Sleep(0);
}

BOOL FreeDBStatusDlg::Create(UINT nIDTemplate, CWnd* pParentWnd) 
{

	int nResult = CDialog::Create(nIDTemplate, pParentWnd);

	//RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_ERASENOW|RDW_INVALIDATE|RDW_INTERNALPAINT|RDW_UPDATENOW);
	g_iLang.TranslateDialog(this, IDD_FREEDB_STATUS);
	SetTimer(32130, 500, 0);
	
	return nResult;
}

void FreeDBStatusDlg::OnTimer(UINT nIDEvent) 
{
	m_mLock.Lock();
	UpdateData(FALSE);
	m_mLock.Unlock();
	CDialog::OnTimer(nIDEvent);
}

void FreeDBStatusDlg::OnCancelMode() 
{
	KillTimer(32130);
	CDialog::OnCancelMode();
}
