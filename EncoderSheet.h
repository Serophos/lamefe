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

#if !defined(AFX_ENCODERSHEET_H__7484382F_3ABE_492F_ADD7_EFDD38EFF96C__INCLUDED_)
#define AFX_ENCODERSHEET_H__7484382F_3ABE_492F_ADD7_EFDD38EFF96C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EncoderSheet.h : Header-Datei
//
#include "BladeMP3EncDLL.h"
#include "MyComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CEncoderSheet 

class CEncoderSheet : public CPropertyPage
{
	DECLARE_DYNCREATE(CEncoderSheet)

// Konstruktion
public:
	void OnCancel();
	virtual void OnOK();
	BOOL OnInitDialog();
	void init(CString wd);
	CEncoderSheet();
	~CEncoderSheet();
	CToolTipCtrl* m_pToolTip;

// Dialogfelddaten
	//{{AFX_DATA(CEncoderSheet)
	enum { IDD = IDD_ENCODER_PAGE };
	CMyComboBox	m_cThreadPriority;
	CEdit	c_abr;
	CMyComboBox	c_maxBitrate;
	CMyComboBox	m_qualityPreset;
	CMyComboBox	c_vbrQuality;
	CMyComboBox	c_vbrMethod;
	CMyComboBox	c_MPEG;
	CButton	c_private;
	CButton	c_original;
	CButton	c_writeId3v1;
	CButton c_writeId3v2;
	CButton c_convertTo16Bit;
	CButton	c_copyright;
	CButton	c_checkSum;
	CMyComboBox	c_channels;
	CMyComboBox	c_bitsPerSample;
	CString	m_encoderVersion;
	CButton c_tlen;
	int		m_abr;
	//}}AFX_DATA


// Überschreibungen
	// Der Klassen-Assistent generiert virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CEncoderSheet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CEncoderSheet)
	afx_msg void OnSelchangeVbrMethod();
	afx_msg void OnSelchangeQuality();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void saveSettings();
	CString wd;
	BEVERSION beVersion;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_ENCODERSHEET_H__7484382F_3ABE_492F_ADD7_EFDD38EFF96C__INCLUDED_
