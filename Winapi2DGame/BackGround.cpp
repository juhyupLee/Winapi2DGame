#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include "GlobalVariable.h"
#include "BackBuffer.h"
#include "Sprite.h"
#include "BackGround.h"
#include "SpriteManager.h"


CBackGround::CBackGround(IBaseScene* pScene)
    :
    IBaseObject(pScene),
    m_Sprite(nullptr)
{
     m_Sprite = new CSprite(L"./Sprite_Data/_Map.bmp");
}

CBackGround::~CBackGround()
{
    delete m_Sprite;
}

void CBackGround::Render()
{
    __int32 iWidth = SINGLETON(CSpriteManager)->GetSprite(IBaseObject::BACKGROUND, 0, 0, 0)->m_iWidth;
    __int32 iHeight = SINGLETON(CSpriteManager)->GetSprite(IBaseObject::BACKGROUND, 0, 0, 0)->m_iHeight;
    __int32 iPitch = SINGLETON(CSpriteManager)->GetSprite(IBaseObject::BACKGROUND, 0, 0, 0)->m_iPitch;
    BYTE* bypImage = SINGLETON(CSpriteManager)->GetSprite(IBaseObject::BACKGROUND, 0, 0, 0)->m_bypImage;
    CBackBuffer::GetInstance()->DrawSprite(0, 0,0,0, iWidth, iHeight, iPitch, bypImage);
}

bool CBackGround::Update()
{
    return true;
}

__int32 CBackGround::GetType()
{
    return 0;
}

RECT CBackGround::GetHitBox()
{
    return RECT();
}

RECT CBackGround::GetCollisionBox()
{
    return RECT();
}


