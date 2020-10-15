#pragma once
class CGameScene : public IBaseScene
{

public:
	CGameScene();
	virtual ~CGameScene();
	virtual void Update();
	virtual void Render();
	virtual bool Loading();
	void KeyProcess();

private:
	void SpriteLoading();
	bool		m_bStand;
	__int32		m_iDirection;
	__int32		m_iMoveDirection;
	__int32		m_iStatus;

};