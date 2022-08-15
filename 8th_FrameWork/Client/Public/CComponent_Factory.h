#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"

BEGIN(Engine)
END

BEGIN(Client)

enum COMPONENT_TYPE
{
	COM_TRANSFORM,
	COM_COLLIDER,
	COM_RENDERER,
	COM_PHYSICS,
	COM_TEXTURE,
	COM_SHADER,
	COM_MESH,
	COM_FADER,
	COM_END
};

class CComponent_Factory
{
public:
	template <typename T>
	static T* Clone_Component(CGameObject* pOwner)
	{
		T* pComponent = CGameInstance::Get_Instance()->Clone_Component<T>();

		if (!pComponent)
		{
			return nullptr;
		}

		pComponent->Set_Owner(pOwner);

		if (FAILED(pComponent->Initialize()))
		{
			Call_MsgBox(L"Failed to Initialize : CComponent_Factory");
			return nullptr;
		}

		return pComponent;
	}

public:
	static CComponent* Clone_Component(CGameObject* pOwner, _hashcode hcClassName);

};

END