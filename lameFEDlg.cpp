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
#include "resource.h"
#include "lameFE.h"
#include "lameFEDlg.h"
#include "ID3TagEditor.h"
#include "CDRip/CDRip.h"
#include "dsiPlayer/dsiPlayer.h"
#include "SettingsSheet.h"
#include "cfgFile.h"
#include "MP3File.h"
#include "CDdbQueryDlg.h"
#include "mfccddb.h"

#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"


#define SCROLLAMOUNT				-1
#define DISPLAY_SLOW				70
#define DISPLAY_MEDIUM				40
#define DISPLAY_FAST				10
#define DISPLAY_SPEED				DISPLAY_MEDIUM

#define RED						RGB(255,0,0)
#define GREEN					RGB(0,255,0)
#define WHITE   				RGB(255,255,255)
#define YELLOW  				RGB(255,255,0)
#define BLACK       			RGB(0,0,0)

#define BACKGROUND_COLOR        BLACK
#define TOP_LEVEL_TITLE_COLOR	RED
#define TOP_LEVEL_GROUP_COLOR   YELLOW
#define GROUP_TITLE_COLOR       GREEN
#define NORMAL_TEXT_COLOR		WHITE

#define TOP_LEVEL_TITLE_HEIGHT	21		
#define TOP_LEVEL_GROUP_HEIGHT  19     
#define GROUP_TITLE_HEIGHT    	17     
#define	NORMAL_TEXT_HEIGHT		15

#define TOP_LEVEL_TITLE			'\n'
#define TOP_LEVEL_GROUP         '\r'
#define GROUP_TITLE           	'\t'
#define NORMAL_TEXT				'\f' 
#define DISPLAY_BITMAP			'\b'

#define		ARRAYCOUNT		42
char *pArrCredit[] = { "LAMEFE\b",
						"",
						 "lameFE 2.0 \n",
						 "",
						 "Copyright (c) 2002 \f",
						 "Thees Christian Winkler \f",
						 "All Rights Reserved \f",
						 "",
						 "",
						 "Project Lead \t",
						 "",
						 "Thees Christian Winkler \f",
						 "",
						 "Engineering \t",
						 "",
						 "Thees Ch. Winkler , Albert L. Faber ,\f",
						 "Mark Findlay , P.J. Naughter \f", 
						 "",
						 "",
						 "Documentation \t",
						 "",
						 "Thees Ch. Winkler \f",
						 "",
						 "",
						 "Beta Testing \t",
						 "",
						 "Jann Peterecky , Ken Hughes , Jan Ch. Winkler \f",
						 "",
						 "",
						 "Provided by \t",
						 "",
						 "Sourceforge.net \f",
						 "http://www.sourceforge.net \f",
						 "http://lamefe.sourceforge.net \f",
						 "",
						 "",
						 "Distributed under the Terms of the GNU \f",
						 "General Public License version 2 or later \f",
						 "",
						 "* * * * * * * * * \t",
						 "",
						 ""
};

///
class CAboutDlg : public CDialog
{
public:

	
	CAboutDlg(CString wd);

	#define     DISPLAY_TIMER_ID		150		// timer id

	RECT        m_ScrollRect,r;		   // rect of Static Text frame
	int         nArrIndex,nCounter;		   // work ints
	CString     m_szWork;			   // holds display line
	BOOL        m_bFirstTime;
	BOOL        m_bDrawText;
	int         nClip;
	int         nCurrentFontHeight;

	CWnd*       m_pDisplayFrame;

	CBitmap     m_bmpWork;                  // bitmap holder
	CBitmap* 	pBmpOld;                    // other bitmap work members
	CBitmap*    m_bmpCurrent;
	HBITMAP 	m_hBmpOld;

	CSize 		m_size;                     // drawing helpers
	CPoint 		m_pt;
	BITMAP 		m_bmpInfo;
	CDC 		m_dcMem;
	BOOL 		m_bProcessingBitmap;

// Dialogfelddaten
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual void OnOK();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg(CString wd) : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

void CAboutDlg::OnOK() 
{
	KillTimer(DISPLAY_TIMER_ID);
	CDialog::OnOK();
}

//************************************************************************
//	 InitDialog
//
//	 	Setup the display rect and start the timer.
//************************************************************************
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	BOOL bRet;
    	UINT nRet;
	
	nCurrentFontHeight = NORMAL_TEXT_HEIGHT;
	
	CClientDC dc(this);
	bRet = m_dcMem.CreateCompatibleDC(&dc);
	
	
	m_bProcessingBitmap=FALSE;
	
	nArrIndex=0;
	nCounter=1;
	nClip=0;
	m_bFirstTime=TRUE;
	m_bDrawText=FALSE;
	m_hBmpOld = 0;
	
	m_pDisplayFrame=(CWnd*)GetDlgItem(IDC_DISPLAY_STATIC);	

	// If you assert here, you did not assign your static display control
	// the IDC_ value that was used in the GetDlgItem(...). This is the
    // control that will display the credits.
	_ASSERTE(m_pDisplayFrame);
				 
