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

#if !defined(AFX_MULTIMEDIAFILE_H__64DAAEEE_6BEE_4458_B912_C37D830EEE48__INCLUDED_)
#define AFX_MULTIMEDIAFILE_H__64DAAEEE_6BEE_4458_B912_C37D830EEE48__INCLUDED_

#include "ID3Info.h"	// Hinzugefügt von der Klassenansicht
#include <mmreg.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMultimediaFile  
{
public:
	BOOL GetRename();
	CMultimediaFile(CString path = "", CString pluginName = "");
	virtual ~CMultimediaFile();

	CString			GetSaveAs(CString wdir, CString strExt);
	CString			GetFileName();
	CID3Info*		GetID3Info();
	WAVEFORMATEX*	GetWfx();
	CID3Info		m_id3Info;

protected:
	BOOL			m_bRename;
	CString			m_strPluginname;
	BOOL			m_bIsEncoded;
	WAVEFORMATEX	m_wfx;
	CString			m_strInputFile;
	CString			m_strSaveAs;

private:
	BOOL bAlerted;
	void InitID3Tag();
};

#endif // !defined(AFX_MULTIMEDIAFILE_H__64DAAEEE_6BEE_4458_B912_C37D830EEE48__INCLUDED_)
