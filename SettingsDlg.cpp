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
#include "SettingsDlg.h"
#include "Ini.h"

#include "SettingsLookNFeel.h"
#include "SettingsLameFE.h"
#include "WinampPlugin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString		g_strIniFile;
/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSettingsDlg 


CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsDlg)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
	bShowMP3		= FALSE;
	bShowDecoders	= FALSE;
	bShowCDRipper	= FALSE;
	m_strDecoder	= "";
	m_strEncoder	= "";
	m_strAlbumInfo	= "";
	m_strBatchMode	= "";
	m_strFilenames	= "";
	m_strLookNFeel  = "";
}


void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsDlg)
	DDX_Control(pDX, IDC_MODULES, m_ctrlModules);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingsDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MODULES, OnSelchangedModules)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CSettingsDlg 


BOOL CSettingsDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	TCHAR	szBuffer[_MAX_PATH]; 
	VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH));
	
	m_strWd = szBuffer;
	m_strWd = m_strWd.Left(m_strWd.ReverseFind('\\'));

	m_strDecoder.LoadString(IDS_DECODER);
	m_strEncoder.LoadString(IDS_ENCODER);
	m_strAlbumInfo.LoadString(IDS_ALBUMINFO);
	m_strBatchMode.LoadString(IDS_BATCHMODE);
	m_strFilenames.LoadString(IDS_FILENAMES);
	m_strLookNFeel.LoadString(IDS_LOOKNFEEL);

	HIMAGELIST hList = ImageList_Create(16,16, ILC_COLOR8 |ILC_MASK , 4, 1);
	m_cImageList.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_SETTINGS_IMAGES);
	m_cImageList.Add(&cBmp, RGB(255,0, 255));
	cBmp.DeleteObject();


	m_ctrlModules.SetImageList(&m_cImageList, TVSIL_NORMAL);

	HTREEITEM hLameFE  = m_ctrlModules.InsertItem("LameFE", 0, 1, TVI_ROOT, TVI_LAST);
	HTREEITEM hDecoder = m_ctrlModules.InsertItem(m_strDecoder, 0, 1, TVI_ROOT, TVI_LAST);
	HTREEITEM hEncoder = m_ctrlModules.InsertItem(m_strEncoder, 0, 1, TVI_ROOT, TVI_LAST);
	HTREEITEM hAlbum   = m_ctrlModules.InsertItem(m_strAlbumInfo, 0, 1, TVI_ROOT, TVI_LAST);

	if(!hLameFE || !hDecoder || !hEncoder || !hAlbum){

		TRACE("Error, item handle is zero\n");
		return TRUE;
	}
	
	m_ctrlModules.InsertItem(m_strLookNFeel, 0, 1, hLameFE);
	m_ctrlModules.InsertItem(m_strFilenames, 0, 1, hLameFE);

	HTREEITEM hCDRip = m_ctrlModules.InsertItem("CD Digital Audio", 0, 1, hDecoder);
	m_ctrlModules.InsertItem(m_strBatchMode, 0, 1, hCDRip);
	
	m_ctrlModules.InsertItem("MP3 (L.A.M.E.)", 0, 1, hEncoder);
	m_ctrlModules.InsertItem("FreeDB", 0, 1, hAlbum);

	CheckForDecoders(hDecoder);
	CheckForEncoders(hEncoder);

	m_ctrlModules.Expand(hLameFE,TVE_EXPAND);
	m_ctrlModules.Expand(hDecoder,TVE_EXPAND);
	m_ctrlModules.Expand(hCDRip,TVE_EXPAND);
	m_ctrlModules.Expand(hEncoder,TVE_EXPAND);
	m_ctrlModules.Expand(hAlbum,TVE_EXPAND);

	InitSubDlgs();
	
	if(bShowDecoders){

		m_ctrlModules.Select(hDecoder, TVGN_CARET);
	}
	else if(bShowMP3){

		m_ctrlModules.Select(m_ctrlModules.GetNextItem(hEncoder, TVGN_NEXTVISIBLE), TVGN_CARET);
	}
	else if(bShowCDRipper){

		m_ctrlModules.Select(hCDRip, TVGN_CARET);
	}
	else{

		m_ctrlModules.Select(hLameFE, TVGN_CARET);
	}
	return TRUE;
}