	m_pDisplayFrame->GetClientRect(&m_ScrollRect);


	nRet = SetTimer(DISPLAY_TIMER_ID,DISPLAY_SPEED,NULL);
    _ASSERTE(nRet != 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//************************************************************************
//	 OnTimer
//
//	 	On each of the display timers, scroll the window 1 unit. Each 20
//      units, fetch the next array element and load into work string. Call
//      Invalidate and UpdateWindow to invoke the OnPaint which will paint 
//      the contents of the newly updated work string.
//************************************************************************
void CAboutDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent != DISPLAY_TIMER_ID)
		{
		CDialog::OnTimer(nIDEvent);
		return;
		}

	if (!m_bProcessingBitmap)
	if (nCounter++ % nCurrentFontHeight == 0)	 // every x timer events, show new line
		{
		nCounter=1;
		m_szWork = pArrCredit[nArrIndex++];
		
		if (nArrIndex > ARRAYCOUNT-1)
			nArrIndex=0;
		nClip = 0;
		m_bDrawText=TRUE;
		}
	
	m_pDisplayFrame->ScrollWindow(0,SCROLLAMOUNT,&m_ScrollRect,&m_ScrollRect);
	nClip = nClip + abs(SCROLLAMOUNT);	
	
    CRect r;
    CWnd* pWnd = GetDlgItem(IDC_DISPLAY_STATIC);
    ASSERT_VALID(pWnd);
    pWnd->GetClientRect(&r);
    pWnd->ClientToScreen(r);
    ScreenToClient(&r);
    InvalidateRect(r,FALSE); // FALSE does not erase background

	CDialog::OnTimer(nIDEvent);
}


