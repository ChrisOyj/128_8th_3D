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
    //������ ��θ� �д´�.
    json LevelJson;

    if (FAILED(CUtility_Json::Load_Json(CUtility_Json::Complete_Path(m_eLevelType).c_str(), &LevelJson)))
    {
        Call_MsgBox_Index(L"Failed to Load_Json : CLevel_Default", m_eLevelType);
        return E_FAIL;
    }

    //�����ؾ��ϴ� ������Ʈ���� Ű�� ��� �����Ѵ�.
    for (_uint i = 0; i < LevelJson.size(); ++i)
    {
        json    GameObjectDataJson = LevelJson["GameObjects"][i];
        GameObejctData  tData;
        tData.iPrototypeID = GameObjectDataJson["GameObject_ID"];
        tData.eGroupType = GameObjectDataJson["eGroupType"];
        tData.vStartPosition = CUtility_Json::Get_VectorFromJson(GameObjectDataJson["vStartPosition"]);
        tData.vStartLook = CUtility_Json::Get_VectorFromJson(GameObjectDataJson["vStartLook"]);

        if (!Safe_CheckID(tData.iPrototypeID, ID_LEVEL))
        {
            return E_FAIL;
        }

        m_tGameObjectDataList.push_back(tData);
    }

    return S_OK;
}

HRESULT CLevel_Default::Enter()
{
    //�����س��� ������Ʈ���� Ű�� ���� ��� �����Ѵ�.
    for (auto& tData : m_tGameObjectDataList)
    {
        Create_GameObjectInLevel(tData);
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

HRESULT CLevel_Default::Create_GameObjectInLevel(const GameObejctData& tData)
{
    CGameObject* pGameObject = nullptr;

    if (!Safe_CheckID(tData.iPrototypeID, ID_GAMEOBJECT))
    {
        Call_MsgBox_Index(L"Out of Range GameObject Prototype ID", tData.iPrototypeID);
        return E_FAIL;
    }

    pGameObject = CGameObject_Factory::Create_FromJson(tData.iPrototypeID);

    if (!pGameObject)
    {
        Call_MsgBox_Index(L"Failed to Create_GameObjectInLevel : CLevel_Default", tData.iPrototypeID);
        return E_FAIL;
    }

    pGameObject->Get_Transform()->Set_World(WORLD_POS, tData.vStartPosition);
    pGameObject->Get_Transform()->Set_Look(tData.vStartLook);
    
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
