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

#include "stdafx.h"
#include "stdafx.h"
#include "EncoderFeeder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CEncoderFeeder::CEncoderFeeder(CEncoder *syncEncoder, int nBufferSize, int nBuffers, BOOL &bAbortEnc)
	: m_bAbortEnc(bAbortEnc)
{
	m_pEncoder      = syncEncoder;  //Encoder
	m_bLastBlock	= FALSE;		//last Block ist true when ripping is done and buffer can be read out safely
	bRunning		= FALSE;		//thread running?
	m_pThread		= NULL;			//handle to the encoding thread
	m_nBuffers		= nBuffers;		//How many buffers can be stored in the buffer
	m_nBufferSize	= nBufferSize;	//size of each buffer

	m_Buffer.Init(nBufferSize, nBuffers);   //init the buffer

	m_pThread = AfxBeginThread(FeederProc, this); //start encoder proc

	::Sleep(0);

}

CEncoderFeeder::~CEncoderFeeder()
{

}

BOOL CEncoderFeeder::AddData(PSHORT pData, DWORD length)
{

	TRACE("Entering CEncoderFeeder:AddData()\n");
	if(!bRunning || m_bAbortEnc){

		return TRUE;
	}

	if(m_Buffer.BuffersAvailable()){

		// we wait until half the buffers are availble are reached.
		while((m_Buffer.BuffersAvailable() < m_nBuffers / 2) && !m_bAbortEnc &&bRunning){

			TRACE("Not enough Buffers available\n");
			Sleep(50);
		}
	}

	if(length){

		m_ArrayLock.Lock();    //lock array
		m_Buffer.Write(pData, length); // add Data to buffer
		m_ArrayLock.Unlock(); //unlock array
	}

	TRACE("Leaving CEncoderFeeder:AddData()\n");
	return TRUE;
}

void CEncoderFeeder::EncProc(void)
{
	TRACE("Entering EncderFeeder:encProc. Thread Started\n");
	// indicate thread is active
	bRunning			= TRUE;
	SHORT*		pData	= NULL;  //data to encode
	INT			nSize	= 0;



	pData = new SHORT[m_nBufferSize];

	while(!m_bAbortEnc)
	{
		// are there any entries in the buffer ?
		if( m_Buffer.BuffersAvailable() < m_nBuffers )
		{
		
			m_ArrayLock.Lock();	// lock array
			nSize= m_Buffer.Read(pData); 	// get top most array entry
			m_ArrayLock.Unlock(); 	// get top most array entry

			if (nSize)
			{
				m_pEncoder->PassBuffer(pData, nSize);
			}
		}
		else
		{
			if (m_bLastBlock)
				break;

			Sleep(50);

		}


	}

	delete [] pData;

	// indicate thread has been finished
	TRACE0("## Leaving EncoderFeeder::encProc()\n");

	bRunning = FALSE;
}


void CEncoderFeeder::WaitForFinished()
{
	TRACE("Entering CEncoderFeeder::WaitForFinished\n");
	int nSize = -1;

	DWORD dwStillAlive = 0;

	m_bLastBlock = TRUE;

	dwStillAlive = WaitForSingleObject(m_pThread->m_hThread, 100000);
	

	if (dwStillAlive == WAIT_TIMEOUT)	{
		
		TRACE0("Waited long enough. Kill thread!\n");
		::TerminateThread( m_pThread->m_hThread, 0 );
	}

	TRACE("## Leaving CEncoderFeeder::WaitForFinished()\n");

}

UINT CEncoderFeeder::FeederProc(LPVOID param)
{

	((CEncoderFeeder*)param)->EncProc();

	TRACE("## CEncoderFeeder terminated ##\n");
	return 0;
}

int CEncoderFeeder::GetBufferStatus()
{

	TRACE("Entering CEncoderFeeder:GetBufferStatus()\n");

	return (int)((float)m_Buffer.BuffersAvailable() / (float)m_nBuffers*100);
}


