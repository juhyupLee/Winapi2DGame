#pragma once
#include <Windows.h>
#include <tchar.h>
#define PRO_BEGIN(TagName) ProfileBegin(TagName)
#define PRO_END(TagName) ProfileEnd(TagName)
#define INFO_CNT 100
#define MAX 0
#define MIN 92233720368547
#define CONVERT_US 10.0
void ProfileBegin(const TCHAR* szName);
void ProfileEnd(const TCHAR* szName);
void ProfileDataOutText(const TCHAR* szFileName);
void ProfileReset();