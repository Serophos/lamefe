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
//#include "tagWriter/tagWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite CEncoderSheet 

IMPLEMENT_DYNCREATE(CEncoderSheet, CPropertyPage)

CEncoderSheet::CEncoderSheet() : CPropertyPage(CEncoderSheet::IDD)
{

	//{{AFX_DATA_INIT(CEncoderSheet)
	m_encoderVersion = _T("");
	m_abr = 0;
	//}}AFX_DATA_INIT
	beVersion = NULL;
	m_pToolTip = NULL;
}

CEncoderSheet::~CEncoderSheet()
{

	delete m_pToolTip;
}

void CEncoderSheet::DoDataExchange(CDataExchange* pDX)
{

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEncoderSheet)
	DDX_Control(pDX, IDC_CONVERTTO16BIT, c_convertTo16Bit);
	DDX_Control(pDX, IDC_ID3V1, c_writeId3v1);
	DDX_Control(pDX, IDC_ABR, c_abr);
	DDX_Control(pDX, IDC_BITSPERSAMPLE_MAX, c_maxBitrate);
	DDX_Control(pDX, IDC_QUALITY, m_qualityPreset);
	DDX_Control(pDX, IDC_VBR_QUALITY, c_vbrQuality);
	DDX_Control(pDX, IDC_VBR_METHOD, c_vbrMethod);
	DDX_Control(pDX, IDC_MPEG, c_MPEG);
	DDX_Control(pDX, IDC_PRIVATE, c_private);
	DDX_Control(pDX, IDC_ORIGINAL, c_original);
	DDX_Control(pDX, IDC_ID3_2, c_writeId3v2);
	DDX_Control(pDX, IDC_COPYRIGHT, c_copyright);
	DDX_Control(pDX, IDC_CHECKSUM, c_checkSum);
	DDX_Control(pDX, IDC_CHANNELS, c_channels);
	DDX_Control(pDX, IDC_PLAYINGTIME, c_tlen);
	DDX_Control(pDX, IDC_BITSPERSAMPLE, c_bitsPerSample);
	DDX_Text(pDX, IDC_ENCODER_VERSION, m_encoderVersion);
	DDX_Text(pDX, IDC_ABR, m_abr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEncoderSheet, CPropertyPage)
	//{{AFX_MSG_MAP(CEncoderSheet)
	ON_CBN_SELCHANGE(IDC_VBR_METHOD, OnSelchangeVbrMethod)
	ON_CBN_SELCHANGE(IDC_QUALITY, OnSelchangeQuality)
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

	 	//MP3_VERSION mp3Version;

//	memset(&mp3Version, 0, sizeof(mp3Version));
}