//************************************************************************
//	 OnPaint
//
//	 	Send the newly updated work string to the display rect.
//************************************************************************
void CAboutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	PAINTSTRUCT ps;
	CDC* pDc = m_pDisplayFrame->BeginPaint(&ps);
	
	pDc->SetBkMode(TRANSPARENT);


	//*********************************************************************
	//	FONT SELECTION
    	CFont m_fntArial;
	CFont* pOldFont;
	BOOL bSuccess;
	
	BOOL bUnderline;
	BOOL bItalic;


	if (!m_szWork.IsEmpty())
	switch (m_szWork[m_szWork.GetLength()-1] )
		{
		case NORMAL_TEXT:
		default:
			bItalic = FALSE;
			bUnderline = FALSE;
			nCurrentFontHeight = NORMAL_TEXT_HEIGHT;
   			bSuccess = m_fntArial.CreateFont(NORMAL_TEXT_HEIGHT, 0, 0, 0, 
   								FW_THIN, bItalic, bUnderline, 0, 
   								ANSI_CHARSET,
                               	OUT_DEFAULT_PRECIS,
                               	CLIP_DEFAULT_PRECIS,
                               	PROOF_QUALITY,
                               	VARIABLE_PITCH | 0x04 | FF_DONTCARE,
                               	(LPSTR)"Arial");
			pDc->SetTextColor(NORMAL_TEXT_COLOR);
			pOldFont  = pDc->SelectObject(&m_fntArial);
			break;

		case TOP_LEVEL_GROUP:
			bItalic = FALSE;
			bUnderline = FALSE;
			nCurrentFontHeight = TOP_LEVEL_GROUP_HEIGHT;
   			bSuccess = m_fntArial.CreateFont(TOP_LEVEL_GROUP_HEIGHT, 0, 0, 0, 
   								FW_BOLD, bItalic, bUnderline, 0, 
   								ANSI_CHARSET,
                               	OUT_DEFAULT_PRECIS,
                               	CLIP_DEFAULT_PRECIS,
                               	PROOF_QUALITY,
                               	VARIABLE_PITCH | 0x04 | FF_DONTCARE,
                               	(LPSTR)"Arial");
			pDc->SetTextColor(TOP_LEVEL_GROUP_COLOR);
			pOldFont  = pDc->SelectObject(&m_fntArial);
			break;
		
		
		
		case GROUP_TITLE:
			bItalic = FALSE;
			bUnderline = FALSE;
			nCurrentFontHeight = GROUP_TITLE_HEIGHT;
   			bSuccess = m_fntArial.CreateFont(GROUP_TITLE_HEIGHT, 0, 0, 0, 
   								FW_BOLD, bItalic, bUnderline, 0, 
   								ANSI_CHARSET,
                               	OUT_DEFAULT_PRECIS,
                               	CLIP_DEFAULT_PRECIS,
                               	PROOF_QUALITY,
                               	VARIABLE_PITCH | 0x04 | FF_DONTCARE,
                               	(LPSTR)"Arial");
			pDc->SetTextColor(GROUP_TITLE_COLOR);
			pOldFont  = pDc->SelectObject(&m_fntArial);
			break;
		
		
		case TOP_LEVEL_TITLE:
			bItalic = FALSE;
			bUnderline = TRUE;
			nCurrentFontHeight = TOP_LEVEL_TITLE_HEIGHT;
			bSuccess = m_fntArial.CreateFont(TOP_LEVEL_TITLE_HEIGHT, 0, 0, 0, 
								FW_BOLD, bItalic, bUnderline, 0, 
								ANSI_CHARSET,
	                           	OUT_DEFAULT_PRECIS,
	                           	CLIP_DEFAULT_PRECIS,
	                           	PROOF_QUALITY,
	                           	VARIABLE_PITCH | 0x04 | FF_DONTCARE,
	                           	(LPSTR)"Arial");
			pDc->SetTextColor(TOP_LEVEL_TITLE_COLOR);
			pOldFont  = pDc->SelectObject(&m_fntArial);
			break;
		
		case DISPLAY_BITMAP:
			if (!m_bProcessingBitmap)
				{
				CString szBitmap = m_szWork.Left(m_szWork.GetLength()-1);
	   			if (!m_bmpWork.LoadBitmap((const char *)szBitmap))
					{
					CString str; 
					str.Format("Could not find bitmap resource \"%s\". "
                               "Be sure to assign the bitmap a QUOTED resource name", szBitmap); 
					KillTimer(DISPLAY_TIMER_ID); 
					MessageBox(str); 
					return; 
					}
				m_bmpCurrent = &m_bmpWork;
	   			m_bmpCurrent->GetObject(sizeof(BITMAP), &m_bmpInfo);
			
				m_size.cx = m_bmpInfo.bmWidth;	// width  of dest rect
				RECT workRect;
				m_pDisplayFrame->GetClientRect(&workRect);
				m_pDisplayFrame->ClientToScreen(&workRect);
				ScreenToClient(&workRect);
				// upper left point of dest
				m_pt.x = (workRect.right - 
							((workRect.right-workRect.left)/2) - (m_bmpInfo.bmWidth/2));
				m_pt.y = workRect.bottom;
				
				
				pBmpOld = m_dcMem.SelectObject(m_bmpCurrent);
				if (m_hBmpOld == 0)
					m_hBmpOld = (HBITMAP) pBmpOld->GetSafeHandle();
				m_bProcessingBitmap = TRUE;
				}
			break;

		}
	
	
	
	
	CBrush bBrush(BLACK);
	CBrush* pOldBrush;
	pOldBrush  = pDc->SelectObject(&bBrush);
	// Only fill rect comprised of gap left by bottom of scrolling window
	r=m_ScrollRect;
	r.top = r.bottom-abs(SCROLLAMOUNT); 
	pDc->DPtoLP(&r);
	
	if (m_bFirstTime)
		{
		m_bFirstTime=FALSE;
		pDc->FillRect(&m_ScrollRect,&bBrush);
		}
	else
		pDc->FillRect(&r,&bBrush);
	
	r=m_ScrollRect;
	r.top = r.bottom-nClip;
	
	
	if (!m_bProcessingBitmap)
		{
		int x = pDc->DrawText((const char *)m_szWork,m_szWork.GetLength()-1,&r,DT_TOP|DT_CENTER|
					DT_NOPREFIX | DT_SINGLELINE);	
		m_bDrawText=FALSE;
		}
	else
		{
    	dc.StretchBlt( m_pt.x, m_pt.y-nClip, m_size.cx, nClip, 
                   		&m_dcMem, 0, 0, m_bmpInfo.bmWidth-1, nClip,
                   		SRCCOPY );
		if (nClip > m_bmpInfo.bmHeight)
			{
			m_bmpWork.DeleteObject();
			m_bProcessingBitmap = FALSE;
			nClip=0;
			m_szWork.Empty();
			nCounter=1;
			}
		pDc->SelectObject(pOldBrush);
		bBrush.DeleteObject();
		m_pDisplayFrame->EndPaint(&ps);
		return;
		}
	
	
	pDc->SelectObject(pOldBrush);
	bBrush.DeleteObject();
	
	if (!m_szWork.IsEmpty())
		{
		pDc->SelectObject(pOldFont);
		m_fntArial.DeleteObject();
		}

	m_pDisplayFrame->EndPaint(&ps);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CAboutDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
    m_dcMem.SelectObject(CBitmap::FromHandle(m_hBmpOld));
    m_bmpWork.DeleteObject();
	
}


/////////////////////////////////////////////////////////////////////////////
// CLameFEDlg Dialogfeld

CLameFEDlg::CLameFEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLameFEDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLameFEDlg)
	m_strList = _T("");
	//}}AFX_DATA_INIT
	// Beachten Sie, dass LoadIcon unter Win32 keinen nachfolgenden DestroyIcon-Aufruf benötigt

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	p = NULL;

}

CLameFEDlg::~CLameFEDlg(){

}

void CLameFEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLameFEDlg)
	DDX_Control(pDX, IDC_LIST_TITLE, m_listTitle);
	DDX_Control(pDX, IDC_PRESETS, m_Settings);
	DDX_Control(pDX, IDC_ID3_TAG, m_id3Tag);
	DDX_Control(pDX, IDC_ENCODE, m_Encode);
	DDX_Control(pDX, IDC_CDDB, m_CDdb);
	DDX_Control(pDX, IDC_EJECT_CD, c_eject);
	DDX_Control(pDX, IDC_REFRESH, c_refresh);
	DDX_Control(pDX, IDC_WAV_MP3, c_wav2mp3);
	DDX_Control(pDX, IDC_CD_WAV, c_cd2wave);
	DDX_Control(pDX, IDC_CD_MP3, c_cd2mp3);
	DDX_Control(pDX, IDC_NEXT, m_next);
	DDX_Control(pDX, IDC_PLAY, m_play);
	DDX_Control(pDX, IDC_STOP, m_stop);
	DDX_Control(pDX, IDC_PREV, m_prev);
	DDX_Control(pDX, IDC_PAUSE, m_pause);
	DDX_Control(pDX, IDC_LIST, m_ctrlList);
	DDX_LBString(pDX, IDC_LIST, m_strList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLameFEDlg, CDialog)
	//{{AFX_MSG_MAP(CLameFEDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_PRESET, OnNewPreset)
	ON_COMMAND(ID_ADD_FILE, OnAddFile)
	ON_BN_CLICKED(IDC_ENCODE, OnEncode)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_REMOVE_FILE, OnRemoveFile)
	ON_COMMAND(ID_SETTINGS, OnSettings)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_TOOLS_EDITID3TAG, OnToolsEditid3tag)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_CD_MP3, OnCdMp3)
	ON_BN_CLICKED(IDC_CD_WAV, OnCdWav)
	ON_BN_CLICKED(IDC_WAV_MP3, OnWavMp3)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_COMMAND(ICD_CDDB_SETTINGS, OnCddbSettings)
	ON_BN_CLICKED(IDC_EJECT_CD, OnEjectCd)
	ON_COMMAND(ID_PREFERENCES_CDREADERSETTINGS, OnPreferencesCdreadersettings)
	ON_COMMAND(ID_PREFERENCES_TAGWRITERSETTINGS, OnPreferencesTagwritersettings)
	ON_COMMAND(ID_CDDB_READFROMREMOTECDDBSERVER, OnCddbReadfromremotecddbserver)
	ON_COMMAND(ID_EDIT_ID3, OnEditId3)
	ON_BN_CLICKED(IDC_CDDB, OnCddb)
	ON_COMMAND(ID_PREFERENCES_PLUGINS, OnPreferencesPlugins)
	ON_COMMAND(ID_REMOVE, OnRemoveFile)
	ON_COMMAND(ID_ADD, OnAddFile)
	ON_BN_CLICKED(IDC_ID3_TAG, OnToolsEditid3tag)
	ON_BN_CLICKED(IDC_PRESETS, OnNewPreset)
	ON_LBN_DBLCLK(IDC_LIST, OnEditId3)
	ON_COMMAND(ID_HELP_LICENSE, OnHelpLicense)
	ON_COMMAND(ID_HELP_HELPINDEX, OnHelpHelpindex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLameFEDlg Nachrichten-Handler

BOOL CLameFEDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	files = new CPtrArray();
	isEjected = FALSE;

	char buffer[1024];
	_getcwd(buffer,1024);
	wd = buffer;

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{	
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden
	

#ifdef _DEBUG
	SetWindowText("lameFE 2.0 (Debug Compilation | Working dir is \"" + wd + "\")");
#endif
	
	CR_Init(wd + "\\lameFE.ini");

	//Set up Button icons
	int ICON = IDI_STOP;
		HANDLE hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		14,
		14,
		LR_DEFAULTCOLOR
	);
	m_stop.SetIcon(static_cast<HICON>(hIcon));
	
	
    ICON  = IDI_EJECT;
	hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		14,
		14,
		LR_DEFAULTCOLOR
	);
	c_eject.SetIcon(static_cast<HICON>(hIcon));
	
    ICON  = IDI_PREV;
	hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		14,
		14,
		LR_DEFAULTCOLOR
	);
	m_prev.SetIcon(static_cast<HICON>(hIcon));

    ICON  = IDI_PLAY;
	hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		14,
		14,
		LR_DEFAULTCOLOR
	);

	m_play.SetIcon(static_cast<HICON>(hIcon));

    ICON  = IDI_PAUSE;
	hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		14,
		14,
		LR_DEFAULTCOLOR
	);
	m_pause.SetIcon(static_cast<HICON>(hIcon));
	
	
    ICON  = IDI_NEXT;
	hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		14,
		14,
		LR_DEFAULTCOLOR
	);
	m_next.SetIcon(static_cast<HICON>(hIcon));

/*    ICON  = IDI_CD_WAV;
	hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		32,
		32,
		LR_DEFAULTCOLOR
	);
	c_cd2wave.SetIcon(static_cast<HICON>(hIcon));

    ICON  = IDI_CD_MP3;
	hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		32,
		32,
		LR_DEFAULTCOLOR
	);
	c_cd2mp3.SetIcon(static_cast<HICON>(hIcon));

    ICON  = IDI_WAV_MP3;
	hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		32,
		32,
		LR_DEFAULTCOLOR
	);
	c_wav2mp3.SetIcon(static_cast<HICON>(hIcon));

    ICON  = IDI_START;

	hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		32,
		32,
		LR_DEFAULTCOLOR
	);
	m_Encode.SetIcon(static_cast<HICON>(hIcon));

    ICON  = IDI_ID3;
	hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		32,
		32,
		LR_DEFAULTCOLOR
	);
	m_id3Tag.SetIcon(static_cast<HICON>(hIcon));*/

