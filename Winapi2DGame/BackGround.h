#pragma once
class CSprite;
class CBackGround :public IBaseObject
{
public:
	CBackGround();
	~CBackGround();
	virtual void Render() ;
	virtual bool Update() ;
	virtual __int32 GetType();
};