/**
>Winapi 2D Action Game (네트워크)
>Lee Juhyup
>개발중...
**/
#pragma comment(lib,"imm32.lib")
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
#include "MemoryTracer.h"

#define MAX_LOADSTRING 100



// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


// 전역 변수:
BOOL Init_Window(HINSTANCE hInstance);
void Init_Game();

HWND g_hWnd = 0;
RECT g_WindowRect;
BOOL g_bActiveWindow;

WCHAR g_FPSBuf[100];

HIMC g_hOldIMC;

int g_iLogicCount = 0 ;
int g_iRenderCount=0;

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
            SINGLETON(CTimeManager)->GameTimeStart();
            if (!SINGLETON(CSceneManager)->Run())    
            {
                break;
            }
            ++fpsCount;
            ProfileEnd(L"GameLogicLoop");
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
        g_bActiveWindow = (BOOL)wParam;
        break;
       
    case WM_LBUTTONDOWN:
    {
        WCHAR buf[1024];
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
        wsprintf(buf, L"X:%d Y:%d", x, y);
        MessageBox(hWnd, buf, MB_OK, MB_OK);
        break;
    }
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
