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
#include "CDDBPage.h"
#include "cfgFile.h"
#include "mfccddb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite CCDDBPage 

IMPLEMENT_DYNCREATE(CCDDBPage, CPropertyPage)

CCDDBPage::CCDDBPage() : CPropertyPage(CCDDBPage::IDD)
{
	//{{AFX_DATA_INIT(CCDDBPage)
	m_address = _T("");
	m_eMail = _T("");
	m_location = _T("");
	m_passwd = _T("");
	m_path = _T("");
	m_proxyAddress = _T("");
	m_userName = _T("");
	m_Authentication = FALSE;
	m_timeOut = 0;
	m_remotePort = 0;
	m_proxyPort = 0;
	//}}AFX_DATA_INIT
}

CCDDBPage::~CCDDBPage()
{
}

void CCDDBPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCDDBPage)
	DDX_Control(pDX, IDC_AUTHENTICATION, c_authentication);
	DDX_Control(pDX, IDC_USERNAME, c_userName);
	DDX_Control(pDX, IDC_PASSWORD, c_password);
	DDX_Control(pDX, IDC_PROXY_PORT, c_proxyPort);
	DDX_Control(pDX, IDC_PROXY, c_proxyAddress);
	DDX_Control(pDX, IDC_USE_PROXY, c_useProxy);
	DDX_Control(pDX, IDC_REMOTE_SERVER, c_remoteServer);
	DDX_Text(pDX, IDC_ADDRESS, m_address);
	DDX_Text(pDX, IDC_EMAIL, m_eMail);
	DDX_Text(pDX, IDC_LOCATION, m_location);
	DDX_Text(pDX, IDC_PASSWORD, m_passwd);
	DDX_Text(pDX, IDC_PATH, m_path);
	DDX_Text(pDX, IDC_PROXY, m_proxyAddress);
	DDX_Text(pDX, IDC_USERNAME, m_userName);
	DDX_Check(pDX, IDC_AUTHENTICATION, m_Authentication);
	DDX_Text(pDX, IDC_TIMEOUT, m_timeOut);
	DDX_Text(pDX, IDC_REMOTE_PORT, m_remotePort);
	DDX_Text(pDX, IDC_PROXY_PORT, m_proxyPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCDDBPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCDDBPage)
	ON_BN_CLICKED(IDC_USE_PROXY, OnUseProxy)
	ON_CBN_SELENDOK(IDC_REMOTE_SERVER, OnSelendokRemoteServer)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_AUTHENTICATION, OnAuthentication)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CCDDBPage 

void CCDDBPage::init(CString wdir)
{
	wd = wdir;
}

BOOL CCDDBPage::OnInitDialog()
{
	
	CPropertyPage::OnInitDialog();

	ReadServerList();

	cfgFile cfg(wd);

	m_passwd = cfg.GetStringValue(PASSWORD);
	m_proxyAddress = cfg.GetStringValue(PROXYADDR);
	m_proxyPort = cfg.GetValue(PROXYPORT, FALSE);
	c_remoteServer.SetCurSel(cfg.GetValue(CDDBSERVER, FALSE));
	OnSelendokRemoteServer();
	c_useProxy.SetCheck(cfg.GetValue(PROXY, FALSE));
	m_userName = cfg.GetStringValue(USERNAME);
	m_eMail = cfg.GetStringValue(EMAIL);
	m_Authentication = cfg.GetValue(AUTH, FALSE);
	m_timeOut = cfg.GetValue(TIMEOUT, FALSE);
	UpdateData(FALSE);

	OnUseProxy();

	return true;
}

void CCDDBPage::OnUseProxy() 
{

	c_proxyAddress.EnableWindow(c_useProxy.GetCheck());
	c_proxyPort.EnableWindow(c_useProxy.GetCheck());
	c_authentication.EnableWindow(c_useProxy.GetCheck());
	OnAuthentication();
}

void CCDDBPage::OnAuthentication() 
{

	c_password.EnableWindow(c_useProxy.GetCheck() && c_authentication.GetCheck());
	c_userName.EnableWindow(c_useProxy.GetCheck() && c_authentication.GetCheck());
}

void CCDDBPage::OnSelendokRemoteServer() 
{
	//Format:
	//server, port, path, latiude, longitude, Location
	//Example
	//de.freedb.org, 80, /~cddb/cddb.cgi, N052.53, E013.31, Berlin, Germany	
	UpdateData(TRUE);
	CString serverString;
	c_remoteServer.GetLBText(c_remoteServer.GetCurSel(), serverString);
	
	CString sBuf;
	int iLen = serverString.GetLength();
	int iPosB = 0;

	//Address
	int iPosE = serverString.Find(',');
	m_address = serverString.Left(iPosE);
	iPosB = iPosE + 2;
	
	//Port
	iPosE = serverString.Find(',', iPosB);
	sBuf = serverString.Mid(iPosB, iPosE - iPosB);
	m_remotePort = atoi(sBuf.GetBuffer(10));
	sBuf.ReleaseBuffer();
	iPosB = iPosE + 2;

	//Path
	iPosE = serverString.Find(',', iPosB);
	m_path = serverString.Mid(iPosB, iPosE - iPosB);
	iPosB = iPosE + 2;

	//Location
	m_location = serverString.Right(iLen - iPosB);
	UpdateData(FALSE);
	
	

}

