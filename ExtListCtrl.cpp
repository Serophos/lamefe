// ExtListCtrl.cpp: Implementierungsdatei
//

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

}

CExtListCtrl::~CExtListCtrl()
{
}


BEGIN_MESSAGE_MAP(CExtListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CExtListCtrl)
	ON_NOTIFY_REFLECT(LVN_INSERTITEM, OnInsertitem)
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CExtListCtrl 

void CExtListCtrl::OnInsertitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	//ListView_SetExtendedListViewStyle(m_hWnd,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	*pResult = 0;
}

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

BOOL CExtListCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);;
}

BOOL CExtListCtrl::Init()
{

	ListView_SetExtendedListViewStyle(m_hWnd,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
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
