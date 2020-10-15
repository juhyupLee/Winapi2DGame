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
	// �� �Լ��� �ѹ���  ������ �ȴ�
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
	// LogicTimeStart�� ������ �Ǿ�߸� �����̵ȴ�
	// ������ ��ٸ� ���� ������ �ð��� ���ϰ�, 
	// ���� FPS(50)�� �ش��ϴ� 1�����ӽð� (20msec) ���� ���������� �ð�������
	// ���� �ð��� �Ǵµ�, ���� �������ӿ� Sleep�� ���� ���� �ð��� �־��ٸ�, 
	// Sleep�� ������ �ֱ⶧����,  QPC�� �ð��� �ٽ� üũ�ϴ°��� �ƴϰ�, 
	// Start �ð����� �����ð��� �� �ؼ� ������ �Ѵ�.
	//---------------------------------
	if (m_bFirstTimeCheckEnd)
	{
		m_bSleepTimeUsed = false;

		QueryPerformanceCounter(&m_LogicTimeEnd);
		m_PrevLogicTime = (__int64)((m_LogicTimeEnd.QuadPart - m_LogicTimeStart.QuadPart) / 10000.0);
	
		m_i64SpareTime = (__int64)m_FrameTime - m_PrevLogicTime;

		//---------------------------------
		// ���½ð��� ������, �ʰ���ٴ� ���ε�,
		// �� �ʰ��� �ð��� OverTimeSum�� �������ϰ�
		// SpareTime �� 0���� ����������
		// 
		// SpareTime�� ����� �׸�ŭ sleep�� �ϴ°��� �´µ�,
		// ������ OverTimeSum�� 20msec ����ũ�� ,Sleep�� �ϸ� �ȵȴ�
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
	// GetSleepTime�� LogicTimeEnd�� ���� ȣ���� �Ǿ� �� ���� ������ �ð��� ����̵ǰ�,
	// ���� ������ �ð��� ���Ҵٸ� ���� �ð��� Sleep���� ä���ִ°��̴�
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
