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
#include "MyComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyComboBox

CMyComboBox::CMyComboBox()
{

	m_bLBtnDown	= FALSE;
	m_bPainted	= FALSE;
	m_bHasFocus	= FALSE;
	m_bAutoComp = TRUE;

	m_clrBtnHilite  = ::GetSysColor(COLOR_BTNHILIGHT);
	m_clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);
	m_clrBtnFace    = ::GetSysColor(COLOR_BTNFACE);
	m_nOffset		= ::GetSystemMetrics(SM_CXHTHUMB);
}

CMyComboBox::~CMyComboBox()
{
}


BEGIN_MESSAGE_MAP(CMyComboBox, CComboBox)
	//{{AFX_MSG_MAP(CMyComboBox)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(CBN_SETFOCUS,   OnSetFocus)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS,  OnKillFocus)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CMyComboBox 

BOOL CMyComboBox::OnMouseMove(UINT nFlags, CPoint point) 
{

	SetTimer(1,10,NULL);
	OnTimer(1);
	CComboBox::OnMouseMove(nFlags, point);

	return FALSE;
}

void CMyComboBox::OnLButtonDown(UINT nFlags, CPoint point) 
{

	m_bLBtnDown = TRUE;
	CComboBox::OnLButtonDown(nFlags, point);
}

void CMyComboBox::OnLButtonUp(UINT nFlags, CPoint point) 
{

	m_bLBtnDown = FALSE;
	Invalidate();
	CComboBox::OnLButtonUp(nFlags, point);
}

void CMyComboBox::OnTimer(UINT nIDEvent) 
{

	POINT pt;
	GetCursorPos(&pt);
	CRect rcItem;
	GetWindowRect(&rcItem);

	// OnLButtonDown, show pressed.
	if(m_bLBtnDown == TRUE){

		KillTimer (1);
		if (m_bPainted == TRUE){

			DrawCombo( pressed, m_clrBtnShadow, m_clrBtnHilite );
			m_bPainted = FALSE;
		}
		return;
	}

	// If mouse leaves, show flat.
	if (!rcItem.PtInRect(pt) && !m_bHasFocus){

		KillTimer (1);
		if (m_bPainted == TRUE){

			DrawCombo( normal, m_clrBtnFace, m_clrBtnFace );
			m_bPainted = FALSE;
		}
		return;
	}
	// On mouse over, show raised.
	else {

		if (m_bPainted == TRUE){

			return;
		}
		else{

			m_bPainted = TRUE;
			DrawCombo( raised, m_clrBtnShadow, m_clrBtnHilite );
		}
	}
	
	CComboBox::OnTimer(nIDEvent);
}

void CMyComboBox::OnPaint() 
{

	ModifyStyleEx (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE,
		0, SWP_FRAMECHANGED);
	
	Default();

	CPoint pt;
	GetCursorPos(&pt);

	CRect rcItem;
	GetWindowRect(&rcItem);

	if ((rcItem.PtInRect(pt)) || m_bHasFocus){

		DrawCombo(raised, m_clrBtnShadow, m_clrBtnHilite);
	}
	else{

		DrawCombo(normal, m_clrBtnFace, m_clrBtnFace);
	}
}

void CMyComboBox::DrawCombo(STATE eState, COLORREF clrTopLeft, COLORREF clrBottomRight)
{

	CRect rcItem;
	GetClientRect(&rcItem);
	CDC* pDC = GetDC();
	
	// Cover up dark 3D shadow.
	pDC->Draw3dRect(rcItem, clrTopLeft, clrBottomRight);
	rcItem.DeflateRect(1,1);
	
	if(!IsWindowEnabled()){

		pDC->Draw3dRect(rcItem, m_clrBtnHilite, m_clrBtnHilite);
	}
	else{

		pDC->Draw3dRect(rcItem, m_clrBtnFace, m_clrBtnFace);
	}

	// Cover up dark 3D shadow on drop arrow.
	rcItem.DeflateRect(1,1);
	rcItem.left = rcItem.right-m_nOffset;
	pDC->Draw3dRect(rcItem, m_clrBtnFace, m_clrBtnFace);
	
	// Cover up normal 3D shadow on drop arrow.
	rcItem.DeflateRect(1,1);
	pDC->Draw3dRect(rcItem, m_clrBtnFace, m_clrBtnFace);
	
	if (!IsWindowEnabled()){

		ReleaseDC(pDC);
		return;
	}

	switch (eState){

	case normal:
		rcItem.top -= 1;
		rcItem.bottom += 1;
		pDC->Draw3dRect( rcItem, m_clrBtnHilite, m_clrBtnHilite );
		rcItem.left -= 1;
		pDC->Draw3dRect( rcItem, m_clrBtnHilite, m_clrBtnHilite );
		break;

	case raised:
		rcItem.top -= 1;
		rcItem.bottom += 1;
		pDC->Draw3dRect( rcItem, m_clrBtnHilite, m_clrBtnShadow);
		break;

	case pressed:
		rcItem.top -= 1;
		rcItem.bottom += 1;
		rcItem.OffsetRect(1,1);
		pDC->Draw3dRect( rcItem, m_clrBtnShadow, m_clrBtnHilite );
		break;
	}

	ReleaseDC(pDC);
}

BOOL CMyComboBox::OnSetFocus()
{

	m_bHasFocus = TRUE;
	DrawCombo( raised, m_clrBtnShadow, m_clrBtnHilite );
	return FALSE;
}

BOOL CMyComboBox::OnKillFocus() 
{

	m_bHasFocus = FALSE;
	DrawCombo(normal, m_clrBtnFace, m_clrBtnFace);
	return FALSE;
}

BOOL CMyComboBox::PreTranslateMessage(MSG* pMsg) 
{

	// Catch the Alt key so we don't choke if focus 
	// is going to an owner drawn button
	//if (pMsg->message == WM_SYSCHAR){

	//	return TRUE;
	//}
		
	return CComboBox::PreTranslateMessage(pMsg);
}

void CMyComboBox::OnEditUpdate() 
{
	// if we are not to auto update the text, get outta here
	if (!m_bAutoComp){

		return;
	}
	
	// Get the text in the edit box
	CString str;
	GetWindowText(str);
	int nLength = str.GetLength();
	
	// Currently selected range
	DWORD dwCurSel = GetEditSel();
	WORD dStart = LOWORD(dwCurSel);
	WORD dEnd   = HIWORD(dwCurSel);
	
	// Search for, and select in, and string in the combo box that is prefixed
	// by the text in the edit box
	if (SelectString(-1, str) == CB_ERR){

		SetWindowText(str);		// No text selected, so restore what was there before
		if (dwCurSel != CB_ERR){

			SetEditSel(dStart, dEnd);	//restore cursor postion
		}
	}
	
	// Set the text selection as the additional text that we have added
	if (dEnd < nLength && dwCurSel != CB_ERR){

		SetEditSel(dStart, dEnd);
	}
	else{

		SetEditSel(nLength, -1);
	}
}

void CMyComboBox::OnSysColorChange() 
{

	CComboBox::OnSysColorChange();

	// re-initialize system colors.
	m_clrBtnHilite  = ::GetSysColor(COLOR_BTNHILIGHT);
	m_clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);
	m_clrBtnFace    = ::GetSysColor(COLOR_BTNFACE);
}

inline void CMyComboBox::EnableAutoCompletion(BOOL bEnable/*=TRUE*/)
{

	ASSERT(::IsWindow(m_hWnd)); 
	m_bAutoComp = bEnable; 
}
