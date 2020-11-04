#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Monster.h"
#include "Player.h"
#include "GlobalVariable.h"
#include "PacketDefine.h"
//#include "MemoryTracer.h"

#include "BackBuffer.h"
#include <cassert>

CMonster::CMonster()
    :
    IBaseObject(),
    m_iStatus(0),
    m_iDirection(0),
    m_iAni_CurrentFrame(0),
    m_bActionStand(false),
    m_dwActionCur(0),
    m_stCurrentSprite(nullptr),
    m_rectHitBox{0,},
    m_rectCollisionBox{0,},
    m_bFirst(true),
    m_iMoveDirection(0),
    m_iSpeed(3),
    m_iHP(100)
   
{
    m_stCurrentSprite = SINGLETON(CSpriteManager)->GetSprite(IBaseObject::PLAYER, RIGHT, STAND, 0);
    m_iX = 150;
    m_iY = 150;
}
CMonster::~CMonster()
{

}
void CMonster::Render()
{
    //----------------------------------------
    // HP ������ 
    //----------------------------------------
    SINGLETON(CBackBuffer)->DrawSprite(m_iX - 30, m_iY - 120, SINGLETON(CSpriteManager)->GetSprite(IBaseObject::HP_BAR, 0, 0, 0), 0xffffffff);
    //----------------------------------------
    // �׸��� ��������Ʈ
    //----------------------------------------
    SINGLETON(CBackBuffer)->DrawSprite(m_iX , m_iY, SINGLETON(CSpriteManager)->GetSprite(IBaseObject::SHADOW, 0, 0, 0), 0x00ffffff);                 
    //----------------------------------------
    // ĳ���� ��������Ʈ 
    //----------------------------------------
    SINGLETON(CBackBuffer)->DrawSpriteRed1(m_iX, m_iY, m_stCurrentSprite, 0xffffffff);
    HDC hdc = GetDC(g_hWnd);
   
    WCHAR buf[1024];
    wsprintf(buf, L"X:%d Y:%d", (int)m_iX, (int)m_iY);
    TextOut(hdc, 5, 5, buf, (int)wcslen(buf));


    //Rectangle(hdc, m_rectCollisionBox.left, m_rectCollisionBox.top, m_rectCollisionBox.right, m_rectCollisionBox.bottom);
    Rectangle(hdc, m_rectHitBox.left, m_rectHitBox.top, m_rectHitBox.right, m_rectHitBox.bottom);
    ReleaseDC(g_hWnd, hdc);
}

bool CMonster::Update()
{
   // KeyProcess();

    //---------------------------------------------------------
    // GameScene���� Űó�� or ��Ʈ��ũ ó�� --> ActionInput() ȣ�� -->  ���¿� ����,������ ���� 
    //---------------------------------------------------------

    if (GetAsyncKeyState(VK_HOME))
    {
        --m_iY;
    }
    if (GetAsyncKeyState(VK_END))
    {
        ++m_iY;
    }
    if (GetAsyncKeyState(VK_DELETE))
    {
        --m_iX;
    }
    if (GetAsyncKeyState(VK_NEXT))
    {
        ++m_iX;
    }

    ActionProc();
    m_iAni_CurrentFrame = GetSprite();
    NextFrame();
    //---------------------------------------------------------
    // �Է� ����� �ش��ϴ� Sprite ����
    //---------------------------------------------------------
    m_stCurrentSprite = SINGLETON(CSpriteManager)->GetSprite(IBaseObject::PLAYER, m_iDirection, m_iStatus, m_iAni_CurrentFrame);
   
    UpdateRect();

    return true;
}

__int32 CMonster::GetType()
{
    return PLAYER;
}

void CMonster::ActionInput(__int32 iStatus, __int32 iDirection, __int32 iMoveDirection)
{
    if (m_bActionStand)
    {
        return;
    }
    if (m_bFirst)
    {
        m_bFirst = false;
        SetSprite(0,
            SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, iStatus)->m_iFrameMax,
            SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, iStatus)->m_iDelay);
    }
    if (m_iStatus != iStatus || m_iDirection != iDirection)
    {
        SetSprite(0,
            SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, iStatus)->m_iFrameMax,
            SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::PLAYER, iStatus)->m_iDelay);
    }
    m_iStatus = iStatus;
    m_iDirection = iDirection;
    m_iMoveDirection = iMoveDirection;
}

