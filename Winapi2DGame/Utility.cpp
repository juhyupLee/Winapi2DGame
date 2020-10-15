#include "Utility.h"
#include <stdlib.h>
#include <time.h>
CUtility::CUtility()
{
	srand((unsigned int)time(NULL));
}

CUtility::~CUtility()
{
}

CUtility* CUtility::GetInstance()
{
	static CUtility utility;
	return &utility;
}

void CUtility::BitInsert(unsigned int* out, int bitposition, bool value)
{
	if (bitposition <0 || bitposition > 31  )
	{
		return;
	}
	unsigned int insert = 0x00000001;
	if (value)
	{
		*out = *out | (insert << bitposition);
	}
	else
	{
		*out = *out &~(insert << bitposition);
	}	
}
int CUtility::GenerateRandomNum(int min, int max)
{
	int num = (rand() % (max + 1)) + min;
	if (num > max)
	{
		num--;
	}
	return  num;
}
