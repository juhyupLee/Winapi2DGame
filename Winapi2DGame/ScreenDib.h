#pragma once
//DIB�� ����� GDI �� ��ũ�� ����
//�������� HDC�� DIB�� ����Ͽ� �׸��� ����ش�
class CScreenDib
{
public:
	CScreenDib(int width, int height, int colorBit);
	virtual  ~CScreenDib();

protected:
	void CreateDibBuffer(int width, int height, int colorbit);
	void ReleaseDibBuffer(void);

public:
	void Flip(HWND hWnd, int x = 0, int y = 0);

	BYTE* GetDibBuffer(void);
	int GetWidth(void);
	int GetHeight(void);
	int GetPitch(void);


protected:

	BITMAPINFO	m_DibInfo;
	BYTE* m_Buffer;

	int m_Width;
	int m_Height;
	int m_Pitch;
	int m_ColorBit;
	int m_BufferSize;
};