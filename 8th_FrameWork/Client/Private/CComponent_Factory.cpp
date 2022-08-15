#include "stdafx.h"
#include "CComponent_Factory.h"

#include "GameInstance.h"

#include "Transform.h"
#include "CCollider_Sphere.h"

#include "CUtility_Json.h"
#include "GameObject.h"
#include "Texture.h"
#include "CMesh_Rect.h"
#include "Renderer.h"
#include "CFader.h"
#include "Physics.h"
#include "CShader.h"

#include "CPrototype_Factory.h"

CComponent* CComponent_Factory::Clone_Component(CGameObject* pOwner, _hashcode hcClassName)
{
	CComponent* pComponent = CGameInstance::Get_Instance()->Clone_Component(hcClassName);

	if (!pComponent)
	{		
		return nullptr;
	}

	if (FAILED(pComponent->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CComponent_Factory");
		return nullptr;
	}

	return pComponent;
}
