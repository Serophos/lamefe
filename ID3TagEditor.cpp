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
#include "ID3TagEditor.h"
#include "MP3File.h"
#include "tagWriter/tagWriter.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CID3TagEditor 


CID3TagEditor::CID3TagEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CID3TagEditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CID3TagEditor)
	m_file = _T("");
	m_comment = _T("");
	m_album = _T("");
	m_artist = _T("");
	m_song = _T("");
	m_status = _T("");
	m_track = 0;
	m_year = 0;
	//}}AFX_DATA_INIT

}


void CID3TagEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CID3TagEditor)
	DDX_Control(pDX, IDC_ID3TAG, c_id3Tag);
	DDX_Control(pDX, IDC_ALBUM_MODE, c_AlbumMode);
	DDX_Control(pDX, IDC_FILE_NAME, c_filename);
	DDX_Control(pDX, IDC_ARTIST_A, c_ArtistA);
	DDX_Control(pDX, IDC_YEAR_A, c_YearA);
	DDX_Control(pDX, IDC_TRACK_A, c_TrackA);
	DDX_Control(pDX, IDC_SONG_A, c_SongA);
	DDX_Control(pDX, IDC_GENRE_A, c_GenreA);
	DDX_Control(pDX, IDC_COMMENT_A, c_CommentA);
	DDX_Control(pDX, IDC_AM_YEAR, c_YearAM);
	DDX_Control(pDX, IDC_AM_GENRE, c_GenreAM);
	DDX_Control(pDX, IDC_AM_COMMENT, c_CommentAM);
	DDX_Control(pDX, IDC_AM_ARTIST, c_ArtistAM);
	DDX_Control(pDX, IDC_AM_ALBUM, c_AlbumAM);
	DDX_Control(pDX, IDC_ALBUM_A, c_AlbumA);
	DDX_Control(pDX, IDC_GENRE, c_genre);
	DDX_Control(pDX, IDC_RENAME, c_rename);
	DDX_Control(pDX, IDC_YEAR, c_year);
	DDX_Control(pDX, IDC_TRACK, c_track);
	DDX_Control(pDX, IDC_SONG, c_song);
	DDX_Control(pDX, IDC_ARTIST, c_artist);
	DDX_Control(pDX, IDC_ALBUM, c_album);
	DDX_Control(pDX, IDC_COMMENT, c_comment);
	DDX_Control(pDX, IDC_FILES, c_file);
	DDX_LBString(pDX, IDC_FILES, m_file);
	DDX_Text(pDX, IDC_COMMENT, m_comment);
	DDX_Text(pDX, IDC_ALBUM, m_album);
	DDX_Text(pDX, IDC_ARTIST, m_artist);
	DDX_Text(pDX, IDC_SONG, m_song);
	DDX_Text(pDX, IDC_STATUS, m_status);
	DDX_Text(pDX, IDC_TRACK, m_track);
	DDV_MinMaxInt(pDX, m_track, 0, 999);
	DDX_Text(pDX, IDC_YEAR, m_year);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CID3TagEditor, CDialog)
	//{{AFX_MSG_MAP(CID3TagEditor)
	ON_LBN_SELCHANGE(IDC_FILES, OnSelchangeFiles)
	ON_BN_CLICKED(IDC_SONG_A, OnSongA)
	ON_BN_CLICKED(IDC_ALBUM_A, OnAlbumA)
	ON_BN_CLICKED(IDC_AM_ALBUM, OnAmAlbum)
	ON_BN_CLICKED(IDC_AM_ARTIST, OnAmArtist)
	ON_BN_CLICKED(IDC_AM_COMMENT, OnAmComment)
	ON_BN_CLICKED(IDC_AM_GENRE, OnAmGenre)
	ON_BN_CLICKED(IDC_AM_YEAR, OnAmYear)
	ON_BN_CLICKED(IDC_ARTIST_A, OnArtistA)
	ON_BN_CLICKED(IDC_COMMENT_A, OnCommentA)
	ON_BN_CLICKED(IDC_GENRE_A, OnGenreA)
	ON_BN_CLICKED(IDC_TRACK_A, OnTrackA)
	ON_BN_CLICKED(IDC_YEAR_A, OnYearA)
	ON_BN_CLICKED(IDC_ID3TAG, OnId3tag)
	ON_BN_CLICKED(IDC_ALBUM_MODE, OnAlbumMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CID3TagEditor 


BOOL CID3TagEditor::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	InitFileBox();

 	MP3_VERSION mp3Version;

	memset(&mp3Version, 0, sizeof(mp3Version));

	TW_GetVersion(&mp3Version);
 	
 	m_status.Format("Using tagWriter.dll Version: %d.%d Compilation Date %d/%d/%d\nHome: %s",
 			mp3Version.MajorVersion,
 			mp3Version.MinorVersion,
 			mp3Version.rMonth,
 			mp3Version.rDay,
 			mp3Version.rYear,
 			mp3Version.Homepage);

	initGenreList();
	c_genre.SetCurSel(95);
	c_ArtistA.SetCheck(TRUE);
	c_AlbumA.SetCheck(TRUE);
	c_CommentA.SetCheck(TRUE);
	c_GenreA.SetCheck(TRUE);
	c_SongA.SetCheck(TRUE);
	c_TrackA.SetCheck(TRUE);
	c_YearA.SetCheck(TRUE);
	c_id3Tag.SetCheck(TRUE);

	m_curSel = -1;
	c_file.SetCurSel(m_nPreSelect);
	OnSelchangeFiles();

	UpdateData(FALSE);
	return TRUE;
}

