#include <iostream>
#include <Windows.h>
#include "SpriteDib.h"

CSpriteDib::CSpriteDib(int maxSprite, DWORD colorKey)
{
    m_MaxSprite = maxSprite;
    m_ColorKey = colorKey;

    m_Sprite = NULL;

    //---------------------
    //최대 읽어올 개수만큼 미리 할당받는다.
    //-----------------------------

    m_Sprite = new st_SPRITE[m_MaxSprite];
    memset(m_Sprite, 0, sizeof(st_SPRITE) * m_MaxSprite);

}

CSpriteDib::~CSpriteDib()
{
   
    //전체를 돌면서 모두 지우자
    for (int count = 0; count < m_MaxSprite; ++count)
    {
        ReleaseSprite(count);
    }
}
BOOL CSpriteDib::LoadDibSprite(__int32 spriteIndex, WCHAR* fileName, __int32 centerPointX, __int32 centerPointY)
{
    HANDLE hFile;
    DWORD read;
    int pitch;
    int imageSize;

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    //--------------------------------
    //일단 파일을 열자
    //-------------------------------

    hFile = CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        return FALSE;
    }
    ReleaseSprite(spriteIndex); //  한번 Release 하고 로딩을 하나보네??

    ReadFile(hFile, &fileHeader, sizeof(BITMAPINFOHEADER), &read, NULL);
    if (0x4d42 == fileHeader.bfType)
    {
        //-----------------------------------
        //인포헤더를 읽어서 저장 & 32비트 확인
        //-----------------------------
        ReadFile(hFile, &infoHeader, sizeof(BITMAPINFOHEADER), &read, NULL);
        if (32 == infoHeader.biBitCount)
        {
            //한줄 한줄의 피치값을 구한다.
            pitch = ((infoHeader.biWidth * 4) + 3) & ~3;
            //원장님은 괄호저렇게 안씌었는데, 어느게 맞는거지?
            //-----------------------------------------
            //스프라이트 구조체에 크기 저장
            //-----------------------------------------

            m_Sprite[spriteIndex].m_Width = infoHeader.biWidth;
            m_Sprite[spriteIndex].m_Height = infoHeader.biHeight;
            m_Sprite[spriteIndex].m_Pitch = pitch;

            //이미지에 대한 전체 크기를 구하고, 메모리할당
            imageSize = pitch * infoHeader.biHeight;
            m_Sprite[spriteIndex].m_Image = new BYTE[imageSize];

            //---------------------------------------
            //이미지 부분은 스프라이트 버퍼로 읽어온다
            //DIB는 뒤집어져 있으므로 이를 다시 뒤집자.
            BYTE* tempBuffer = new BYTE[imageSize];
            BYTE* spriteTemp = m_Sprite[spriteIndex].m_Image;
            BYTE* turnTemp;

            ReadFile(hFile, tempBuffer, imageSize, &read, NULL);
            //-----------------------
            //한줄 한줄 뒤집자
            //-----------------------
            turnTemp = tempBuffer + pitch * (infoHeader.biHeight - 1);
            
            for (int count = 0; count < infoHeader.biHeight; ++count)
            {
                memcpy(spriteTemp, turnTemp, pitch);
                spriteTemp += pitch;
                turnTemp -= pitch;
            }
            delete[] tempBuffer;

            m_Sprite[spriteIndex].m_CenterPointX = centerPointX;
            m_Sprite[spriteIndex].m_CenterPointY = centerPointY;
            CloseHandle(hFile);
            return TRUE;

        }

    }
    CloseHandle(hFile);
    return FALSE;
}

void CSpriteDib::ReleaseSprite(__int32 spriteIndex)
{
    //-----------------------
    //최대 할당된 스프라이트를 넘어서면 안돼요~
    //-----------------------
    if (m_MaxSprite <= spriteIndex)
    {
        return;
    }
    if (NULL != m_Sprite[spriteIndex].m_Image)
    {
        //--------------------------
        //삭제 후 초기화
        //--------------------------
        delete[] m_Sprite[spriteIndex].m_Image;
        memset(&m_Sprite[spriteIndex], 0, sizeof(st_SPRITE));
    }
}

