#if !defined(AFX_LAMEFESPLASH_H__B5365431_D895_445C_9AA0_55B5754BF136__INCLUDED_)
#define AFX_LAMEFESPLASH_H__B5365431_D895_445C_9AA0_55B5754BF136__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LameFESplash.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CLameFESplash 

class CLameFESplash : public CWnd
{
// Konstruktion
public:
	CLameFESplash(UINT nBitmapID, UINT nDuration = 2500);

// Attribute
public:
    BOOL Create();


// Operationen
public:

// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CLameFESplash)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CLameFESplash();

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	//{{AFX_MSG(CLameFESplash)
		// HINWEIS - Der Klassen-Assistent f�gt hier Member-Funktionen ein und entfernt diese.
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL GetBitmapAndPalette(UINT nIDResource, CBitmap &bitmap, CPalette &pal);

protected:
	UINT m_nBitmapID;
	UINT m_nDuration;
	UINT m_nTimerID;
	CBitmap m_bitmap;
	CPalette m_pal;
	CWnd m_wndInvisible;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_LAMEFESPLASH_H__B5365431_D895_445C_9AA0_55B5754BF136__INCLUDED_
