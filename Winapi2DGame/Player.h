#pragma once
class CPlayer :public IBaseObject
{
public:
	enum
	{
		RIGHT=0,
		LEFT,
		UP,
		DOWN,
		LEFT_UP,
		RIGHT_UP,
		LEFT_DOWN,
		RIGHT_DOWN
	};
	enum
	{
		RANGE_MOVE_TOP= 50,
		RANGE_MOVE_LEFT = 10,
		RANGE_MOVE_RIGHT = 630,
		RANGE_MOVE_BOTTOM = 470
	};
	enum
	{
		STAND,
		MOVE,
		ATTACK1,
		ATTACK2,
		ATTACK3
	};
	CPlayer(IBaseScene* pScene);
	virtual ~CPlayer();
	virtual void Render() ;
	virtual bool Update() ;
	virtual __int32 GetType();
	virtual RECT GetHitBox() ;
	virtual RECT GetCollisionBox() ;

	void ActionInput(__int32 iStatus, __int32 iDirection, __int32 iMoveDirection);
	
private:
	//--------------------------------------
	// Procademy 
	//--------------------------------------
	void ActionProc();
	void InputActionProc();
	//--------------------------------------
	void UpdateRect();
private:
	int32_t m_iStatus;
	int32_t m_iDirection;
	int32_t m_iAni_CurrentFrame;
	bool m_bActionStand; //액션상태 유지
	DWORD m_dwActionCur;
	CSpriteManager::stSprite* m_stCurrentSprite;

	RECT m_rectHitBox;
	RECT m_rectCollisionBox;
	bool m_bFirst;
	__int32		m_iMoveDirection;
	__int32		m_iSpeed;
	__int32		m_iHP;


};