/*    ICON  = IDI_CDDB;
	hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		32,
		32,
		LR_DEFAULTCOLOR
	);
	m_CDdb.SetIcon(static_cast<HICON>(hIcon));
 		
   ICON  = IDI_ENCODER;
	hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		32,
		32,
		LR_DEFAULTCOLOR
	);
	m_Settings.SetIcon(static_cast<HICON>(hIcon));
	
    ICON  = IDI_REFRESH;
	hIcon = LoadImage(
		AfxGetInstanceHandle(),
		reinterpret_cast<LPCSTR>(ICON),
		IMAGE_ICON,
		48,
		16,
		LR_DEFAULTCOLOR
	);
	c_refresh.SetIcon(static_cast<HICON>(hIcon));
	*/

	UpdateData(FALSE);

	c_cd2mp3.SetCheck(TRUE);

	OnCdMp3(); //Default is extract audio cd to mp3

	return TRUE;  
}

void CLameFEDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout(wd);
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CLameFEDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CLameFEDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLameFEDlg::OnNewPreset() 
{

	OnPreferencesTagwritersettings();
}


void CLameFEDlg::OnAddFile() 
{

	if(!c_wav2mp3.GetCheck()){
		
		CString strMsg;
		strMsg.LoadString(IDS_MAIN_ADDFILECD);
		AfxMessageBox(strMsg, MB_OK+MB_ICONEXCLAMATION);
		return;
	}

	cfgFile lcfg(wd);
	
	static char BASED_CODE szFilter[] = "Wave Audio (*.wav)|*.wav|All files (*.*)|*.*|";
	
	CFileDialog FileOpen(TRUE, "*.wav", 0, OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT  , szFilter, 0);
	
	FileOpen.m_ofn.lpstrInitialDir = (const char*)lcfg.GetStringValue(OUTPUT);
	CString strFileName;
	int maxChar = 1000;
	FileOpen.m_ofn.lpstrFile = strFileName.GetBuffer(maxChar);
	FileOpen.m_ofn.nMaxFile = maxChar;

	int nResponse = FileOpen.DoModal();
	strFileName.ReleaseBuffer();
	if (nResponse == IDOK){
	
		POSITION pos = FileOpen.GetStartPosition();
		CString path;
		
		while (pos != NULL){
		
			path = FileOpen.GetNextPathName(pos);
			m_ctrlList.AddString(path);
			
			files->Add(new MP3File(path, wd));
		}
	}

	UpdateData(FALSE);
		
}

void CLameFEDlg::OnRemoveFile() 
{

	if(!c_wav2mp3.GetCheck()){

		CString strMsg;
		strMsg.LoadString(IDS_MAIN_ADDFILECD);
		AfxMessageBox(strMsg, MB_OK+MB_ICONEXCLAMATION);
		return;
	}
	
	if (m_ctrlList.GetCount != 0 ){
	
		UpdateData(TRUE);
		int iCnt,i;
		iCnt = m_ctrlList.GetSelCount();
		
		if (iCnt != 0){
		
			int *pIndex;
			pIndex = new int[iCnt];
			m_ctrlList.GetSelItems(iCnt, pIndex);
			for(i = iCnt - 1; i >= 0; i--){

				m_ctrlList.DeleteString(pIndex[i]);
				files->RemoveAt(pIndex[i], 1);
			}
			
			delete pIndex;
		}
		else{
			
			CString strMsg;
			strMsg.LoadString(IDS_EMPTYLIST);
			AfxMessageBox(strMsg, MB_OK+MB_ICONINFORMATION,0);
		}
	}
	else{
		
		CString strMsg;
		strMsg.LoadString(IDS_EMPTYLIST);
		AfxMessageBox(strMsg,MB_OK+MB_ICONINFORMATION,0);
	}
}



/////////////////////////////////////////////////////////////////
//////// Menu

void CLameFEDlg::OnExit() 
{

	if(p){
		
		p->stop();
		p->deInit();
	}
	
	ResetFileList();

	delete p;
	delete files;

	CR_LockCD(FALSE); //Just to be sure :)

	CDialog::OnCancel();	
}




void CLameFEDlg::OnAbout() 
{
	CAboutDlg dlg(wd);
	dlg.DoModal();
}

void CLameFEDlg::OnHelpLicense() 
{

	WinExec("notepad.exe " + wd + "\\license.txt", SW_SHOW);
}

void CLameFEDlg::OnHelpHelpindex() 
{

	HtmlHelp(
		0,
        "lameFE.chm",
        HH_DISPLAY_INDEX,
        NULL) ;
}