void CCDDBPage::OnOK()
{
	
	SaveSettings();
}

void CCDDBPage::SaveSettings()
{

	UpdateData(TRUE);

	cfgFile cfg(wd);

	cfg.SetValue(PROXYADDR, (LPVOID)m_proxyAddress.GetBuffer(10));
	m_address.ReleaseBuffer();
	cfg.SetValue(EMAIL, (LPVOID)m_eMail.GetBuffer(10));
	m_eMail.ReleaseBuffer();
	cfg.SetValue(PASSWORD,(LPVOID) m_passwd.GetBuffer(10));
	m_passwd.ReleaseBuffer();
	cfg.SetValue(PROXYPORT, (LPVOID)m_proxyPort);
	cfg.SetValue(CDDBSERVER, (LPVOID)c_remoteServer.GetCurSel());
	cfg.SetValue(PROXY, (LPVOID)c_useProxy.GetCheck());
	cfg.SetValue(USERNAME, (LPVOID)m_userName.GetBuffer(10));
	m_userName.ReleaseBuffer();
	cfg.SetValue(AUTH, (LPVOID)c_authentication.GetCheck());
	cfg.SetValue(TIMEOUT, (LPVOID)m_timeOut);
}

void CCDDBPage::OnUpdate() 
{

	if (!AfxSocketInit()){

		AfxMessageBox(IDS_CDDB_ERR_WINSOCK, MB_OK+MB_ICONSTOP);
		return;
	}

 	CArray<CCDDBSite, CCDDBSite&> sites;
	CCDDB cddb;

	BOOL bSuccess = cddb.Sites(sites);

	if (!bSuccess)
	{

		CString sBuf;
		sBuf.Format(IDS_CDDB_ERR, cddb.GetLastError(), cddb.GetLastCommandResponse());
		AfxMessageBox(sBuf, MB_OK+MB_ICONSTOP);
		return;
	}
	if(bSuccess){

		c_remoteServer.ResetContent();
	}
//	TRACE(_T("Retrieved the following CDDB sites from the CDDB server\n"));
	for (int i = 0; i < sites.GetSize(); i++)
	{
		CCDDBSite s = sites.ElementAt(i);
		CString sLat;
		sLat.Format(_T("%c%03d.%02d"), s.m_bNorthing ? _T('N') : _T('S'), s.m_nLatitudeMinutes / 60, s.m_nLatitudeMinutes % 60);
		CString sLong;
		sLong.Format(_T("%c%03d.%02d"), s.m_bEasting ? _T('E') : _T('W'), s.m_nLongitudeMinutes / 60, s.m_nLongitudeMinutes % 60);
		CString sBuf;
		sBuf.Format(_T("%s, %d, %s, %s, %s, %s"), s.m_sSite, s.m_nPort, s.m_sAddress, sLat, sLong, s.m_sDescription);
//		TRACE(sBuf + "\n");
		c_remoteServer.AddString(sBuf);
	}
	if(sites.GetSize()){

		c_remoteServer.SetCurSel(0);
		OnSelendokRemoteServer();
		WriteServerList();
	}
}

BOOL CCDDBPage::WriteServerList()
{

	CStdioFile server;
	CFileException e;
	CString serverString;
	TRY{

		server.Open(wd + "\\cddb.cfg", CFile::modeCreate | CFile::modeWrite | CFile::typeText, &e);

		for(int i = 0; i < c_remoteServer.GetCount(); i++){
			
			c_remoteServer.GetLBText(i, serverString);
			server.WriteString(serverString + "\n");
		}
		server.Flush();
		server.Close();
	}
	CATCH(CFileException, e){

		AfxMessageBox(IDS_CDDB_ERR_CFGWRITE, MB_OK+MB_ICONSTOP);
		return FALSE;
	}
	END_CATCH;

	return TRUE;
}



BOOL CCDDBPage::ReadServerList()
{
	

	CStdioFile server;
	CFileException e;
	CString serverString;
	TRY{

		server.Open(wd + "\\cddb.cfg", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead | CFile::typeText, &e);

		while(server.GetPosition() != server.GetLength()){

			server.ReadString(serverString);
			c_remoteServer.AddString(serverString);
		}
		server.Close();
	}
	CATCH(CFileException, e){

		AfxMessageBox(IDS_CDDB_ERR_CFGREAD, MB_OK+MB_ICONSTOP);
		return FALSE;
	}
	END_CATCH;

	c_remoteServer.SetCurSel(0);
	OnSelendokRemoteServer();
	return TRUE;
}

