/*
** Copyright (C) 2002 Thees Winkler
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "stdafx.h"
#include "lameFE.h"
#include "AlbumInfoCtrl.h"
#include "I18n.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CAlbumInfoCtrl 

static const UINT UWM_ALBUMINFO_UPDATED = ::RegisterWindowMessage(_T("UWM_RESET_VIEW--{4E7F6EC0-6ADC-11d3-BC36-006067709674}"));
static const UINT UWM_TAG_COMMAND = ::RegisterWindowMessage(_T("UWM_TAG_COMMAND--{4E7F6EC0-6ADC-11d3-BC36-006067709674}"));



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


extern CI18n	 g_iLang;


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
	m_strEncodedBy = _T("");
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
	DDX_Text(pDX, IDC_ENCODEDBY, m_strEncodedBy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlbumInfoCtrl, CDialog)
	//{{AFX_MSG_MAP(CAlbumInfoCtrl)
	ON_EN_KILLFOCUS(IDC_ALBUMNAME,		OnChangeAlbumname)
	ON_CBN_EDITCHANGE(IDC_GENRE,	OnChangeAlbumInfo)
	ON_BN_CLICKED(IDC_SEL_ID3V1, OnSelId3v1)
	ON_BN_CLICKED(IDC_SEL_ID3V2, OnSelId3v2)
	ON_BN_CLICKED(IDC_CASE, OnCase)
	ON_BN_CLICKED(IDC_SWAP_TITELINTERPRET, OnModify)
	ON_COMMAND(ID_SWAP_ARTISTALBUM, OnSwapArtistalbum)
	ON_COMMAND(ID_ID3_CLEANUPTAGS, OnId3Cleanuptags)
	ON_COMMAND(ID_SWAP_TITLEARTIST, OnSwapTitleartist)
	ON_COMMAND(ID_ID3_COPYID3V2ID3V1, OnId3Copyid3v2id3v1)
	ON_COMMAND(ID_ID3_COPYID3V1ID3V2, OnId3Copyid3v1id3v2)
	ON_COMMAND(ID_ID3_FIXVARIOUSARTISTAG, OnId3Fixvariousartistag)
	ON_EN_KILLFOCUS(IDC_SONGTITLE,		OnChangeAlbumname)
	ON_EN_KILLFOCUS(IDC_SONGINTERPRET,	OnChangeAlbumname)
	ON_EN_KILLFOCUS(IDC_TRACKNUMBER,	OnChangeAlbumname)
	ON_EN_KILLFOCUS(IDC_YEAR,			OnChangeAlbumname)
	ON_EN_KILLFOCUS(IDC_COMMENT,		OnChangeAlbumname)
	ON_CBN_KILLFOCUS(IDC_GENRE,			OnChangeAlbumname)
	ON_EN_CHANGE(IDC_ALBUMNAME,		OnChangeAlbumInfo)
	ON_EN_CHANGE(IDC_SONGTITLE,		OnChangeAlbumInfo)
	ON_EN_CHANGE(IDC_SONGINTERPRET, OnChangeAlbumInfo)
	ON_EN_CHANGE(IDC_TRACKNUMBER,	OnChangeAlbumInfo)
	ON_EN_CHANGE(IDC_YEAR,			OnChangeAlbumInfo)
	ON_EN_CHANGE(IDC_COMMENT,		OnChangeAlbumInfo)
	ON_BN_CLICKED(IDC_EMPTY_TAG, Clear)
	ON_COMMAND(ID_ID3_CLEARALLFIELDS, Clear)
	ON_COMMAND(ID_ID3_FIX_NAME, OnId3FixName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CAlbumInfoCtrl 

void CAlbumInfoCtrl::SetInfo(CID3Info *info)
{

	m_id3.Copy(info);
	DisplayTag();
}


void CAlbumInfoCtrl::OnChangeAlbumname() 
{

	TRACE("Entering CAlbumInfoCtrl::OnChangeAlbumname()\n");
	if(!IsWindowVisible() || !m_bDataChanged){

		// Window is not visible so of course has no focus
		// or no data was modified dont send message either
		TRACE("Albumdata not changed\n");
		return;
	}
	TRACE("Sending message\n");

	UpdateData(TRUE);

	BOOL bV1 = ((CButton*)GetDlgItem(IDC_SEL_ID3V1))->GetCheck();
	BOOL bV2 = ((CButton*)GetDlgItem(IDC_SEL_ID3V2))->GetCheck();
	ID3T_Tag iType = ID3T_ID3;

	if(bV1 && !bV2) iType = ID3T_ID3V1;
	else if(!bV1 && bV2) iType = ID3T_ID3V2;
	else iType = ID3T_ID3;

	m_id3.SetAlbum(m_strAlbum, iType);
	m_id3.SetArtist(m_strInterpret, iType);
	m_id3.SetComment(m_strComment, iType);
	m_id3.SetEncodedBy(m_strEncodedBy);
	m_id3.SetGenre(m_strGenre, iType);
	m_id3.SetSong(m_strSong, iType);
	m_id3.SetTrackNum(m_nTrack, iType);
	m_id3.SetYearNum(m_nYear, iType);

	// Get MainWindow
	CFrameWnd *pFrame =  (CFrameWnd*)AfxGetApp()->m_pMainWnd;

	// Get the active view attached to the FrameWnd
	CView *pView = (CView *) pFrame->GetActiveView();
	
	// Send UPDATED message
	pView->SendMessage(UWM_ALBUMINFO_UPDATED, 0, 0);

	// message has been send so set modified flag to false
	m_bDataChanged = FALSE;

	TRACE("Leaving CAlbumInfoCtrl::OnChangeAlbumname()\n");
}

BOOL CAlbumInfoCtrl::OnInitDialog()
{

	CDialog::OnInitDialog();

	g_iLang.TranslateDialog(this, IDD_ALBUMINFOCTRL);

	int n = 0;
	while(n < GENRE_NAME_COUNT){

		m_ctrlGenre.AddString(genre_names[n++]);
	}

	((CButton*)GetDlgItem(IDC_SEL_ID3V1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_SEL_ID3V2))->SetCheck(TRUE);
	
	return TRUE;
}

void CAlbumInfoCtrl::OnChangeAlbumInfo() 
{

	TRACE("Entering CAlbumInfoCtrl::OnChangeAlbumInfo()\n");

	m_bDataChanged = TRUE;	

	TRACE("Leaving CAlbumInfoCtrl::OnChangeAlbumInfo()\n");
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
	GetDlgItem(IDC_ENCODEDBY)->EnableWindow(bEnable);
	GetDlgItem(IDC_SEL_ID3V1)->EnableWindow(bEnable);
	GetDlgItem(IDC_SEL_ID3V2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EMPTY_TAG)->EnableWindow(bEnable);
	GetDlgItem(IDC_CASE)->EnableWindow(bEnable);
	GetDlgItem(IDC_SWAP_TITELINTERPRET)->EnableWindow(bEnable);
}


void CAlbumInfoCtrl::OnCancel()
{

}

void CAlbumInfoCtrl::OnOK()
{
	OnChangeAlbumname();
}


void CAlbumInfoCtrl::OnSelId3v1() 
{

	DisplayTag();
}

void CAlbumInfoCtrl::OnSelId3v2() 
{

	DisplayTag();
}

void CAlbumInfoCtrl::DisplayTag()
{


	BOOL bV1 = ((CButton*)GetDlgItem(IDC_SEL_ID3V1))->GetCheck();
	BOOL bV2 = ((CButton*)GetDlgItem(IDC_SEL_ID3V2))->GetCheck();

	if(bV1 & !bV2){

		EnableControls(TRUE);
		m_strAlbum   = m_id3.GetAlbum(ID3T_ID3V1);
		m_strInterpret  = m_id3.GetArtist(ID3T_ID3V1);
		m_strComment = m_id3.GetComment(ID3T_ID3V1);
		m_strSong    = m_id3.GetSong(ID3T_ID3V1);
		m_nYear      = m_id3.GetYearNum(ID3T_ID3V1);
		m_nTrack     = m_id3.GetTrackNum(ID3T_ID3V1);
		m_strGenre   = genre_names[m_id3.GetGenreNum(ID3T_ID3V1)];
		m_ctrlGenre.SetCurSel(m_id3.GetGenreNum(ID3T_ID3V1));
	}
	else if(bV2 & !bV1){
	
		EnableControls(TRUE);
		m_strAlbum   = m_id3.GetAlbum(ID3T_ID3V2);
		m_strInterpret  = m_id3.GetArtist(ID3T_ID3V2);
		m_strComment = m_id3.GetComment(ID3T_ID3V2);
		m_strSong    = m_id3.GetSong(ID3T_ID3V2);
		m_nYear      = atoi(m_id3.GetYear(ID3T_ID3V2));
		m_nTrack     = atoi(m_id3.GetTrack(ID3T_ID3V2));
		m_strGenre   = m_id3.GetGenre();
	}
	else if(bV1 & bV2){
	
		EnableControls(TRUE);
		m_strAlbum   = m_id3.GetAlbum(ID3T_ID3V2);
		m_strInterpret  = m_id3.GetArtist(ID3T_ID3V2);
		m_strComment = m_id3.GetComment(ID3T_ID3V2);
		m_strSong    = m_id3.GetSong(ID3T_ID3V2);
		m_nYear      = atoi(m_id3.GetYear(ID3T_ID3V2));
		m_nTrack     = atoi(m_id3.GetTrack(ID3T_ID3V2));
		m_strGenre   = m_id3.GetGenre();
	}
	else{

		EnableControls(FALSE);
	}

	UpdateData(FALSE);
}
/*
#define AIC_SWAP_ARTISTALBUM	0
#define AIC_SWAP_TITLEARTIST	1
#define AIC_CORRECT_CASE		2
#define AIC_CLEAR_TAG			3
#define AIC_COPY_V1_V2			4
#define AIC_COPY_V2_V1			5
#define AIC_FIX_VARIOUS			6
#define AIC_CLEANUP				7
*/