void CLameFEDlg::OnHelp() 
{

	//CDialog::OnHelp();	
	HtmlHelp(
		0,
        "lameFE.chm",
        HH_DISPLAY_TOC,
        NULL) ;

}


void CLameFEDlg::OnToolsEditid3tag() 
{

	if(!files->GetSize()){

		CString strMsg;
		strMsg.LoadString(IDS_EMPTYLIST);
		AfxMessageBox(strMsg, MB_OK+MB_ICONEXCLAMATION);
		return;
	}

	//save current selection
	int iCnt = m_ctrlList.GetSelCount();
	int *pIndex = new int[iCnt];
	m_ctrlList.GetSelItems(iCnt, pIndex);

	CID3TagEditor id3Dlg;
	id3Dlg.ParseFileInformation(files, m_ctrlList.GetCurSel());
	id3Dlg.DoModal();
	
	if(!c_wav2mp3.GetCheck()){

		m_ctrlList.ResetContent();
		CString tmp;

		for(int i = 0; i < files->GetSize(); i++){
			
			tmp.Format("[%2d] \t", i + 1);
			tmp += ((MP3File*)files->GetAt(i))->getID3Info()->getArtist() + " - " + ((MP3File*)files->GetAt(i))->getID3Info()->getSong();
			m_ctrlList.AddString(tmp);
		}
	}

	//restore selection
	SelItems(iCnt, pIndex);
	delete pIndex;
}


BOOL CLameFEDlg::OnEncode() 
{
	
	if(p){
		
		p->stop();
	}
	
	int nResult = IDCANCEL;

	if(c_wav2mp3.GetCheck()){

		if(files->GetSize() == 0){

			CString strMsg;
			strMsg.LoadString(IDS_EMPTYLIST);
			AfxMessageBox(strMsg, MB_OK+MB_ICONSTOP);
			return FALSE;
		}

		ShowWindow(SW_HIDE);

		esdlg = new EncodingStatusDlg();

		AfxGetApp()->m_pActiveWnd = esdlg;
		
		esdlg->setJob(WAVE_TO_MP3);
		esdlg->parseFiles(files, wd, 0);
		nResult =esdlg->DoModal();

		AfxGetApp()->m_pActiveWnd = this;
		
		ShowWindow(SW_SHOW);
		
		m_ctrlList.ResetContent();
		ResetFileList();

		delete esdlg;
		esdlg = NULL;
	}

	else{

		if (!m_ctrlList.GetCount()){

			CString strMsg;
			strMsg.LoadString(IDS_MAIN_NOAUDIOCD);
			AfxMessageBox(strMsg, MB_OK+MB_ICONSTOP);
			return FALSE;
		}


	
		UpdateData(TRUE);
		int iCnt,i;
		iCnt = m_ctrlList.GetSelCount();
		if(!iCnt){
			
			CString strMsg;
			strMsg.LoadString(IDS_NOTRACKSSELECTED);
			AfxMessageBox(strMsg, MB_OK+MB_ICONINFORMATION);
			return FALSE;
		}

		int *pIndex;
		pIndex = new int[iCnt];
		m_ctrlList.GetSelItems(iCnt, pIndex);
		SortArray(pIndex, iCnt);

		CPtrArray list;

		for(i = 0; i < iCnt; i++){
			
			if(CR_GetTocEntry(pIndex[i]).btFlag == 16){
				
				list.Add((MP3File*)files->GetAt(pIndex[i]));
			}
			else{
				
				CString a, b;
				b.LoadString(IDS_DATATRACK);
				a.Format(b, CR_GetTocEntry(pIndex[i]).btTrackNumber);
				AfxMessageBox(a,  MB_OK+MB_ICONINFORMATION);
			}
		}
		delete pIndex;
		if(!list.GetSize()){
			
			CString strMsg;
			strMsg.LoadString(IDS_MAIN_NOAUDIOCD);
			AfxMessageBox(strMsg,  MB_OK+MB_ICONEXCLAMATION);
			return FALSE;
		}

		ShowWindow(SW_HIDE);

		esdlg = new EncodingStatusDlg();
		
		AfxGetApp()->m_pActiveWnd = esdlg;

		esdlg->setJob((c_cd2mp3.GetCheck() ? RIP_TO_MP3 : RIP_TO_WAVE));
		esdlg->parseFiles(&list, wd, 0);
		nResult =esdlg->DoModal();	
		
		AfxGetApp()->m_pActiveWnd = this;
		
		ShowWindow(SW_SHOW);
		
		m_ctrlList.ResetContent();
		ResetFileList();
		delete esdlg;
		esdlg = NULL;

	}

	if(nResult == IDOK){

		cfgFile cfg(wd);
		
		if(cfg.GetValue(QUIT, FALSE)){

			OnExit();
		}

		if(cfg.GetValue(SHUTDOWN, FALSE)){

			//ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, NULL);
			InitiateSystemShutdown(NULL, "System shutdown",10,FALSE,FALSE);
		}
	}

	return TRUE;
}




void CLameFEDlg::OnPlay() 
{

	p->setPlaylist(files);
	p->play();
}

