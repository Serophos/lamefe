// ConfigDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "ConfigDlg.h"
//#include "cfgFile.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CConfigDlg 


CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	m_nCompressionLevel = -1;
	//}}AFX_DATA_INIT
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, IDC_COMPRESSION_LEVEL, m_cCompression);
	DDX_CBIndex(pDX, IDC_COMPRESSION_LEVEL, m_nCompressionLevel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CConfigDlg 

BOOL CConfigDlg::OnInitDialog()
{

	CDialog::OnInitDialog();
	
	UpdateData(FALSE);
	return TRUE;
}

void CConfigDlg::OnOK() 
{

	CDialog::OnOK();
}

