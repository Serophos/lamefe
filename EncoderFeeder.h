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

#if !defined(AFX_ENCODERFEEDER_H__893474D9_A825_4A87_82E8_7DC472689C8C__INCLUDED_)
#define AFX_ENCODERFEEDER_H__893474D9_A825_4A87_82E8_7DC472689C8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GenericEncoder.h"
#include "Buffer.h"

class CEncoderFeeder  
{
public:
	CEncoderFeeder(CGenericEncoder *syncEncoder, int nBufferSize, int nBuffers, BOOL &bAbortEnc);
	virtual ~CEncoderFeeder();

	int  GetBufferStatus();
	BOOL AddData(PSHORT pData, DWORD lenght);
	void WaitForFinished();
	static UINT FeederProc(LPVOID param);

private:

	BOOL &m_bAbortEnc;
	void EncProc();

	CBuffer		m_Buffer;
	CMutex		m_ArrayLock;
	BOOL		m_bLastBlock;
	BOOL		bRunning;
	CGenericEncoder*	m_pEncoder;
	CWinThread*	m_pThread;
	int			m_nBufferSize;
	int			m_nBuffers;

};

#endif // !defined(AFX_ENCODERFEEDER_H__893474D9_A825_4A87_82E8_7DC472689C8C__INCLUDED_)
