#pragma once
class Effect :public IBaseObject
{
public:
	Effect(IBaseScene* pScene, int x, int y);
	~Effect();
	virtual void Render() ;
	virtual bool Update() ;
	virtual __int32 GetType();
	virtual RECT GetHitBox();
	virtual RECT GetCollisionBox();
	
private:
	CSpriteManager::stSprite* m_stCurrentSprite;
	
};