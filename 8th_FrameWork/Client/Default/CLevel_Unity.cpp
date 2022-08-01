#include "CLevel_Unity.h"

CLevel_Unity::CLevel_Unity()
{
}

CLevel_Unity::~CLevel_Unity()
{
}

CLevel_Unity* CLevel_Unity::Create()
{
    CLevel_Unity* pLevel = new CLevel_Unity();

    return pLevel;
}

HRESULT CLevel_Unity::Enter()
{
    return S_OK;
}

void CLevel_Unity::Tick()
{
}

void CLevel_Unity::Late_Tick()
{
}

HRESULT CLevel_Unity::Render()
{
    return S_OK;
}

HRESULT CLevel_Unity::Exit()
{
    return S_OK;
}
