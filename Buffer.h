/*
** Copyright (C) 2002 Thees Winkler
** Based on code from CDEx (c) 2002 by Albert L. Faber http://cdexos.sourceforge.net
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

#if !defined(AFX_BUFFER_H__C52B4F34_7DAE_4150_B85C_08B6169B6D14__INCLUDED_)
#define AFX_BUFFER_H__C52B4F34_7DAE_4150_B85C_08B6169B6D14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBuffer  
{
public:
	CBuffer();
	virtual ~CBuffer();

	BOOL Init( int nBufferSize, int nBuffers );
	void DeInit();
	INT	 Write( PSHORT pData, int nSize );
	INT	 Read( PSHORT pData );
	INT	 BuffersAvailable();

private:
	PSHORT	m_pDataBuffer;
	PINT	m_pSizeBuffer;
	INT		m_nReadIdx;
	INT		m_nWriteIdx;
	INT		m_nBuffers;
	INT		m_nBufferSize;
	INT		m_nBuffersAvailable;

};

#endif // !defined(AFX_BUFFER_H__C52B4F34_7DAE_4150_B85C_08B6169B6D14__INCLUDED_)
