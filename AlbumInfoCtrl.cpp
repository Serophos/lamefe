// AlbumInfoCtrl.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lameFE.h"
#include "AlbumInfoCtrl.h"
//#include "LameFEView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CAlbumInfoCtrl 

static const UINT UWM_ALBUMINFO_UPDATED = ::RegisterWindowMessage(_T("UWM_RESET_VIEW--{4E7F6EC0-6ADC-11d3-BC36-006067709674}"));

static const char *const genre_names[] =
{
	"Acapella", "Acid", "Acid Jazz", "Acid Punk", "Acoustic", "Alternative", "Alternative Rock", "Ambient", 
	"Anime", "Avantgarde", "Ballad", "Bass", "Beat", "Bebob", "Big Band", "Black Metal", "Bluegrass", "Blues", 
	"Booty Bass", "BritPop", "Cabaret", "Celtic", "Chamber Music", "Chanson", "Chorus", "Christian Gangsta", 
	"Christian Rap", "Christian Rock", "Classic Rock", "Classical", "Club", "Club-House", "Comedy", 
	"Contemporary C", "Country", "Crossover", "Cult", "Dance", "Dance Hall", "Darkwave", "Death Metal", 
	"Disco", "Dream", "Drum & Bass", "Drum Solo", "Duet", "Easy Listening", "Electronic", "Ethnic", "Eurodance", 
	"Euro-House", "Euro-Techno", "Fast Fusion", "Folk", "Folklore", "Folk-Rock", "Freestyle", "Funk", "Fusion", 
	"Game", "Gangsta", "Goa", "Gospel", "Gothic", "Gothic Rock", "Grunge", "Hard Rock", "Hardcore", "Heavy Metal", 
	"Hip-Hop", "House", "Humour", "Indie", "Industrial", "Instrumental", "Instrumental Pop", "Instrumental Rock", 
	"Jazz", "Jazz+Funk", "JPop", "Jungle", "Latin", "Lo-Fi", "Meditative", "Merengue", "Metal", "Musical", "National Folk", 
	"Native US", "Negerpunk", "New Age", "New Wave", "Noise", "Oldies", "Opera", "Other", "Polka", "Polsk Punk", "Pop", 
	"Pop/Funk", "Pop-Folk", "Porn Groove", "Power Ballad", "Pranks", "Primus", "Progressive Rock", "Psychadelic", 
	"Psychedelic Rock", "Punk", "Punk Rock", "R&B", "Rap", "Rave", "Reggae", "Retro", "Revival", "Rhytmic Soul", "Rock", 
	"Rock & Roll", "Salsa", "Samba", "Satire", "Showtunes", "Ska", "Slow Jam", "Slow Rock", "Sonata", "Sound Clip", 
	"Soundtrack", "Southern Rock", "Space", "Speech", "Swing", "Symphonic Rock", "Symphony", "SynthPop", "Tango", 
	"Techno", "Techno-Industrial", "Terror", "Thrash Metal", "Top ", "Trailer", "Trance", "Tribal", "Trip-Hop", 
	"Vocal"
};

#define GENRE_NAME_COUNT   ((int)(sizeof genre_names / sizeof (const char *const)))




CAlbumInfoCtrl::CAlbumInfoCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CAlbumInfoCtrl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlbumInfoCtrl)
	m_strAlbum = _T("");
	m_strComment = _T("");
	m_strGenre = _T("");
	m_strInterpret = _T("");
	m_strSong = _T("");
	m_nTrack = 0;
	m_nYear = 0;
	//}}AFX_DATA_INIT
	m_bDataChanged = FALSE;
}


void CAlbumInfoCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlbumInfoCtrl)
	DDX_Control(pDX, IDC_YEAR, m_cYear);
	DDX_Control(pDX, IDC_TRACKNUMBER, m_cTrackNumber);
	DDX_Control(pDX, IDC_SONGTITLE, m_cSongTitle);
	DDX_Control(pDX, IDC_SONGINTERPRET, m_cSongInterpret);
	DDX_Control(pDX, IDC_COMMENT, m_cComment);
	DDX_Control(pDX, IDC_ALBUMNAME, m_cAlbumInfo);
	DDX_Control(pDX, IDC_GENRE, m_ctrlGenre);
	DDX_Text(pDX, IDC_ALBUMNAME, m_strAlbum);
	DDX_Text(pDX, IDC_COMMENT, m_strComment);
	DDX_CBString(pDX, IDC_GENRE, m_strGenre);
	DDX_Text(pDX, IDC_SONGINTERPRET, m_strInterpret);
	DDX_Text(pDX, IDC_SONGTITLE, m_strSong);
	DDX_Text(pDX, IDC_TRACKNUMBER, m_nTrack);
	DDV_MinMaxInt(pDX, m_nTrack, 0, 100);
	DDX_Text(pDX, IDC_YEAR, m_nYear);
	DDV_MinMaxInt(pDX, m_nYear, 0, 3000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlbumInfoCtrl, CDialog)
	//{{AFX_MSG_MAP(CAlbumInfoCtrl)
	ON_EN_KILLFOCUS(IDC_ALBUMNAME, OnChangeAlbumname)
	ON_EN_KILLFOCUS(IDC_SONGTITLE, OnChangeAlbumname)
	ON_EN_KILLFOCUS(IDC_SONGINTERPRET, OnChangeAlbumname)
	ON_EN_KILLFOCUS(IDC_TRACKNUMBER, OnChangeAlbumname)
	ON_EN_KILLFOCUS(IDC_YEAR, OnChangeAlbumname)
	ON_EN_KILLFOCUS(IDC_COMMENT, OnChangeAlbumname)
	ON_CBN_KILLFOCUS(IDC_GENRE, OnChangeAlbumname)
	ON_EN_KILLFOCUS(IDC_ALBUMNAME, OnChangeAlbumname)
	ON_EN_CHANGE(IDC_SONGTITLE, OnChangeAlbumInfo)
	ON_EN_CHANGE(IDC_SONGINTERPRET, OnChangeAlbumInfo)
	ON_EN_CHANGE(IDC_TRACKNUMBER, OnChangeAlbumInfo)
	ON_EN_CHANGE(IDC_YEAR, OnChangeAlbumInfo)
	ON_EN_CHANGE(IDC_COMMENT, OnChangeAlbumInfo)
	ON_CBN_EDITCHANGE(IDC_GENRE, OnChangeAlbumInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CAlbumInfoCtrl 

void CAlbumInfoCtrl::SetInfo(CID3Info *info)
{

	m_nTrack = info->GetTrack();
	m_nYear  = info->GetYear();

	m_strAlbum		= info->GetAlbum();
	m_strComment	= info->GetComment();
	m_strGenre		= info->GetGenre();
	m_strInterpret	= info->GetArtist();
	m_strSong		= info->GetSong();
	UpdateData(FALSE);
}


void CAlbumInfoCtrl::OnChangeAlbumname() 
{

	if(!IsWindowVisible() || !m_bDataChanged){

		// Window is not visible so of course has no focus
		// or no data was modified dont send message either
		return;
	}
	TRACE("Sending message\n");

	UpdateData(TRUE);

	// Get MainWindow
	CFrameWnd *pFrame =  (CFrameWnd*)AfxGetApp()->m_pMainWnd;

	// Get the active view attached to the FrameWnd
	CView *pView = (CView *) pFrame->GetActiveView();
	
	// Send UPDATED message
	pView->SendMessage(UWM_ALBUMINFO_UPDATED, 0, 0);

	// message has been send so set modified flag to false
	m_bDataChanged = FALSE;

}

BOOL CAlbumInfoCtrl::OnInitDialog()
{

	CDialog::OnInitDialog();

	int n = 0;
	while(n < GENRE_NAME_COUNT){

		m_ctrlGenre.AddString(genre_names[n++]);
	}

	return TRUE;
}

void CAlbumInfoCtrl::OnChangeAlbumInfo() 
{

	m_bDataChanged = TRUE;	
}

void CAlbumInfoCtrl::EnableControls(BOOL bEnable)
{

	GetDlgItem(IDC_ALBUMNAME)->EnableWindow(bEnable);
	GetDlgItem(IDC_SONGTITLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_SONGINTERPRET)->EnableWindow(bEnable);
	GetDlgItem(IDC_TRACKNUMBER)->EnableWindow(bEnable);
	GetDlgItem(IDC_YEAR)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMMENT)->EnableWindow(bEnable);
	GetDlgItem(IDC_GENRE)->EnableWindow(bEnable);
}

void CAlbumInfoCtrl::Clear()
{

	m_nTrack = 0;
	m_nYear  = 0;

	m_strAlbum		= "";
	m_strComment	= "";
	m_strGenre		= "";
	m_strInterpret	= "";
	m_strSong		= "";

	UpdateData(FALSE);
}

void CAlbumInfoCtrl::OnCancel()
{

}
