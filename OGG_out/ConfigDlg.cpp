/*
** Copyright (C) 2002 - 2003 Thees Winkler
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

#include "ConfigDlg.h"
#include "../out_plugin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CConfigDlg 


CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/, LF_OUT* plf)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	m_nMaxBitrate = -1;
	m_nMinBitrate = -1;
	m_nNomBitrate = -1;
	m_nQuality = 0;
	//}}AFX_DATA_INIT

	m_plfOut = plf;
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, IDC_MIN_BITRATE, m_cMinBitrate);
	DDX_CBIndex(pDX, IDC_MIN_BITRATE, m_nMinBitrate);
	DDX_Control(pDX, IDC_NOMINAL_BITRATE, m_cNomBitrate);
	DDX_CBIndex(pDX, IDC_NOMINAL_BITRATE, m_nNomBitrate);
	DDX_Control(pDX, IDC_MAX_BITRATE, m_cMaxBitrate);
	DDX_CBIndex(pDX, IDC_MAX_BITRATE, m_nMaxBitrate);
	DDX_Control(pDX, IDC_QUALITY_SLIDER, m_slQuality);
	DDX_Slider(pDX, IDC_QUALITY_SLIDER, m_nQuality);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_BN_CLICKED(IDC_QUALITYMANAGED, OnQualitymanaged)
	ON_BN_CLICKED(IDC_VARIABLE, OnVariable)
	ON_BN_CLICKED(IDC_AVERAGE, OnAverage)
	ON_BN_CLICKED(IDC_CONSTANT, OnConstant)
	ON_CBN_SELCHANGE(IDC_NOMINAL_BITRATE, OnSelchangeNominalBitrate)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	//MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CConfigDlg 

BOOL CConfigDlg::OnInitDialog()
{

	CDialog::OnInitDialog();
	
	m_plfOut->TranslateDialog(this, IDD_CONFIG);

	m_cMinBitrate.SetCurSel(m_nMinBitrate);
	m_cNomBitrate.SetCurSel(m_nNomBitrate);
	m_cMaxBitrate.SetCurSel(m_nMaxBitrate);
	m_slQuality.SetRange(0, 1000);
	m_slQuality.SetTicFreq(100);
	m_slQuality.SetPos(m_nQuality);

	UpdateData(FALSE);
	
	switch(m_nMode){

	case 0:  // QUALITY MANAGEMENT
		OnQualitymanaged();
		break;
	case 1:  // variable Bitrate
		OnVariable();
		break; 
	case 2:  // average Bitrate
		OnAverage();
		break;
	case 3:  // constant Bitrate
		OnConstant();
		break;

	}
	
	return TRUE;
	
}

void CConfigDlg::OnOK() 
{

	UpdateData(TRUE);
	CDialog::OnOK();
}

void CConfigDlg::OnQualitymanaged() 
{

	m_nMode = 0;

	((CButton*)GetDlgItem(IDC_QUALITYMANAGED))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_VARIABLE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_AVERAGE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CONSTANT))->SetCheck(FALSE);
	m_slQuality.EnableWindow(TRUE);
	m_cMinBitrate.EnableWindow(FALSE);
	m_cNomBitrate.EnableWindow(FALSE);
	m_cMaxBitrate.EnableWindow(FALSE);
	UpdateQuality();

}

void CConfigDlg::OnVariable() 
{

		
	m_nMode = 1;
	((CButton*)GetDlgItem(IDC_VARIABLE))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_QUALITYMANAGED))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_AVERAGE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CONSTANT))->SetCheck(FALSE);
	m_slQuality.EnableWindow(FALSE);
	m_cMinBitrate.EnableWindow(TRUE);
	m_cNomBitrate.EnableWindow(FALSE);
	m_cMaxBitrate.EnableWindow(TRUE);
}

void CConfigDlg::OnAverage() 
{

	
	m_nMode = 2;
	((CButton*)GetDlgItem(IDC_AVERAGE))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_VARIABLE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_QUALITYMANAGED))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CONSTANT))->SetCheck(FALSE);
	m_slQuality.EnableWindow(FALSE);
	m_cMinBitrate.EnableWindow(FALSE);
	m_cNomBitrate.EnableWindow(TRUE);
	m_cMaxBitrate.EnableWindow(FALSE);
}

void CConfigDlg::OnConstant() 
{

	
	m_nMode = 3;
	((CButton*)GetDlgItem(IDC_CONSTANT))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_VARIABLE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_AVERAGE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_QUALITYMANAGED))->SetCheck(FALSE);

	OnSelchangeNominalBitrate();

	m_slQuality.EnableWindow(FALSE);
	m_cMinBitrate.EnableWindow(FALSE);
	m_cNomBitrate.EnableWindow(TRUE);
	m_cMaxBitrate.EnableWindow(FALSE);
}


void CConfigDlg::OnSelchangeNominalBitrate() 
{

	if(m_nMode == 3){

		UpdateData(TRUE);
		m_nMaxBitrate = m_nNomBitrate;
		m_nMinBitrate = m_nNomBitrate;
		UpdateData(FALSE);
	}	
}

void CConfigDlg::UpdateQuality()
{

//! calculates approximate bitrate for given quality value
/*! this function is Copyright (c) 2002 John Edwards
    see source code for OggDropXPd.
    input: quality value, range 0.f ... 10.f
    output: estimated bitrate in kbps
*/
	if(m_nMode != 0){

		SetDlgItemText(IDC_QUALITY_BITRATE, "");
		SetDlgItemText(IDC_QUALITY, "");
	}

	UpdateData(TRUE);
	
	float fBitrate;
	float fQuality = m_nQuality / 100.f;

	if (fQuality < 4.10)
	{
	  fBitrate = fQuality*16 + 64;
	}
	else if (fQuality < 8.10)
	{
	  fBitrate = fQuality*32;
	}
	else if (fQuality < 9.10)
	{
	  fBitrate = fQuality*32 + (fQuality-8.f)*32;
	}
	else
	{
	  fBitrate = fQuality*32 + (fQuality-8.f)*32 + (fQuality-9.f)*116;
	}
	
	CString strTmp;
	strTmp.Format("Bitrate %.2f kbps", fBitrate);
	SetDlgItemText(IDC_QUALITY_BITRATE, strTmp);
	
	strTmp.Format("Quality: %2.2f", fQuality);
	SetDlgItemText(IDC_QUALITY, strTmp);

}


void CConfigDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen und/oder Standard aufrufen
	UpdateQuality();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