void CSpriteDib::DrawSprite(__int32 spriteIndex, __int32 drawX, __int32 drawY, BYTE* dest, __int32 destWidth, __int32 destHeight, __int32 destPitch, __int32 drawLen)
{
    //--------------------------------
    //최대 스프라이트 개수를 초과하거나, 로드되지않는 스프라이트라면 무시
    //--------------------------------
    if (spriteIndex >= m_MaxSprite)
    {
        return;
    }

    if (NULL == m_Sprite[spriteIndex].m_Image)
    {
        return;
    }

    st_SPRITE* sprite = &m_Sprite[spriteIndex];

    //----------------------------------------
    //스프라이트 출력을 위해 사이즈 저장
    //----------------------------------------

    __int32 spriteWidth = sprite->m_Width;
    __int32 spriteHeight = sprite->m_Height;
    __int32 countX= 0;
    __int32 countY=0;

    //-------------------------------
    // 출력 길이 설정
    //-------------------------------
    spriteWidth = spriteWidth * drawLen / 100;

    DWORD* pdwDest = (DWORD*)dest;
    DWORD* dwpSprite = (DWORD*)(sprite->m_Image);

    //---------------------------------------------
    //상단에 대한 스프라이트 출력 위치 계산(상단 클리핑)
    //---------------------------------------------
    if(0>drawY)
    {
        spriteHeight = spriteHeight - (-drawY);
        dwpSprite = (DWORD*)(sprite->m_Image + sprite->m_Pitch * (drawY));
        //--------------------
        //위쪽이 짤리는 경우이므로 스프라이트 시작 위치를 아래로 내려준다
        //-------------------

        drawY = 0;
    }
    //--------------------------------------
     //하단에 사이즈 계산 (하단클리핑)
     //---------------------------------------
    if (destHeight < drawY + sprite->m_Height)
    {
        spriteHeight -= ((drawY + sprite->m_Height) - destHeight);
    }
    //--------------------------------
    //왼쪽 출력 위치 계산 (좌측 클리핑)
    //--------------------------------
    if (0 > drawX)
    {
        //이부분 잘 이해가 안되네????
        //spriteWidth 에서 왜 drawX만큼 더빼지?
        spriteWidth = spriteWidth - (-drawX);
        dwpSprite = dwpSprite + (-drawX);
        //-------------------------------------------------------
        //왼쪽이 조금 잘리므로 출력 시작 위치를 오른쪽으로 민다.
        //-------------------------------------------------------
        drawX = 0;

    }

    //--------------------------------
    //오른쪽 출력 위치 계산 (우측 클리핑)
    //--------------------------------
    if (destWidth < drawX + sprite->m_Width)
    {
        spriteWidth -= ((drawX + sprite->m_Width) - destHeight);
    }

    //--------------------------------
    //찍을 그림이 없다면 종료한다
    //--------------------------------
    if (spriteWidth <= 0 || spriteHeight <= 0)
        return;

    //---------------------------------
    //전체 크기를 돌면서 복사한다
    //---------------------------------
    for (countY = 0; spriteHeight > countY; countY++)
    {
        memcpy(pdwDest, dwpSprite, spriteWidth * 4);
        //---------------------
        //다음 줄로 이동, 화면과 스프라이트 모두
        //------------------------------------
        pdwDest = (DWORD*)((BYTE*)pdwDest + destPitch);
        dwpSprite = (DWORD*)((BYTE*)dwpSprite + sprite->m_Pitch);
    }
}


void CSpriteDib::DrawSprite50(__int32 spriteIndex, __int32 drawX, __int32 drawY, BYTE* dest, __int32 destWidth, __int32 destHeight, __int32 destPitch)
{
}

void CSpriteDib::DrawSpriteRed(__int32 spriteIndex, __int32 drawX, __int32 drawY, BYTE* dest, __int32 destWidth, __int32 destHeight, __int32 destPitch)
{
}

void CSpriteDib::DrawImage(int spriteIndex, int drawX, int drawY, BYTE* dest, __int32 destWidth, __int32 destHeight, __int32 destPitch, int drawLen)
{
}
