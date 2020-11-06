/**
>Winapi 2D Action Game (네트워크)
>Lee Juhyup
>개발중...
**/
#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "framework.h"
#include "Winapi2DGame.h"
#include "windowsx.h"
#include <Windows.h>
#include <iostream>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include "GlobalVariable.h"
#include "BackBuffer.h"
#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include "GlobalVariable.h"
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "MyLinkedList.h"
#include "BackGround.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "LogManager.h"
#include "Profiler.h"
//#include "MemoryTracer.h"
#include <iostream>
#include "RingBuffer.h"
#include "GlobalFunction.h"

#include <Windows.h>
#include "MyLinkedList.h"
#include "BaseScene.h"
#include "BaseObject.h"
#include <unordered_map>
#include "ObjectManager.h"

#include <stdio.h>
#include <WinSock2.h>
#include "LogManager.h"
#include "SocketLog.h"
#define MAX_LOADSTRING 100



// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


// 전역 변수:
BOOL Init_Window(HINSTANCE hInstance);
void Init_Game();
void Init_Network();
void SelectProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void RecvEvent();

HWND g_hWnd = 0;
RECT g_WindowRect;
BOOL g_bActiveWindow=true;

WCHAR g_FPSBuf[100];
HIMC g_hOldIMC;

int g_iLogicCount = 0 ;
int g_iRenderCount=0;
RingBuffer g_RecvRingBuffer;
RingBuffer g_SendRingBuffer;

SOCKET g_Socket;
bool g_bConnected = false;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    if (Init_Window(hInstance) == FALSE)
    {
        return 0;
    }
    Init_Game();
    Init_Network();
    MSG msg;
    int fpsCount = 0;
    long long fpsTime = GetTickCount64();
   
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            //-----------------------------------------
            //> FPS 출력
            //-----------------------------------------
            if (GetTickCount64() - fpsTime > 1000)
            {
                fpsTime = GetTickCount64();
                wsprintf(g_FPSBuf, L"LogicCount :%d  RenderCount :%d",g_iLogicCount,g_iRenderCount);
                g_iLogicCount = 0;
                g_iRenderCount = 0;
            }            
            //-----------------------------------------
            //> 게임 로직이 도는곳(Update Render KeyProcess Network)
            //-----------------------------------------
            if (!SINGLETON(CSceneManager)->Run())    
            {
                break;
            }
            ++fpsCount;
        }
    }
    return (int) msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{

#pragma region REGISTER_CLASS

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI2DGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = L"Winapi2D";
    wcex.lpszClassName  = L"Winapi2D";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
#pragma endregion REGISTER_CLASS

    return RegisterClassExW(&wcex);
}

BOOL Init_Window(HINSTANCE hInstance)
{

    MyRegisterClass(hInstance);

    g_hWnd = CreateWindowW(L"Winapi2D", L"Winapi2D", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINSIZE_WIDTH, WINSIZE_HEIGHT, nullptr, nullptr, hInstance, nullptr);

    if (!g_hWnd)
    {
        return FALSE;
    }
    GetClientRect(g_hWnd, &g_WindowRect);
    ShowWindow(g_hWnd, SW_SHOW);
    UpdateWindow(g_hWnd);
    SetFocus(g_hWnd);
    
    RECT windowRect = { 0,0,WINSIZE_WIDTH,WINSIZE_HEIGHT };
    AdjustWindowRectEx(&windowRect,
                     GetWindowStyle(g_hWnd),
                     GetMenu(g_hWnd)!=NULL,
                     GetWindowExStyle(g_hWnd));

    //------------------------
    //> 화면의 크기를 얻어서 정 중앙으로
    //-------------------------
    __int32 iX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (WINSIZE_WIDTH / 2);
    __int32 iY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (WINSIZE_HEIGHT / 2);

    MoveWindow(
        g_hWnd,
        iX,
        iY,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,TRUE);


    g_hOldIMC = ImmAssociateContext(g_hWnd, NULL);
    return TRUE;
}

