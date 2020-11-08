#pragma once
class SerializeBuffer
{
public:
	enum
	{
		BUFFER_SIZE=1500
	};
	SerializeBuffer();
	virtual ~SerializeBuffer();

	void Release();
	void Clear();

	int32_t GetBufferSize();
	int32_t GetDataSize();

	char* GetBufferPtr(void);

	int32_t MoveWritePos(int size);
	int32_t MoveReadPos(int size);

	SerializeBuffer& operator<<(BYTE inValue);
	SerializeBuffer& operator<<(char inValue);
	SerializeBuffer& operator<<(short inValue);
	SerializeBuffer& operator<<(WORD inValue);
	SerializeBuffer& operator<<(int32_t inValue);
	SerializeBuffer& operator<<(DWORD inValue);
	SerializeBuffer& operator<<(float inValue);
	SerializeBuffer& operator<<(int64_t inValue);
	SerializeBuffer& operator<<(double inValue);

	SerializeBuffer& operator>>(BYTE& outValue);
	SerializeBuffer& operator>>(char& outValue);
	SerializeBuffer& operator>>(short& outValue);
	SerializeBuffer& operator>>(WORD& outValue);
	SerializeBuffer& operator>>(int32_t& outValue);
	SerializeBuffer& operator>>(DWORD& outValue);
	SerializeBuffer& operator>>(float& outValue);
	SerializeBuffer& operator>>(int64_t& outValue);
	SerializeBuffer& operator>>(double& outValue);

	int32_t GetData(char* dest, int size);
	int32_t PutData(char* src, int size);

private:
	void Crash();
protected:
	int32_t m_FreeSize;
	int32_t m_UsedSize;
	char m_Buffer[BUFFER_SIZE];
	int32_t m_WritePos;
	int32_t m_ReadPos;



};