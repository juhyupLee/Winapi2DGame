#include "MemoryTracer.h"
#include <stdio.h>
#include <iostream> 
#include <Windows.h>
#include <time.h>


class CMemoryTracer
{
private:
	struct stMemory
	{
		void* m_Pointer;
		const char* m_FileName;
		int m_FileLine;
		size_t m_Size;
		bool m_bArray;
		bool m_bUsed;
	};
public:
	enum
	{
		ARRAY_SIZE = 500,
		FILE_NAME = 255
	};
	enum
	{
		DELETE_POSSIBLE,
		LEAK,
		WRONG_ARRAY,
		NOALLOC
	};
public:
	CMemoryTracer();
	~CMemoryTracer();

	void PushMemory(void* pointer, size_t size, const char* fileName, int line, bool bArray);		//new 를 할당한 메모리와 그관련된 정보를 넣는 함수
	int ReleaseCheck(void* pointer, int* idx);	//메모리정보배열을 조사하여, 해당포인터가 지울수 있는지 
										//지울수 없다면, 왜 못지우는지 ( 1.관련 메모리가 없다 2. 잘못된 delete 선택(배열or하나)  3. 지울수 있음.
	void WriteLog(void* ptr, int status, int index);
private:
	char m_FileName[FILE_NAME];
	stMemory m_stMemoryInfo[ARRAY_SIZE];

};

CMemoryTracer::CMemoryTracer()
{
	memset(m_FileName, 0, FILE_NAME);
	memset(m_stMemoryInfo, 0, ARRAY_SIZE);
	FILE* filePtr = nullptr;

	tm timeResult;
	time_t  timeT;
	time(&timeT);
	localtime_s(&timeResult, &timeT);

	sprintf_s(m_FileName, 125, "./MemoryLeakLog/Alloc_%d%d%d_%d[H]%d[M]%d[S].txt", timeResult.tm_year + 1900,
		timeResult.tm_mon + 1,
		timeResult.tm_mday,
		timeResult.tm_hour,
		timeResult.tm_min,
		timeResult.tm_sec);
	fopen_s(&filePtr, m_FileName, "w");

	if (filePtr == nullptr)
	{
		return;
	}
	fclose(filePtr);

}
CMemoryTracer::~CMemoryTracer()
{
	for (int index = 0; index < ARRAY_SIZE; ++index)
	{
		if (m_stMemoryInfo[index].m_bUsed)
		{
			WriteLog(nullptr, LEAK, index);
		}
	}
}

void CMemoryTracer::PushMemory(void* pointer, size_t size, const char* fileName, int line, bool bArray)
{
	for (int index = 0; index < ARRAY_SIZE; ++index)
	{
		if (m_stMemoryInfo[index].m_bUsed == false)
		{
			m_stMemoryInfo[index].m_bUsed = true;
			m_stMemoryInfo[index].m_Pointer = pointer;
			m_stMemoryInfo[index].m_Size = size;
			m_stMemoryInfo[index].m_FileName = fileName;
			m_stMemoryInfo[index].m_FileLine = line;

			if (bArray)
			{
				m_stMemoryInfo[index].m_bArray = true;
			}
			else
			{
				m_stMemoryInfo[index].m_bArray = false;
			}
			break;
		}
	}
}

int CMemoryTracer::ReleaseCheck(void* pointer, int* idx)
{
	for (int index = 0; index < ARRAY_SIZE; ++index)
	{
		if (m_stMemoryInfo[index].m_bUsed == true)
		{
			if (m_stMemoryInfo[index].m_Pointer == pointer)
			{
				m_stMemoryInfo[index].m_bUsed = false;
				*idx = index;
				return DELETE_POSSIBLE;
			}
			else
			{
				if (((ULONG_PTR)pointer - sizeof(ULONG_PTR) == (ULONG_PTR)m_stMemoryInfo[index].m_Pointer))
				{
					m_stMemoryInfo[index].m_bUsed = false;
					*idx = index;
					return  WRONG_ARRAY;
				}
			}
		}

	}

	return NOALLOC;
}

void CMemoryTracer::WriteLog(void* ptr, int status, int index)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, m_FileName, "a");

	if (pFile == nullptr)
	{
		return;
	}
	switch (status)
	{
	case CMemoryTracer::NOALLOC:
		fprintf(pFile, "NOALLOC \t[%p]\n", ptr);
		break;
	case CMemoryTracer::WRONG_ARRAY:
		fprintf(pFile, "ARRAY \t[%p] [%zu] %s : %d\n", m_stMemoryInfo[index].m_Pointer, m_stMemoryInfo[index].m_Size, m_stMemoryInfo[index].m_FileName, m_stMemoryInfo[index].m_FileLine);
		break;
	case CMemoryTracer::LEAK:
		fprintf(pFile, "LEAK \t[%p] [%zu] %s : %d\n", m_stMemoryInfo[index].m_Pointer, m_stMemoryInfo[index].m_Size, m_stMemoryInfo[index].m_FileName, m_stMemoryInfo[index].m_FileLine);
		break;
	}
	fclose(pFile);

}


CMemoryTracer g_MemoryTrace;

#ifdef new 
#undef new
#endif 

void* operator new(size_t size, const char* file, int line)
{
	void* pointer = malloc(size);
	g_MemoryTrace.PushMemory(pointer, size, file, line, false);
	return pointer;
}

void* operator new[](size_t size, const char* file, int line)
{
	void* pointer = malloc(size);
	g_MemoryTrace.PushMemory(pointer, size, file, line, true);
	return pointer;
}

void operator delete(void* p)
{
	int index = 0;

	switch (g_MemoryTrace.ReleaseCheck(p, &index))
	{
	case CMemoryTracer::DELETE_POSSIBLE:
		free(p);
		p = nullptr;
		break;
	case CMemoryTracer::WRONG_ARRAY:
		g_MemoryTrace.WriteLog(p, CMemoryTracer::WRONG_ARRAY, index);
		p = (void*)((ULONG_PTR)p - sizeof(ULONG_PTR));
		free(p);
		break;
	case CMemoryTracer::NOALLOC:
		g_MemoryTrace.WriteLog(p, CMemoryTracer::NOALLOC, index);
		break;

	}
}
void operator delete[](void* p)
{
	int index = 0;
	switch (g_MemoryTrace.ReleaseCheck(p, &index))
	{
	case CMemoryTracer::DELETE_POSSIBLE:
		free(p);
		p = nullptr;
		break;
	case CMemoryTracer::WRONG_ARRAY:
		g_MemoryTrace.WriteLog(p, CMemoryTracer::WRONG_ARRAY, index);
		break;
	case CMemoryTracer::NOALLOC:
		g_MemoryTrace.WriteLog(p, CMemoryTracer::NOALLOC, index);
		break;

	}
}
void operator delete(void* p, const char* file, int line)
{

}

void operator delete[](void* p, const char* file, int line)
{

}
