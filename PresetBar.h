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


#if !defined(AFX_PRESETBAR_H__05E0173E_A1C1_42F8_8088_73E53690B449__INCLUDED_)
#define AFX_PRESETBAR_H__05E0173E_A1C1_42F8_8088_73E53690B449__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyDialogBar.h"
#include "MyComboBox.h"

class CPresetBar : public CMyDialogBar  
{
public:
	CPresetBar();
	virtual ~CPresetBar();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDialogBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	CMyComboBox m_mcPreset;
	CString		m_strPreset;
// Implementation
public:
	void Enable(BOOL bEnable);
	virtual void OnInitDialogBar();

	// Generated message map functions
protected:
	CStatusBar*		m_pStatus;
	CString			m_strWd;
	//{{AFX_MSG(CMyDialogBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnSelchangePreset();
	DECLARE_MESSAGE_MAP()


};

#endif // !defined(AFX_PRESETBAR_H__05E0173E_A1C1_42F8_8088_73E53690B449__INCLUDED_)
