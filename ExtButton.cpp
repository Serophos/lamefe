// ExtButton.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lameFE.h"
#include "ExtButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtButton

CExtButton::CExtButton()
{

	m_bLBtnDown	= FALSE;
	m_bPainted	= FALSE;
	m_bHasFocus	= FALSE;

	m_clrBtnHilite  = ::GetSysColor(COLOR_3DHILIGHT);//COLOR_BTNFACE);
	m_clrBtnShadow  = ::GetSysColor(COLOR_BTNFACE);
	m_clrBtnFace    = ::GetSysColor(COLOR_WINDOW);
	m_nOffset		= ::GetSystemMetrics(SM_CXHTHUMB);
}

CExtButton::~CExtButton()
{
}


BEGIN_MESSAGE_MAP(CExtButton, CButton)
	//{{AFX_MSG_MAP(CExtButton)
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CExtButton 

void CExtButton::OnKillFocus(CWnd* pNewWnd) 
{

	m_bHasFocus = FALSE;
	DrawButton(normal, m_clrBtnHilite, m_clrBtnHilite);
	CButton::OnKillFocus(pNewWnd);
}

void CExtButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	m_bLBtnDown = TRUE;
	CButton::OnLButtonDown(nFlags, point);
}

void CExtButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	
	m_bLBtnDown = FALSE;
	CButton::OnLButtonUp(nFlags, point);
}

void CExtButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	SetTimer(1,10,NULL);
	OnTimer(1);
	CButton::OnMouseMove(nFlags, point);
}

void CExtButton::OnPaint() 
{

	//CPaintDC dc(this); // device context for painting
	ModifyStyleEx (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE,
		0, SWP_FRAMECHANGED);
	
	Default();

	CPoint pt;
	GetCursorPos(&pt);

	CRect rcItem;
	GetWindowRect(&rcItem);

	if ((rcItem.PtInRect(pt)) || m_bHasFocus){

		DrawButton(raised, m_clrBtnShadow, m_clrBtnHilite);
	}
	else{

		DrawButton(normal, m_clrBtnHilite, m_clrBtnHilite);
	}
	CButton::OnPaint();
}

void CExtButton::OnSetFocus(CWnd* pOldWnd) 
{

	m_bHasFocus = TRUE;
	DrawButton( raised, m_clrBtnShadow, m_clrBtnHilite );
	CButton::OnSetFocus(pOldWnd);
}

void CExtButton::OnTimer(UINT nIDEvent) 
{

	POINT pt;
	GetCursorPos(&pt);
	CRect rcItem;
	GetWindowRect(&rcItem);

	// OnLButtonDown, show pressed.
	if(m_bLBtnDown == TRUE){

		KillTimer (1);
		if (m_bPainted == TRUE){

			DrawButton( pressed, m_clrBtnHilite, m_clrBtnShadow );
			m_bPainted = FALSE;
		}
		return;
	}

	// If mouse leaves, show flat.
	if (!rcItem.PtInRect(pt) && !m_bHasFocus){

		KillTimer (1);
		if (m_bPainted == TRUE){

			DrawButton( normal, m_clrBtnFace, m_clrBtnFace );
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
			DrawButton( raised, m_clrBtnShadow, m_clrBtnHilite );
		}
	}
	CButton::OnTimer(nIDEvent);
}

void CExtButton::DrawButton(STATE eState, COLORREF clrTopLeft, COLORREF clrBottomRight)
{

	CRect rcItem;
	GetClientRect(&rcItem);
	
	rcItem.DeflateRect(0, 1, rcItem.Width() - 13, 1);
	
	CDC* pDC = GetDC();
	rcItem.DeflateRect(1,1);
	// Cover up dark 3D shadow.
	pDC->Draw3dRect(rcItem, clrBottomRight, clrTopLeft);
	
	rcItem.DeflateRect(1,1);
	
	if(!IsWindowEnabled()){

		pDC->Draw3dRect(rcItem, m_clrBtnHilite, m_clrBtnShadow);
	}
	else {

		pDC->Draw3dRect(rcItem, m_clrBtnFace, m_clrBtnShadow);
	}
/*
	rcItem.DeflateRect(1,1);
	pDC->Draw3dRect(rcItem, m_clrBtnFace, m_clrBtnFace);
	rcItem.DeflateRect(1,1);
	pDC->Draw3dRect(rcItem, m_clrBtnFace, m_clrBtnFace);
	rcItem.DeflateRect(1,1);
	pDC->Draw3dRect(rcItem, m_clrBtnFace, m_clrBtnFace);

	rcItem.InflateRect(4, 4);

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

	}*/
}
