#pragma once
class CBackBuffer
{
public:
	static CBackBuffer* GetInstance();
	BYTE* GetBackBuffer();
	BITMAPINFO* GetDIBInfo();
	int32_t GetFileSize();
	void DrawSprite(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer,__int32 iDrawLen=100);
	void DrawSprite(int destX, int destY, CSpriteManager::stSprite* pSprite,DWORD dwColorKey, __int32 iDrawLen = 100);
	//void DrawSprite50(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen = 100);
	void DrawSpriteRed1(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen = 100);
	void DrawSpriteRed1(int destX, int destY, CSpriteManager::stSprite* pSprite, DWORD dwColorKey, __int32 iDrawLen = 100);
	void DrawSpriteTransParent(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen = 100);
	void DrawImage(int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int srcPitch, BYTE* srcBuffer, __int32 iDrawLen = 100);
private:
	CBackBuffer();
	~CBackBuffer();
private:
	BYTE* m_Buffer;
	int32_t m_Pitch;
	int32_t m_Height;
	int32_t m_Width;
	int32_t m_FileSize;
	BITMAPINFO m_DIBInfo;
};