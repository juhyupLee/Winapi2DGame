#include "Profiler.h"
#include <stdio.h>
#include <cmath>
LARGE_INTEGER frequency;
bool g_bFirst = true;

struct stProfile
{
	stProfile()
		:m_bUsed(false),
		m_szName{ 0 },
		m_StartTime{ 0 },
		m_iTotalTime(0),
		m_iMin{ MIN,MIN },
		m_iMax{ MAX,MAX}, 
		m_iCall(0)
	{

	}
	bool m_bUsed;
	TCHAR m_szName[64];
	LARGE_INTEGER m_StartTime;
	__int64 m_iTotalTime;
	__int64 m_iMin[2];
	__int64 m_iMax[2];
	__int64 m_iCall;
};

stProfile g_ProfileInfo[INFO_CNT];

void ProfileBegin(const TCHAR* szName)
{
	LARGE_INTEGER beginTime;
	QueryPerformanceCounter(&beginTime);
	if (g_bFirst)
	{
		QueryPerformanceFrequency(&frequency);
		g_bFirst = false;
	}
	bool bRedundant = false;
	int redundantIndex = 0;
	//태그가 이미 존재하는지 먼저 체크 
	for (int index = 0; index < INFO_CNT; ++index)
	{
		if (_tcscmp(g_ProfileInfo[index].m_szName, szName)==0)
		{
			bRedundant = true;
			redundantIndex = index;
			break;
		}
	}
	//태그가 이미 존재하는거라면 , 시간만 갱신.
	if (bRedundant)
	{
		g_ProfileInfo[redundantIndex].m_StartTime = beginTime;
	}
	else
	{
		//그렇지 않으면 신규등록.
		for (int index = 0; index < INFO_CNT; ++index)
		{
			if (g_ProfileInfo[index].m_bUsed == false)
			{
				//Profile Begin에서는 사용유무와, 이름, 시작시간을 체크한다.
				g_ProfileInfo[index].m_bUsed = true;
				_tcscpy_s(g_ProfileInfo[index].m_szName, _countof(g_ProfileInfo[index].m_szName), szName);
				g_ProfileInfo[index].m_StartTime = beginTime;
				//QueryPerformanceCounter(&g_ProfileInfo[index].m_StartTime);
				break;
			}
		}

	}
	

}
void ProfileEnd(const TCHAR* szName)
{
	bool bRedundant = false;
	int redundantIndex = 0;
	LARGE_INTEGER endTime;
	QueryPerformanceCounter(&endTime);

	//태그가 이미 존재하는지 먼저 체크 
	for (int index = 0; index < INFO_CNT; ++index)
	{
		if (_tcscmp(g_ProfileInfo[index].m_szName, szName) == 0)
		{
			bRedundant = true;
			redundantIndex = index;
			break;
		}
	}

	if (!bRedundant)
	{
		return;
	}

	__int64 workingTime = endTime.QuadPart - g_ProfileInfo[redundantIndex].m_StartTime.QuadPart;
	g_ProfileInfo[redundantIndex].m_iTotalTime += workingTime;
	g_ProfileInfo[redundantIndex].m_iCall++;

	// Max 1,2 Setting
	if (g_ProfileInfo[redundantIndex].m_iMax[0] < workingTime)
	{
		g_ProfileInfo[redundantIndex].m_iMax[1] = g_ProfileInfo[redundantIndex].m_iMax[0];
		g_ProfileInfo[redundantIndex].m_iMax[0] = workingTime;
	}
	else
	{
		if (g_ProfileInfo[redundantIndex].m_iMax[1] < workingTime)
		{
			g_ProfileInfo[redundantIndex].m_iMax[1] = workingTime;
		}
	}

	// Min 1,2 Setting
	if (g_ProfileInfo[redundantIndex].m_iMin[0] > workingTime)
	{
		g_ProfileInfo[redundantIndex].m_iMin[1] = g_ProfileInfo[redundantIndex].m_iMin[0];
		g_ProfileInfo[redundantIndex].m_iMin[0] = workingTime;
	}
	else
	{
		if (g_ProfileInfo[redundantIndex].m_iMin[1] > workingTime)
		{
			g_ProfileInfo[redundantIndex].m_iMin[1] = workingTime;
		}
	}
}

void ProfileDataOutText(const TCHAR* szFileName)
{
	FILE* pFile = nullptr;
	errno_t error = _tfopen_s(&pFile, szFileName, _T("wb"));

	while (error != 0)
	{
		error = _tfopen_s(&pFile, szFileName, _T("wb"));
	}
	_ftprintf(pFile, _T("----------------------------------------------------------------------------------------------------------------------------\n"));
	_ftprintf(pFile, _T("                Name |                  Average |                     Min |                      Max |                  Call|\n"));
	_ftprintf(pFile, _T("----------------------------------------------------------------------------------------------------------------------------\n"));
	for (int index = 0; index < INFO_CNT; ++index)
	{
		if (g_ProfileInfo[index].m_bUsed)
		{
			__int64 valuableResult =	g_ProfileInfo[index].m_iTotalTime -
										(g_ProfileInfo[index].m_iMax[0] +
										 g_ProfileInfo[index].m_iMax[1] +
										 g_ProfileInfo[index].m_iMin[0] +
										 g_ProfileInfo[index].m_iMin[1]);

			double average = ((double)valuableResult / (((double)g_ProfileInfo[index].m_iCall-(double)4)))/ CONVERT_US;
			double min = g_ProfileInfo[index].m_iMin[0] / CONVERT_US;
			double max = g_ProfileInfo[index].m_iMax[0] / CONVERT_US;

			_ftprintf(pFile, _T("%20s | %20.4lf[us] |%20.4lf[us] | %20.4lf[us] | %20lld |\n"), g_ProfileInfo[index].m_szName,
																 average,
																 min,
																 max,
																 g_ProfileInfo[index].m_iCall);
		}
	}
	_ftprintf(pFile, _T("----------------------------------------------------------------------------------------------------------------------------\n"));

	fclose(pFile);
}

void ProfileReset()
{
	for (int index = 0; index < INFO_CNT; ++index)
	{
		if (g_ProfileInfo[index].m_bUsed)
		{
			g_ProfileInfo[index].m_iCall = 0;
			g_ProfileInfo[index].m_iMax[0] = MAX;
			g_ProfileInfo[index].m_iMax[1] = MAX;
			g_ProfileInfo[index].m_iMin[0] = MIN;
			g_ProfileInfo[index].m_iMin[1] = MIN;
			g_ProfileInfo[index].m_iTotalTime = 0;
			g_ProfileInfo[index].m_StartTime = { 0 };

		}
	}
}
