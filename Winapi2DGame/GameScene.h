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
	void OtherActionInput();
	void Marshalling();

	void CreateMyCharacter(char* payload);
	void CreateOtherCharacter(char* payload);
	void MoveStart(char* payload);
	void MoveStop(char* payload);
	void Attack1(char* payload);
	void Attack2(char* payload);
	void Attack3(char* payload);
	void Damage(char* payload);
	void DeleteCharacter(char* payload);
private:
	void SpriteLoading();
	bool		m_bStand;
	__int32		m_iDirection;
	__int32		m_iMoveDirection;
	__int32		m_iStatus;

};