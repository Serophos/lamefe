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
#include "MyEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FE_MOUSEOVER 1
#define FE_MOUSEOUT 2

/////////////////////////////////////////////////////////////////////////////
// CMyEditCtrl

CMyEditCtrl::CMyEditCtrl(BOOL bDrawSeperator /* = TRUE */)
{

	m_clrHilite		= GetSysColor(COLOR_BTNHIGHLIGHT);
	m_clrShadow		= GetSysColor(COLOR_BTNSHADOW);
	m_clrDkShad		= GetSysColor(COLOR_3DDKSHADOW);
	m_clrButton		= GetSysColor(COLOR_BTNFACE);
	m_bHasFocus		= FALSE;
	m_bDrawGripper  = FALSE;
	m_bDrawSeperator = bDrawSeperator;
	m_bFlatStyle    = TRUE;
}

CMyEditCtrl::~CMyEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CMyEditCtrl)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_TIMER()
	ON_CONTROL_REFLECT(EN_SETFOCUS,   OnSetFocus)
	ON_CONTROL_REFLECT_EX(EN_KILLFOCUS,  OnKillFocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CMyEditCtrl 

BOOL CMyEditCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{

	SetTimer(1, 100, NULL);
	CEdit::OnMouseMove(nFlags, point);
	return FALSE;
}

void CMyEditCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{

	m_bLBtnDown = TRUE;
	CEdit::OnLButtonDown(nFlags, point);
}

void CMyEditCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{

	m_bLBtnDown = FALSE;
	CEdit::OnLButtonUp(nFlags, point);
}

BOOL CMyEditCtrl::OnSetFocus()
{

	m_bHasFocus = TRUE;
	SetTimer(1, 10, NULL);
	CEdit::OnSetFocus(0);
	return FALSE;
}

BOOL CMyEditCtrl::OnKillFocus() 
{

	m_bHasFocus = FALSE;
	CEdit::OnKillFocus(0);
	
	return FALSE;
}

void CMyEditCtrl::OnTimer(UINT nIDEvent) 
{

	POINT pt;
	GetCursorPos(&pt);
	CRect rcItem;
	GetWindowRect(&rcItem);

	static BOOL bPainted = FALSE;

	if(!rcItem.PtInRect(pt) && !m_bHasFocus){

		KillTimer(1);
		DrawEdit(FE_MOUSEOUT, m_clrButton, m_clrButton);
		return;
	}
	else{

		DrawEdit(FE_MOUSEOVER, m_clrShadow, m_clrHilite);
	}

	CEdit::OnTimer(nIDEvent);
}

void CMyEditCtrl::OnPaint() 
{

	Default();
	DrawEdit(FE_MOUSEOUT, m_clrButton, m_clrButton);
}

void CMyEditCtrl::OnSysColorChange() 
{

	CEdit::OnSysColorChange();
	m_clrHilite = GetSysColor(COLOR_BTNHIGHLIGHT);
	m_clrShadow = GetSysColor(COLOR_BTNSHADOW);
	m_clrDkShad = GetSysColor(COLOR_3DDKSHADOW);
	m_clrButton = GetSysColor(COLOR_BTNFACE);
	
}

void CMyEditCtrl::DrawEdit(int style, COLORREF clrTopLeft, COLORREF clrBottomRight)
{

	CRect rcItem;
	CRect rcGripper;
	CRect rcSeperator;

	GetClientRect(&rcItem);
	GetClientRect(&rcGripper);
	GetClientRect(&rcSeperator);

	CDC* pDC = GetDC();
	COLORREF clrBlack;
	clrBlack = RGB(0, 0, 0);

	if(m_bFlatStyle){

		pDC->Draw3dRect(rcItem, clrTopLeft, clrBottomRight);
		rcItem.InflateRect(1,1);
		
		if (!IsWindowEnabled()) {

			pDC->Draw3dRect(rcItem, m_clrHilite, m_clrHilite);
		}
		else {

			pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton);
		}

		rcItem.InflateRect(1,1);
		pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton);
		rcItem.InflateRect(1,1);
		pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton);
		rcItem.InflateRect(1,1);
		pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton);
	
		rcItem.DeflateRect(4, 4);

		if(m_bDrawGripper){

			rcGripper.top = rcItem.top;
			rcGripper.bottom = rcItem.bottom;
			rcGripper.left -= 4;
			rcGripper.right = rcGripper.left +2;
			pDC->Draw3dRect(rcGripper, m_clrHilite, m_clrShadow);
			rcGripper.left -= 3;
			rcGripper.right = rcGripper.left +2;
			pDC->Draw3dRect(rcGripper, m_clrHilite, m_clrShadow);
		}

		if(m_bDrawSeperator){

			rcSeperator.top = rcItem.top;
			rcSeperator.bottom = rcItem.bottom;
			rcSeperator.left = rcItem.right + 2;
			rcSeperator.right = rcSeperator.left + 2;
			pDC->Draw3dRect(rcSeperator, m_clrShadow, m_clrHilite);
		}
	
		if (!IsWindowEnabled()){

			return;
		}
	
		if(style == FE_MOUSEOVER){

			pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton); //added this
			rcItem.top -= 1; //added this
			rcItem.left -= 1;
			rcItem.bottom += 1;
			rcItem.right += 1; //added this
			pDC->Draw3dRect(rcItem, m_clrShadow, m_clrHilite); //was hilite, shadow
		}
	}
	else{

		pDC->Draw3dRect(rcItem, clrBlack, clrBlack);
		rcItem.InflateRect(1, 1);
		pDC->Draw3dRect(rcItem, clrBlack, clrBlack);
		pDC->Draw3dRect(rcItem, clrBlack, clrBlack);
		rcItem.InflateRect(1,1);

		pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton);
		rcItem.InflateRect(1,1);
		pDC->Draw3dRect(rcItem, m_clrButton, m_clrButton);
		rcItem.DeflateRect(4, 4);
		
		if(m_bDrawGripper){

			rcGripper.top = rcItem.top - 2;
			rcGripper.bottom = rcItem.bottom + 2;
			rcGripper.left -= 6;
			rcGripper.right = rcGripper.left + 2;
			pDC->Draw3dRect(rcGripper, clrBlack, clrBlack);
			rcGripper.left -= 3;
			rcGripper.right -= 3;
			pDC->Draw3dRect(rcGripper, clrBlack, clrBlack);
		}

		if(m_bDrawSeperator){

			rcSeperator.top = rcItem.top - 2;
			rcSeperator.bottom = rcItem.bottom + 2;
			rcSeperator.left = rcItem.right + 4;
			rcSeperator.right = rcSeperator.left + 2;
			pDC->Draw3dRect(rcSeperator, clrBlack, clrBlack);
		}
	}

	ReleaseDC(pDC);
}

void CMyEditCtrl::SetIndent(int margin)
{

	SetMargins(margin, 1);
}
