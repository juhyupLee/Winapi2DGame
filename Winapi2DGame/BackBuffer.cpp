#include <iostream>
#include <Windows.h>
#include "BaseScene.h"
#include "BaseObject.h"
#include "SpriteManager.h"
#include "Player.h"
#include "GlobalVariable.h"
#include "BackBuffer.h"
#include "MemoryTracer.h"

CBackBuffer* CBackBuffer::GetInstance()
{
    static CBackBuffer backBuffer;
    return &backBuffer;
}

BYTE* CBackBuffer::GetBackBuffer()
{
    return m_Buffer;
}

BITMAPINFO* CBackBuffer::GetDIBInfo()
{
    return &m_DIBInfo;
}

int32_t CBackBuffer::GetFileSize()
{
    return m_FileSize;
}

void CBackBuffer::DrawSprite(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen )
{
    //------------------------------------
    //> dest는 백버퍼에 그릴  시작 위치
    //> src는  소스에 그릴 시작 위치
    //------------------------------------
    int clippingRight = srcWidth;      //> 우측 클리핑 시, 어디까지 그려야되는지에 대한 범위
    int clippingLeft = 0;              //> 좌측 클리핑 시, 어디부터 시작해야될지
    int clippingUp = 0;                //> 위쪽 클리핑 시, 어디부터 시작해야될지
    int clippingDown = srcHeight;      //> 아래 클리핑 시, 어디까지 그려야되는지에 대한 범위

    if (destY < 0 )
    {
        clippingUp = -destY;
        destY = 0;
    }    
    if (destY > WINSIZE_HEIGHT - srcHeight)     //> 만일 클리핑 영역 아래를 침범 하면
    {
        clippingDown = WINSIZE_HEIGHT - destY;  //> src는 0까지 그리되,destY~ winsize Y까지 밖에 그릴 수박에 없다
    }   
    if (destX < 0)       //>왼쪽 영역을 넘어가서 클리핑을 해줘야 할 경우
    {
        //ㅁ|------------|
        //  |------------|
        clippingLeft = -destX;      //>절대값을 구해서 그만큼을 빼고 Sprite를 그리기
        destX = 0;
    }
    if (destX+srcWidth > WINSIZE_WIDTH)     //>오른쪽 영역을 넘어가서 클리핑을 해줘야 할 경우
    {
        //|------------|ㅁ
        //|------------|
        //넘어간 영역 구해주기
        clippingRight = WINSIZE_WIDTH - destX;
    }
    
    int startX = destX*4;                                   //> 백버퍼에서 Sprite를  그릴 시작위치 X
    int startY = destY * m_Pitch;                           //> 백버퍼에서 Sprite를  그릴 시작위치 Y
    
    int cpyX = (srcX + clippingLeft)*4;                     //> Sprite 파일의 시작좌표 보통 0으로 세팅 만약에, 클리핑 왼쪽영역을 침범했을 경우, 
    int cpyY = (srcY+ clippingUp) * srcPitch;               //> 그만큼 침범영역을 제외하고 그려야 함

    BYTE* byteDestPointer = m_Buffer + startX + startY;     //> 백버퍼에 Sprite 시작위치를 고려한 버퍼 포인터    
    BYTE* byteSrcPointer = srcBuffer + cpyX+ cpyY;          //> 스프라이트 파일 버퍼 포인터

      
    DWORD* dwSrcPointer = (DWORD*)byteSrcPointer;           //> RGBA 값을 고려하여 4바이트 단위로 포인터 연산이 필요하므로,
    DWORD* dwDestPointer = (DWORD*)byteDestPointer;         //> DWORD 포인터 형태로 캐스팅

    // 출력 율 지정 (0~100%)
    clippingRight =(__int32)((double)clippingRight*(iDrawLen / 100.0));

    for (int heightCnt = clippingUp; heightCnt < clippingDown; ++heightCnt) //> 특별한 이상이 없다면 0~Width까지 그리겠지만
    {                                                                       //> 클리핑 처리를 해야하는 경우, Sprite의 일부만 그려야 한다.
 
        for (int widthCnt = clippingLeft; widthCnt < clippingRight; ++widthCnt)
        {
                
                if (*dwSrcPointer != 0xffffffff)                //> Sprite버퍼의 픽셀이 흰색이 아니라면 RGB(255,255,255,255)(알파값 포함)
                {
                    *dwDestPointer = *dwSrcPointer;          //>백버퍼 포인터에 그 Sprite파일의 픽셀을 쓴다.
                }
                dwSrcPointer++;
                dwDestPointer++;
        }
        byteSrcPointer += srcPitch;
        dwSrcPointer = (DWORD*)byteSrcPointer;
        byteDestPointer += m_Pitch;
        dwDestPointer =(DWORD*) byteDestPointer;
    }
}

