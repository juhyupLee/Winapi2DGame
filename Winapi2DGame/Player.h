#pragma once
class CPlayer :public IBaseObject
{
public:
	enum
	{
		RANGE_MOVE_TOP= 51,
		RANGE_MOVE_LEFT = 13,
		RANGE_MOVE_RIGHT = 628,
		RANGE_MOVE_BOTTOM = 468
	};
	enum
	{
		STAND,
		MOVE,
		ATTACK1,
		ATTACK2,
		ATTACK3
	};

	CPlayer(int id, int direction, int x, int y, int hp);
	int GetID();
	virtual ~CPlayer();
	virtual void Render() ;
	virtual bool Update() ;
	virtual __int32 GetType();

	void ActionInput(__int32 iStatus, __int32 iDirection, __int32 iMoveDirection);
	void SetXY(int x, int y);
	void SetInputStatus(bool ActionInput);
	void SetHP(int damage);
	void SetAttackFlag(bool bAttack);
	void SetDamageFlag(bool bDamage);
	void SetDamagePlayedXY(int x, int y);
	void GenEffect();
	void SetDelete(bool bDelete);
private:
	//--------------------------------------
	// Procademy 
	//--------------------------------------
	void ActionProc();
	void InputActionProc();
	//--------------------------------------
private:
	int32_t			m_iStatus;
	int32_t			m_PrevStatus;
	BYTE			m_PrevDirection;
	BYTE			m_iMoveDirection;

	BYTE			m_iDirection;
	int32_t			m_iAni_CurrentFrame;
	bool			m_bActionStand; //액션상태 유지

	CSpriteManager::stSprite* m_stCurrentSprite;

	bool		m_bFirst;
	
	__int32		m_XSpeed;
	__int32		m_YSpeed;
	__int32		m_iHP;

	__int32		m_ID;

	bool		m_bInputSetting;
	bool		m_bAttackFlag;
	bool		m_bDamageFlag;

	int			m_DamagedPlayerX;
	int			m_DamagedPlayerY;
	bool		m_bDelete;
};

