#pragma once
#include <iostream>
#include "RingBuffer.h"

#define SINGLETON(TYPE) TYPE::GetInstance()
#define WINSIZE_WIDTH 640
#define WINSIZE_HEIGHT 480
#define WM_NETWORK WM_USER+1
#define SERVER_PORT 5000
//#define SERVER_IP L"127.0.0.1"
#define SERVER_IP L"192.168.30.16"
extern HANDLE g_Handle;
extern HWND  g_hWnd;

extern BOOL g_bActiveWindow;
extern CPlayer* g_pPlayer;

extern int g_iLogicCount;
extern int g_iRenderCount;

extern SOCKET g_Socket;
extern RingBuffer g_SendRingBuffer;
extern RingBuffer g_RecvRingBuffer;
extern bool g_bConnected;

//---------------------------------------
//Procademy 
//---------------------------------------
#define dfACTION_MOVE_LL 0
#define dfACTION_MOVE_LU 1
#define dfACTION_MOVE_UU 2
#define dfACTION_MOVE_RU 3
#define dfACTION_MOVE_RR 4
#define dfACTION_MOVE_RD 5
#define dfACTION_MOVE_DD 6
#define dfACTION_MOVE_LD 7

#define dfACTION_ATTACK1 8
#define dfACTION_ATTACK2 9
#define dfACTION_ATTACK3 10
#define dfACTION_STAND 11
//---------------------------------------