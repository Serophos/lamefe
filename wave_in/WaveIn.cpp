// WaveIn.cpp: Implementierung der Klasse CWaveIn.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaveIn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CWaveIn::CWaveIn()
{

	m_sndFile = NULL;
}

CWaveIn::~CWaveIn()
{

	delete m_sndFile;
}

void CWaveIn::Init()
{

}

void CWaveIn::Quit()
{

}

int CWaveIn::Open(const char *path)
{

	m_sndFile = sf_open_read(path, &m_sndInfo);

	if(!m_sndFile){
		
		return -1; //0 success -1 failed
	}
	return 0;
}

size_t CWaveIn::Read(short *ptr, size_t items)
{
	if(!m_sndFile){

		return -1;
	}

	return sf_read_short(m_sndFile, ptr, items);
}

int CWaveIn::GetFileSize()
{

	return 0;
}

void CWaveIn::Close()
{

	if(m_sndFile){

		sf_close(m_sndFile);
	}
}
