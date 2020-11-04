#pragma once
class Effect :public IBaseObject
{
public:
	Effect(int x, int y);
	~Effect();
	virtual void Render() ;
	virtual bool Update() ;
	virtual __int32 GetType();
	
private:
	CSpriteManager::stSprite* m_stCurrentSprite;
	
};