// ExtListBox.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lameFE.h"
#include "ExtListBox.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtListBox

CExtListBox::CExtListBox()
{

	//m_cmnMenu.LoadMenu(IDR_LIST_MENU);
}

CExtListBox::~CExtListBox()
{

}


BEGIN_MESSAGE_MAP(CExtListBox, CListBox)
	//{{AFX_MSG_MAP(CExtListBox)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CExtListBox 

void CExtListBox::OnRButtonDown(UINT nFlags, CPoint point) 
{
/*	RECT r;
	CWnd *owner = NULL;
	owner = GetParent();
	ASSERT(owner);
	owner->GetWindowRect(&r);
	m_cmnMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON ,point.x + r.top ,point.y + r.left, this);
	m_cmnMenu.GetSubMenu(0)->SetDefaultItem(0,TRUE);*/
}


