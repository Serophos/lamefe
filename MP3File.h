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


#if !defined(AFX_MP3FILE_H__2760B98F_4733_4AD9_93A3_3EB55FE03EEB__INCLUDED_)
#define AFX_MP3FILE_H__2760B98F_4733_4AD9_93A3_3EB55FE03EEB__INCLUDED_

#include "ID3Info.h"	// Hinzugefügt von der Klassenansicht
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <mmreg.h>
#include <string>
#include <fstream>
#include "CDRip/CDRip.h"
#include "mfccddb.h"


#define TRACKSPERSEC 75
#define CB_CDDASECTOR 2352

class MP3File : public CObject
{
public:
	CCDDBTrackPosition getTrackDuration();
	long GetEndSector();
	long GetStartSector();
	void UpdateTrackInfo(CString sTrackInfo);
	long getEndSect();
	long getStartSect();
	int getCDTrackNum();

	MP3File(CString filename, CString wd);
	MP3File(TOCENTRY tocEntry, CString wd, BOOL mp3OrWave);

	virtual ~MP3File();

	WAVEFORMATEX*	getWfx();
	ID3Info*		getID3Info();

	void			setEncoded(BOOL aflag);
	void			setErrorString(UINT stringID);
	void			setErrorString(CString error);
	float			getFileSize();
	CString			getFileName();
	CString			getSaveAs();
	void			setRename(BOOL aflag);
	CString			getFileShort();
	CString			getError();
	BOOL			isValidateWave();
	BOOL			isEncoded();
	void			setID3Info(ID3Info *id3Info);
	BOOL			getRename();


protected:

	BOOL mp3Output;
	int cdTrack;
	float		 m_filesize;
	BOOL		 rename;
	CString		 m_filename;
	CString		 m_saveas;
	CString		 m_fileshort;
	WAVEFORMATEX wfx;
	BOOL		 valid;
	BOOL		 encoded;
	CString		 error_string;
	ID3Info		 *id3Tag;

	BOOL		 validateWaveFormat();

private:
	void verifyFilename(CString &strFilename);
	//BOOL m_bSubDirs;
	CString wd;
	CString getValue(CString val);
	CString m_strFormat;
	CString savePath;
	long endSect;
	long startSect;
};

#endif // !defined(AFX_MP3FILE_H__2760B98F_4733_4AD9_93A3_3EB55FE03EEB__INCLUDED_)
