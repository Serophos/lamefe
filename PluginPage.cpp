// PluginPage.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lameFE.h"
#include "PluginPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CPluginPage 

IMPLEMENT_DYNCREATE(CPluginPage, CPropertyPage)

CPluginPage::CPluginPage()  : CPropertyPage(CPluginPage::IDD)
{
	//{{AFX_DATA_INIT(CPluginPage)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
}


void CPluginPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluginPage)
		// HINWEIS: Der Klassen-Assistent fügt hier DDX- und DDV-Aufrufe ein
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPluginPage, CDialog)
	//{{AFX_MSG_MAP(CPluginPage)
		// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CPluginPage 
