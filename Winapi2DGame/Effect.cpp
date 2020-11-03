#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include "GlobalVariable.h"
#include "BackBuffer.h"
#include "Sprite.h"
#include "Effect.h"
#include "SpriteManager.h"


Effect::Effect(IBaseScene* pScene,int x ,int y)
    :
    IBaseObject(pScene,x,y),
    m_stCurrentSprite(nullptr)
{
     //m_Sprite = new CSprite(L"./Sprite_Data/_Map.bmp");
    SetSprite(0,
        SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::EFFECT, 0)->m_iFrameMax,
        SINGLETON(CSpriteManager)->GetAnimationStatus(IBaseObject::EFFECT, 0)->m_iDelay);

    m_stCurrentSprite = SINGLETON(CSpriteManager)->GetSprite(IBaseObject::EFFECT, 0, 0, 0);


}

Effect::~Effect()
{
}

void Effect::Render()
{
    SINGLETON(CBackBuffer)->DrawSprite(m_iX, m_iY-50, m_stCurrentSprite, 0xffffffff);
}

bool Effect::Update()
{
    NextFrame();
    m_stCurrentSprite = SINGLETON(CSpriteManager)->GetSprite(IBaseObject::EFFECT, 0, 0, m_iSpriteNow);

    if (IsEndFrame())
    {
        return false;
    }
    return true;
}

__int32 Effect::GetType()
{
    return 0;
}

RECT Effect::GetHitBox()
{
    return RECT();
}

RECT Effect::GetCollisionBox()
{
    return RECT();
}


