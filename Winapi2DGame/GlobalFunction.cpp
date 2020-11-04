#include "GlobalFunction.h"
#include <iostream>
#include <Windows.h>
#include "LogManager.h"
#include "PacketDefine.h"
#include "PacketStructure.h"
#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include <Windows.h>
#include "MyLinkedList.h"
#include "BaseScene.h"
#include "BaseObject.h"
#include <unordered_map>
#include "ObjectManager.h"
#include "GlobalVariable.h"

HANDLE g_Handle;

void Marshalling()
{

	SINGLETON(CLogManager)->PrintConsoleLog(L"CPlayer:%d\n", sizeof(CPlayer));
	char tempBuffer[1000];
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

void SendEvent()
{
    char tempBuffer[10000];

    while (g_SendRingBuffer.GetUsedSize() != 0)
    {
        int directDeQSize = g_SendRingBuffer.GetDirectDequeueSize();
        int peekRtn = g_SendRingBuffer.Peek(tempBuffer, directDeQSize);
        int sendRtn = send(g_Socket, tempBuffer, peekRtn, 0);
        if (sendRtn <= 0)
        {
#ifdef _DEBUG
            SINGLETON(CLogManager)->PrintConsoleLog(L"Send Error:%d\n", WSAGetLastError());
#endif
            return;
        }
        g_SendRingBuffer.MoveFront(sendRtn);
    }
}
void CreateMyCharacter(char* payload)
{
	Packet_SC_MY_Character* packet = (Packet_SC_MY_Character*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] ID:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] Direction:%d\n", packet->direction);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] X:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] Y:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create My Character(SC)] HP:%d\n", packet->hp);

	g_pPlayer = new CPlayer(packet->id, packet->direction, packet->x, packet->y, packet->hp);

	SINGLETON(CObjectManager)->AddObject(g_pPlayer);
}

void CreateOtherCharacter(char* payload)
{
	Packet_SC_Other_Character* packet = (Packet_SC_Other_Character*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] ID:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] Direction:%d\n", packet->direction);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] X:%d\n", packet->x);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] Y:%d\n", packet->y);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Create Other Character(SC)] HP:%d\n", packet->hp);

	CPlayer* otherPlayer;	
	otherPlayer = new CPlayer(packet->id, packet->direction, packet->x, packet->y, packet->hp);
	SINGLETON(CObjectManager)->AddObject(otherPlayer);

}

void MoveStart(char* payload)
{
	Packet_SC_Move_Start* packet = (Packet_SC_Move_Start*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Start(SC)] ID:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Start(SC)] Direction:%d\n", packet->direction);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Start(SC)] X:%d\n", packet->x);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Start(SC)] Y:%d\n", packet->y);

	SINGLETON(CObjectManager)->OtherPlayerActionInput(packet->id, CPlayer::MOVE, packet->direction, packet->x, packet->y, true);
}

void MoveStop(char* payload)
{
	Packet_SC_Move_Stop* packet = (Packet_SC_Move_Stop*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Stop(SC)] ID:%d\n", packet->id);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Stop(SC)] Direction:%d\n", packet->direction);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Stop(SC)] X:%d\n", packet->x);
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Move Stop(SC)] Y:%d\n", packet->y);

	SINGLETON(CObjectManager)->OtherPlayerActionInput(packet->id, CPlayer::STAND, packet->direction, packet->x, packet->y, false);

}

void Attack1(char* payload)
{
	Packet_SC_Attack1* packet = (Packet_SC_Attack1*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Attack1(SC)] ID:%d // Direction:%d // X:%d // Y:%d\n", packet->id, packet->direction, packet->x, packet->y);
	SINGLETON(CObjectManager)->OtherPlayerActionInput(packet->id, CPlayer::ATTACK1, packet->direction, packet->x, packet->y, true);
}

void Attack2(char* payload)
{
	Packet_SC_Attack2* packet = (Packet_SC_Attack2*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Attack2 (SC)] ID:%d // Direction:%d // X:%d // Y:%d\n", packet->id, packet->direction, packet->x, packet->y);
	SINGLETON(CObjectManager)->OtherPlayerActionInput(packet->id, CPlayer::ATTACK2, packet->direction, packet->x, packet->y, true);
}

void Attack3(char* payload)
{
	Packet_SC_Attack3* packet = (Packet_SC_Attack3*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Attack3 (SC)] ID:%d // Direction:%d // X:%d // Y:%d\n", packet->id, packet->direction, packet->x, packet->y);
	SINGLETON(CObjectManager)->OtherPlayerActionInput(packet->id, CPlayer::ATTACK3, packet->direction, packet->x, packet->y, true);
}

void Damage(char* payload)
{

	Packet_SC_Damage* packet = (Packet_SC_Damage*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Damage (SC)] Attack ID:%d // Damage ID:%d // Damage HP:%d\n", packet->attackID, packet->damageID, packet->damageHP, 0);
	SINGLETON(CObjectManager)->AttackCheck(packet->attackID, packet->damageID, packet->damageHP);
}

void DeleteCharacter(char* payload)
{
	Packet_SC_Delete_Character* packet = (Packet_SC_Delete_Character*)payload;
	SINGLETON(CLogManager)->PrintConsoleLog(L"[Delete (SC)]  ID:%d \n", packet->id);
	SINGLETON(CObjectManager)->DeletePlayer(packet->id);
}