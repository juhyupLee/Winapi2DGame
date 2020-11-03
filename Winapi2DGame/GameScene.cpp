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
#include "PacketStructure.h"
#include <iostream>
#include <Windows.h>
#include "LogManager.h"
#include "PacketDefine.h"


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
	Marshalling();
	SINGLETON(CObjectManager)->Update(CObjectManager::GAME_SCENE);
	SendEvent();
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
	SINGLETON(CObjectManager)->AddObject(CObjectManager::GAME_SCENE, new CBackGround(this));
	
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

void CGameScene::OtherActionInput()
{

}

void CGameScene::Marshalling()
{
	char tempBuffer[150];
	while (true)
	{
		if (g_RecvRingBuffer.GetUsedSize() < sizeof(PacketHeader))
			break;
		PacketHeader tempPacketHeader;

		int peekRtn = g_RecvRingBuffer.Peek((char*)&tempPacketHeader, sizeof(PacketHeader));

		if (tempPacketHeader.code != CODE)
		{
			SINGLETON(CLogManager)->PrintConsoleLog(L"Packet Code Error!!", 0);
			break;
		}
		if (g_RecvRingBuffer.GetUsedSize() < sizeof(PacketHeader) + tempPacketHeader.size)
			break;

		g_RecvRingBuffer.MoveFront(sizeof(PacketHeader));

		int deQRtn = g_RecvRingBuffer.Dequeue(tempBuffer, tempPacketHeader.size);

		if (deQRtn != tempPacketHeader.size)
		{
			SINGLETON(CLogManager)->PrintConsoleLog(L"Dequeue Fail", 0);
			break;
		}

		switch (tempPacketHeader.type)
		{
		case dfPACKET_SC_CREATE_MY_CHARACTER:
			CreateMyCharacter(tempBuffer);
			break;
		case dfPACKET_SC_CREATE_OTHER_CHARACTER:
			CreateOtherCharacter(tempBuffer);
			break;
		case dfPACKET_SC_DELETE_CHARACTER:
			DeleteCharacter(tempBuffer);
			break;
		case dfPACKET_SC_MOVE_START:
			MoveStart(tempBuffer);
			break;
		case dfPACKET_SC_MOVE_STOP:
			MoveStop(tempBuffer);
			break;
		case dfPACKET_SC_ATTACK1:
			Attack1(tempBuffer);
			break;
		case dfPACKET_SC_ATTACK2:
			Attack2(tempBuffer);
			break;
		case dfPACKET_SC_ATTACK3:
			Attack3(tempBuffer);
			break;
		case dfPACKET_SC_DAMAGE:
			Damage(tempBuffer);
			break;

		}
	}
	
}

void CGameScene::CreateMyCharacter(char* payload)
{
	Packet_SC_MY_Character* packet = (Packet_SC_MY_Character*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] ID:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] Direction:%d\n", packet->direction);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] X:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] Y:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] HP:%d\n", packet->hp);

	m_iMoveDirection = packet->direction;
	g_pPlayer = new CPlayer(this, packet->id, packet->direction, packet->x, packet->y, packet->hp);

	SINGLETON(CObjectManager)->AddObject(CObjectManager::GAME_SCENE, g_pPlayer);
}

void CGameScene::CreateOtherCharacter(char* payload)
{
	Packet_SC_Other_Character* packet = (Packet_SC_Other_Character*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] ID:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] Direction:%d\n", packet->direction);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] X:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] Y:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] HP:%d\n", packet->hp);

	CPlayer* otherPlayer;

	otherPlayer = new CPlayer(this, packet->id, packet->direction, packet->x, packet->y, packet->hp);

	SINGLETON(CObjectManager)->AddObject(CObjectManager::GAME_SCENE, otherPlayer);
}

void CGameScene::MoveStart(char* payload)
{
	Packet_SC_Move_Start* packet = (Packet_SC_Move_Start*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Start(SC)] ID:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Start(SC)] Direction:%d\n", packet->direction);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Start(SC)] X:%d\n", packet->x);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Start(SC)] Y:%d\n", packet->y);

	SINGLETON(CObjectManager)->OtherPlayerActionInput(CObjectManager::GAME_SCENE, packet->id, CPlayer::MOVE, packet->direction, packet->x, packet->y,true);
}

void CGameScene::MoveStop(char* payload)
{
	Packet_SC_Move_Stop* packet = (Packet_SC_Move_Stop*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Stop(SC)] ID:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Stop(SC)] Direction:%d\n", packet->direction);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Stop(SC)] X:%d\n", packet->x);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Stop(SC)] Y:%d\n", packet->y);

	SINGLETON(CObjectManager)->OtherPlayerActionInput(CObjectManager::GAME_SCENE, packet->id,CPlayer::STAND, packet->direction, packet->x, packet->y,false);

}

void CGameScene::Attack1(char* payload)
{
	Packet_SC_Attack1* packet = (Packet_SC_Attack1*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Attack1(SC)] ID:%d // Direction:%d // X:%d // Y:%d\n", packet->id,packet->direction, packet->x, packet->y);
	SINGLETON(CObjectManager)->OtherPlayerActionInput(CObjectManager::GAME_SCENE, packet->id, CPlayer::ATTACK1, packet->direction, packet->x, packet->y, true);
}

void CGameScene::Attack2(char* payload)
{
	Packet_SC_Attack2* packet = (Packet_SC_Attack2*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Attack2 (SC)] ID:%d // Direction:%d // X:%d // Y:%d\n", packet->id, packet->direction, packet->x, packet->y);
	SINGLETON(CObjectManager)->OtherPlayerActionInput(CObjectManager::GAME_SCENE, packet->id, CPlayer::ATTACK2, packet->direction, packet->x, packet->y, true);
}

void CGameScene::Attack3(char* payload)
{
	Packet_SC_Attack3* packet = (Packet_SC_Attack3*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Attack3 (SC)] ID:%d // Direction:%d // X:%d // Y:%d\n", packet->id, packet->direction, packet->x, packet->y);
	SINGLETON(CObjectManager)->OtherPlayerActionInput(CObjectManager::GAME_SCENE, packet->id, CPlayer::ATTACK3, packet->direction, packet->x, packet->y, true);
}

void CGameScene::Damage(char* payload)
{

	Packet_SC_Damage* packet = (Packet_SC_Damage*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Damage (SC)] Attack ID:%d // Damage ID:%d // Damage HP:%d\n", packet->attackID, packet->damageID, packet->damageHP,0);
	SINGLETON(CObjectManager)->AttackCheck(CObjectManager::GAME_SCENE, packet->attackID, packet->damageID, packet->damageHP);
}

void CGameScene::DeleteCharacter(char* payload)
{

	Packet_SC_Delete_Character* packet = (Packet_SC_Delete_Character*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Delete (SC)]  ID:%d \n", packet->id);
	SINGLETON(CObjectManager)->DeletePlayer(CObjectManager::GAME_SCENE, packet->id);
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
	SINGLETON(CSpriteManager)->LoadingSprite(IBaseObject::BACKGROUND, 0, 0, 0, L"_Map.bmp", 0, 0, collisionBox);

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