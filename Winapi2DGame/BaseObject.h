#pragma once
class IBaseObject
{
public:
	enum
	{
		PLAYER,
		MONSTER,
		WALL,
		BACKGROUND,
		HP_BAR,
		SHADOW
	};
	IBaseObject(IBaseScene* pScene);
	virtual	~IBaseObject();
	virtual	void Render() = 0;
	virtual	bool Update() = 0;
	virtual	RECT GetHitBox()=0;
	virtual	RECT GetCollisionBox()=0;
	virtual	__int32 GetType() = 0;
	virtual void SetScene(IBaseScene* pScene);

public:
	//------------------------------------
	//프로카데미  
	//------------------------------------
	void SetSprite(int iSpriteStart, int iSpriteMax, int iFrameDelay);
	int GetSprite(void);
	bool IsEndFrame();
	void NextFrame();
	__int32 GetY();

	//------------------------------------
protected:
	__int32		m_iX;
	__int32		m_iY;
	IBaseScene* m_pCurrentScene;
	__int32		m_iSpriteStart;
	__int32		m_iSpriteMax;
	__int32		m_iFrameDelay;
	__int32		m_iSpriteNow;
	__int32		m_iDelayCount;
	bool		m_bEndFrame;
};