void CBackBuffer::DrawSprite(int destX, int destY, CSpriteManager::stSprite* pSprite, DWORD dwColorKey, __int32 iDrawLen )
{
    if (pSprite == nullptr)
    {
        return;
    }
    int srcWidth = pSprite->m_iWidth;
    //---------------------------------------
    //출력 율 지정 (0~100%)
    //---------------------------------------
    srcWidth = (__int32)((double)srcWidth * (iDrawLen / 100.0));
    //srcWidth = (__int32)(srcWidth * iDrawLen / 100);
    int srcHeight = pSprite->m_iHeight;
    int srcPitch = pSprite->m_iPitch;
    BYTE* srcBuffer = pSprite->m_bypImage;

    destX -= pSprite->m_iOriginX;
    destY -= pSprite->m_iOriginY;

    //------------------------------------
    //> dest는 백버퍼에 그릴  시작 위치
    //> src는  소스에 그릴 시작 위치
    //------------------------------------
    int clippingRight = srcWidth;      //> 우측 클리핑 시, 어디까지 그려야되는지에 대한 범위
    int clippingLeft = 0;              //> 좌측 클리핑 시, 어디부터 시작해야될지
    int clippingUp = 0;                //> 위쪽 클리핑 시, 어디부터 시작해야될지
    int clippingDown = srcHeight;      //> 아래 클리핑 시, 어디까지 그려야되는지에 대한 범위

    if (destY < 0)
    {
        clippingUp = -destY;
        destY = 0;
    }
    if (destY > WINSIZE_HEIGHT - srcHeight)     //> 만일 클리핑 영역 아래를 침범 하면
    {
        clippingDown = WINSIZE_HEIGHT - destY;  //> src는 0까지 그리되,destY~ winsize Y까지 밖에 그릴 수박에 없다
    }
    if (destX < 0)       //>왼쪽 영역을 넘어가서 클리핑을 해줘야 할 경우
    {
        //ㅁ|------------|
        //  |------------|
        clippingLeft = -destX;      //>절대값을 구해서 그만큼을 빼고 Sprite를 그리기
        destX = 0;
    }
    if (destX + srcWidth > WINSIZE_WIDTH)     //>오른쪽 영역을 넘어가서 클리핑을 해줘야 할 경우
    {
        //|------------|ㅁ
        //|------------|
        //넘어간 영역 구해주기
        clippingRight = WINSIZE_WIDTH - destX;
    }

    int startX = destX * 4;                                   //> 백버퍼에서 Sprite를  그릴 시작위치 X
    int startY = destY * m_Pitch;                           //> 백버퍼에서 Sprite를  그릴 시작위치 Y

    int cpyX = clippingLeft * 4;                     //> Sprite 파일의 시작좌표 보통 0으로 세팅 만약에, 클리핑 왼쪽영역을 침범했을 경우, 
    int cpyY = clippingUp * srcPitch;               //> 그만큼 침범영역을 제외하고 그려야 함

    BYTE* byteDestPointer = m_Buffer + startX + startY;     //> 백버퍼에 Sprite 시작위치를 고려한 버퍼 포인터    
    BYTE* byteSrcPointer = srcBuffer + cpyX + cpyY;          //> 스프라이트 파일 버퍼 포인터


    DWORD* dwSrcPointer = (DWORD*)byteSrcPointer;           //> RGBA 값을 고려하여 4바이트 단위로 포인터 연산이 필요하므로,
    DWORD* dwDestPointer = (DWORD*)byteDestPointer;         //> DWORD 포인터 형태로 캐스팅

    for (int heightCnt = clippingUp; heightCnt < clippingDown; ++heightCnt) //> 특별한 이상이 없다면 0~Width까지 그리겠지만
    {                                                                       //> 클리핑 처리를 해야하는 경우, Sprite의 일부만 그려야 한다.

        for (int widthCnt = clippingLeft; widthCnt < clippingRight; ++widthCnt)
        {

            if (*dwSrcPointer != dwColorKey)                //> Sprite버퍼의 픽셀이 흰색이 아니라면 RGB(255,255,255,255)(알파값 포함)
            {
                *dwDestPointer = *dwSrcPointer;          //>백버퍼 포인터에 그 Sprite파일의 픽셀을 쓴다.
            }
           
            dwSrcPointer++;
            dwDestPointer++;
        }
        byteSrcPointer += srcPitch;
        dwSrcPointer = (DWORD*)byteSrcPointer;
        byteDestPointer += m_Pitch;
        dwDestPointer = (DWORD*)byteDestPointer;
    }

}
//void CBackBuffer::DrawSprite50(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen)
//{
//
//}

