// TimeStatus.cpp: Implementierung der Klasse CTimeStatus.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdafx.h"
#include "TimeStatus.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CTimeStatus::CTimeStatus()
{

	ReInit();
}

CTimeStatus::~CTimeStatus()
{

}

void CTimeStatus::ReInit()
{
	m_dwStartTime=GetTickCount();
	m_dwElapsed=0;
	m_dwEstimated=0;
	m_dwRemaining=0;
	m_nPrevPercent=0;
}


CString CTimeStatus::GetElapsedString()
{
	CString strTmp;
	strTmp.Format("%02d:%02d",m_dwElapsed/60,m_dwElapsed%60);
	return strTmp;
}

CString CTimeStatus::GetEstimateString()
{

	CString strTmp;
	strTmp.Format("%02d:%02d",m_dwEstimated/60,m_dwEstimated%60);
	return strTmp;
}

CString CTimeStatus::GetRemainingString()
{
	CString strTmp;
	strTmp.Format("%02d:%02d",m_dwRemaining/60,m_dwRemaining%60);
	return strTmp;
}


void CTimeStatus::Calculate(int nPercent)
{

	m_dwElapsed=(GetTickCount()-m_dwStartTime)/1000;

	// Update every two percent
	if ((nPercent-m_nPrevPercent)>=3)
	{
		if (nPercent)
		{
			m_dwEstimated=(DWORD)max(0,m_dwElapsed*100.0/nPercent);
			m_nPrevPercent=nPercent;
		}
	}
	m_dwRemaining=(DWORD)max(0,(double)m_dwEstimated-(double)m_dwElapsed);
}

