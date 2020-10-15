#pragma once
class CTimeManager
{
public:
	
private:
	CTimeManager();
	~CTimeManager();
	
public:
	static CTimeManager* GetInstance();
	void SetFrameCount(int frameCount);
	int GetFrameCount();			//설정된 프레임을 얻어옴
	int GetFrameTime();				//설정된 프레임에 의해 계산된 프레임당 시간을 얻어옴
	void LogicTimeStart();
	__int64 LogicTimeEnd();
	
	void GameTimeStart();
	__int64 GameTimeEnd();

	__int64 GetLogicTime();
	__int64 GetGameTime();
	__int64 GetSleepTime();
	__int64 GetSpareTime();

	bool IsRenderSkip();

private:
	__int32 m_FrameCount;
	__int32 m_FrameTime;
	LARGE_INTEGER m_LogicTimeStart;
	LARGE_INTEGER m_LogicTimeEnd;
	LARGE_INTEGER m_GameTimeStart;
	LARGE_INTEGER m_GameTimeEnd;
	LARGE_INTEGER m_Freq;
	__int64 m_PrevLogicTime;

	bool m_bSleepTimeUsed;
	__int64 m_GameTime;
	__int64 m_i64SpareTime;

	bool m_bFirstTimeCheckStart;
	bool m_bFirstTimeCheckEnd;
	__int64 m_iOvertimeSum;

};