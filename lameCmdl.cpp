// lameCmdl.cpp: Implementierung der Klasse lameCmdl.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdafx.h"
#include "lameCmdl.h"
#include "cfgFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

lameCmdl::lameCmdl()
{
	kbSize = 0;
}

lameCmdl::~lameCmdl()
{

}

void lameCmdl::addFile(CString filename, CString wd)
{
	
	cmdl tmp;
	LPCTSTR test = filename.GetBuffer(10);		//Get the C-Style String from CString buffer
	strcpy(tmp.file ,test);					//Copy to fileIn
	filename.ReleaseBuffer();					//release CString buffer
	
	tmp.fileShort = filename.Right(filename.GetLength() - filename.ReverseFind('\\') - 1) ;
		
	tmp.id3Album = "";
	tmp.id3Artist = "";
	tmp.id3Comment = "";
	tmp.id3Genre = "";
	tmp.id3Name = tmp.fileShort;
	tmp.id3Track = "";
	tmp.id3Year = "";
	cfgFile lcfg(wd);
#ifdef MP3RENAMER
	tmp.saveAs = tmp.file;
#else
	CString path = lcfg.GetPath(OUTPUT);
	if (path.GetLength() != 0){
		tmp.saveAs = path + "\\" + tmp.id3Name.Left(tmp.id3Name.GetLength() - 3) + "mp3";
	}
	else{
		tmp.saveAs = filename.Left(filename.GetLength() - 3) + "mp3";
	}

	CFile f;
	CFileException e;
	TRY{
		f.Open(filename,CFile::modeRead, &e);
		tmp.kbSize = (float)f.GetLength();
		f.Close();
		kbSize += tmp.kbSize;
	}

	CATCH(CFileException, e){
		AfxMessageBox("Error opening file: " + path , MB_OK, 0);
		tmp.kbSize = 0;
	} 
	END_CATCH
#endif
	fileList.Add(tmp);
}

void lameCmdl::removeFile(int Index)
{
	cmdl tmp = fileList.GetAt(Index);
	kbSize -=  tmp.kbSize;
	fileList.RemoveAt(Index ,1);
	fileList.FreeExtra();  //free unused memory
}


cmdl lameCmdl::getLameCmdl(int fileIndex)
{
	cmdl tmp;
	tmp = fileList.GetAt(fileIndex);
	return tmp;
}

int lameCmdl::GetSize()
{
	return fileList.GetSize();
}

void lameCmdl::setId3(cmdl id3, int fileIndex, BOOL id3Filename)
{
	cmdl tmp;
	tmp = fileList.GetAt(fileIndex);
	tmp.id3Album = id3.id3Album;
	tmp.id3Artist = id3.id3Artist;
	tmp.id3Comment = id3.id3Comment;
	tmp.id3Genre = id3.id3Genre;
	tmp.id3Name = id3.id3Name;
	tmp.id3Track = id3.id3Track;
	tmp.id3Year = id3.id3Year;
	if (id3Filename){
		tmp.saveAs = tmp.saveAs.Left(tmp.saveAs.ReverseFind('\\') +1) + tmp.id3Artist + " - " + tmp.id3Name + ".mp3";
	}
	fileList.SetAt(fileIndex, tmp);
}



BOOL lameCmdl::writeM3U(CString filename)
{
	CStdioFile  m3u;
	cmdl tmp;
	int			n = 0;
	CFileException e;
	TRY{
		m3u.Open(filename, CFile::modeCreate|CFile::modeWrite|CFile::typeText,&e);
		while (n < fileList.GetSize()){
			tmp = fileList.GetAt(n++);
			m3u.WriteString(tmp.saveAs +"\n");
		}
		m3u.Flush();
		m3u.Close();
	}
	CATCH(CFileException, e){
		return FALSE;
	}END_CATCH

	
	return TRUE;
}

void lameCmdl::ResetContent()
{
	fileList.RemoveAll();
	kbSize = 0;
}

void lameCmdl::addCmdl(cmdl cmd)
{
	fileList.Add(cmd);
	kbSize += cmd.kbSize;
}
