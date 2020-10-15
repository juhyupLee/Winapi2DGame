#pragma once
class CSprite;
class CBackGround :public IBaseObject
{
public:
	CBackGround(IBaseScene* pScene);
	~CBackGround();
	virtual void Render() ;
	virtual bool Update() ;
	virtual __int32 GetType();
	virtual RECT GetHitBox();
	virtual RECT GetCollisionBox();
	CSprite* m_Sprite;
};