void CBackBuffer::DrawSpriteRed1(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen)
{
    //------------------------------------
    //> dest는 백버퍼에 그릴  시작 위치
    //> src는  소스에 그릴 시작 위치
    //------------------------------------
    int clippingRight = srcWidth;      //> 우측 클리핑 시, 어디까지 그려야되는지에 대한 범위
    int clippingLeft = 0;       //> 좌측 클리핑 시, 어디부터 시작해야될지
    int clippingUp = 0;         //> 위쪽 클리핑 시, 어디부터 시작해야될지
    int clippingDown = srcHeight;       //> 아래 클리핑 시, 어디까지 그려야되는지에 대한 범위

    if (destY < 0)
    {
        clippingUp = -destY;
        destY = 0;
    }
    if (destY > WINSIZE_HEIGHT - srcHeight)     //> 만일 클리핑 영역 아래를 침범 하면
    {
        clippingDown = WINSIZE_HEIGHT - destY;  //> src는 0까지 그리되,destY~ winsize Y까지 밖에 그릴 수박에 없다
    }
    if (destX < 0)       //>왼쪽 영역을 넘어가서 클리핑을 해줘야 할 경우
    {
        //ㅁ|------------|
        //  |------------|
        clippingLeft = -destX;      //>절대값을 구해서 그만큼을 빼고 Sprite를 그리기
        destX = 0;
    }
    if (destX + srcWidth > WINSIZE_WIDTH)     //>오른쪽 영역을 넘어가서 클리핑을 해줘야 할 경우
    {
        //|------------|ㅁ
        //|------------|
        //넘어간 영역 구해주기
        clippingRight = WINSIZE_WIDTH - destX;
    }

    int startX = destX * 4;                                   //> 백버퍼에서 Sprite를  그릴 시작위치 X
    int startY = destY * m_Pitch;                           //> 백버퍼에서 Sprite를  그릴 시작위치 Y

    int cpyX = (srcX + clippingLeft) * 4;                     //> Sprite 파일의 시작좌표 보통 0으로 세팅 만약에, 클리핑 왼쪽영역을 침범했을 경우, 
    int cpyY = (srcY + clippingUp) * srcPitch;               //> 그만큼 침범영역을 제외하고 그려야 함

    BYTE* byteDestPointer = m_Buffer + startX + startY;     //> 백버퍼에 Sprite 시작위치를 고려한 버퍼 포인터    
    BYTE* byteSrcPointer = srcBuffer + cpyX + cpyY;          //> 스프라이트 파일 버퍼 포인터


    DWORD* dwSrcPointer = (DWORD*)byteSrcPointer;           //> RGBA 값을 고려하여 4바이트 단위로 포인터 연산이 필요하므로,
    DWORD* dwDestPointer = (DWORD*)byteDestPointer;         //> DWORD 포인터 형태로 캐스팅

    // 출력 율 지정 (0~100%)
    clippingRight = (__int32)((double)clippingRight * (iDrawLen / 100.0));

    for (int heightCnt = clippingUp; heightCnt < clippingDown; ++heightCnt) //> 특별한 이상이 없다면 0~Width까지 그리겠지만
    {                                                                       //> 클리핑 처리를 해야하는 경우, Sprite의 일부만 그려야 한다.

        for (int widthCnt = clippingLeft; widthCnt < clippingRight; ++widthCnt)
        {

            if (*dwSrcPointer != 0xffffffff)                //> Sprite버퍼의 픽셀이 흰색이 아니라면 RGB(255,255,255,255)(알파값 포함)
            {
                BYTE* bypTempSrc = (BYTE*)dwSrcPointer;
                *(bypTempSrc + 2) = 0xff;
                *dwDestPointer = *dwSrcPointer;          //>백버퍼 포인터에 그 Sprite파일의 픽셀을 쓴다.
            }
            dwSrcPointer++;
            dwDestPointer++;
        }
        byteSrcPointer += srcPitch;
        dwSrcPointer = (DWORD*)byteSrcPointer;
        byteDestPointer += m_Pitch;
        dwDestPointer = (DWORD*)byteDestPointer;
    }
}