ID3T_Tag CAlbumInfoCtrl::SendModifyCommand(int nCmd)
{

	UpdateData(TRUE);

	BOOL bV1 = ((CButton*)GetDlgItem(IDC_SEL_ID3V1))->GetCheck();
	BOOL bV2 = ((CButton*)GetDlgItem(IDC_SEL_ID3V2))->GetCheck();
	ID3T_Tag iType = ID3T_ID3;

	if(bV1 && !bV2) iType = ID3T_ID3V1;
	else if(!bV1 && bV2) iType = ID3T_ID3V2;
	else iType = ID3T_ID3;

	// Get MainWindow
	CFrameWnd *pFrame =  (CFrameWnd*)AfxGetApp()->m_pMainWnd;

	// Get the active view attached to the FrameWnd
	CView *pView = (CView *) pFrame->GetActiveView();
	
	// Send UPDATED message
	pView->SendMessage(UWM_TAG_COMMAND, nCmd, iType);

	// message has been send so set modified flag to false
	m_bDataChanged = FALSE;

	return iType;
}
void CAlbumInfoCtrl::OnCase() 
{

	m_id3.Case(SendModifyCommand(AIC_CORRECT_CASE));	
	DisplayTag();
}

void CAlbumInfoCtrl::Clear()
{

	m_id3.Empty(SendModifyCommand(AIC_CLEAR_TAG));

	DisplayTag();
}

