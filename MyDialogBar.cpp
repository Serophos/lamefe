// MyDialogBar.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lamefe.h"
#include "MyDialogBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDialogBar
//#define WM_INITDIALOGBAR WM_USER + 1

static const UINT WM_INITDIALOGBAR = ::RegisterWindowMessage(_T("WM_INITDIALOGBAR--{4E7F6EC0-6ADC-11d3-BC36-006067709674}"));

CMyDialogBar::CMyDialogBar()
{
}

CMyDialogBar::~CMyDialogBar()
{
}


BEGIN_MESSAGE_MAP(CMyDialogBar, CDialogBar)
	//{{AFX_MSG_MAP(CMyDialogBar)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(WM_INITDIALOGBAR , InitDialogBarHandler )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CMyDialogBar 

int CMyDialogBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	if (CDialogBar::OnCreate(lpCreateStruct) == -1){

		return -1;
	}
 	

	PostMessage(WM_INITDIALOGBAR , 0 , 0 );
	return 0;
}


void CMyDialogBar::InitDialogBarHandler(WORD wParam, DWORD lParam)
{
	UpdateData(FALSE);
	OnInitDialogBar() ;
}



void CMyDialogBar::OnInitDialogBar()
{


}
