// tbhide.h : main header file for the TBHIDE application
//

#if !defined(AFX_TBHIDE_H__435A1759_127F_11D4_9830_00105AA5E657__INCLUDED_)
#define AFX_TBHIDE_H__435A1759_127F_11D4_9830_00105AA5E657__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTBHideApp:
// See tbhide.cpp for the implementation of this class
//

class CTBHideApp : public CWinApp
{
public:
	CTBHideApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTBHideApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTBHideApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TBHIDE_H__435A1759_127F_11D4_9830_00105AA5E657__INCLUDED_)
