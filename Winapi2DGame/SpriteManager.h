#pragma once
class CSpriteManager
{
private:
	CSpriteManager();
	~CSpriteManager();
	enum
	{
		OBJECT_MAX=10,
		DIRECTION_MAX = 2,
		STATUS_MAX = 8,
		FRAME_MAX =12
	};
	
public:
	struct stSprite
	{
		__int32		m_iWidth;
		__int32		m_iHeight;
		__int32		m_iPitch;
		__int32		m_iOriginX;
		__int32		m_iOriginY;
		BYTE		*m_bypImage;
		RECT		m_rectCollisionBox;
		RECT		m_rectHitBox;
	};
	struct stAnimationStatus
	{
		__int32 m_iFrameMax;
		__int32 m_iDelay;
	};

public:
	static CSpriteManager* GetInstance();
public:
	bool LoadingSprite(	__int32 iObjectType,
						__int32 iDirection,
						__int32 iStatus,
						__int32 iFrame,
						const WCHAR* szFileName,
						__int32 iOriginX,
						__int32 iOriginY,
						RECT rectCollisionBox,
						RECT rectHitBox=RECT());

	stSprite* GetSprite(__int32 iObjectType, __int32 iDirection, __int32 iStatus, __int32 iFrame);
	stAnimationStatus* GetAnimationStatus(__int32 iObjectType, __int32 iStatus);
	void SetAnimationStatus(__int32 iObjectType, __int32 iStatus, __int32 iDelay, __int32 iFrameMax);

private:
	stSprite m_stSprite[OBJECT_MAX][DIRECTION_MAX][STATUS_MAX][FRAME_MAX];
	stAnimationStatus m_stAnimationStatus[OBJECT_MAX][STATUS_MAX];

};