/*
** Copyright (C) 2002-2003 Thees Winkler
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

#if !defined(AFX_CDPLAYERINI_H__AC80BDFC_1625_4BE0_A8AC_58F577487BC9__INCLUDED_)
#define AFX_CDPLAYERINI_H__AC80BDFC_1625_4BE0_A8AC_58F577487BC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CompactDisk.h"

class CCDPlayerIni  
{
public:
	int Write();
	CString		m_strMCDI;
	BOOL		Read();
	CCDPlayerIni(CCompactDisc* pFiles);
	virtual ~CCDPlayerIni();

private:
	BOOL ReadFile();
	void SetValues();
	BOOL GetItem(LPCSTR item, CString& str);
	CStringArray m_strACurrentCD;
	CStringArray m_strAEntries;
	BOOL FindEntry();
	CCompactDisc* m_pFiles;
};

#endif // !defined(AFX_CDPLAYERINI_H__AC80BDFC_1625_4BE0_A8AC_58F577487BC9__INCLUDED_)
