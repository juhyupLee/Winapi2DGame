#include "BaseScene.h"

unsigned __int32 IBaseScene::GetKeyResult()
{
    return m_dwKeyResult;
}

void IBaseScene::ResetKey()
{
    m_dwKeyResult = 0;
}
