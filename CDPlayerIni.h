// CDPlayerIni.h: Schnittstelle für die Klasse CCDPlayerIni.
//
//////////////////////////////////////////////////////////////////////

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
