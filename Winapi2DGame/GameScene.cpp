#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
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

CPlayer* g_pPlayer = nullptr;

CGameScene::CGameScene()
	:m_iDirection(CPlayer::RIGHT)
{
	Loading();
}

CGameScene::~CGameScene()
{
}
void CGameScene::Update()
{
	SINGLETON(CObjectManager)->Update(CObjectManager::GAME_SCENE);
	++g_iLogicCount;
}
void CGameScene::Render()
{
	
	SINGLETON(CObjectManager)->Render(CObjectManager::GAME_SCENE);
	++g_iRenderCount;
	InvalidateRect(g_hWnd, nullptr, FALSE);
}

bool CGameScene::Loading()
{
	g_pPlayer = new CPlayer(this);

	SINGLETON(CObjectManager)->AddObject(CObjectManager::GAME_SCENE, new CBackGround(this));
	SINGLETON(CObjectManager)->AddObject(CObjectManager::GAME_SCENE, g_pPlayer);
	SINGLETON(CObjectManager)->AddObject(CObjectManager::GAME_SCENE,new CMonster(this));
	
	SpriteLoading();
	return true;
}

void CGameScene::KeyProcess()
{
	m_bStand = true;

	if (!g_bActiveWindow)
	{
		return;
	}
	if (GetAsyncKeyState(VK_LEFT)&& !GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_DOWN))
	{
		m_iDirection = CPlayer::LEFT;
		m_iMoveDirection = CPlayer::LEFT;
		m_iStatus = CPlayer::MOVE;
		m_bStand = false;
	}
	else if (GetAsyncKeyState(VK_RIGHT)&& !GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_DOWN))
	{
		m_iDirection = CPlayer::RIGHT;
		m_iMoveDirection = CPlayer::RIGHT;
		m_iStatus = CPlayer::MOVE;
		m_bStand = false;
	}
	else if (GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_RIGHT) && !GetAsyncKeyState(VK_LEFT))
	{
		m_iDirection = m_iDirection;
		m_iMoveDirection = CPlayer::UP;
		m_iStatus = CPlayer::MOVE;
		m_bStand = false;

	}
	else if (GetAsyncKeyState(VK_DOWN) && !GetAsyncKeyState(VK_RIGHT) && !GetAsyncKeyState(VK_LEFT))
	{
		m_iDirection = m_iDirection;
		m_iMoveDirection = CPlayer::DOWN;
		m_iStatus = CPlayer::MOVE;
		m_bStand = false;
	}
	else if (GetAsyncKeyState(VK_LEFT)&& GetAsyncKeyState(VK_UP))
	{
		m_iDirection = CPlayer::LEFT;
		m_iMoveDirection = CPlayer::LEFT_UP;
		m_iStatus = CPlayer::MOVE;
		m_bStand = false;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_UP))
	{
		m_iDirection = CPlayer::RIGHT;
		m_iMoveDirection = CPlayer::RIGHT_UP;
		m_iStatus = CPlayer::MOVE;
		m_bStand = false;
	}
	else if (GetAsyncKeyState(VK_DOWN)&& GetAsyncKeyState(VK_RIGHT))
	{
		m_iDirection = CPlayer::RIGHT;
		m_iMoveDirection = CPlayer::RIGHT_DOWN;
		m_iStatus = CPlayer::MOVE;
		m_bStand = false;
	}
	else if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_LEFT))
	{
		m_iDirection = CPlayer::LEFT;
		m_iMoveDirection = CPlayer::LEFT_DOWN;
		m_iStatus = CPlayer::MOVE;
		m_bStand = false;
	}
	if (GetAsyncKeyState('Q'))
	{
		m_iStatus = CPlayer::ATTACK1;
		m_bStand = false;
	}
	if (GetAsyncKeyState('W'))
	{
		m_iStatus = CPlayer::ATTACK2;
		m_bStand = false;

	}
	if (GetAsyncKeyState('E'))
	{
		m_iStatus = CPlayer::ATTACK3;
		m_bStand = false;
	}
	if (m_bStand == true)
	{
		m_iStatus = CPlayer::STAND;
	}

	g_pPlayer->ActionInput(m_iStatus, m_iDirection, m_iMoveDirection);

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
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::BACKGROUND, 0, 0, 0, L"_Map.bmp", 0, 0, collisionBox);

	//----------------------------------------
	//Player Sprite  : Attack1
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK1, 0, L"./Sprite_Data/Attack1_L_01.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK1, 1, L"./Sprite_Data/Attack1_L_02.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK1, 2, L"./Sprite_Data/Attack1_L_03.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK1, 3, L"./Sprite_Data/Attack1_L_04.bmp", 71, 90, collisionBox, hitBoxLeft);

	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK1, 0, L"./Sprite_Data/Attack1_R_01.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK1, 1, L"./Sprite_Data/Attack1_R_02.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK1, 2, L"./Sprite_Data/Attack1_R_03.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK1, 3, L"./Sprite_Data/Attack1_R_04.bmp", 71, 90, collisionBox, hitBoxRight);

	SINGLETON(CSpriteManager)->SetAnimationStatus(IBaseObject::PLAYER, CPlayer::ATTACK1, 3, 4);
	//----------------------------------------
	//Player Sprite  : Attack2
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK2, 0, L"./Sprite_Data/Attack2_L_01.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK2, 1, L"./Sprite_Data/Attack2_L_02.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK2, 2, L"./Sprite_Data/Attack2_L_03.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK2, 3, L"./Sprite_Data/Attack2_L_04.bmp", 71, 90, collisionBox, hitBoxLeft);

	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK2, 0, L"./Sprite_Data/Attack2_R_01.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK2, 1, L"./Sprite_Data/Attack2_R_02.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK2, 2, L"./Sprite_Data/Attack2_R_03.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK2, 3, L"./Sprite_Data/Attack2_R_04.bmp", 71, 90, collisionBox, hitBoxRight);

	SINGLETON(CSpriteManager)->SetAnimationStatus(IBaseObject::PLAYER, CPlayer::ATTACK2, 4, 4);
	//----------------------------------------
	//Player Sprite  : Attack3
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK3, 0, L"./Sprite_Data/Attack3_L_01.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK3, 1, L"./Sprite_Data/Attack3_L_02.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK3, 2, L"./Sprite_Data/Attack3_L_03.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK3, 3, L"./Sprite_Data/Attack3_L_04.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK3, 4, L"./Sprite_Data/Attack3_L_05.bmp", 71, 90, collisionBox, hitBoxLeft);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::ATTACK3, 5, L"./Sprite_Data/Attack3_L_06.bmp", 71, 90, collisionBox, hitBoxLeft);

	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK3, 0, L"./Sprite_Data/Attack3_R_01.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK3, 1, L"./Sprite_Data/Attack3_R_02.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK3, 2, L"./Sprite_Data/Attack3_R_03.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK3, 3, L"./Sprite_Data/Attack3_R_04.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK3, 4, L"./Sprite_Data/Attack3_R_05.bmp", 71, 90, collisionBox, hitBoxRight);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::ATTACK3, 5, L"./Sprite_Data/Attack3_R_06.bmp", 71, 90, collisionBox, hitBoxRight);

	SINGLETON(CSpriteManager)->SetAnimationStatus(IBaseObject::PLAYER, CPlayer::ATTACK3, 4, 6);
	//----------------------------------------
	//Player Sprite  : Move
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::MOVE, 0, L"./Sprite_Data/Move_L_01.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::MOVE, 1, L"./Sprite_Data/Move_L_02.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::MOVE, 2, L"./Sprite_Data/Move_L_03.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::MOVE, 3, L"./Sprite_Data/Move_L_04.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::MOVE, 4, L"./Sprite_Data/Move_L_05.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::MOVE, 5, L"./Sprite_Data/Move_L_06.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::MOVE, 6, L"./Sprite_Data/Move_L_07.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::MOVE, 7, L"./Sprite_Data/Move_L_08.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::MOVE, 8, L"./Sprite_Data/Move_L_09.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::MOVE, 9, L"./Sprite_Data/Move_L_10.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::MOVE, 10, L"./Sprite_Data/Move_L_11.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::MOVE, 11, L"./Sprite_Data/Move_L_12.bmp", 71, 90, collisionBox);

	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::MOVE, 0, L"./Sprite_Data/Move_R_01.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::MOVE, 1, L"./Sprite_Data/Move_R_02.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::MOVE, 2, L"./Sprite_Data/Move_R_03.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::MOVE, 3, L"./Sprite_Data/Move_R_04.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::MOVE, 4, L"./Sprite_Data/Move_R_05.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::MOVE, 5, L"./Sprite_Data/Move_R_06.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::MOVE, 6, L"./Sprite_Data/Move_R_07.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::MOVE, 7, L"./Sprite_Data/Move_R_08.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::MOVE, 8, L"./Sprite_Data/Move_R_09.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::MOVE, 9, L"./Sprite_Data/Move_R_10.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::MOVE, 10, L"./Sprite_Data/Move_R_11.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::MOVE, 11, L"./Sprite_Data/Move_R_12.bmp", 71, 90, collisionBox);

	SINGLETON(CSpriteManager)->SetAnimationStatus(IBaseObject::PLAYER, CPlayer::MOVE, 4, 12);

	//----------------------------------------
	//Player Sprite  : Stand
	//----------------------------------------
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::STAND, 0, L"./Sprite_Data/Stand_L_01.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::STAND, 1, L"./Sprite_Data/Stand_L_02.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::LEFT, CPlayer::STAND, 2, L"./Sprite_Data/Stand_L_03.bmp", 71, 90, collisionBox);

	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::STAND, 0, L"./Sprite_Data/Stand_R_01.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::STAND, 1, L"./Sprite_Data/Stand_R_02.bmp", 71, 90, collisionBox);
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::PLAYER, CPlayer::RIGHT, CPlayer::STAND, 2, L"./Sprite_Data/Stand_R_03.bmp", 71, 90, collisionBox);

	SINGLETON(CSpriteManager)->SetAnimationStatus(IBaseObject::PLAYER, CPlayer::STAND, 5, 3);

}