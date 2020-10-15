#include <Windows.h>
#include "ScreenDib.h"

CScreenDib::CScreenDib(int width, int height, int colorBit)
{

    CreateDibBuffer(width, height, colorBit);
}

CScreenDib::~CScreenDib()
{
    ReleaseDibBuffer();
}

void CScreenDib::CreateDibBuffer(int width, int height, int colorbit)
{
    m_Pitch = width * (colorbit / 8);
    m_Pitch = (m_Pitch + 3) & ~3;

    //���� bimheader ����� ��������
    m_DibInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_DibInfo.bmiHeader.biWidth = width;
    m_DibInfo.bmiHeader.biHeight = -height;
    m_DibInfo.bmiHeader.biPlanes = 1;
    m_DibInfo.bmiHeader.biBitCount = colorbit;
    m_DibInfo.bmiHeader.biCompression = 0;
    m_DibInfo.bmiHeader.biSizeImage = 0;
    m_DibInfo.bmiHeader.biClrUsed = 0;
    m_DibInfo.bmiHeader.biClrImportant = 0;

    //������ ������� pitch X ���� ? �°���?
    m_Buffer = new BYTE[(long long)m_Pitch * height];

    memset(m_Buffer, 255, (long long)m_Pitch * height);
    m_Width = width;
    m_Height = height;
    m_ColorBit = colorbit;
    m_BufferSize = m_Pitch * height;
    

}

void CScreenDib::ReleaseDibBuffer(void)
{
    delete[] m_Buffer;
}

void CScreenDib::Flip(HWND hWnd, int x, int y)
{
    //�ϴ� ������ WM_PAINT�� �׷��ߵǴ���,
    //������ �׸���
    HDC hdc = GetDC(hWnd);

    StretchDIBits(hdc, x, y, m_Width, m_Height, 0, 0, m_Width, m_Height, m_Buffer, &m_DibInfo, DIB_RGB_COLORS,SRCCOPY);

    ReleaseDC(hWnd, hdc);

}

BYTE* CScreenDib::GetDibBuffer(void)
{

    return m_Buffer;
}

int CScreenDib::GetWidth(void)
{
    return m_Width;
}

int CScreenDib::GetHeight(void)
{
    return m_Height;
}

int CScreenDib::GetPitch(void)
{
    return m_Pitch;
}

