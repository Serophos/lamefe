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
#include "CheckNewVersion.h"
#include <afxinet.h>
#include "I18n.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CCheckNewVersion 

extern CI18n	 g_iLang;

CCheckNewVersion::CCheckNewVersion(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckNewVersion::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckNewVersion)
	m_strVersionMsg = _T("");
	m_strNewVersion = _T("");
	m_strOldVersion = _T("");
	m_strChangeLog = _T("");
	//}}AFX_DATA_INIT
}


void CCheckNewVersion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckNewVersion)
	DDX_Control(pDX, IDC_DOWNLOAD, m_cDownload);
	DDX_Text(pDX, IDC_MESSAGE_VERSION, m_strVersionMsg);
	DDX_Text(pDX, IDC_NEWESTVERSION, m_strNewVersion);
	DDX_Text(pDX, IDC_YOUR_VERSION, m_strOldVersion);
	DDX_Text(pDX, IDC_CHANGELOG, m_strChangeLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckNewVersion, CDialog)
	//{{AFX_MSG_MAP(CCheckNewVersion)
	ON_BN_CLICKED(IDC_DOWNLOAD, OnDownload)
	ON_BN_CLICKED(ID_CHECKNOW, OnOK)
	ON_BN_CLICKED(ID_CLOSE, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen f�r Nachrichten CCheckNewVersion 

void CCheckNewVersion::OnOK() 
{

	CWaitCursor			wc;
	CInternetSession	is;
	CString				strTmp;
	
	m_strChangeLog.Empty();

	try{

		CHttpFile* pFile = (CHttpFile*) is.OpenURL("http://lamefe.sourceforge.net/version.info");
		//some ISPs interfear with a proxy to display adds when the first page is loaded
		//so we close and opem the file again
		pFile->Close();
		pFile = NULL;
		pFile = (CHttpFile*) is.OpenURL("http://lamefe.sourceforge.net/version.info");
		
		if(pFile != NULL){


			pFile->ReadString(m_strNewVersion);
			
			while(pFile->ReadString(strTmp)){

				m_strChangeLog += strTmp + "\r\n";
			}
			pFile->Close();
			delete pFile;

			m_strNewVersion.TrimLeft();
			m_strNewVersion.TrimRight();
		}

		is.Close();
		wc.Restore();
	}
	catch(CInternetException* pEx){
		
		TCHAR   szCause[255];
		CString strFormatted;

		pEx->GetErrorMessage(szCause, 255);
		strFormatted.Format(g_iLang.GetString(IDS_UPDATE_ERROR), szCause);
		AfxMessageBox(strFormatted, MB_OK+MB_ICONEXCLAMATION);
	}

	if(m_strNewVersion != STR_VERSION_DLG){
		
		m_strVersionMsg = g_iLang.GetString(IDS_UPDATE_AVAILABLE);
		m_cDownload.EnableWindow(TRUE);
	}
	else{

		m_strVersionMsg = g_iLang.GetString(IDS_UPDATE_NO);
	}
	UpdateData(FALSE);
}

void CCheckNewVersion::OnDownload() 
{



	//http://belnet.dl.sourceforge.net/sourceforge/lamefe/LameFE2.2b6.exe
	CString strFilename = "http://lamefe.sourceforge.net/index.php?showpage=download";
	//strFilename += m_strInstallerName;

	ShellExecute(GetSafeHwnd(), 
				"open", 
				strFilename,
				NULL,
				NULL,
				SW_SHOW);

	OnCancel();
}

BOOL CCheckNewVersion::OnInitDialog()
{

	CDialog::OnInitDialog();

	g_iLang.TranslateDialog(this, IDD_CHECKNEWVERSION);

	m_strOldVersion = STR_VERSION_DLG;
	m_strNewVersion = g_iLang.GetString(IDS_UPDATE_NOTQUERIED);

	UpdateData(FALSE);

	return TRUE;
}
