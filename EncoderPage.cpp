// EncoderPage.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lameFE.h"
#include "EncoderPage.h"
#include "cfgFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CEncoderPage 


CEncoderPage::CEncoderPage(CWnd* pParent /*=NULL*/)
	: CDialog(CEncoderPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEncoderPage)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
	
}


void CEncoderPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEncoderPage)
		// HINWEIS: Der Klassen-Assistent fügt hier DDX- und DDV-Aufrufe ein
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEncoderPage, CDialog)
	//{{AFX_MSG_MAP(CEncoderPage)
		// HINWEIS: Der Klassen-Assistent fügt hier Zuordnungsmakros für Nachrichten ein
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CEncoderPage 
