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

#if !defined(AFX_ID3TAGEDITOR_H__BD29D145_C03F_49FA_ACD3_29ACB06393BF__INCLUDED_)
#define AFX_ID3TAGEDITOR_H__BD29D145_C03F_49FA_ACD3_29ACB06393BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ID3TagEditor.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CID3TagEditor 

class CID3TagEditor : public CDialog
{
// Konstruktion
public:
	BOOL ParseFileInformation(CPtrArray *fileInfo, int nSelect = 0);
	CID3TagEditor(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CID3TagEditor)
	enum { IDD = IDD_ID3EDIT };
	CButton	c_id3Tag;
	CButton	c_AlbumMode;
	CButton	c_filename;
	CButton	c_ArtistA;
	CButton	c_YearA;
	CButton	c_TrackA;
	CButton	c_SongA;
	CButton	c_GenreA;
	CButton	c_CommentA;
	CButton	c_YearAM;
	CButton	c_GenreAM;
	CButton	c_CommentAM;
	CButton	c_ArtistAM;
	CButton	c_AlbumAM;
	CButton	c_AlbumA;
	CComboBox	c_genre;
	CButton	c_rename;
	CEdit	c_year;
	CEdit	c_track;
	CEdit	c_song;
	CEdit	c_artist;
	CEdit	c_album;
	CEdit	c_comment;
	CListBox	c_file;
	CString	m_file;
	CString	m_comment;
	CString	m_album;
	CString	m_artist;
	CString	m_song;
	CString	m_status;
	int		m_track;
	int		m_year;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CID3TagEditor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	void checkID3();
	BOOL checkState();
	BOOL initGenreList();
	BOOL InitFileBox();
	virtual BOOL OnInitDialog();

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CID3TagEditor)
	afx_msg void OnSelchangeFiles();
	virtual void OnOK();
	afx_msg void OnSongA();
	afx_msg void OnAlbumA();
	afx_msg void OnAmAlbum();
	afx_msg void OnAmArtist();
	afx_msg void OnAmComment();
	afx_msg void OnAmGenre();
	afx_msg void OnAmYear();
	afx_msg void OnArtistA();
	afx_msg void OnCommentA();
	afx_msg void OnGenreA();
	afx_msg void OnTrackA();
	afx_msg void OnYearA();
	afx_msg void OnId3tag();
	afx_msg void OnAlbumMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int m_curSel;
	CPtrArray *fileList;
	//HINSTANCE hDLL;

private:
	int m_nPreSelect;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_ID3TAGEDITOR_H__BD29D145_C03F_49FA_ACD3_29ACB06393BF__INCLUDED_