void CBackBuffer::DrawSpriteRed1(int destX, int destY, CSpriteManager::stSprite* pSprite, DWORD dwColorKey, __int32 iDrawLen)
{
    if (pSprite == nullptr)
    {
        return;
    }
    int srcWidth = pSprite->m_iWidth;
    //---------------------------------------
    //출력 율 지정 (0~100%)
    //---------------------------------------
    srcWidth = (__int32)((double)srcWidth * (iDrawLen / 100.0));

    int srcHeight = pSprite->m_iHeight;
    int srcPitch = pSprite->m_iPitch;
    BYTE* srcBuffer = pSprite->m_bypImage;

    destX -= pSprite->m_iOriginX;
    destY -= pSprite->m_iOriginY;

    //------------------------------------
    //> dest는 백버퍼에 그릴  시작 위치
    //> src는  소스에 그릴 시작 위치
    //------------------------------------
    int clippingRight = srcWidth;      //> 우측 클리핑 시, 어디까지 그려야되는지에 대한 범위
    int clippingLeft = 0;              //> 좌측 클리핑 시, 어디부터 시작해야될지
    int clippingUp = 0;                //> 위쪽 클리핑 시, 어디부터 시작해야될지
    int clippingDown = srcHeight;      //> 아래 클리핑 시, 어디까지 그려야되는지에 대한 범위

    if (destY < 0)
    {
        clippingUp = -destY;
        destY = 0;
    }
    if (destY > WINSIZE_HEIGHT - srcHeight)     //> 만일 클리핑 영역 아래를 침범 하면
    {
        clippingDown = WINSIZE_HEIGHT - destY;  //> src는 0까지 그리되,destY~ winsize Y까지 밖에 그릴 수박에 없다
    }
    if (destX < 0)       //>왼쪽 영역을 넘어가서 클리핑을 해줘야 할 경우
    {
        //ㅁ|------------|
        //  |------------|
        clippingLeft = -destX;      //>절대값을 구해서 그만큼을 빼고 Sprite를 그리기
        destX = 0;
    }
    if (destX + srcWidth > WINSIZE_WIDTH)     //>오른쪽 영역을 넘어가서 클리핑을 해줘야 할 경우
    {
        //|------------|ㅁ
        //|------------|
        //넘어간 영역 구해주기
        clippingRight = WINSIZE_WIDTH - destX;
    }

    int startX = destX * 4;                                   //> 백버퍼에서 Sprite를  그릴 시작위치 X
    int startY = destY * m_Pitch;                           //> 백버퍼에서 Sprite를  그릴 시작위치 Y

    int cpyX = clippingLeft * 4;                     //> Sprite 파일의 시작좌표 보통 0으로 세팅 만약에, 클리핑 왼쪽영역을 침범했을 경우, 
    int cpyY = clippingUp * srcPitch;               //> 그만큼 침범영역을 제외하고 그려야 함

    BYTE* byteDestPointer = m_Buffer + startX + startY;     //> 백버퍼에 Sprite 시작위치를 고려한 버퍼 포인터    
    BYTE* byteSrcPointer = srcBuffer + cpyX + cpyY;          //> 스프라이트 파일 버퍼 포인터


    DWORD* dwSrcPointer = (DWORD*)byteSrcPointer;           //> RGBA 값을 고려하여 4바이트 단위로 포인터 연산이 필요하므로,
    DWORD* dwDestPointer = (DWORD*)byteDestPointer;         //> DWORD 포인터 형태로 캐스팅

    for (int heightCnt = clippingUp; heightCnt < clippingDown; ++heightCnt) //> 특별한 이상이 없다면 0~Width까지 그리겠지만
    {                                                                       //> 클리핑 처리를 해야하는 경우, Sprite의 일부만 그려야 한다.

        for (int widthCnt = clippingLeft; widthCnt < clippingRight; ++widthCnt)
        {

            if (*dwSrcPointer != dwColorKey)                //> Sprite버퍼의 픽셀이 흰색이 아니라면 RGB(255,255,255,255)(알파값 포함)
            {
                //BYTE* bypTempSrc = (BYTE*)dwSrcPointer;
                //*(bypTempSrc + 2) = 0xff;
                *dwDestPointer = *dwSrcPointer;          //>백버퍼 포인터에 그 Sprite파일의 픽셀을 쓴다.
                BYTE* bypTempDest = (BYTE*)dwDestPointer;
                *(bypTempDest + 2) = 0xff;
            }

            dwSrcPointer++;
            dwDestPointer++;
        }
        byteSrcPointer += srcPitch;
        dwSrcPointer = (DWORD*)byteSrcPointer;
        byteDestPointer += m_Pitch;
        dwDestPointer = (DWORD*)byteDestPointer;
    }

}