BOOL CSettingsDlg::InitSubDlgs()
{

	CRect rcSheet;
	GetDlgItem(IDC_PLACEHOLDER_STATIC)->GetWindowRect(&rcSheet);
	ScreenToClient(&rcSheet);
	
	m_dBatchmode.Create(IDD_SETTINGS_BATCHMODE, this);
	m_dCDRipper.Create(IDD_SETTINGS_CDRIPPER, this);
	m_dDecoder.Create(IDD_SETTINGS_DECODER, this);
	m_dFilenames.Create(IDD_SETTINGS_FILENAMES, this);
	m_dFreeDB.Create(IDD_SETTINGS_FREEDB, this);
	m_dLameFE.Create(IDD_SETTINGS_LAMEFE, this);
	m_dLookNFeel.Create(IDD_SETTINGS_LOOKNFEEL, this);
	m_dMP3.Create(IDD_SETTINGS_MP3, this);
	m_dPlugin.Create(IDD_SETTINGS_PLUGINCODEC, this);

	m_dBatchmode.SetWindowPos(NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_dCDRipper.SetWindowPos(NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_dDecoder.SetWindowPos(NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_dFilenames.SetWindowPos(NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_dFreeDB.SetWindowPos(NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_dLameFE.SetWindowPos(NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_dLookNFeel.SetWindowPos(NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_dMP3.SetWindowPos(NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	m_dPlugin.SetWindowPos(NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

	m_dBatchmode.Init(m_strWd);
	m_dCDRipper.Init(m_strWd);
	m_dDecoder.Init(m_strWd);
	m_dFilenames.Init(m_strWd);
	m_dFreeDB.Init(m_strWd);
	m_dLameFE.Init(m_strWd);
	m_dLookNFeel.Init(m_strWd);
	m_dMP3.Init(m_strWd);
	m_dPlugin.Init(m_strWd);


	//m_strActiveDlg = "LameFE";

	return TRUE;
}

void CSettingsDlg::OnSelchangedModules(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CString strActivate = m_ctrlModules.GetItemText(m_ctrlModules.GetSelectedItem());
	
	m_dBatchmode.ShowWindow(SW_HIDE);
	m_dCDRipper.ShowWindow(SW_HIDE);
	m_dDecoder.ShowWindow(SW_HIDE);
	m_dFilenames.ShowWindow(SW_HIDE);
	m_dFreeDB.ShowWindow(SW_HIDE);
	m_dLameFE.ShowWindow(SW_HIDE);
	m_dLookNFeel.ShowWindow(SW_HIDE);
	m_dMP3.ShowWindow(SW_HIDE);
	m_dPlugin.ShowWindow(SW_HIDE);

	if(strActivate == "LameFE"){

		m_dLameFE.ShowWindow(SW_SHOW);
	}
	else if(strActivate == m_strLookNFeel){

		m_dLookNFeel.ShowWindow(SW_SHOW);
	}
	else if(strActivate == m_strFilenames){

		m_dFilenames.ShowWindow(SW_SHOW);
	}
	else if(strActivate == m_strDecoder){

		m_dDecoder.ShowWindow(SW_SHOW);
	}
	else if(strActivate == "CD Digital Audio"){

		m_dCDRipper.ShowWindow(SW_SHOW);
	}
	else if(strActivate == m_strBatchMode){

		m_dBatchmode.ShowWindow(SW_SHOW);
	}
	else if(strActivate == "MP3 (L.A.M.E.)"){

		m_dMP3.ShowWindow(SW_SHOW);
	}
	else if(strActivate == "FreeDB"){

		m_dFreeDB.ShowWindow(SW_SHOW);
	}
	else{

		HTREEITEM hTmp = m_ctrlModules.GetParentItem(m_ctrlModules.GetSelectedItem());
		if(m_ctrlModules.GetItemText(hTmp) == m_strDecoder){

			if(strActivate.Right(8) != "(Winamp)"){


				m_dPlugin.LoadPlugin(strActivate + "_in.dll");
			}
			else{

				CString strTmp = strActivate.Left(strActivate.GetLength() - 9);
				m_dPlugin.LoadPlugin("in_" + strTmp + ".dll");
			}
			m_dPlugin.ShowWindow(SW_SHOW);
		}
		if(m_ctrlModules.GetItemText(hTmp) == m_strEncoder){

			m_dPlugin.LoadPlugin(strActivate + "_out.dll", FALSE);
			m_dPlugin.ShowWindow(SW_SHOW);
		}
		else{
			
			strActivate.LoadString(IDS_NONESELECTED);
		}
	}

	m_strActiveDlg = strActivate;
	SetWindowText("Settings :: " + m_strActiveDlg);
	*pResult = 0;
}

void CSettingsDlg::OnOK() 
{

	
	if(m_dBatchmode.Save() && m_dCDRipper.Save() &&	m_dFilenames.Save()
		&& 	m_dFreeDB.Save() &&	m_dLameFE.Save() &&	m_dLookNFeel.Save()
		&&	m_dMP3.Save() && m_dDecoder.Save())
	{

		CDialog::OnOK();
	}
}

void CSettingsDlg::OnApply() 
{

	m_dBatchmode.Save();
	m_dCDRipper.Save();
	m_dFilenames.Save();
	m_dFreeDB.Save();
	m_dLameFE.Save();
	m_dLookNFeel.Save();
	m_dMP3.Save();
	m_dDecoder.Save();
}


void CSettingsDlg::CheckForDecoders(HTREEITEM hDecoder)
{

	CFileFind finder;
	CString   strPlugin;
	
	CIni cfg;
	cfg.SetIniFileName(g_strIniFile);
	m_strPluginPath = cfg.GetValue("LameFE", "WinampPluginPath", "");

	BOOL bResult = finder.FindFile(m_strWd + "\\Plugins\\*_in.dll");

	while(bResult){

		bResult = finder.FindNextFile();
		
		strPlugin = finder.GetFileName();
		strPlugin = strPlugin.Left(strPlugin.GetLength() - 7);
		m_ctrlModules.InsertItem(strPlugin, 0, 1, hDecoder);
		TRACE("Added plugin %s.\n", finder.GetFileName());

	}

	bResult = finder.FindFile(m_strPluginPath + "\\in_*.dll");

	while(bResult){

		bResult = finder.FindNextFile();
		CWinampPlugin tmp(finder.GetFilePath());
		if(tmp.Load(0)){

			if(tmp.UsesOutput()){

				strPlugin = finder.GetFileName();
				strPlugin = strPlugin.Left(strPlugin.GetLength() - 4);
				strPlugin = strPlugin.Right(strPlugin.GetLength() -3);
				strPlugin += " (Winamp)";
				m_ctrlModules.InsertItem(strPlugin, 0, 1, hDecoder);
				TRACE("Added plugin %s.\n", finder.GetFileName());
			}
			tmp.Unload();
		}
	}
}

void CSettingsDlg::CheckForEncoders(HTREEITEM hEncoder)
{

	CFileFind finder;
	CString   strPlugin;

	BOOL bResult = finder.FindFile(m_strWd + "\\Plugins\\*_out.dll");

	while(bResult){

		bResult = finder.FindNextFile();
		
		strPlugin = finder.GetFileName();
		strPlugin = strPlugin.Left(strPlugin.GetLength() - 8);
		m_ctrlModules.InsertItem(strPlugin, 0, 1, hEncoder);
		TRACE("Added plugin %s.\n", finder.GetFileName());

	}
}

void CSettingsDlg::ShowMP3()
{

	bShowMP3 = TRUE;
}

void CSettingsDlg::ShowDecoders()
{
	bShowDecoders = TRUE;
}

void CSettingsDlg::ShowCDRipper()
{

	bShowCDRipper = TRUE;
}

