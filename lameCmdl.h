// lameCmdl.h: Schnittstelle für die Klasse lameCmdl.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAMECMDL_H__EDA110C0_BFBC_40EC_ACC8_D94001DC4AEE__INCLUDED_)
#define AFX_LAMECMDL_H__EDA110C0_BFBC_40EC_ACC8_D94001DC4AEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxtempl.h" 

struct cmdl{
	char	file[256];
	CString fileShort;
	CString id3Name;    
	CString id3Artist;  
	CString id3Album;	
	CString id3Genre;	
	CString id3Year;	
	CString id3Track;	
	CString id3Comment; 
	CString saveAs;
	float	kbSize;
};



class lameCmdl  
{
public:
	void addCmdl(cmdl cmd);
	void ResetContent();
	float kbSize;
	BOOL writeM3U(CString filename);

	void setId3(cmdl id3, int fileIndex, BOOL id3Filename);
	int GetSize();
	cmdl getLameCmdl(int fileIndex);
	void removeFile(int Index);
	void addFile(CString filename, CString wd);
	lameCmdl();
	virtual ~lameCmdl();

protected:
	CArray<cmdl,cmdl&> fileList;

};

#endif // !defined(AFX_LAMECMDL_H__EDA110C0_BFBC_40EC_ACC8_D94001DC4AEE__INCLUDED_)