void CLameFEDlg::OnNext() 
{

	m_bPaused = TRUE;
	m_play.EnableWindow(m_bPaused);
	p->next();
}

void CLameFEDlg::OnPause() 
{

	m_bPaused = !m_bPaused;
	m_play.EnableWindow(m_bPaused);
	p->pause();
}

void CLameFEDlg::OnPrev() 
{

	m_bPaused = TRUE;
	m_play.EnableWindow(m_bPaused);
	p->prev();	
}

void CLameFEDlg::OnStop() 
{

	m_bPaused = TRUE;
	m_play.EnableWindow(m_bPaused);
	p->stop();
}

void CLameFEDlg::OnCdMp3() 
{
	
	if(p){

		p->stop();
		delete p;
		p = NULL;
	}

	p = new InternalPlayer(PLAYER_MODE_CD, NULL);

	SetListTitle();
	if(c_cd2mp3.GetCheck()){
		
		c_cd2wave.SetCheck(FALSE);
		c_wav2mp3.SetCheck(FALSE);

		c_refresh.EnableWindow(TRUE);

		m_ctrlList.ResetContent();

		ResetFileList();
		readCDContents();
		SetPlayerStatus(TRUE);
	}
	else{
		c_cd2mp3.SetCheck(TRUE);
	}
}

void CLameFEDlg::OnCdWav() 
{

	if(p){

		p->stop();
		delete p;
		p = NULL;
	}
	p = new InternalPlayer(PLAYER_MODE_CD, NULL);	

	SetListTitle();

	if(c_cd2wave.GetCheck()){

		c_refresh.EnableWindow(TRUE);
		c_cd2mp3.SetCheck(FALSE);
		c_wav2mp3.SetCheck(FALSE);
		
		m_ctrlList.ResetContent();
		ResetFileList();
		files->RemoveAll();
		readCDContents();
		SetPlayerStatus(TRUE);
	}
	else{
		c_cd2wave.SetCheck(TRUE);
	}
}

void CLameFEDlg::OnWavMp3() 
{
	if(p){

		p->stop();
		p->deInit();
		delete p;
		p = NULL;
	}

	//HWND hWnd = GetSafeHwnd();
	//p = new InternalPlayer(PLAYER_MODE_WAVE, hWnd);	
	
	if(c_wav2mp3.GetCheck()){
		
		c_refresh.EnableWindow(FALSE);
		c_cd2wave.SetCheck(FALSE);
		c_cd2mp3.SetCheck(FALSE);

		m_ctrlList.ResetContent();
		files->RemoveAll();
		ResetFileList();
		SetListTitle();
		SetPlayerStatus(FALSE);
	}
	else{
		c_wav2mp3.SetCheck(TRUE);
	}
}


void CLameFEDlg::readCDContents()
{
	
	CWaitCursor wCursor;

	Sleep(400);
	if(p){

		p->stop();
	}
	
	ResetFileList();
	
	CDEX_ERR bErr   = FALSE;
	CString  track;
	if (CR_IsUnitReady() == 0x01)
	{
		
		bErr = CR_LoadSettings();

		if(bErr != CDEX_OK){

			CString strMsg;
			strMsg.LoadString(IDS_DRIVENOTREADY);
			track.Format(strMsg, bErr);
			MessageBox(track, "lameFE 2.0", MB_OK+MB_ICONSTOP);
			return;
		}
		
		bErr = CR_ReadToc();
		
		if(bErr != CDEX_OK){

			CString strMsg;
			strMsg.LoadString(IDS_ERRTOC);
			track.Format(strMsg, bErr);
			MessageBox(track, "lameFE 2.0", MB_OK+MB_ICONSTOP);
			return;
		}
		
		// Get the number of 
		int nTocEntries = CR_GetNumTocEntries();

		for (int i = 0; i < nTocEntries; i++){
			
			if(CR_GetTocEntry(i).btFlag == 16){
				
				track.Format("Audio Track %d", i + 1);
				files->Add(new MP3File(CR_GetTocEntry(i), wd, (c_cd2mp3.GetCheck() ? TRUE : FALSE)));
			}
			else{

				track.LoadString(IDS_MAIN_DATATRACK);
			}

			m_ctrlList.AddString(track);
		}
		
		cfgFile cfg(wd);
		if(cfg.GetValue(SELECT, TRUE)){

			m_ctrlList.SelItemRange(TRUE, 0, m_ctrlList.GetCount() - 1);
		}
		UpdateData(FALSE);
	}
	else{

		m_ctrlList.ResetContent();
	}
}

void CLameFEDlg::OnRefresh() 
{
	
	isEjected = FALSE;
	CR_EjectCD(FALSE);

	m_ctrlList.ResetContent();
	ResetFileList();
	readCDContents();
}

void CLameFEDlg::OnEjectCd() 
{

	if(!c_wav2mp3.GetCheck()){
		CWaitCursor wCursor;
		
		isEjected = !isEjected;

		CR_EjectCD(isEjected);

		if(!isEjected){

			Sleep(400);
		}

		readCDContents();
	}

	else{
		
		OnAddFile();
	}

}

