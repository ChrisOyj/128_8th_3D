#include "stdafx.h"
#include "CLoading_Turn.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "CMesh_Rect.h"
#include "Physics.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

CLoading_Turn::CLoading_Turn()
{
}

CLoading_Turn::~CLoading_Turn()
{
}

CLoading_Turn* CLoading_Turn::Create(const _float4& vStartPosition)
{
	CLoading_Turn* pInstance = new CLoading_Turn;

	if (FAILED(pInstance->SetUp_Components(vStartPosition)))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CLoading_Turn");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CLoading_Turn");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CLoading_Turn::SetUp_Components(const _float4& vStartPosition)
{
	m_pTransform->Set_World(WORLD_POS, CFunctor::OrthoToRealPosition(vStartPosition));
	m_pTransform->Set_Scale(_float4(100.f, 100.f, 0.5f, 1.f));

	CPhysics* pPhysics = Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));
	
	pPhysics->Set_TurnDir(_float4(0.f, 0.f, 1.f, 0.f));
	pPhysics->Set_TurnSpeed(1.f);

	CTexture* pTexture = CTexture::Create(CP_AFTER_TRANSFORM,
		L"../bin/resources/textures/ui/loading_screen/T_UI_Load_Icon_BC.png", 1);

	Add_Component(pTexture);

	return S_OK;
}