BOOL CEncoderSheet::OnInitDialog()
{

	CPropertyPage::OnInitDialog();



	//Set up the tooltip
	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
	   TRACE("Unable To create ToolTip\n");
	   return TRUE;
	}

	//m_pToolTip->AddTool(this, "Encoder Settings");
	m_pToolTip->AddTool(&c_copyright, IDS_TOOL_CP);
	m_pToolTip->AddTool(&c_checkSum, IDS_TOOL_CRC);
	m_pToolTip->AddTool(&c_original, IDS_TOOL_ORIG);
	m_pToolTip->AddTool(&c_private, IDS_TOOL_PRIVATE);
	m_pToolTip->AddTool(&c_channels, IDS_TOOL_CHANNELS);
	m_pToolTip->AddTool(&c_bitsPerSample, IDS_TOOL_MINBITRATE);
	m_pToolTip->AddTool(&c_abr, IDS_TOOL_ABR);
	m_pToolTip->AddTool(&c_vbrMethod, IDS_TOOL_VBRMETHOD);
	m_pToolTip->AddTool(&c_vbrQuality, IDS_TOOL_VBRQUALITY);
	m_pToolTip->AddTool(&c_maxBitrate, IDS_TOOL_MAXBITRATE);
	m_pToolTip->AddTool(&c_MPEG, IDS_TOOL_MPEGVER);
	m_pToolTip->AddTool(&c_writeId3v2, IDS_TOOL_WRITEID3);
	m_pToolTip->AddTool(&m_qualityPreset, IDS_TOOL_QUALPRESET);

	m_pToolTip->Activate(TRUE);

	cfgFile cfg(wd);

	c_bitsPerSample.SetCurSel(cfg.GetValue("bitrate"));
	c_maxBitrate.SetCurSel(cfg.GetValue("maxbitrate"));
	m_qualityPreset.SetCurSel(cfg.GetValue("qualitypreset"));
	c_channels.SetCurSel(cfg.GetValue("channels"));
	c_checkSum.SetCheck(cfg.GetValue("crc"));
	c_copyright.SetCheck(cfg.GetValue("copyright"));
	c_original.SetCheck(cfg.GetValue("original"));
	c_private.SetCheck(cfg.GetValue("private"));
	c_writeId3v2.SetCheck(cfg.GetValue("id3v2"));
	c_vbrMethod.SetCurSel(cfg.GetValue("vbrmethod"));
	c_vbrQuality.SetCurSel(cfg.GetValue("vbrquality"));
	m_abr = cfg.GetValue("abr");
	c_MPEG.SetCurSel(cfg.GetValue("mpegver"));
	c_tlen.SetCheck(cfg.GetValue("writetlantag"));
	c_convertTo16Bit.SetCheck(cfg.GetValue("convertto16bit"));
	c_writeId3v1.SetCheck(cfg.GetValue("id3v1"));
	OnSelchangeVbrMethod();
	UpdateData(FALSE);

	return TRUE;
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
	
	UpdateData(TRUE);

	cfgFile cfg(wd);
	
	cfg.SetValue("bitrate", c_bitsPerSample.GetCurSel());
	cfg.SetValue("channels", c_channels.GetCurSel());
	cfg.SetValue("crc", c_checkSum.GetCheck());
	cfg.SetValue("copyright", c_copyright.GetCheck());
	cfg.SetValue("original", c_original.GetCheck());
	cfg.SetValue("private", c_private.GetCheck());
	cfg.SetValue("id3v2", c_writeId3v2.GetCheck());
	cfg.SetValue("maxbitrate", c_maxBitrate.GetCurSel());
	cfg.SetValue("qualitypreset", m_qualityPreset.GetCurSel());
	cfg.SetValue("vbrmethod", c_vbrMethod.GetCurSel());
	cfg.SetValue("vbrquality", c_vbrQuality.GetCurSel());
	cfg.SetValue("mpegver", c_MPEG.GetCurSel());
	cfg.SetValue("abr", m_abr);
	cfg.SetValue("writetlantag", c_tlen.GetCheck());
	cfg.SetValue("id3v1", c_writeId3v1.GetCheck());
	cfg.SetValue("convertto16bit", c_convertTo16Bit.GetCheck());
}


void CEncoderSheet::OnSelchangeVbrMethod() 
{

	if(c_vbrMethod.GetCurSel() != 0){

		c_maxBitrate.EnableWindow(TRUE);
		c_vbrQuality.EnableWindow(TRUE);
	}
	else{

		c_maxBitrate.EnableWindow(FALSE);
		c_vbrQuality.EnableWindow(FALSE);
	}
	if(c_vbrMethod.GetCurSel() == 5){

		c_abr.EnableWindow(TRUE);
	}
	else{

		c_abr.EnableWindow(FALSE);
	}
}

BOOL CEncoderSheet::PreTranslateMessage(MSG* pMsg) 
{

	if (NULL != m_pToolTip){
		
		m_pToolTip->RelayEvent(pMsg);
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}


void CEncoderSheet::OnSelchangeQuality() 
{

	ASSERT((m_qualityPreset.GetCurSel() >= 0) && (m_qualityPreset.GetCurSel() < 12));

	switch(m_qualityPreset.GetCurSel()){

	case 0: // Normal (default)
		c_bitsPerSample.EnableWindow(TRUE);
		c_maxBitrate.EnableWindow(TRUE);
		c_vbrMethod.EnableWindow(TRUE);
		c_vbrQuality.EnableWindow(TRUE);
		OnSelchangeVbrMethod();
		break;
	case 1: // Low
	case 2: // High
	case 3: // Voice
	case 4: // R3Mix
	case 5: // Very High
	case 7: // Standard
	case 8: // Fast Standard
	case 9: // Extreme
	case 10: // Fast Extreme
	case 11: // Insane
		c_bitsPerSample.EnableWindow(FALSE);
		c_maxBitrate.EnableWindow(FALSE);
		c_vbrMethod.EnableWindow(FALSE);
		c_vbrQuality.EnableWindow(FALSE);
		c_abr.EnableWindow(FALSE);
		c_vbrQuality.EnableWindow(FALSE);
	}		

}