void CLameFEDlg::OnCddbSettings() 
{

	int old = CR_GetActiveCDROM();
	CSettingsSheet settings("lameFE Settings", this, 4);
	settings.init(wd);
	settings.DoModal();
	SetActiveCD(old);
}


void CLameFEDlg::OnPreferencesPlugins() 
{
	int old = CR_GetActiveCDROM();
	CSettingsSheet settings("lameFE Settings", this, 2);
	settings.init(wd);
	settings.DoModal();
	SetActiveCD(old);
	
}

void CLameFEDlg::OnSettings() 
{
	
	int old = CR_GetActiveCDROM();
	CSettingsSheet settings("lameFE Settings", this, 0);
	settings.init(wd);
	settings.DoModal();
	SetActiveCD(old);


}

void CLameFEDlg::OnPreferencesCdreadersettings() 
{
	int old = CR_GetActiveCDROM();
	CSettingsSheet settings("lameFE Settings", this, 1);
	settings.init(wd);
	settings.DoModal();
	SetActiveCD(old);
}

void CLameFEDlg::OnPreferencesTagwritersettings() 
{

	int old = CR_GetActiveCDROM();
	CSettingsSheet settings("lameFE Settings", this, 3);
	settings.init(wd);
	settings.DoModal();
	
}

void CLameFEDlg::ResetFileList()
{
	
	MP3File *tmp;

	for(int i = files->GetSize() - 1; i >= 0; i--){

		tmp = (MP3File*)files->GetAt(i);
		files->RemoveAt(i);
		delete tmp;
	}

	files->FreeExtra();
	files->SetSize(0);
}

void CLameFEDlg::OnCddbReadfromremotecddbserver() 
{

	if(!c_wav2mp3.GetCheck()){
		
		//save current selection
		int iCnt = m_ctrlList.GetSelCount();
		int *pIndex = new int[iCnt];
		m_ctrlList.GetSelItems(iCnt, pIndex);

		if(!files->GetSize()){
			
			CString strMsg;
			strMsg.LoadString(IDS_MAIN_NOAUDIOCD);
			AfxMessageBox(strMsg, MB_OK+MB_ICONSTOP);
			delete pIndex;
			return;
		}
		CWaitCursor wc;

		CCDdbQueryDlg cddbDlg(this, files, CR_GetActiveCDROM(), wd);
		cddbDlg.DoModal();

		m_ctrlList.ResetContent();
		CString tmp;

		for(int i = 0; i < files->GetSize(); i++){
			
			tmp.Format("[%2d] \t", i + 1);
			tmp += ((MP3File*)files->GetAt(i))->getID3Info()->getArtist() + " - " + ((MP3File*)files->GetAt(i))->getID3Info()->getSong();
			m_ctrlList.AddString(tmp);
		}

		//restore selection
		SelItems(iCnt, pIndex);
		delete pIndex;

	}
	else{
		
		CString strMsg;
		strMsg.LoadString(IDS_CDDBONWAVERR);
		AfxMessageBox(strMsg, MB_OK+MB_ICONINFORMATION);
	}
}

void CLameFEDlg::OnCancel() 
{
	
	OnExit();
}

void CLameFEDlg::OnEditId3() 
{

	OnToolsEditid3tag();
}

void CLameFEDlg::OnCddb() 
{

	OnCddbReadfromremotecddbserver();
}

void CLameFEDlg::SortArray(int *pIndex, int size)
{


	int tmp;

	for(int i = 1; i < size; i++) {
	
		for(int j = 1; j < size; j++) {

		 if(pIndex[j - 1] > pIndex[j]) {
			tmp = pIndex[j - 1];
			pIndex[j-pIndex[j]];
			pIndex[j] = tmp;
		 }
	  }
	}
}

void CLameFEDlg::SelItems(int iCnt, int *pIndex)
{

	for(int i = 0; i < iCnt; i++){

		m_ctrlList.SetSel(pIndex[i], TRUE);
	}
}

void CLameFEDlg::SetListTitle()
{

	if(c_cd2wave.GetCheck() || c_cd2mp3.GetCheck()){


		CDROMPARAMS cdParams;
		CR_GetCDROMParameters(&cdParams);
		CString title;
		title.Format(IDS_MAIN_LIST_TITLE, cdParams.lpszCDROMID);
		m_listTitle.SetWindowText(title);
	}
	else if (c_wav2mp3.GetCheck()){

		CString title;
		title.LoadString(IDS_MAIN_LIST_TITLE2);
		m_listTitle.SetWindowText(title);
	}
}

void CLameFEDlg::SetPlayerStatus(BOOL bEnabled)
{
	
	m_next.EnableWindow(bEnabled);
	m_pause.EnableWindow(bEnabled);
	m_play.EnableWindow(bEnabled);
	m_prev.EnableWindow(bEnabled);
	m_stop.EnableWindow(bEnabled);
}

void CLameFEDlg::SetActiveCD(int old)
{

	if (old!= CR_GetActiveCDROM()){

		SetListTitle();
		m_ctrlList.ResetContent();
		readCDContents();
	}
}


