// TimeStatus.h: Schnittstelle für die Klasse CTimeStatus.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMESTATUS_H__0FEB5732_F4C3_4DC3_B24F_C3C018AB4919__INCLUDED_)
#define AFX_TIMESTATUS_H__0FEB5732_F4C3_4DC3_B24F_C3C018AB4919__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTimeStatus  
{
public:
	CTimeStatus();
	virtual ~CTimeStatus();

	DWORD	m_dwStartTime;
	DWORD	m_dwElapsed;
	DWORD	m_dwEstimated;
	DWORD	m_dwRemaining;
	int		m_nPrevPercent;

	void ReInit();
	CString GetElapsedString();
	CString GetEstimateString();
	CString GetRemainingString();
	void	Calculate(int nPercent);

};

#endif // !defined(AFX_TIMESTATUS_H__0FEB5732_F4C3_4DC3_B24F_C3C018AB4919__INCLUDED_)