void CBackBuffer::DrawSpriteTransParent(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen)
{
    //------------------------------------
//> dest는 백버퍼에 그릴  시작 위치
//> src는  소스에 그릴 시작 위치
//------------------------------------
    int clippingRight = srcWidth;      //> 우측 클리핑 시, 어디까지 그려야되는지에 대한 범위
    int clippingLeft = 0;       //> 좌측 클리핑 시, 어디부터 시작해야될지
    int clippingUp = 0;         //> 위쪽 클리핑 시, 어디부터 시작해야될지
    int clippingDown = srcHeight;       //> 아래 클리핑 시, 어디까지 그려야되는지에 대한 범위

    if (destY < 0)
    {
        clippingUp = -destY;
        destY = 0;
    }
    if (destY > WINSIZE_HEIGHT - srcHeight)     //> 만일 클리핑 영역 아래를 침범 하면
    {
        clippingDown = WINSIZE_HEIGHT - destY;  //> src는 0까지 그리되,destY~ winsize Y까지 밖에 그릴 수박에 없다
    }
    if (destX < 0)       //>왼쪽 영역을 넘어가서 클리핑을 해줘야 할 경우
    {
        //ㅁ|------------|
        //  |------------|
        clippingLeft = -destX;      //>절대값을 구해서 그만큼을 빼고 Sprite를 그리기
        destX = 0;
    }
    if (destX + srcWidth > WINSIZE_WIDTH)     //>오른쪽 영역을 넘어가서 클리핑을 해줘야 할 경우
    {
        //|------------|ㅁ
        //|------------|
        //넘어간 영역 구해주기
        clippingRight = WINSIZE_WIDTH - destX;
    }

    int startX = destX * 4;                                   //> 백버퍼에서 Sprite를  그릴 시작위치 X
    int startY = destY * m_Pitch;                           //> 백버퍼에서 Sprite를  그릴 시작위치 Y

    int cpyX = (srcX + clippingLeft) * 4;                     //> Sprite 파일의 시작좌표 보통 0으로 세팅 만약에, 클리핑 왼쪽영역을 침범했을 경우, 
    int cpyY = (srcY + clippingUp) * srcPitch;               //> 그만큼 침범영역을 제외하고 그려야 함

    BYTE* byteDestPointer = m_Buffer + startX + startY;     //> 백버퍼에 Sprite 시작위치를 고려한 버퍼 포인터    
    BYTE* byteSrcPointer = srcBuffer + cpyX + cpyY;          //> 스프라이트 파일 버퍼 포인터


    DWORD* dwSrcPointer = (DWORD*)byteSrcPointer;           //> RGBA 값을 고려하여 4바이트 단위로 포인터 연산이 필요하므로,
    DWORD* dwDestPointer = (DWORD*)byteDestPointer;         //> DWORD 포인터 형태로 캐스팅

    // 출력 율 지정 (0~100%)
    clippingRight = (__int32)((double)clippingRight * (iDrawLen / 100.0));

    for (int heightCnt = clippingUp; heightCnt < clippingDown; ++heightCnt) //> 특별한 이상이 없다면 0~Width까지 그리겠지만
    {                                                                       //> 클리핑 처리를 해야하는 경우, Sprite의 일부만 그려야 한다.

        for (int widthCnt = clippingLeft; widthCnt < clippingRight; ++widthCnt)
        {

            if (*dwSrcPointer != 0xffffffff)                //> Sprite버퍼의 픽셀이 흰색이 아니라면 RGB(255,255,255,255)(알파값 포함)
            {
                BYTE* bypTempSrc = (BYTE*)dwSrcPointer;
                int id = (*bypTempSrc + *(bypTempSrc + 1) + *(bypTempSrc + 2)) / 20;
                *dwDestPointer = *(dwSrcPointer+id) & 0xf0f0f0f0;          //>백버퍼 포인터에 그 Sprite파일의 픽셀을 쓴다.
            }
            dwSrcPointer++;
            dwDestPointer++;
        }
        byteSrcPointer += srcPitch;
        dwSrcPointer = (DWORD*)byteSrcPointer;
        byteDestPointer += m_Pitch;
        dwDestPointer = (DWORD*)byteDestPointer;
    }
}

