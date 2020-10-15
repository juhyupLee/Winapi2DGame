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
	int GetFrameCount();			//������ �������� ����
	int GetFrameTime();				//������ �����ӿ� ���� ���� �����Ӵ� �ð��� ����
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