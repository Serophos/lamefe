// ExtendeListView.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lameFE.h"
#include "ExtendeListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtendeListView

CExtendeListView::CExtendeListView()
{
}

CExtendeListView::~CExtendeListView()
{
}


BEGIN_MESSAGE_MAP(CExtendeListView, CListCtrl)
	//{{AFX_MSG_MAP(CExtendeListView)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CExtendeListView 

void CExtendeListView::OnInsertitem(NMHDR *pNMHDR, LRESULT *pResult)
{

	ListView_SetExtendedListViewStyle(m_hWnd,LVS_EX_FULLROWSELECT);
}
