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
#include "lameFE.h"
#include "MyFileDialog.h"
#include <dlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFileDialog

IMPLEMENT_DYNAMIC(CMyFileDialog, CFileDialog)

CMyFileDialog::CMyFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(CMyFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CMyFileDialog)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMyFileDialog::OnInitDialog()
{

	const UINT iExtraSize = 100;
    // Number of controls in the File Dialog
    const UINT nControls = 8;    
    
	CWnd *wndDlg = GetParent();

    RECT Rect;
    wndDlg->GetWindowRect(&Rect);
    // Change the size
    wndDlg->SetWindowPos(NULL, 0, 0, 
                        Rect.right - Rect.left + iExtraSize, 
                        Rect.bottom - Rect.top + iExtraSize, 
                        SWP_NOMOVE);

   // Control ID's - defined in <dlgs.h>
    UINT Controls[nControls] = {stc3, stc2, // The two label controls
                                edt1, cmb1, // The edit control and the drop-down box
                                IDOK, IDCANCEL, 
                                lst1, chx1}; // The Explorer window


    // Go through each of the controls in the dialog box, and move them to a new position
    for (int i=0 ; i<nControls ; i++)
    {
        CWnd *wndCtrl = wndDlg->GetDlgItem(Controls[i]);
        wndCtrl->GetWindowRect(&Rect);
        wndDlg->ScreenToClient(&Rect); // Remember it is child controls

        // Move all the controls according to the new size of the dialog.
        if ((Controls[i] == IDOK) || (Controls[i] == IDCANCEL)){
            wndCtrl->SetWindowPos(NULL, 
                            Rect.left + iExtraSize, Rect.top + iExtraSize,
                            0, 0, SWP_NOSIZE);
        }
		else if((Controls[i] == stc3)  || (Controls[i] == stc2)  || (Controls[i] == chx1)){ //Move the labels
            wndCtrl->SetWindowPos(NULL, 
                            Rect.left, Rect.top + iExtraSize,
                            0, 0, SWP_NOSIZE);
		}
		else if ((Controls[i] == edt1) || (Controls[i] == cmb1)){ //Move and resize the edit control & drop-down
            wndCtrl->SetWindowPos(NULL, 
								Rect.left, 
								Rect.top + iExtraSize,
                                Rect.right - Rect.left + iExtraSize, 
                                Rect.bottom - Rect.top, 
                                0);
		}
		else{ // This is the explorer like window. It should be sized - not moved.
            wndCtrl->SetWindowPos(NULL, 0, 0,
                                Rect.right - Rect.left + iExtraSize, 
                                Rect.bottom - Rect.top + iExtraSize, 
                                SWP_NOMOVE);
		}
    }

	return CFileDialog::OnInitDialog();
}
