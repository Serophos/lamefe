// WaveIn.h: Schnittstelle für die Klasse CWaveIn.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEIN_H__6AEB841D_1ABA_471B_ADB7_949861D3F4DE__INCLUDED_)
#define AFX_WAVEIN_H__6AEB841D_1ABA_471B_ADB7_949861D3F4DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\LibSndFile\src\sndfile.h"

class CWaveIn  
{
public:
	void Close();
	int GetFileSize();
	size_t Read(short *ptr, size_t items);
	int Open(const char *path);
	void Quit();
	void Init();
	CWaveIn();
	virtual ~CWaveIn();

private:
	SF_INFO  m_sndInfo;
	SNDFILE* m_sndFile;
};

#endif // !defined(AFX_WAVEIN_H__6AEB841D_1ABA_471B_ADB7_949861D3F4DE__INCLUDED_)
