// DirectSoundPlayer.h: Schnittstelle für die Klasse DirectSoundPlayer.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRECTSOUNDPLAYER_H__919A4D9E_FDBE_464A_B8A1_2751E66BB766__INCLUDED_)
#define AFX_DIRECTSOUNDPLAYER_H__919A4D9E_FDBE_464A_B8A1_2751E66BB766__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class DirectSoundPlayer  
{
public:
	BOOL isPlaying();
	BOOL deInit();
	void play(LPCTSTR path, LPCSTR filename);
	BOOL init();
	DirectSoundPlayer();
	virtual ~DirectSoundPlayer();

};

#endif // !defined(AFX_DIRECTSOUNDPLAYER_H__919A4D9E_FDBE_464A_B8A1_2751E66BB766__INCLUDED_)
