// WaveFile.h: Schnittstelle für die Klasse WaveFile.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEFILE_H__4FD1DC34_BC6B_42E1_88C4_211F143FD592__INCLUDED_)
#define AFX_WAVEFILE_H__4FD1DC34_BC6B_42E1_88C4_211F143FD592__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmreg.h>
#include <string>
#include <fstream>

class WaveFile  
{
public:
	BOOL Open(const std::string &filename, WAVEFORMATEX *pwfx);
	WaveFile();
	virtual ~WaveFile();

};

#endif // !defined(AFX_WAVEFILE_H__4FD1DC34_BC6B_42E1_88C4_211F143FD592__INCLUDED_)
