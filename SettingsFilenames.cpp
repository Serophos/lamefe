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
#include "SettingsFilenames.h"
#include "Settings.h"
#include "PathDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsFilenames 

extern CSettings g_sSettings;

CSettingsFilenames::CSettingsFilenames(CWnd* pParent /*=NULL*/)
	: CMySettingsPage(CSettingsFilenames::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsFilenames)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	m_strAlbumMode = _T("");
	m_strAlbumPrev = _T("");
	m_strFilenamePrev = _T("");
	m_strFilename = _T("");
	m_strPlaylist = _T("");
	m_strPlaylistPrev = _T("");
	m_bRename = FALSE;
	//}}AFX_DATA_INIT
}


void CSettingsFilenames::DoDataExchange(CDataExchange* pDX)
{
	CMySettingsPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsFilenames)
		// HINWEIS: Der Klassen-Assistent fügt hier DDX- und DDV-Aufrufe ein
	DDX_Control(pDX, IDC_ALBUM_MODE_FILENAME, m_ctrlAlbum);
	DDX_Control(pDX, IDC_PLAYLIST_FILENAME, m_ctrlPlaylist);
	DDX_Control(pDX, IDC_FORMAT, m_ctrlFilename);
	DDX_Text(pDX, IDC_ALBUM_MODE_FILENAME, m_strAlbumMode);
	DDX_Text(pDX, IDC_ALBUM_MODE_VIS, m_strAlbumPrev);
	DDX_Text(pDX, IDC_FILENAME_VIS, m_strFilenamePrev);
	DDX_Text(pDX, IDC_FORMAT, m_strFilename);
	DDX_Text(pDX, IDC_PLAYLIST_FILENAME, m_strPlaylist);
	DDX_Text(pDX, IDC_PLAYLIST_VIS, m_strPlaylistPrev);
	DDX_Check(pDX, IDC_RENAME, m_bRename);
	DDX_Text(pDX, IDC_OUTPUTPATH, m_strOutputPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsFilenames, CMySettingsPage)
	//{{AFX_MSG_MAP(CSettingsFilenames)
		// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	ON_BN_CLICKED(IDC_RENAME, OnRename)
	ON_BN_CLICKED(IDC_PATH, OnPath)
	ON_EN_CHANGE(IDC_ALBUM_MODE_FILENAME, OnChangeAlbumModeFilename)
	ON_EN_CHANGE(IDC_FORMAT, OnChangeFormat)
	ON_EN_CHANGE(IDC_PLAYLIST_FILENAME, OnChangePlaylistFilename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsFilenames 

void CSettingsFilenames::OnRename() 
{

	UpdateData(TRUE);
	m_ctrlFilename.EnableWindow(m_bRename);

}

void CSettingsFilenames::OnChangeAlbumModeFilename() 
{
	
	UpdateData(TRUE);
	CreatePreview(m_strAlbumMode, m_strAlbumPrev);
	UpdateData(FALSE);

	m_mToolTip.ShowToolTip(IDC_ALBUM_MODE_FILENAME);
}

void CSettingsFilenames::OnChangeFormat() 
{

	UpdateData(TRUE);
	CreatePreview(m_strFilename, m_strFilenamePrev);
	UpdateData(FALSE);

	m_mToolTip.ShowToolTip(IDC_FORMAT);
}

void CSettingsFilenames::OnChangePlaylistFilename() 
{

	UpdateData(TRUE);
	CreatePreview(m_strPlaylist, m_strPlaylistPrev);
	UpdateData(FALSE);

	m_mToolTip.ShowToolTip(IDC_PLAYLIST_FILENAME);
}

void CSettingsFilenames::CreatePreview(CString &in, CString &out)
{

	if(in.Replace('/', '-')	|| in.Replace('*', '-') ||
		in.Replace('?', '-') || in.Replace('\"', '\'') ||	in.Replace('<', '(') ||
		in.Replace('>', ')') || in.Replace('|', '-') || in.Replace(':', '-')){

		AfxMessageBox(IDS_ID3SETT_INV_FSTR, MB_OK+MB_ICONINFORMATION);
	}

	if(in.GetAt(0) == '\\'){

		in = in.Right(in.GetLength() - 1);
		AfxMessageBox(IDS_BKSLHWARN, MB_OK+MB_ICONINFORMATION);
	}

	for(int i = 0; i < in.GetLength(); i++){

		if((in.GetAt(i) == '\\') && ((i+1) < in.GetLength())){
			
			if((in.GetAt(i) == '\\') && (in.GetAt(i + 1) == '\\')){

				CString tmp = in.Left(i);
				tmp += in.Right(in.GetLength() - i - 1);
				in = tmp;
				AfxMessageBox(IDS_DBLBACKSLASH, MB_OK+MB_ICONINFORMATION);
			}
		}
	}


	out = in;
	out.Replace("%1", "<Artist>");
	out.Replace("%2", "<Song>");
	out.Replace("%3", "<Album>");
	out.Replace("%5", "<Genre>");

	out.Replace("%U1", "<ARTIST>");
	out.Replace("%U2", "<SONG>");
	out.Replace("%U3", "<ALBUM>");
	out.Replace("%U5", "<GENRE>");

	out.Replace("%L1", "<artist>");
	out.Replace("%L2", "<song>");
	out.Replace("%L3", "<album>");
	out.Replace("%L5", "<genre>");

	out.Replace("%4", "<Year>");
	out.Replace("%U4", "<Year>");
	out.Replace("%L4", "<Year>");

	out.Replace("%a", "<Track #>");
	out.Replace("%Ua", "<Track #>");
	out.Replace("%La", "<Track #>");

	out.Replace("%b", "<Total #>");
	out.Replace("%Ub", "<Total #>");
	out.Replace("%Lb", "<Total #>");
	
	out.Replace("%c", "<Disc ID>");
	out.Replace("%Uc", "<DISC ID>");
	out.Replace("%Lc", "<disc id>");
	
	out.Replace("%d", "<Volume ID>");
	out.Replace("%Ud", "<VOLUME ID>");
	out.Replace("%Ld", "<volume ID>");
}

void CSettingsFilenames::OnPath()
{

	UpdateData(TRUE);

	CPathDialog dlg("Select output folder", "Select output directory. Non existent directories will be created.", m_strOutputPath);
	int nResult = dlg.DoModal();
	if(nResult == IDOK){

		m_strOutputPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CSettingsFilenames::Init(CString strWd)
{
	
	CMySettingsPage::Init(strWd);


	m_strFilename	= g_sSettings.GetFilename();
	m_strAlbumMode	= g_sSettings.GetAlbumFilename();
	m_strPlaylist	= g_sSettings.GetPlaylistFilename();
	m_strOutputPath = g_sSettings.GetBasePath();
	m_bRename		= g_sSettings.GetRenameFiles();
	
	CreatePreview(m_strAlbumMode, m_strAlbumPrev);
	CreatePreview(m_strFilename, m_strFilenamePrev);
	CreatePreview(m_strPlaylist, m_strPlaylistPrev);

	//Set up the identifer helper :-)
	m_mToolTip.Create(this);
	m_mToolTip.AddControlInfo(IDC_FORMAT, IDS_TOOL_FILEFORMAT);
	m_mToolTip.AddControlInfo(IDC_ALBUM_MODE_FILENAME, IDS_TOOL_FILEFORMAT);
	m_mToolTip.AddControlInfo(IDC_PLAYLIST_FILENAME, IDS_TOOL_FILEFORMAT);

	if(m_pToolTip != NULL){

		m_pToolTip->AddTool(&m_ctrlFilename, IDS_TOOL_IDENTCOMB);
		m_pToolTip->AddTool(&m_ctrlPlaylist, IDS_TOOL_IDENTCOMB);
		m_pToolTip->AddTool(&m_ctrlAlbum, IDS_TOOL_IDENTCOMB);
	}

	UpdateData(FALSE);

}

BOOL CSettingsFilenames::Save()
{

	UpdateData(TRUE);


	if((!m_strFilename.GetLength() && m_bRename)){

		AfxMessageBox("Filename Format String is empty!", MB_ICONSTOP+MB_OK);
		return FALSE;
	}

	if(!m_strAlbumMode.GetLength()){

		AfxMessageBox("Filename For Album Mode Format String is empty!", MB_ICONSTOP+MB_OK);
		return FALSE;
	}

	if(!m_strAlbumMode.GetLength() || !m_strPlaylist.GetLength()){

		AfxMessageBox("Playlist Filename Format String is empty!", MB_ICONSTOP+MB_OK);
		return FALSE;
	}

	
	CreatePreview(m_strPlaylist, m_strPlaylistPrev);
	CreatePreview(m_strFilename, m_strFilenamePrev);
	CreatePreview(m_strAlbumMode, m_strAlbumPrev);

	g_sSettings.SetBasePath(m_strOutputPath);
	g_sSettings.SetFilename(m_strFilename);
	g_sSettings.SetAlbumFilename(m_strAlbumMode);
	g_sSettings.SetPlaylistFilename(m_strPlaylist);
	g_sSettings.SetRenameFiles(m_bRename);
	g_sSettings.Save();

	return TRUE;
}

BOOL CSettingsFilenames::PreTranslateMessage( MSG *pMsg )
{

	if( pMsg->message == WM_MOUSEMOVE )
	{
		POINT pt = pMsg->pt;    
		ScreenToClient( &pt );

		// this is the only function to call
		m_mToolTip.ShowToolTip( (CPoint)pt );
	}

	return CMySettingsPage::PreTranslateMessage(pMsg);
}
