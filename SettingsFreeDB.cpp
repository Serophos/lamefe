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
#include "SettingsFreeDB.h"

#include "lameFE.h"
#include "Settings.h"
#include "Utils.h"
#include "mfccddb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings g_sSettings;

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsFreeDB 


CSettingsFreeDB::CSettingsFreeDB(CWnd* pParent /*=NULL*/)
	: CMySettingsPage(CSettingsFreeDB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsFreeDB)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
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


void CSettingsFreeDB::DoDataExchange(CDataExchange* pDX)
{
	CMySettingsPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsFreeDB)
	DDX_Control(pDX, IDC_REMOTE_PORT, c_port);
	DDX_Control(pDX, IDC_ADDRESS, c_address);
	DDX_Control(pDX, IDC_PATH, c_path);
	DDX_Control(pDX, IDC_LOCATION, c_location);
	DDX_Control(pDX, IDC_EMAIL, m_cEMail);
	DDX_Control(pDX, IDC_TIMEOUT, m_cTimeOut);
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


BEGIN_MESSAGE_MAP(CSettingsFreeDB, CMySettingsPage)
	//{{AFX_MSG_MAP(CSettingsFreeDB)
		// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	ON_BN_CLICKED(IDC_USE_PROXY, OnUseProxy)
	ON_CBN_SELENDOK(IDC_REMOTE_SERVER, OnSelendokRemoteServer)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_AUTHENTICATION, OnAuthentication)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsFreeDB 

void CSettingsFreeDB::Init(CString strWd)
{

	CMySettingsPage::Init(strWd);
	ReadServerList();

	c_remoteServer.SetCurSel(g_sSettings.GetFreeDBServer());
	c_useProxy.SetCheck(g_sSettings.GetUseProxy());

	m_eMail			 = g_sSettings.GetEmail();
	m_timeOut		 = g_sSettings.GetTimeOut();
	m_userName		 = g_sSettings.GetUsername();
	m_proxyPort		 = g_sSettings.GetProxyPort();
	m_proxyAddress	 = g_sSettings.GetProxyAdress();
	m_passwd		 = Utils::DecryptString(g_sSettings.GetPassowrd());
	m_Authentication = g_sSettings.GetProxyAuthentication();

	UpdateData(FALSE);
	OnSelendokRemoteServer();
	OnUseProxy();

	if(m_pToolTip != NULL){

		m_pToolTip->AddTool(&c_authentication, IDS_TOOL_FDBAUTH);
		m_pToolTip->AddTool(&c_password, IDS_TOOL_FDBPASS);
		m_pToolTip->AddTool(&c_proxyAddress, IDS_TOOL_FDBPROXYADD);
		m_pToolTip->AddTool(&c_proxyPort, IDS_TOOL_FDBPROXYPORT);
		m_pToolTip->AddTool(&c_remoteServer, IDS_TOOL_FDBSERVER);
		m_pToolTip->AddTool(&c_useProxy, IDS_TOOL_FDBUSEPROXY);
		m_pToolTip->AddTool(&c_userName, IDS_TOOL_FDBUSER);
		m_pToolTip->AddTool(&m_cEMail, IDS_TOOL_FDBMAIL);
		m_pToolTip->AddTool(&m_cTimeOut, IDS_TOOL_FDBTIMEOUT);
		m_pToolTip->Activate(TRUE);
	}
}

BOOL CSettingsFreeDB::Save()
{

	if(UpdateData(TRUE) == NULL){

		return FALSE;
	}

	//cfg.SetIniFileName(
	g_sSettings.SetFreeDBServer(c_remoteServer.GetCurSel());
	g_sSettings.SetUseProxy(c_useProxy.GetCheck());
	g_sSettings.SetEmail(m_eMail);
	g_sSettings.SetTimeOut(m_timeOut);
	g_sSettings.SetUsername(m_userName);
	g_sSettings.SetProxyPort(m_proxyPort);
	g_sSettings.SetProxyAdress(m_proxyAddress);
	g_sSettings.SetPassowrd(Utils::EncryptString(m_passwd));
	g_sSettings.SetProxyAuthentication(m_Authentication);
	g_sSettings.Save();

	return TRUE;
}


void CSettingsFreeDB::OnUseProxy() 
{

	c_proxyAddress.EnableWindow(c_useProxy.GetCheck());
	c_proxyPort.EnableWindow(c_useProxy.GetCheck());
	c_authentication.EnableWindow(c_useProxy.GetCheck());
	OnAuthentication();
}

void CSettingsFreeDB::OnAuthentication() 
{

	c_password.EnableWindow(c_useProxy.GetCheck() && c_authentication.GetCheck());
	c_userName.EnableWindow(c_useProxy.GetCheck() && c_authentication.GetCheck());
}

void CSettingsFreeDB::OnSelendokRemoteServer() 
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
	m_remotePort = atoi(sBuf);
	//sBuf.ReleaseBuffer();
	iPosB = iPosE + 2;

	//Path
	iPosE = serverString.Find(',', iPosB);
	m_path = serverString.Mid(iPosB, iPosE - iPosB);
	iPosB = iPosE + 2;

	//Location
	m_location = serverString.Right(iLen - iPosB);
	UpdateData(FALSE);
	
	

}


void CSettingsFreeDB::OnUpdate() 
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
	for (int i = 0; i < sites.GetSize(); i++)
	{
		CCDDBSite s = sites.ElementAt(i);
		CString sLat;
		sLat.Format(_T("%c%03d.%02d"), s.m_bNorthing ? _T('N') : _T('S'), s.m_nLatitudeMinutes / 60, s.m_nLatitudeMinutes % 60);
		CString sLong;
		sLong.Format(_T("%c%03d.%02d"), s.m_bEasting ? _T('E') : _T('W'), s.m_nLongitudeMinutes / 60, s.m_nLongitudeMinutes % 60);
		CString sBuf;
		sBuf.Format(_T("%s, %d, %s, %s, %s, %s"), s.m_sSite, s.m_nPort, s.m_sAddress, sLat, sLong, s.m_sDescription);
		c_remoteServer.AddString(sBuf);
	}
	if(sites.GetSize()){

		c_remoteServer.SetCurSel(0);
		OnSelendokRemoteServer();
		WriteServerList();
		AfxMessageBox(IDS_CDDB_UPDATEDSERVERS, MB_OK+MB_ICONINFORMATION);
	}
	else{

		AfxMessageBox(IDS_CDDB_UPDATEFAILED, MB_OK+MB_ICONINFORMATION);
	}
}

BOOL CSettingsFreeDB::WriteServerList()
{

	CStdioFile server;
	CFileException e;
	CString serverString;
	TRY{

		server.Open(m_strWd + "\\cddb.cfg", CFile::modeCreate | CFile::modeWrite | CFile::typeText, &e);

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



BOOL CSettingsFreeDB::ReadServerList()
{
	

	CStdioFile server;
	CFileException e;
	CString serverString;
	TRY{

		server.Open(m_strWd + "\\cddb.cfg", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead | CFile::typeText, &e);

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