void Init_Game()
{
    SINGLETON(CSceneManager)->ChangeScene(CSceneManager::GAME_SCENE);
}
void Init_Network()
{
    WSAData wsaData;
    SOCKADDR_IN serverAddr;
    if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        SINGLETON(CLogManager)->PrintConsoleLog(L"WSAStartUp Error:%d\n", WSAGetLastError());
    }

    g_Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (g_Socket == INVALID_SOCKET)
    {
        SINGLETON(CLogManager)->PrintConsoleLog(L"socket() Error:%d\n", WSAGetLastError());
    }

    memset(&serverAddr, 0, sizeof(SOCKADDR_IN));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    InetPton(AF_INET, SERVER_IP, &serverAddr.sin_addr.S_un.S_addr);


    if (0 != WSAAsyncSelect(g_Socket, g_hWnd, WM_NETWORK, FD_CONNECT | FD_CLOSE | FD_READ | FD_WRITE))
    {
        SINGLETON(CLogManager)->PrintConsoleLog(L"WSAAsncSelect Error:%d\n", WSAGetLastError());
    }
    if (0 != connect(g_Socket, (sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)))
    {
        SINGLETON(CLogManager)->PrintConsoleLog(L"connect() Error:%d\n", WSAGetLastError());
    }

}
void SelectProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (WSAGETSELECTERROR(lParam))
    {
        SINGLETON(CLogManager)->PrintConsoleLog(L"SelectProcess() error%d", WSAGETSELECTERROR(lParam));
        SINGLETON(CLogManager)->PrintLog(L"SelectProcess() error", WSAGETSELECTERROR(lParam));
    }
    switch (WSAGETSELECTEVENT(lParam))
    {
    case FD_CONNECT:
        SINGLETON(CLogManager)->PrintConsoleLog(L"Connect Success!!\n",0);
        g_bConnected = true;
        break;
    case FD_READ:
        RecvEvent();
        break;
    case FD_WRITE:
        SendEvent();
        break;
    case FD_CLOSE:
        SINGLETON(CLogManager)->PrintLog(L"FD_CLOSE", WSAGetLastError());
        MessageBox(hWnd, L"You Died........ ", MB_OK, 0);
        PostQuitMessage(0);
        break;
    }
}
void RecvEvent()
{
    int directEnQSize = g_RecvRingBuffer.GetDirectEnqueueSize();
    char* tempBuffer = g_RecvRingBuffer.GetRearBufferPtr();

    if (g_RecvRingBuffer.GetFreeSize() == 0)
    {
        SINGLETON(CLogManager)->PrintLog(L"ErrorLog.txt", L"Recv() error", WSAGetLastError());
    }
    int recvRtn = recv(g_Socket, tempBuffer, directEnQSize, 0);

#ifdef _DEBUG
    SINGLETON(CLogManager)->PrintConsoleLog(L"RecvEvent RecvReturn:%d\n", recvRtn);
#endif
    if (recvRtn <= 0)
    {
#ifdef _DEBUG
        SINGLETON(CLogManager)->PrintConsoleLog(L"Recv Error:%d\n", recvRtn);
        SINGLETON(CLogManager)->PrintLog(L"ErrorLog.txt", L"Recv() error", WSAGetLastError());
#endif
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            MessageBox(g_hWnd, L"Close or Recv error", MB_OK, 0);
            return;
        }
    }
    g_RecvRingBuffer.MoveRear(recvRtn);
    Marshalling();
}


//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_ACTIVATEAPP:
        //g_bActiveWindow = (BOOL)wParam;
        break;
      
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            StretchDIBits(hdc, 0, 0, WINSIZE_WIDTH, WINSIZE_HEIGHT, 0, 0, WINSIZE_WIDTH, WINSIZE_HEIGHT, CBackBuffer::GetInstance()->GetBackBuffer(), CBackBuffer::GetInstance()->GetDIBInfo(), DIB_RGB_COLORS, SRCCOPY);
            TextOut(hdc, 10, 20, g_FPSBuf, (int)wcslen(g_FPSBuf));
            EndPaint(hWnd, &ps);
            //TextOut(hdc, 10, 20, g_FPSBuf, (int)wcslen(g_FPSBuf));
        }
        break;

    case WM_NETWORK:
        SelectProcess(hWnd, message, wParam, lParam);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
