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
#include "EncoderSheet.h"
#include "cfgFile.h"
#include "BladeMP3EncDLL.h"
#include "tagWriter/tagWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEVERSION beVersion=NULL;

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite CEncoderSheet 

IMPLEMENT_DYNCREATE(CEncoderSheet, CPropertyPage)

CEncoderSheet::CEncoderSheet() : CPropertyPage(CEncoderSheet::IDD)
{
	//{{AFX_DATA_INIT(CEncoderSheet)
	m_encoderVersion = _T("");
	m_FormatString = _T("");
	//}}AFX_DATA_INIT


}

CEncoderSheet::~CEncoderSheet()
{
}

void CEncoderSheet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEncoderSheet)
	DDX_Control(pDX, IDC_FORMAT, c_Format);
	DDX_Control(pDX, IDC_MPEG, c_MPEG);
	DDX_Control(pDX, IDC_RENAME, c_rename);
	DDX_Control(pDX, IDC_PRIVATE, c_private);
	DDX_Control(pDX, IDC_ORIGINAL, c_original);
	DDX_Control(pDX, IDC_ID3_2, c_writeId3v2);
	DDX_Control(pDX, IDC_COPYRIGHT, c_copyright);
	DDX_Control(pDX, IDC_CHECKSUM, c_checkSum);
	DDX_Control(pDX, IDC_CHANNELS, c_channels);
	DDX_Control(pDX, IDC_BITSPERSAMPLE, c_bitsPerSample);
	DDX_Text(pDX, IDC_ENCODER_VERSION, m_encoderVersion);
	DDX_Text(pDX, IDC_FORMAT, m_FormatString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEncoderSheet, CPropertyPage)
	//{{AFX_MSG_MAP(CEncoderSheet)
	ON_BN_CLICKED(IDC_RENAME, OnRename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CEncoderSheet 

void CEncoderSheet::init(CString wdir)
{
	
	wd = wdir;
	HINSTANCE	lameDLL			= NULL;
	BE_VERSION	Version			= {0,};
	
	lameDLL=LoadLibrary(wd + "\\lame_enc.dll");

	if(lameDLL==NULL)
	{
		m_encoderVersion = "Using LAME-Encoder.\nhttp://www.mp3devel.org";
	}
	else{	
		// Get Interface function beVersion
		beVersion		= (BEVERSION) GetProcAddress(lameDLL, TEXT_BEVERSION);
		if(!beVersion )
		{
			m_encoderVersion = "Using LAME-Encoder.\nhttp://www.mp3devel.org";
		}
		else{
			beVersion(&Version);
			m_encoderVersion.Format("lame_enc.dll version %u.%02u (%u/%u/%u)\n"
					"lame_enc Engine %u.%02u\n"
					"lame_enc homepage at %s\n\n",	
					Version.byDLLMajorVersion, Version.byDLLMinorVersion,
					Version.byDay, Version.byMonth, Version.wYear,
					Version.byMajorVersion, Version.byMinorVersion,
					Version.zHomepage);
		}
	}

	 	MP3_VERSION mp3Version;

	memset(&mp3Version, 0, sizeof(mp3Version));
}

BOOL CEncoderSheet::OnInitDialog()
{

	CPropertyPage::OnInitDialog();

	cfgFile cfg(wd);

	c_bitsPerSample.SetCurSel(cfg.GetValue(BITRATE, FALSE));
	c_channels.SetCurSel(cfg.GetValue(NUMCHNLS, FALSE));
	c_checkSum.SetCheck(cfg.GetValue(CRC, FALSE));
	c_copyright.SetCheck(cfg.GetValue(COPYRIGHT, FALSE));
	c_original.SetCheck(cfg.GetValue(ORIGINAL, FALSE));
	c_private.SetCheck(cfg.GetValue(PRIVATE, FALSE));
	c_writeId3v2.SetCheck(cfg.GetValue(ID3V2, FALSE));
	c_rename.SetCheck(cfg.GetValue(RENAME, FALSE));
	m_FormatString = cfg.GetStringValue(FORMATSTR);
	OnRename();
	UpdateData(FALSE);

	return true;
}

void CEncoderSheet::OnOK()
{

	saveSettings();
	CPropertyPage::OnOK();
}

void CEncoderSheet::OnCancel()
{

	
	CPropertyPage::OnCancel();
}

void CEncoderSheet::saveSettings()
{
	
	cfgFile cfg(wd);

	cfg.SetValue(BITRATE, (LPVOID)c_bitsPerSample.GetCurSel());
	cfg.SetValue(NUMCHNLS, (LPVOID)c_channels.GetCurSel());
	cfg.SetValue(CRC, (LPVOID)c_checkSum.GetCheck());
	cfg.SetValue(COPYRIGHT, (LPVOID)c_copyright.GetCheck());
	cfg.SetValue(ORIGINAL, (LPVOID)c_original.GetCheck());
	cfg.SetValue(PRIVATE, (LPVOID)c_private.GetCheck());
	cfg.SetValue(ID3V2, (LPVOID)c_writeId3v2.GetCheck());
	cfg.SetValue(RENAME, (LPVOID)c_rename.GetCheck());

	UpdateData(TRUE);

	cfg.SetValue(FORMATSTR, (LPVOID)m_FormatString.GetBuffer(10));
	m_FormatString.ReleaseBuffer();
}


void CEncoderSheet::OnRename() 
{

	c_Format.EnableWindow(c_rename.GetCheck());
}
