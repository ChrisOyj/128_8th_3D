#include "stdafx.h"
#include "CGameObject_Factory.h"

#include "CComponent_Factory.h"

#include "GameInstance.h"

#include "Transform.h"
#include "CCollider_Sphere.h"

#include "CUtility_Json.h"

#include "GameObject.h"

#include "CEmpty_GameObject.h"

CGameObject* CGameObject_Factory::Clone_GameObject(_hashcode hcClassName)
{
	CGameObject* pGameObject = CGameInstance::Get_Instance()->Clone_GameObject(hcClassName);

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