BOOL CID3TagEditor::ParseFileInformation(CPtrArray *fileInfo, int nSelect)
{
	fileList = fileInfo;
	m_nPreSelect = nSelect;
	return TRUE;
}

BOOL CID3TagEditor::InitFileBox()
{
	if(fileList->GetSize() != 0){

		int n = 0;
		MP3File *tmp;
		while(n < fileList->GetSize()){

			tmp = (MP3File*)fileList->GetAt(n++);
			c_file.AddString( tmp->getFileShort());
			c_filename.SetWindowText("Please choose file from listbox.");
		}

		UpdateData(FALSE);
	}
	
	return TRUE;
}

void CID3TagEditor::OnSelchangeFiles() 
{
	//Save data from Dialog
	UpdateData(TRUE);

	if (m_curSel >= 0){

		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setAlbum(m_album);
		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setArtist(m_artist);
		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setComment(m_comment);
		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setGenre(genre_names[c_genre.GetCurSel()]);
		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setSong(m_song);
		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setTrack(m_track);
		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setYear(m_year);
		((MP3File*)fileList->GetAt(m_curSel))->setRename(c_rename.GetCheck());
	}
	//Now Update Dialog
	c_filename.SetWindowText(m_file);
	m_album = ((MP3File*)fileList->GetAt(m_curSel = c_file.GetCurSel()))->getID3Info()->getAlbum();
	m_artist = ((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->getArtist();
	m_comment = ((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->getComment();
	int position = c_genre.FindStringExact(0, ((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->getGenre());

	if (position == CB_ERR){

		c_genre.SetCurSel(95);
	}
	else{
		
		c_genre.SetCurSel(position);
	}

	m_song = ((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->getSong();
	m_track = ((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->getTrack();
	m_year = ((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->getYear();
	c_rename.SetCheck(((MP3File*)fileList->GetAt(m_curSel))->getRename());
	UpdateData(FALSE);

}

BOOL CID3TagEditor::initGenreList()
{
	int n = 0;
	while(n < GENRE_NAME_COUNT){

		c_genre.AddString(genre_names[n++]);
	}
	return TRUE;
}

void CID3TagEditor::OnOK() 
{
	//Save data from Dialog
	UpdateData(TRUE);
	if (m_curSel >= 0){

		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setAlbum(m_album);
		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setArtist(m_artist);
		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setComment(m_comment);
		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setGenre(genre_names[c_genre.GetCurSel()]);
		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setSong(m_song);
		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setTrack(m_track);
		((MP3File*)fileList->GetAt(m_curSel))->getID3Info()->setYear(m_year);

		((MP3File*)fileList->GetAt(m_curSel))->setRename(c_rename.GetCheck());
	}
	
	CDialog::OnOK();
}


void CID3TagEditor::OnSongA() 
{

	c_song.EnableWindow(c_SongA.GetCheck());
	checkID3();
}

void CID3TagEditor::OnAlbumA() 
{

	c_album.EnableWindow(c_AlbumA.GetCheck());
	checkID3();
}

void CID3TagEditor::OnAmAlbum() 
{

	if(c_AlbumAM.GetCheck() && m_curSel >= 0){
		
		int nResult = AfxMessageBox(IDS_ID3_ALBUMMODE, MB_YESNO + MB_ICONQUESTION);
		if(nResult == IDYES){

			c_album.EnableWindow(FALSE);
			
			UpdateData(TRUE);
			for(int i = 0; i < fileList->GetSize(); i++){
				

				((MP3File *)fileList->GetAt(i))->getID3Info()->setAlbum(m_album);
			}
		}
		else{
		
			c_AlbumAM.SetCheck(FALSE);
		}
	}
	else{

		if(m_curSel < 0){
		
			AfxMessageBox(IDS_NOSELECTION, MB_OK+MB_ICONINFORMATION);
		}
		c_AlbumAM.SetCheck(FALSE);
		c_album.EnableWindow(TRUE);
	}
	checkState();
	
}

void CID3TagEditor::OnAmArtist() 
{

	if(c_ArtistAM.GetCheck() && m_curSel >= 0){
		
		int nResult = AfxMessageBox(IDS_ID3_ARTISTMODE, MB_YESNO + MB_ICONQUESTION);
		if(nResult == IDYES){
			c_artist.EnableWindow(FALSE);
			
			UpdateData(TRUE);

			for(int i = 0; i < fileList->GetSize(); i++){
				
				((MP3File*)fileList->GetAt(i))->getID3Info()->setArtist(m_artist);

			}
		}
		else{
		
			c_ArtistAM.SetCheck(FALSE);
		}
	}
	else{

		if(m_curSel < 0){
		
			AfxMessageBox(IDS_NOSELECTION, MB_OK+MB_ICONINFORMATION);
		}
		c_ArtistAM.SetCheck(FALSE);
		c_artist.EnableWindow(TRUE);
	}

	checkState();

}

void CID3TagEditor::OnAmComment() 
{
	

	if(c_CommentAM.GetCheck() && m_curSel >= 0){
		
		int nResult = AfxMessageBox(IDS_ID3_COMMENTMODE, MB_YESNO + MB_ICONQUESTION);
		if(nResult == IDYES){
			c_comment.EnableWindow(FALSE);
			
			UpdateData(TRUE);
			for(int i = 0; i < fileList->GetSize(); i++){
				
				((MP3File*)fileList->GetAt(i))->getID3Info()->setComment(m_comment);
			}
		}
		else{
		
			c_CommentAM.SetCheck(FALSE);
		}
	}
	else{

		if(m_curSel < 0){
		
			AfxMessageBox(IDS_NOSELECTION, MB_OK+MB_ICONINFORMATION);
		}
		c_CommentAM.SetCheck(FALSE);
		c_comment.EnableWindow(TRUE);
	}
	checkState();
}

void CID3TagEditor::OnAmGenre() 
{
	

	if(c_GenreAM.GetCheck() && m_curSel >= 0){
		
		int nResult = AfxMessageBox(IDS_ID3_GENREMODE, MB_YESNO + MB_ICONQUESTION);
		if(nResult == IDYES){
			c_genre.EnableWindow(FALSE);
			
			UpdateData(TRUE);
			for(int i = 0; i < fileList->GetSize(); i++){
				
				((MP3File*)fileList->GetAt(i))->getID3Info()->setGenre(genre_names[c_genre.GetCurSel()]);

			}
		}
		else{
		
			c_GenreAM.SetCheck(FALSE);
		}
	}
	else{

		if(m_curSel < 0){
		
			AfxMessageBox(IDS_NOSELECTION, MB_OK+MB_ICONINFORMATION);
		}
		c_GenreAM.SetCheck(FALSE);
		c_genre.EnableWindow(TRUE);
	}
	checkState();
}


void CID3TagEditor::OnAmYear() 
{
	

	if(c_YearAM.GetCheck() && m_curSel >= 0){
		
		int nResult = AfxMessageBox(IDS_ID3_YEARMODE, MB_YESNO + MB_ICONQUESTION);
		if(nResult == IDYES){
			c_year.EnableWindow(FALSE);
			
			UpdateData(TRUE);
			for(int i = 0; i < fileList->GetSize(); i++){

				((MP3File*)fileList->GetAt(i))->getID3Info()->setYear(m_year);
			}
		}
		else{
		
			c_YearAM.SetCheck(FALSE);
		}
	}
	else{
		
		if(m_curSel < 0){
		
			AfxMessageBox(IDS_NOSELECTION, MB_OK+MB_ICONINFORMATION);
		}
		c_YearAM.SetCheck(FALSE);
		c_year.EnableWindow(TRUE);
	}
	checkState();
}

void CID3TagEditor::OnArtistA() 
{
	
	c_artist.EnableWindow(c_ArtistA.GetCheck());
	checkID3();
}

void CID3TagEditor::OnCommentA() 
{
	
	c_comment.EnableWindow(c_ArtistA.GetCheck());
	checkID3();
}

void CID3TagEditor::OnGenreA() 
{
	
	c_genre.EnableWindow(c_GenreA.GetCheck());
	checkID3();
}


void CID3TagEditor::OnTrackA() 
{
	
	c_track.EnableWindow(c_TrackA.GetCheck());	
	checkID3();
}

void CID3TagEditor::OnYearA() 
{

	c_year.EnableWindow(c_YearA.GetCheck());
	checkID3();
}

void CID3TagEditor::OnId3tag() 
{

	if(c_id3Tag.GetCheck() == 0 || c_id3Tag.GetCheck() == 2){

		c_id3Tag.SetCheck(0);
		c_AlbumA.SetCheck(false);
		c_ArtistA.SetCheck(false);
		c_CommentA.SetCheck(false);
		c_GenreA.SetCheck(false);
		c_SongA.SetCheck(false);
		c_TrackA.SetCheck(false);
		c_YearA.SetCheck(false);
	}
	else {

		c_AlbumA.SetCheck(true);
		c_ArtistA.SetCheck(true);
		c_CommentA.SetCheck(true);
		c_GenreA.SetCheck(true);
		c_SongA.SetCheck(true);
		c_TrackA.SetCheck(true);
		c_YearA.SetCheck(true);
	}
	OnSongA();
	OnArtistA();
	OnCommentA();
	OnGenreA();
	OnYearA();
	OnAlbumA();
	OnTrackA();

	
}

void CID3TagEditor::OnAlbumMode() 
{

	if(	c_AlbumMode.GetCheck() == 0 || c_AlbumMode.GetCheck() == 2){ //unchecked || undefinite
		
		c_AlbumMode.SetCheck(false);
		c_AlbumAM.SetCheck(false);
		c_ArtistAM.SetCheck(false);
		c_GenreAM.SetCheck(false);
		c_CommentAM.SetCheck(false);
		c_YearAM.SetCheck(false);
	}
	else if( c_AlbumMode.GetCheck() == 1){  //checked  

		c_AlbumAM.SetCheck(true);
		c_ArtistAM.SetCheck(true);
		c_GenreAM.SetCheck(true);
		c_CommentAM.SetCheck(true);
		c_YearAM.SetCheck(true);
		OnAmAlbum();
		OnAmArtist();
		OnAmGenre();
		OnAmComment();
		OnAmYear();
	}
	
}

BOOL CID3TagEditor::checkState()
{

	if(c_ArtistAM.GetCheck() && c_AlbumAM && c_CommentAM.GetCheck() && c_GenreAM.GetCheck() && c_YearAM.GetCheck()){
		
		c_AlbumMode.SetCheck(1);
	}
	else{
	
		if(!c_ArtistAM.GetCheck() && !c_AlbumAM && !c_CommentAM.GetCheck() && !c_GenreAM.GetCheck() && !c_YearAM.GetCheck()){
			
			c_AlbumMode.SetCheck(false);
		}
		else{

			c_AlbumMode.SetCheck(2);
		}

	}

	return c_ArtistAM.GetCheck() && c_AlbumAM && c_CommentAM.GetCheck() && c_GenreAM.GetCheck() && c_YearAM.GetCheck();
}

void CID3TagEditor::checkID3()
{
	
	if(c_ArtistA.GetCheck() && c_AlbumA.GetCheck() && c_CommentA.GetCheck() && c_GenreA.GetCheck() && c_SongA.GetCheck() && c_YearA.GetCheck() && c_TrackA.GetCheck()){

		c_id3Tag.SetCheck(1);

	}
	else{

		if(!(c_ArtistA.GetCheck() + c_AlbumA.GetCheck() + c_CommentA.GetCheck() + c_GenreA.GetCheck() + c_SongA.GetCheck() + c_YearA.GetCheck() + c_TrackA.GetCheck())){

			c_id3Tag.SetCheck(0);
		}
		else{

			c_id3Tag.SetCheck(2);
		}
	}
}


