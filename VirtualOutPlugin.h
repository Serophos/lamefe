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


#if !defined(AFX_VIRTUALOUTPLUGIN_H__833C4B33_04BB_4EFC_B362_BAD4DDF8A621__INCLUDED_)
#define AFX_VIRTUALOUTPLUGIN_H__833C4B33_04BB_4EFC_B362_BAD4DDF8A621__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MultimediaFile.h"


class CVirtualOutPlugin  
{
public:
	CVirtualOutPlugin();
	virtual ~CVirtualOutPlugin();

	//Static members of Winamp fake output "plugin" :)

	static void about(HWND hWnd);
	static void config(HWND hWnd);
	static void init(void);
	static void quit(void);
	static int  open(int samplerate, int numchannels, int bitspersamp, int bufferlenms, int prebufferms);
	static void close(void);
	static int	write(char *buf, int len);
	static int	canwrite(void);
	static int	isplaying(void);
	static int  pause(int pause);
	static void setvolume(int volume);
	static void	setpan(int pan);
	static void flush(int t);
	static int  getoutputtime();
	static int  getwrittentime();

	// other stuff required for In_Module
	static void setinfo(int bitrate, int srate, int stereo, int synched);
	static void SAVSAinit(int maxlatency_in_ms, int srate);
	static void SAVSAdeinit();

	//this is for the eq, vis and dsp plugin stuff
	static void SAaddpcmdata(void *pcmdata, int nch, int bps, int timestamp);
	static int  SAgetmode();
	static void SAadd(void *data, int timestamp, int csa);
	static void VSAaddpcmdata (void *pcmdata, int nch, int bps, int timestamp);
	static int  VSAgetmode(int *specnch, int *wavench);
	static void VSAadd(void *data, int timestamp);
	static void vsasetinfo(int nch, int srate);
	static int  dsp_isactive();
	static int  dsp_dosamples(short int *samples, int numsamples, int bps, int nch, int srate);
	static void eqset(int on, char data[10], int preamp);


};

#endif // !defined(AFX_VIRTUALOUTPLUGIN_H__833C4B33_04BB_4EFC_B362_BAD4DDF8A621__INCLUDED_)
