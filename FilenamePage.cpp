// FilenamePage.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lameFE.h"
#include "FilenamePage.h"
#include "cfgFile.h"
//#include "FolderDialog.h"
#include "PathDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Eigenschaftenseite CFilenamePage 

IMPLEMENT_DYNCREATE(CFilenamePage, CPropertyPage)

CFilenamePage::CFilenamePage() : CPropertyPage(CFilenamePage::IDD)
{
	//{{AFX_DATA_INIT(CFilenamePage)
	m_strAlbumMode = _T("");
	m_strAlbumPrev = _T("");
	m_strFilenamePrev = _T("");
	m_strFilename = _T("");
	m_strPlaylist = _T("");
	m_strPlaylistPrev = _T("");
	m_bRename = FALSE;
	//}}AFX_DATA_INIT
	m_pToolTip = NULL;

}

CFilenamePage::~CFilenamePage()
{
	delete m_pToolTip;
}

void CFilenamePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilenamePage)
	DDX_Control(pDX, IDC_ALBUM_MODE_FILENAME, m_ctrlAlbum);
	DDX_Control(pDX, IDC_PLAYLIST_FILENAME, m_ctrlPlaylist);
	DDX_Control(pDX, IDC_FORMAT, m_ctrlFilename);
	DDX_Text(pDX, IDC_ALBUM_MODE_FILENAME, m_strAlbumMode);
	DDX_Text(pDX, IDC_ALBUM_MODE_VIS, m_strAlbumPrev);
	DDX_Text(pDX, IDC_FILENAME_VIS, m_strFilenamePrev);
	DDX_Text(pDX, IDC_FORMAT, m_strFilename);
	DDX_Text(pDX, IDC_PLAYLIST_FILENAME, m_strPlaylist);
	DDX_Text(pDX, IDC_PLAYLIST_VIS, m_strPlaylistPrev);
	DDX_Check(pDX, IDC_RENAME, m_bRename);
	DDX_Text(pDX, IDC_OUTPUTPATH, m_strOutputPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilenamePage, CPropertyPage)
	//{{AFX_MSG_MAP(CFilenamePage)
	ON_BN_CLICKED(IDC_RENAME, OnRename)
	ON_BN_CLICKED(IDC_PATH, OnPath)
	ON_EN_CHANGE(IDC_ALBUM_MODE_FILENAME, OnChangeAlbumModeFilename)
	ON_EN_CHANGE(IDC_FORMAT, OnChangeFormat)
	ON_EN_CHANGE(IDC_PLAYLIST_FILENAME, OnChangePlaylistFilename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CFilenamePage 

void CFilenamePage::Init(CString strWd)
{

	m_strWd = strWd;
}

BOOL CFilenamePage::OnInitDialog()
{

	CPropertyPage::OnInitDialog();

	m_pToolTip = new CToolTipCtrl;
	if(!m_pToolTip->Create(this))
	{
	   TRACE("Unable To create ToolTip\n");
	   return TRUE;
	}

	m_pToolTip->AddTool(&m_ctrlFilename, IDS_TOOL_IDENTCOMB);
	m_pToolTip->AddTool(&m_ctrlPlaylist, IDS_TOOL_IDENTCOMB);
	m_pToolTip->AddTool(&m_ctrlAlbum, IDS_TOOL_IDENTCOMB);

	cfgFile cfg(m_strWd);

	m_strFilename  = cfg.GetStringValue("formatstr");
	m_strAlbumMode = cfg.GetStringValue("albumstr");
	m_strPlaylist  = cfg.GetStringValue("playliststr");
	m_strOutputPath = cfg.GetStringValue("output");
	m_bRename = cfg.GetValue("rename");
	
	CreatePreview(m_strAlbumMode, m_strAlbumPrev);
	CreatePreview(m_strFilename, m_strFilenamePrev);
	CreatePreview(m_strPlaylist, m_strPlaylistPrev);
	
	UpdateData(FALSE);

	return TRUE;
}

void CFilenamePage::OnRename() 
{

	UpdateData(TRUE);
	m_ctrlFilename.EnableWindow(m_bRename);

}

void CFilenamePage::OnChangeAlbumModeFilename() 
{
	
	UpdateData(TRUE);
	CreatePreview(m_strAlbumMode, m_strAlbumPrev);
	UpdateData(FALSE);
}

void CFilenamePage::OnChangeFormat() 
{

	UpdateData(TRUE);
	CreatePreview(m_strFilename, m_strFilenamePrev);
	UpdateData(FALSE);
}

void CFilenamePage::OnChangePlaylistFilename() 
{

	UpdateData(TRUE);
	CreatePreview(m_strPlaylist, m_strPlaylistPrev);
	UpdateData(FALSE);
}

void CFilenamePage::CreatePreview(CString &in, CString &out)
{

	in.Replace('/', '-');
	in.Remove('*');
	in.Remove('?');
	in.Replace('\"', '\'');
	in.Replace('<', '(');
	in.Replace('>', ')');
	in.Replace('|', '-');
	in.Remove(':');
	if(in.GetAt(0) == '\\'){

		in = in.Right(in.GetLength() - 1);
	}
	out = in;
	out.Replace("%1", "<Artist>");
	out.Replace("%2", "<Song>");
	out.Replace("%3", "<Album>");
	out.Replace("%4", "<Year>");
	out.Replace("%5", "<Genre>");
	out.Replace("%a", "<Track #>");
	out.Replace("%b", "<Total #>");
	out.Replace("%c", "<Disc ID>");
	out.Replace("%d", "<Volume ID>");
}



void CFilenamePage::OnOK() 
{

	UpdateData(TRUE);

	cfgFile cfg(m_strWd);
	
	if((!m_strFilename.GetLength() && m_bRename)){

		AfxMessageBox("Filename Format String is empty!", MB_ICONSTOP+MB_OK);
		return;
	}

	if(!m_strAlbumMode.GetLength()){

		AfxMessageBox("Filename For Album Mode Format String is empty!", MB_ICONSTOP+MB_OK);
		return;
	}

	if(!m_strAlbumMode.GetLength() || !m_strPlaylist.GetLength()){

		AfxMessageBox("Playlist Filename Format String is empty!", MB_ICONSTOP+MB_OK);
		return;
	}

	cfg.SetStringValue("formatstr", m_strFilename);
	cfg.SetStringValue("playliststr", m_strPlaylist);
	cfg.SetStringValue("albumstr", m_strAlbumMode);
	cfg.SetValue("rename", m_bRename);
	cfg.SetStringValue("output", m_strOutputPath);
	CPropertyPage::OnOK();
}

BOOL CFilenamePage::PreTranslateMessage(MSG* pMsg) 
{
	
	if (NULL != m_pToolTip){
		
		m_pToolTip->RelayEvent(pMsg);
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CFilenamePage::OnPath()
{

	UpdateData(TRUE);

	CPathDialog dlg("Select output folder", "Select output directory. Non existent directories will be created.", m_strOutputPath);
	int nResult = dlg.DoModal();
	if(nResult == IDOK){

		m_strOutputPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}