void CAlbumInfoCtrl::OnId3Fixvariousartistag() 
{

	m_id3.FixVariousArtistCD(SendModifyCommand(AIC_FIX_VARIOUS));
	
	DisplayTag();

}


void CAlbumInfoCtrl::OnId3FixName() 
{


	m_id3.FixArtistName(SendModifyCommand(AIC_FIX_NAME));
	
	DisplayTag();

}


void CAlbumInfoCtrl::OnSwapArtistalbum() 
{

	m_id3.SwapAlbumInterpret(SendModifyCommand(AIC_SWAP_ARTISTALBUM));

	DisplayTag();
}

void CAlbumInfoCtrl::OnSwapTitleartist() 
{

	m_id3.SwapTitleInterpret(SendModifyCommand(AIC_SWAP_TITLEARTIST));

	DisplayTag();
}

void CAlbumInfoCtrl::OnModify() 
{

	CMenu   m_mID3Menu;
	m_mID3Menu.LoadMenu(IDR_TAGEDIT_MENU);

	// translate the menu items
	g_iLang.TranslateMenu(&m_mID3Menu, IDR_TAGEDIT_MENU, FALSE);

	POINT point;
	GetCursorPos(&point);

	m_mID3Menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
											point.x,
											point.y,
											this );


}


BOOL CAlbumInfoCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	
	return CDialog::OnCommand(wParam, lParam);
}

CID3Info* CAlbumInfoCtrl::GetTag()
{

	return &m_id3;
}

void CAlbumInfoCtrl::OnId3Cleanuptags() 
{

	OnCase();	
	
}


void CAlbumInfoCtrl::OnId3Copyid3v2id3v1() 
{

	m_id3.Sync(ID3T_ID3V1);
}

void CAlbumInfoCtrl::OnId3Copyid3v1id3v2() 
{

	m_id3.Sync(ID3T_ID3V2);
}

