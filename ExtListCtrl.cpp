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
#include "ExtListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const UINT UWM_LISTCTRL_KEYUP = ::RegisterWindowMessage(_T("UWM_LISTCTRL_KEYUP--{4E7F6EC0-6ADC-11d3-BC36-006067709674}"));

/////////////////////////////////////////////////////////////////////////////
// CExtListCtrl

CExtListCtrl::CExtListCtrl()
{
	m_bLBtnDown	= FALSE;
	m_bPainted	= FALSE;
	m_bHasFocus	= FALSE;
	m_bFlatStyle = FALSE;
	m_clrBtnHilite  = ::GetSysColor(COLOR_BTNHILIGHT);
	m_clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);
	m_clrBtnFace    = ::GetSysColor(COLOR_BTNFACE);
	m_nOffset		= ::GetSystemMetrics(SM_CXHTHUMB);
}

CExtListCtrl::~CExtListCtrl()
{
}


BEGIN_MESSAGE_MAP(CExtListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CExtListCtrl)
	ON_WM_KEYUP()
	ON_NOTIFY_REFLECT(NM_SETFOCUS, OnSetfocus)
	ON_NOTIFY_REFLECT(NM_KILLFOCUS, OnKillfocus)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CExtListCtrl 


int CExtListCtrl::GetSelItems(int nMaxItems, LPINT rgIndex) const
{

	int *items = new int[nMaxItems];
	int nCurrent = 0;

	for(int i = 0; i < GetItemCount(); i++){

		if(GetItemState(i, LVNI_SELECTED)){
			
			items[nCurrent++] = i;
		}
	}

	for(i = 0; i < nCurrent; i++){

		rgIndex[i] = items[i];
	}
	delete items;
	return nCurrent;
}

int CExtListCtrl::GetSelCount() const
{

	int nSelCount = 0;

	for(int i = 0; i < GetItemCount(); i++){

		if(GetItemState(i, LVNI_SELECTED)){

			nSelCount++;
		}
	}
	return nSelCount;
}

void CExtListCtrl::SetSelItems(int iCnt, LPINT pIndex)
{

	for(int i = 0; i < iCnt; i++){

		SetItemState(pIndex[i], LVNI_SELECTED, LVNI_SELECTED);
	}
}

void CExtListCtrl::InvertSelection()
{

	for(int i = 0; i < GetItemCount(); i++){

		SetItemState(i, (GetItemState(i, LVNI_SELECTED) & LVNI_SELECTED ? 0 : LVNI_SELECTED), LVNI_SELECTED);
	}
}

void CExtListCtrl::SelectAll()
{

	for(int i = 0; i < GetItemCount(); i++){

		SetItemState(i, LVNI_SELECTED, LVNI_SELECTED);
	}

	SetHotItem(0);
}

int CExtListCtrl::GetCurSel() const
{

	return GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
}


BOOL CExtListCtrl::Init(BOOL bFlat)
{

	ListView_SetExtendedListViewStyle(m_hWnd,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_bFlatStyle = bFlat;
	return TRUE;
}

void CExtListCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	TRACE("Entering CExtListCtrl::OnKeyUp()\n");

	// Get MainWindow
	CFrameWnd *pFrame =  (CFrameWnd*)AfxGetApp()->m_pMainWnd;

	// Get the active view attached to the FrameWnd
	CView *pView = (CView *) pFrame->GetActiveView();
	
	// Send UPDATED message
	pView->SendMessage(UWM_LISTCTRL_KEYUP, 0, 0);

	CListCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CExtListCtrl::OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{

	m_bHasFocus = TRUE;
	DrawListCtrl( raised, m_clrBtnShadow, m_clrBtnHilite );
	
	*pResult = 0;
}

void CExtListCtrl::OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	m_bHasFocus = FALSE;
	DrawListCtrl(normal, m_clrBtnFace, m_clrBtnFace);

	*pResult = 0;
}

void CExtListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	m_bLBtnDown = TRUE;
	CListCtrl::OnLButtonDown(nFlags, point);
}

void CExtListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{

	m_bLBtnDown = FALSE;
	Invalidate();
	CListCtrl::OnLButtonUp(nFlags, point);
}

void CExtListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{

	SetTimer(1,10,NULL);
	OnTimer(1);
	
	CListCtrl::OnMouseMove(nFlags, point);
}

void CExtListCtrl::OnPaint() 
{

	//CPaintDC dc(this); // device context for painting
	CListCtrl::OnPaint();
	if(m_bFlatStyle){

		ModifyStyleEx (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE,
			0, SWP_FRAMECHANGED);
		
		ListView_SetExtendedListViewStyle(m_hWnd,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		Default();

		CPoint pt;
		GetCursorPos(&pt);

		CRect rcItem;
		GetWindowRect(&rcItem);

		if ((rcItem.PtInRect(pt)) || m_bHasFocus){

			DrawListCtrl(raised, m_clrBtnShadow, m_clrBtnHilite);
		}
		else{

			DrawListCtrl(normal, m_clrBtnFace, m_clrBtnFace);
		}
	}
}

void CExtListCtrl::OnTimer(UINT nIDEvent) 
{

	if(!m_bFlatStyle){

		return;
	}
	POINT pt;
	GetCursorPos(&pt);
	CRect rcItem;
	GetWindowRect(&rcItem);

	// OnLButtonDown, show pressed.
	if(m_bLBtnDown == TRUE){

		KillTimer (1);
		if (m_bPainted == TRUE){

			DrawListCtrl( pressed, m_clrBtnShadow, m_clrBtnHilite );
			m_bPainted = FALSE;
		}
		return;
	}

	// If mouse leaves, show flat.
	if (!rcItem.PtInRect(pt) && !m_bHasFocus){

		KillTimer (1);
		if (m_bPainted == TRUE){

			DrawListCtrl( normal, m_clrBtnFace, m_clrBtnFace );
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
			DrawListCtrl( raised, m_clrBtnShadow, m_clrBtnHilite );
		}
	}
	
	CListCtrl::OnTimer(nIDEvent);
}

void CExtListCtrl::OnSysColorChange() 
{
	CListCtrl::OnSysColorChange();
	
	// re-initialize system colors.
	m_clrBtnHilite  = ::GetSysColor(COLOR_BTNHILIGHT);
	m_clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);
	m_clrBtnFace    = ::GetSysColor(COLOR_BTNFACE);
}

void CExtListCtrl::DrawListCtrl(STATE eState, COLORREF clrTopLeft, COLORREF clrBottomRight)
{

	CRect rcItem;
	GetClientRect(&rcItem);
	CDC* pDC = GetDC();
	
	// Cover up dark 3D shadow.
	pDC->Draw3dRect(rcItem, clrTopLeft, clrBottomRight);
	rcItem.InflateRect(1,1);
	
	if(!IsWindowEnabled()){

		pDC->Draw3dRect(rcItem, m_clrBtnHilite, m_clrBtnHilite);
	}
	else {

		pDC->Draw3dRect(rcItem, m_clrBtnFace, m_clrBtnFace);
	}

	rcItem.InflateRect(1,1);
	pDC->Draw3dRect(rcItem, m_clrBtnFace, m_clrBtnFace);
	rcItem.InflateRect(1,1);
	pDC->Draw3dRect(rcItem, m_clrBtnFace, m_clrBtnFace);
	rcItem.InflateRect(1,1);
	pDC->Draw3dRect(rcItem, m_clrBtnFace, m_clrBtnFace);

	rcItem.DeflateRect(4, 4);

	if (!IsWindowEnabled()){

		ReleaseDC(pDC);
		return;
	}

	switch(eState){
	//normal = 1, raised = 2, pressed = 3 
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
