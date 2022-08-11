#include "stdafx.h"
#include "CLevel_Default.h"

#include "GameInstance.h"

#include "CUtility_Json.h"

#include "CGameObject_Factory.h"
#include "GameObject.h"

#include "Transform.h"

CLevel_Default::CLevel_Default()
{
}

CLevel_Default::~CLevel_Default()
{
}

CLevel_Default* CLevel_Default::Create(LEVEL_TYPE_CLIENT	eLevelID)
{
    CLevel_Default* pLevel = new CLevel_Default;

    pLevel->m_eLevelType = eLevelID;

    if (FAILED(pLevel->Initialize()))
    {
        Call_MsgBox_Index(L"Falied to Initialize : CLevel_Default", eLevelID);
        SAFE_DELETE(pLevel);
        return nullptr;
    }

    return pLevel;
}


HRESULT CLevel_Default::Initialize()
{
    //파일의 경로를 읽는다.
    json LevelJson;

    if (FAILED(CUtility_Json::Load_Json(CUtility_Json::Complete_Path(m_eLevelType).c_str(), &LevelJson)))
    {
        Call_MsgBox_Index(L"Failed to Load_Json : CLevel_Default", m_eLevelType);
        return E_FAIL;
    }

    //생성해야하는 오브젝트들의 키를 모두 저장한다.
    for (_uint i = 0; i < LevelJson["GameObjects"].size(); ++i)
    {

        if (!Safe_CheckID(LevelJson["GameObjects"][i], ID_GAMEOBJECT))
        {
            return E_FAIL;
        }

        GameObjectData  tData;

        tData.iPrototypeID = LevelJson["GameObjects"][i];
        tData.eGroupType = LevelJson["GroupType"][i];

        m_vecGameObjectData.push_back(tData);
        
        m_fLoadingFinish = (_float)(i+1) / (_float)(LevelJson.size());
    }

    return S_OK;
}

HRESULT CLevel_Default::Enter()
{
    //저장해놓은 오브젝트들의 키에 맞춰 모두 생성한다.
    for (_uint i = 0; i < m_vecGameObjectData.size(); ++i)
    {
        Create_GameObjectInLevel(m_vecGameObjectData[i]);
    }


    return S_OK;

}

void CLevel_Default::Tick()
{
}

void CLevel_Default::Late_Tick()
{
}

HRESULT CLevel_Default::Render()
{
    return S_OK;
}

HRESULT CLevel_Default::Exit()
{

    return S_OK;
}

HRESULT CLevel_Default::Create_GameObjectInLevel(const GameObjectData& tData)
{
    CGameObject* pGameObject = nullptr;
    _uint iID = tData.iPrototypeID;

    if (!Safe_CheckID(iID, ID_GAMEOBJECT))
    {
        Call_MsgBox_Index(L"Out of Range GameObject Prototype ID", iID);
        return E_FAIL;
    }

    pGameObject = CGameObject_Factory::Create_FromJson(iID);

    if (!pGameObject)
    {
        Call_MsgBox_Index(L"Failed to Create_GameObjectInLevel : CLevel_Default", iID);
        return E_FAIL;
    }
    
    if (tData.eGroupType == GROUP_STATIC)
    {
        CREATE_STATIC(pGameObject, tData.iPrototypeID);
    }
    else
    {
        CREATE_GAMEOBJECT(pGameObject, tData.eGroupType);
    }



    return S_OK;
}
