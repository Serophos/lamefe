// stdafx.h : Include-Datei f�r Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die h�ufig benutzt, aber
//      in unregelm��igen Abst�nden ge�ndert werden.
//

#if !defined(AFX_STDAFX_H__D135DEED_5DC8_4A51_841F_A71356F32CBE__INCLUDED_)
#define AFX_STDAFX_H__D135DEED_5DC8_4A51_841F_A71356F32CBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden
#define OEMRESOURCE

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxdisp.h>        // MFC Automatisierungsklassen
#include <afxdtctl.h>		// MFC-Unterst�tzung f�r allgemeine Steuerelemente von Internet Explorer 4
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterst�tzung f�r g�ngige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <ATLCONV.H>
#include <afxtempl.h>
#include <afxmt.h>
#include <htmlhelp.h>
#include <afxsock.h>        // MFC socket support
#include <commctrl.h>
#include <mmsystem.h>

#ifdef _DEBUG
# define STR_VERSION		"LameFE Debug"
# define STR_VERSION_DLG	STR_VERSION
#else
# define STR_VERSION		"LameFE 2.2rc1"
# define STR_VERSION_DLG	"LameFE 2.2 Release Candidate 1"
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // !defined(AFX_STDAFX_H__D135DEED_5DC8_4A51_841F_A71356F32CBE__INCLUDED_)
