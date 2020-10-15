#pragma once
#define SINGLETON(TYPE) TYPE::GetInstance()
#define WINSIZE_WIDTH 640
#define WINSIZE_HEIGHT 480

extern HWND  g_hWnd;
extern BOOL g_bActiveWindow;
extern CPlayer* g_pPlayer;

extern int g_iLogicCount;
extern int g_iRenderCount;


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