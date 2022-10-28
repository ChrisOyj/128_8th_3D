#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"

BEGIN(Engine)
class CTransform;
class CGameObject;
END

BEGIN(Client)

class CDefault_UI;

enum GAMEOBJECT_TYPE
{
	OBJ_UNIT,
	OBJ_UI,
	OBJ_EFFECT,
	OBJ_END
};

class CGameObject_Factory
{
public:
	template <typename T>
	static T* Clone_GameObject()
	{
		T* pGameObject = CGameInstance::Get_Instance()->Clone_GameObject<T>();

		if (!pGameObject)
		{
			return nullptr;
		}

		if (FAILED(pGameObject->Initialize()))
		{
			Call_MsgBox(L"Failed to Initialize : CGameObject_Factory");
			return nullptr;
		}

		return pGameObject;
	}

public:
	static CGameObject* Clone_GameObject(_hashcode hcClassName);

public:
	static CDefault_UI* Create_UI(wstring strFileKey);

public:
	static vector<CGameObject*>	Create_vecUI(wstring strFileTagKey);

	static vector<CDefault_UI*>	Create_vecUI_Reverse(wstring strFileTagKey);
};

END