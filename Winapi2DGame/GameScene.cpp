#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include <iostream>
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"

#include "MyLinkedList.h"
#include <unordered_map>
#include "GameScene.h"

#include "ObjectManager.h"
#include "BackBuffer.h"
#include "BackGround.h"
#include "SpriteManager.h"
#include "Utility.h"
#include "GlobalVariable.h"
#include "Monster.h"
#include "PacketStructure.h"
#include <iostream>
#include <Windows.h>
#include "LogManager.h"
#include "PacketDefine.h"
#include <iostream>
#include <Windows.h>
#include <cassert>
#include "SerializeBuffer.h"
#include "GlobalFunction.h"

CPlayer* g_pPlayer = nullptr;

CGameScene::CGameScene()
	:m_iDirection(0)
{
	Loading();
}

CGameScene::~CGameScene()
{
}
void CGameScene::Update()
{
	SINGLETON(CObjectManager)->Update();
	SendEvent();
	++g_iLogicCount;
}
void CGameScene::Render()
{
	
	SINGLETON(CObjectManager)->Render();
	++g_iRenderCount;
	InvalidateRect(g_hWnd, nullptr, FALSE);
}

bool CGameScene::Loading()
{
	SINGLETON(CObjectManager)->AddObject(new CBackGround());
	
	SpriteLoading();
	return true;
}

void CGameScene::KeyProcess()
{
	bool bKeyProcess = false;

	if (!g_bActiveWindow)
	{
		return;
	}
	if (GetAsyncKeyState(VK_LEFT)&& !GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_DOWN))
	{
		m_iDirection = dfPACKET_MOVE_DIR_LL;
		m_iMoveDirection = dfPACKET_MOVE_DIR_LL;
		m_iStatus = CPlayer::MOVE;
		bKeyProcess = true;
	}
	else if (GetAsyncKeyState(VK_RIGHT)&& !GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_DOWN))
	{
		m_iDirection = dfPACKET_MOVE_DIR_RR;
		m_iMoveDirection = dfPACKET_MOVE_DIR_RR;
		m_iStatus = CPlayer::MOVE;
		bKeyProcess = true;
	}
	else if (GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_RIGHT) && !GetAsyncKeyState(VK_LEFT))
	{
		m_iDirection = m_iDirection;
		m_iMoveDirection = dfPACKET_MOVE_DIR_UU;
		m_iStatus = CPlayer::MOVE;
		bKeyProcess = true;

	}
	else if (GetAsyncKeyState(VK_DOWN) && !GetAsyncKeyState(VK_RIGHT) && !GetAsyncKeyState(VK_LEFT))
	{
		m_iDirection = m_iDirection;
		m_iMoveDirection = dfPACKET_MOVE_DIR_DD;
		m_iStatus = CPlayer::MOVE;
		bKeyProcess = true;
	}
	else if (GetAsyncKeyState(VK_LEFT)&& GetAsyncKeyState(VK_UP))
	{
		m_iDirection = dfPACKET_MOVE_DIR_LL;
		m_iMoveDirection = dfPACKET_MOVE_DIR_LU;
		m_iStatus = CPlayer::MOVE;
		bKeyProcess = true;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_UP))
	{
		m_iDirection = dfPACKET_MOVE_DIR_RR;
		m_iMoveDirection = dfPACKET_MOVE_DIR_RU;
		m_iStatus = CPlayer::MOVE;
		bKeyProcess = true;
	}
	else if (GetAsyncKeyState(VK_DOWN)&& GetAsyncKeyState(VK_RIGHT))
	{
		m_iDirection = dfPACKET_MOVE_DIR_RR;
		m_iMoveDirection = dfPACKET_MOVE_DIR_RD;
		m_iStatus = CPlayer::MOVE;
		bKeyProcess = true;
	}
	else if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_LEFT))
	{
		m_iDirection = dfPACKET_MOVE_DIR_LL;
		m_iMoveDirection = dfPACKET_MOVE_DIR_LD;
		m_iStatus = CPlayer::MOVE;
		bKeyProcess = true;
	}
	if (GetAsyncKeyState('Q'))
	{
		m_iStatus = CPlayer::ATTACK1;
		bKeyProcess = true;
	}
	if (GetAsyncKeyState('W'))
	{
		m_iStatus = CPlayer::ATTACK2;
		bKeyProcess = true;

	}
	if (GetAsyncKeyState('E'))
	{
		m_iStatus = CPlayer::ATTACK3;
		bKeyProcess = true;
	}
	
