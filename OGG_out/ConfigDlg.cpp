// ConfigDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"

#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CConfigDlg 


CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/, CString wd)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	m_nMaxBitrate = -1;
	m_nMinBitrate = -1;
	m_nNomBitrate = -1;
	//}}AFX_DATA_INIT

	m_strWd = wd;
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, IDC_MIN_BITRATE, m_cMinBitrate);
	DDX_Control(pDX, IDC_NOMINAL_BITRATE, m_cNomBitrate);
	DDX_Control(pDX, IDC_MAX_BITRATE, m_cMaxBitrate);
	DDX_CBIndex(pDX, IDC_MAX_BITRATE, m_nMaxBitrate);
	DDX_CBIndex(pDX, IDC_MIN_BITRATE, m_nMinBitrate);
	DDX_CBIndex(pDX, IDC_NOMINAL_BITRATE, m_nNomBitrate);
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

	UpdateData(TRUE);
	CDialog::OnOK();
}
