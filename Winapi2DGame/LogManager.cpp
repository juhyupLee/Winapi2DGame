
#include <iostream>
#include <Windows.h>
#include "LogManager.h"
//#include "MemoryTracer.h"
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

CLogManager::CLogManager()
{

}
CLogManager::~CLogManager()
{
}

CLogManager* CLogManager::GetInstance()
{
    static CLogManager logManager;
    return &logManager;
}

void CLogManager::PrintLog(const WCHAR* fileName, const WCHAR* str, __int32 var)
{
    FILE* filePtr = nullptr;
    _wfopen_s(&filePtr, fileName, L"a");
    while (filePtr == nullptr)
    {
        _wfopen_s(&filePtr, fileName, L"a");
    }
    fwprintf(filePtr, str, var);
    fclose(filePtr);
}

void CLogManager::PrintLog(const WCHAR* fileName, const WCHAR* str, __int64 var)
{
    FILE* filePtr = nullptr;
    _wfopen_s(&filePtr, fileName, L"a");
    while (filePtr == nullptr)
    {
        _wfopen_s(&filePtr, fileName, L"a");
    }
    fwprintf(filePtr, str, var);
    fclose(filePtr);
}

void CLogManager::PrintLog(const WCHAR* fileName, const WCHAR* str, float var)
{
    FILE* filePtr = nullptr;
    _wfopen_s(&filePtr, fileName, L"a");
    while (filePtr == nullptr)
    {
        _wfopen_s(&filePtr, fileName, L"a");
    }
    fwprintf(filePtr, str, var);
    fclose(filePtr);
}

void CLogManager::PrintLog(const WCHAR* fileName, const WCHAR* str, double var)
{
    FILE* filePtr = nullptr;
    _wfopen_s(&filePtr, fileName, L"a");
    while (filePtr == nullptr)
    {
        _wfopen_s(&filePtr, fileName, L"a");
    }
    fwprintf(filePtr, str, var);
    fclose(filePtr);
}

void CLogManager::PrintLog(const WCHAR* fileName, const WCHAR* str, bool var)
{
    FILE* filePtr = nullptr;
    _wfopen_s(&filePtr, fileName, L"a");
    while (filePtr == nullptr)
    {
        _wfopen_s(&filePtr, fileName, L"a");
    }
    fwprintf(filePtr, str, var);
    fclose(filePtr);
}

void CLogManager::PrintLog(const WCHAR* fileName, const WCHAR* str, __int32 var1, __int32 var2)
{
    FILE* filePtr = nullptr;
    _wfopen_s(&filePtr, fileName, L"a");
    while (filePtr == nullptr)
    {
        _wfopen_s(&filePtr, fileName, L"a");
    }
    fwprintf(filePtr, str, var1,var2);
    fclose(filePtr);
}

void CLogManager::PrintLog(const WCHAR* fileName, const WCHAR* str, __int64 var1, __int64 var2)
{
    FILE* filePtr = nullptr;
    _wfopen_s(&filePtr, fileName, L"a");
    while (filePtr == nullptr)
    {
        _wfopen_s(&filePtr, fileName, L"a");
    }
    fwprintf(filePtr, str, var1, var2);
    fclose(filePtr);
}

void CLogManager::PrintLog(const WCHAR* fileName, const WCHAR* str, float var1, float var2)
{
    FILE* filePtr = nullptr;
    _wfopen_s(&filePtr, fileName, L"a");
    while (filePtr == nullptr)
    {
        _wfopen_s(&filePtr, fileName, L"a");
    }
    fwprintf(filePtr, str, var1, var2);
    fclose(filePtr);
}

void CLogManager::PrintLog(const WCHAR* fileName, const WCHAR* str, double var1, double var2)
{
    FILE* filePtr = nullptr;
    _wfopen_s(&filePtr, fileName, L"a");
    while (filePtr == nullptr)
    {
        _wfopen_s(&filePtr, fileName, L"a");
    }
    fwprintf(filePtr, str, var1, var2);
    fclose(filePtr);
}

void CLogManager::PrintLog(const WCHAR* fileName, const WCHAR* str, bool var1, bool var2)
{
    FILE* filePtr = nullptr;
    _wfopen_s(&filePtr, fileName, L"a");
    while (filePtr == nullptr)
    {
        _wfopen_s(&filePtr, fileName, L"a");
    }
    fwprintf(filePtr, str, var1, var2);
    fclose(filePtr);
}

void CLogManager::PrintConsoleLog(const WCHAR* str, __int32 var1, __int32 var2)
{
    wprintf(str, var1, var2);
}

void CLogManager::PrintConsoleLog(const WCHAR* str, __int32 var1)
{
    wprintf(str, var1);
}

void CLogManager::PrintConsoleLog(const WCHAR* str, __int32 var1, __int32 var2, __int32 var3, __int32 var4)
{
    wprintf(str, var1,var2,var3,var4);
}