//--------------------------------------------------------------------
// g_Player(본인클라)는 Keyprocess에서 키입력이 있다면 ActionInput으로 
// Player에게 키입력을 전달하지만, 그렇지 않으면  InputSetting 플래그
// 만  false로 세팅한다.
// 플레이어 안에서 입력세팅이 안되어있다면 그에따른 로직처리(STAND 애니메이션)을 한다
//--------------------------------------------------------------------
	if (g_pPlayer != nullptr)
	{
		if (bKeyProcess == true)
		{
			g_pPlayer->ActionInput(m_iStatus, m_iDirection, m_iMoveDirection);
			g_pPlayer->SetInputStatus(true);
		}
		else
		{
			g_pPlayer->SetInputStatus(false);
		}
	}
}

void CGameScene::SpriteLoading()
{
	RECT collisionBox;
	RECT hitBoxLeft;
	RECT hitBoxRight;

	collisionBox.left = 35;
	collisionBox.top = 0;
	collisionBox.right = 135;
	collisionBox.bottom = 93;

	hitBoxLeft.left = 0;
	hitBoxLeft.top = 20;
	hitBoxLeft.right = 50;
	hitBoxLeft.bottom = 30;

	hitBoxRight.left = 92;
	hitBoxRight.top = 20;
	hitBoxRight.right = 142;
	hitBoxRight.bottom = 30;


	//----------------------------------------
	//Effect
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::EFFECT, 0, 0, 0, L"./Sprite_Data/xSpark_1.bmp", 70, 70, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::EFFECT, 0, 0, 1, L"./Sprite_Data/xSpark_2.bmp", 70, 70, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::EFFECT, 0, 0, 2, L"./Sprite_Data/xSpark_3.bmp", 70, 70, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::EFFECT, 0, 0, 3, L"./Sprite_Data/xSpark_4.bmp", 70, 70, collisionBox);

	SINGLETON(CSpriteManager)->SetAnimationStatus(IBaseObject::EFFECT, 0, 3, 4);
	//----------------------------------------
	//HP Gauage
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::HP_BAR, 0, 0, 0, L"./Sprite_Data/HPGuage.bmp", 0, 0, collisionBox);

	//----------------------------------------
	//Shadow
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::SHADOW, 0, 0, 0, L"./Sprite_Data/Shadow.bmp", 32, 4, collisionBox);

	//----------------------------------------
	//BackGround Sprite 
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::BACKGROUND, 0, 0, 0, L"./Sprite_Data/_Map.bmp", 0, 0, collisionBox);

	//----------------------------------------
	//Player Sprite  : Attack1
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK1, 0, L"./Sprite_Data/Attack1_L_01.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK1, 1, L"./Sprite_Data/Attack1_L_02.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK1, 2, L"./Sprite_Data/Attack1_L_03.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK1, 3, L"./Sprite_Data/Attack1_L_04.bmp", 71, 90, collisionBox, hitBoxLeft);

	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK1, 0, L"./Sprite_Data/Attack1_R_01.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK1, 1, L"./Sprite_Data/Attack1_R_02.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK1, 2, L"./Sprite_Data/Attack1_R_03.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK1, 3, L"./Sprite_Data/Attack1_R_04.bmp", 71, 90, collisionBox, hitBoxRight);

	SINGLETON(CSpriteManager)->SetAnimationStatus(IBaseObject::PLAYER, CPlayer::ATTACK1, 3, 4);
	//----------------------------------------
	//Player Sprite  : Attack2
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK2, 0, L"./Sprite_Data/Attack2_L_01.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK2, 1, L"./Sprite_Data/Attack2_L_02.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK2, 2, L"./Sprite_Data/Attack2_L_03.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK2, 3, L"./Sprite_Data/Attack2_L_04.bmp", 71, 90, collisionBox, hitBoxLeft);

	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK2, 0, L"./Sprite_Data/Attack2_R_01.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK2, 1, L"./Sprite_Data/Attack2_R_02.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK2, 2, L"./Sprite_Data/Attack2_R_03.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK2, 3, L"./Sprite_Data/Attack2_R_04.bmp", 71, 90, collisionBox, hitBoxRight);

	SINGLETON(CSpriteManager)->SetAnimationStatus(IBaseObject::PLAYER, CPlayer::ATTACK2, 4, 4);
	//----------------------------------------
	//Player Sprite  : Attack3
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK3, 0, L"./Sprite_Data/Attack3_L_01.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK3, 1, L"./Sprite_Data/Attack3_L_02.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK3, 2, L"./Sprite_Data/Attack3_L_03.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK3, 3, L"./Sprite_Data/Attack3_L_04.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK3, 4, L"./Sprite_Data/Attack3_L_05.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::ATTACK3, 5, L"./Sprite_Data/Attack3_L_06.bmp", 71, 90, collisionBox, hitBoxLeft);

	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK3, 0, L"./Sprite_Data/Attack3_R_01.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK3, 1, L"./Sprite_Data/Attack3_R_02.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK3, 2, L"./Sprite_Data/Attack3_R_03.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK3, 3, L"./Sprite_Data/Attack3_R_04.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK3, 4, L"./Sprite_Data/Attack3_R_05.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::ATTACK3, 5, L"./Sprite_Data/Attack3_R_06.bmp", 71, 90, collisionBox, hitBoxRight);

	SINGLETON(CSpriteManager)->SetAnimationStatus(IBaseObject::PLAYER, CPlayer::ATTACK3, 4, 6);
	//----------------------------------------
	//Player Sprite  : Move
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::MOVE, 0, L"./Sprite_Data/Move_L_01.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::MOVE, 1, L"./Sprite_Data/Move_L_02.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::MOVE, 2, L"./Sprite_Data/Move_L_03.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::MOVE, 3, L"./Sprite_Data/Move_L_04.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::MOVE, 4, L"./Sprite_Data/Move_L_05.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::MOVE, 5, L"./Sprite_Data/Move_L_06.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::MOVE, 6, L"./Sprite_Data/Move_L_07.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::MOVE, 7, L"./Sprite_Data/Move_L_08.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::MOVE, 8, L"./Sprite_Data/Move_L_09.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::MOVE, 9, L"./Sprite_Data/Move_L_10.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::MOVE, 10, L"./Sprite_Data/Move_L_11.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::MOVE, 11, L"./Sprite_Data/Move_L_12.bmp", 71, 90, collisionBox);

	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::MOVE, 0, L"./Sprite_Data/Move_R_01.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::MOVE, 1, L"./Sprite_Data/Move_R_02.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::MOVE, 2, L"./Sprite_Data/Move_R_03.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::MOVE, 3, L"./Sprite_Data/Move_R_04.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::MOVE, 4, L"./Sprite_Data/Move_R_05.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::MOVE, 5, L"./Sprite_Data/Move_R_06.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::MOVE, 6, L"./Sprite_Data/Move_R_07.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::MOVE, 7, L"./Sprite_Data/Move_R_08.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::MOVE, 8, L"./Sprite_Data/Move_R_09.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::MOVE, 9, L"./Sprite_Data/Move_R_10.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::MOVE, 10, L"./Sprite_Data/Move_R_11.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::MOVE, 11, L"./Sprite_Data/Move_R_12.bmp", 71, 90, collisionBox);

	SINGLETON(CSpriteManager)->SetAnimationStatus(IBaseObject::PLAYER, CPlayer::MOVE, 4, 12);

	//----------------------------------------
	//Player Sprite  : Stand
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::STAND, 0, L"./Sprite_Data/Stand_L_01.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::STAND, 1, L"./Sprite_Data/Stand_L_02.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_LL, CPlayer::STAND, 2, L"./Sprite_Data/Stand_L_03.bmp", 71, 90, collisionBox);

	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::STAND, 0, L"./Sprite_Data/Stand_R_01.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::STAND, 1, L"./Sprite_Data/Stand_R_02.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, dfPACKET_MOVE_DIR_RR, CPlayer::STAND, 2, L"./Sprite_Data/Stand_R_03.bmp", 71, 90, collisionBox);

	SINGLETON(CSpriteManager)->SetAnimationStatus(IBaseObject::PLAYER, CPlayer::STAND, 5, 3);

}