void CBackBuffer::DrawImage(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen)
{
    //------------------------------------
//> dest는 백버퍼에 그릴  시작 위치
//> src는  소스에 그릴 시작 위치
//------------------------------------
    int clippingRight = srcWidth;      //> 우측 클리핑 시, 어디까지 그려야되는지에 대한 범위
    int clippingLeft = 0;       //> 좌측 클리핑 시, 어디부터 시작해야될지
    int clippingUp = 0;         //> 위쪽 클리핑 시, 어디부터 시작해야될지
    int clippingDown = srcHeight;       //> 아래 클리핑 시, 어디까지 그려야되는지에 대한 범위

    if (destY < 0)
    {
        clippingUp = -destY;
        destY = 0;
    }
    if (destY > WINSIZE_HEIGHT - srcHeight)     //> 만일 클리핑 영역 아래를 침범 하면
    {
        clippingDown = WINSIZE_HEIGHT - destY;  //> src는 0까지 그리되,destY~ winsize Y까지 밖에 그릴 수박에 없다
    }
    if (destX < 0)       //>왼쪽 영역을 넘어가서 클리핑을 해줘야 할 경우
    {
        //ㅁ|------------|
        //  |------------|
        clippingLeft = -destX;      //>절대값을 구해서 그만큼을 빼고 Sprite를 그리기
        destX = 0;
    }
    if (destX + srcWidth > WINSIZE_WIDTH)     //>오른쪽 영역을 넘어가서 클리핑을 해줘야 할 경우
    {
        //|------------|ㅁ
        //|------------|
        //넘어간 영역 구해주기
        clippingRight = WINSIZE_WIDTH - destX;
    }

    int startX = destX * 4;                                   //> 백버퍼에서 Sprite를  그릴 시작위치 X
    int startY = destY * m_Pitch;                           //> 백버퍼에서 Sprite를  그릴 시작위치 Y

    int cpyX = (srcX + clippingLeft) * 4;                     //> Sprite 파일의 시작좌표 보통 0으로 세팅 만약에, 클리핑 왼쪽영역을 침범했을 경우, 
    int cpyY = (srcY + clippingUp) * srcPitch;               //> 그만큼 침범영역을 제외하고 그려야 함

    BYTE* byteDestPointer = m_Buffer + startX + startY;     //> 백버퍼에 Sprite 시작위치를 고려한 버퍼 포인터    
    BYTE* byteSrcPointer = srcBuffer + cpyX + cpyY;          //> 스프라이트 파일 버퍼 포인터


    DWORD* dwSrcPointer = (DWORD*)byteSrcPointer;           //> RGBA 값을 고려하여 4바이트 단위로 포인터 연산이 필요하므로,
    DWORD* dwDestPointer = (DWORD*)byteDestPointer;         //> DWORD 포인터 형태로 캐스팅

    // 출력 율 지정 (0~100%)
    clippingRight = (__int32)((double)clippingRight * (iDrawLen / 100.0));

    for (int heightCnt = clippingUp; heightCnt < clippingDown; ++heightCnt) //> 특별한 이상이 없다면 0~Width까지 그리겠지만
    {                                                                       //> 클리핑 처리를 해야하는 경우, Sprite의 일부만 그려야 한다.

        for (int widthCnt = clippingLeft; widthCnt < clippingRight; ++widthCnt)
        {
            //-------------------------------
            //이미지 출력
            //-------------------------------
            *dwDestPointer = *dwSrcPointer; 
            dwSrcPointer++;
            dwDestPointer++;
        }
        byteSrcPointer += srcPitch;
        dwSrcPointer = (DWORD*)byteSrcPointer;
        byteDestPointer += m_Pitch;
        dwDestPointer = (DWORD*)byteDestPointer;
    }
}



CBackBuffer::CBackBuffer()
    :
    m_Width(WINSIZE_WIDTH), 
    m_Height(WINSIZE_HEIGHT),
    m_Buffer(nullptr),
    m_Pitch(0)
{
    memset(&m_DIBInfo, 0, sizeof(BITMAPINFO));
    m_Pitch = ((m_Width * 4) + 3) & ~3;
    int fileSize = m_Pitch * m_Height;
    m_Buffer = new BYTE[fileSize];

    memset(m_Buffer, 128, fileSize);

    m_DIBInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_DIBInfo.bmiHeader.biWidth = m_Width;
    m_DIBInfo.bmiHeader.biHeight = -m_Height;
    m_DIBInfo.bmiHeader.biPlanes = 1;
    m_DIBInfo.bmiHeader.biBitCount = 32;
    m_DIBInfo.bmiHeader.biCompression = 0;
    m_DIBInfo.bmiHeader.biSizeImage = 0;
    m_DIBInfo.bmiHeader.biClrUsed = 0;
    m_DIBInfo.bmiHeader.biClrImportant = 0;

}

CBackBuffer::~CBackBuffer()
{
    delete[] m_Buffer;
}
