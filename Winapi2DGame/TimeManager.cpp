#include <iostream>
#include <Windows.h>
#include "TimeManager.h"
#include <math.h>
#include "LogManager.h"
#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include "GlobalVariable.h"
#include "MemoryTracer.h"

CTimeManager::CTimeManager()
	:
	m_FrameCount(50),
	m_LogicTimeEnd{ 0, },
	m_LogicTimeStart{0,},
	m_PrevLogicTime(0),
	m_bSleepTimeUsed(false),
	m_bFirstTimeCheckStart(true),
	m_bFirstTimeCheckEnd(false),
	m_iOvertimeSum(0)
{
	QueryPerformanceFrequency(&m_Freq);
	m_FrameTime = (int)((1.0 / (double)m_FrameCount) * 1000);
}
CTimeManager::~CTimeManager()
{
}
CTimeManager* CTimeManager::GetInstance()
{
	static CTimeManager timeManager;
	return &timeManager;
}

void CTimeManager::SetFrameCount(int frameCount)
{
	m_FrameCount = frameCount;
	m_FrameTime = (int)((1.0 / (double)frameCount)*1000);
}

int CTimeManager::GetFrameCount()
{
	return m_FrameCount;
}

int CTimeManager::GetFrameTime()
{
	return m_FrameTime;
}

//-------------------------------
//Prev Frame Logic Time
//-------------------------------
void CTimeManager::LogicTimeStart()
{
	//---------------------------------
	// 이 함수는 한번만  실행이 된다
	//---------------------------------
	if (m_bFirstTimeCheckEnd == true)
	{
		return;
	}
	if (m_bFirstTimeCheckStart)
	{
		m_bFirstTimeCheckStart = false;
		QueryPerformanceCounter(&m_LogicTimeStart);
	}
	else
	{
		m_bFirstTimeCheckEnd = true;
	}
}
__int64 CTimeManager::LogicTimeEnd()
{
	//---------------------------------
	// LogicTimeStart가 선행이 되어야만 실행이된다
	// 실행이 됬다면 이젠 프레임 시간을 구하고, 
	// 현재 FPS(50)에 해당하는 1프레임시간 (20msec) 에서 이전프레임 시간을빼면
	// 남는 시간이 되는데, 만약 전프레임에 Sleep에 사용된 남는 시간이 있었다면, 
	// Sleep이 오차가 있기때문에,  QPC로 시간을 다시 체크하는것이 아니고, 
	// Start 시간에다 남은시간을 더 해서 갱신을 한다.
	//---------------------------------
	if (m_bFirstTimeCheckEnd)
	{
		m_bSleepTimeUsed = false;

		QueryPerformanceCounter(&m_LogicTimeEnd);
		m_PrevLogicTime = (__int64)((m_LogicTimeEnd.QuadPart - m_LogicTimeStart.QuadPart) / 10000.0);
	
		m_i64SpareTime = (__int64)m_FrameTime - m_PrevLogicTime;

		//---------------------------------
		// 남는시간이 음수면, 초과됬다는 것인데,
		// 그 초과된 시간을 OverTimeSum에 누적을하고
		// SpareTime 을 0으로 만들어버린다
		// 
		// SpareTime이 양수면 그만큼 sleep을 하는것이 맞는데,
		// 누적된 OverTimeSum이 20msec 보다크면 ,Sleep을 하면 안된다
		//---------------------------------
		if (m_i64SpareTime < 0)
		{
			m_iOvertimeSum += -m_i64SpareTime;
			m_i64SpareTime = 0;
		}
		else
		{
			if (m_iOvertimeSum > m_FrameTime)
			{
				m_i64SpareTime = 0;
			}
		}

		m_LogicTimeStart.QuadPart = m_LogicTimeEnd.QuadPart + (__int64)(10000 * m_i64SpareTime);
		return m_PrevLogicTime;
	}
	return -1;
}


//-------------------------------
//Game Time
//-------------------------------
void CTimeManager::GameTimeStart()
{
	QueryPerformanceCounter(&m_GameTimeStart);
}

__int64 CTimeManager::GameTimeEnd()
{
	QueryPerformanceCounter(&m_GameTimeEnd);
	m_GameTime = (__int64)((m_GameTimeEnd.QuadPart - m_GameTimeStart.QuadPart) / 10000.0);
	return m_GameTime;

}

__int64 CTimeManager::GetLogicTime()
{
	return m_PrevLogicTime;
}

__int64 CTimeManager::GetGameTime()
{
	return m_GameTime;
}

__int64 CTimeManager::GetSleepTime()
{
	//---------------------------------
	// GetSleepTime은 LogicTimeEnd가 먼저 호출이 되야 만 이전 프레임 시간이 계산이되고,
	// 이전 프레임 시간이 남았다면 남은 시간을 Sleep으로 채워주는것이다
	//---------------------------------
	if (m_bSleepTimeUsed == false)
	{
		m_bSleepTimeUsed = true;
		if (m_i64SpareTime < 0)
		{
			m_i64SpareTime = 0;
			return m_i64SpareTime;
		}

		return m_i64SpareTime;
	}
	else
	{
		return  0;
	}
}

__int64 CTimeManager::GetSpareTime()
{
	return m_i64SpareTime;
}

bool CTimeManager::IsRenderSkip()
{
	LogicTimeEnd();
	if(m_iOvertimeSum>20)//if (LogicTimeEnd() > m_iOvertimeSum) 
	{
		m_iOvertimeSum -= 20;
		return true;
	}
	return false;
}
