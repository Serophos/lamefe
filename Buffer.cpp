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

#include "stdafx.h"
#include "stdafx.h"
#include "Buffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CBuffer::CBuffer():
	m_pDataBuffer(NULL),
	m_pSizeBuffer(NULL),
	m_nReadIdx(0),
	m_nWriteIdx(0),
	m_nBufferSize(0),
	m_nBuffers(0),
	m_nBuffersAvailable(0)
{

}

CBuffer::~CBuffer()
{
	DeInit();
}

BOOL CBuffer::Init( int nBufferSize, int nBuffers )
{

	m_nBufferSize		= nBufferSize;
	m_nBuffers			= nBuffers;
	m_nBuffersAvailable = nBuffers;
	
	m_pSizeBuffer = new INT [ nBuffers ];
	m_pDataBuffer = new SHORT[ nBufferSize * nBuffers ];

	memset(m_pSizeBuffer, 0, nBuffers * sizeof(INT));
	memset(m_pDataBuffer, 0, nBufferSize * nBuffers * sizeof(SHORT));

	return TRUE;

}



INT CBuffer::BuffersAvailable()
{

	return m_nBuffersAvailable;
}

INT CBuffer::Write( PSHORT pData, int nSize )
{
	memcpy( 
			&m_pDataBuffer[m_nWriteIdx * m_nBufferSize],
			pData,
			nSize * sizeof(SHORT)
			);

	m_pSizeBuffer[m_nWriteIdx] = nSize;
	m_nWriteIdx= (m_nWriteIdx + 1) % m_nBuffers;
	m_nBuffersAvailable--;

	return  nSize;
}


INT CBuffer::Read( PSHORT pData )
{
	if ( m_nReadIdx == m_nWriteIdx )
	{
		return 0;
	}

	INT	nSize = m_pSizeBuffer[ m_nReadIdx ];

	memcpy( 
			pData,
			&m_pDataBuffer[m_nReadIdx * m_nBufferSize],
			nSize * sizeof( SHORT )
			);


	m_nReadIdx= (m_nReadIdx+1) % m_nBuffers;

	m_nBuffersAvailable++;

	return nSize;
}

void CBuffer::DeInit()
{

	delete [] m_pSizeBuffer;
	delete [] m_pDataBuffer;

	m_pSizeBuffer = NULL;
	m_pDataBuffer = NULL;
}
