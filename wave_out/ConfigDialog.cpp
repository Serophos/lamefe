// ConfigDialog.cpp: Implementierungsdatei
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "ConfigDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CConfigDialog 


CConfigDialog::CConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDialog)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT

}


void CConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDialog)
	DDX_Control(pDX, IDC_CHANNELS, m_cChannels);
	DDX_Control(pDX, IDC_SAMPLINGRATE, m_cSamplingRate);
	DDX_Control(pDX, IDC_COMPRESSION, m_cCompression);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDialog, CDialog)
	//{{AFX_MSG_MAP(CConfigDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CConfigDialog 

BOOL CConfigDialog::OnInitDialog()
{
	
	CDialog::OnInitDialog();

//	cfgFile cfg(m_strIniFile);

//	m_cCompression.SetCurSel(cfg.GetValue("wave_out.dll_compression"));
//	m_cSamplingRate.SetCurSel(cfg.GetValue("wave_out.dll_samplerate"));
//	m_cChannels.SetCurSel(cfg.GetValue("wave_out.dll_channels"));

	m_cCompression.SetCurSel(m_nCompression);
	m_cSamplingRate.SetCurSel(m_nSamplingRate);
	m_cChannels.SetCurSel(m_nChannels);
	return FALSE;
}

void CConfigDialog::OnOK() 
{

	
//	cfg.SetValue("wave_out.dll_compression", m_cCompression.GetCurSel());
//	cfg.SetValue("wave_out.dll_samplerate", m_cSamplingRate.GetCurSel());
//	cfg.SetValue("wave_out.dll_channels", m_cChannels.GetCurSel());
		
	m_nCompression = m_cCompression.GetCurSel();
	m_nSamplingRate=  m_cSamplingRate.GetCurSel();
	m_nChannels= m_cChannels.GetCurSel();

	CDialog::OnOK();
}

