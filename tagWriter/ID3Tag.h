// ID3Tag.h: Schnittstelle für die Klasse ID3Tag.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ID3TAG_H__6376D913_BE2B_4C0D_A2FF_7E228A3AEA67__INCLUDED_)
#define AFX_ID3TAG_H__6376D913_BE2B_4C0D_A2FF_7E228A3AEA67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ID3Tag  
{
public:
	BOOL Update();
	ID3Tag();
	virtual ~ID3Tag();

};

#endif // !defined(AFX_ID3TAG_H__6376D913_BE2B_4C0D_A2FF_7E228A3AEA67__INCLUDED_)
