// WaveFile.cpp: Implementierung der Klasse WaveFile.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdafx.h"
#include "WaveFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////


//This class will be removed, when lame_inc.dll checks itself the wave-format
WaveFile::WaveFile()
{

}

WaveFile::~WaveFile()
{

}

BOOL WaveFile::Open(const std::string &filename, WAVEFORMATEX *pwfx)
{
	std::ifstream file;
	char csID[16];
	long wfxsize;
	//long datasize;

	file.open (filename.c_str (), std::ios_base::binary); //open wavefile as binary...
	
    if (file == NULL){
		return TRUE; //return TRUE for isError variable
    }
	
    file.read (csID, 16); //skip first 16bytes containing RIFF-Wave stuff, not interesting
	
	//Get PCM-Wave-Header
    file.read ((char*)&wfxsize, 4); 
    file.read ((char*)pwfx, wfxsize);

	file.close();
	return FALSE;
}

