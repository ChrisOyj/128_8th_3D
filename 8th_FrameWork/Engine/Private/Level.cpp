#include "..\Public\Level.h"

#include "GameInstance.h"
#include "GameObject.h"

CLevel::CLevel()
{
}

CLevel::~CLevel()
{
}

HRESULT CLevel::Enter()
{
    for (_uint i = 0; i < m_vecGameObjects.size(); ++i)
    {
        CREATE_GAMEOBJECT(m_vecGameObjects[i].first, m_vecGameObjects[i].second);
    }

    m_vecGameObjects.clear();


    return S_OK;
}

HRESULT CLevel::Exit()
{
    m_vecGameObjects.clear();
    return S_OK;
}

void CLevel::Ready_GameObject(CGameObject* pGameObject, _uint iGroupIndex)
{
    m_vecGameObjects.push_back(make_pair(pGameObject, iGroupIndex));
}