void CMonster::ActionProc()
{
    //--------------------------------------
    // ��� ���۽�(����)�� ��� ���������� �ش�  ���� ó���� �Ϸ��ؾ߸� �Ѵ�.
    //--------------------------------------
    switch (m_iStatus)
    {
        //--------------------------------------
        // ���� ���۶Ǵ� ������ ������ �ִϸ��̼��� ���������� ���������� �ִϸ��̼��� �Ǿ���ϸ�,
        // �ִϸ��̼��� ���� �� �⺻�������� �ڵ����� ���ư��� �Ѵ�.
        //--------------------------------------
    case ATTACK1:
    case ATTACK2:
    case ATTACK3:
        m_bActionStand = true;
        if (IsEndFrame())
        {
            m_bActionStand = false;
        }
        break;
    default:
        InputActionProc();
        break;
    }

}

void CMonster::InputActionProc()
{
    switch (m_iStatus)
    {
    case MOVE:
        if (m_iMoveDirection == LEFT)
        {
            m_iX -= m_iSpeed;

        }
        else if (m_iMoveDirection == LEFT_UP)
        {
            m_iX -= m_iSpeed;
            m_iY -= m_iSpeed;
        }
        else if (m_iMoveDirection == RIGHT_UP)
        {
            m_iX += m_iSpeed;
            m_iY -= m_iSpeed;
        }
        else if (m_iMoveDirection == RIGHT)
        {
            m_iX += m_iSpeed;
        }
        else if (m_iMoveDirection == RIGHT_DOWN)
        {
            m_iX += m_iSpeed;
            m_iY += m_iSpeed;
        }
        else if (m_iMoveDirection == LEFT_DOWN)
        {
            m_iX -= m_iSpeed;
            m_iY += m_iSpeed;
        }
        else if (m_iMoveDirection == UP)
        {
            m_iY -= m_iSpeed;
        }
        else if (m_iMoveDirection == DOWN)
        {
            m_iY += m_iSpeed;
        }
        break;
    }

    //----------------------------
    // ȭ�� �̵����� ����
    //----------------------------
    if (m_iY < RANGE_MOVE_TOP)
    {
        m_iY = RANGE_MOVE_TOP;
    }
    if (m_iY > RANGE_MOVE_BOTTOM)
    {
        m_iY = RANGE_MOVE_BOTTOM;
    }
    if (m_iX > RANGE_MOVE_RIGHT)
    {
        m_iX = RANGE_MOVE_RIGHT;
    }
    if (m_iX < RANGE_MOVE_LEFT)
    {
        m_iX = RANGE_MOVE_LEFT;
    }

}

void CMonster::UpdateRect()
{
    RECT rectCollisonBox = m_stCurrentSprite->m_rectCollisionBox;
    RECT rectHitBox = m_stCurrentSprite->m_rectHitBox;
    __int32 originX = m_stCurrentSprite->m_iOriginX;
    __int32 originY = m_stCurrentSprite->m_iOriginY;

    int zeroPointX = m_iX - originX;
    int zeroPointY = m_iY - originY;

    int rectLeft = zeroPointX + rectCollisonBox.left;
    int rectTop = zeroPointY + rectCollisonBox.top;
    int rectRight = rectLeft + rectCollisonBox.right - rectCollisonBox.left;
    int rectBottom = rectTop + rectCollisonBox.bottom - rectCollisonBox.top;

    m_rectCollisionBox.left = rectLeft;
    m_rectCollisionBox.top = rectTop;
    m_rectCollisionBox.right = rectRight;
    m_rectCollisionBox.bottom = rectBottom;

    rectLeft = zeroPointX + rectHitBox.left;
    rectTop = zeroPointY + rectHitBox.top;
    rectRight = rectLeft + rectHitBox.right - rectHitBox.left;
    rectBottom = rectTop + rectHitBox.bottom - rectHitBox.top;

    m_rectHitBox.left = rectLeft;
    m_rectHitBox.top = rectTop;
    m_rectHitBox.right = rectRight;
    m_rectHitBox.bottom = rectBottom;

}

