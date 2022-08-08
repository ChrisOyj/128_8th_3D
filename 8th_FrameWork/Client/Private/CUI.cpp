#include "stdafx.h"
#include "CUI.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

CUI::CUI()
{
}

CUI::~CUI()
{
}

CUI* CUI::Create(const _uint& iID, const _float4& vStartPosition)
{
    CUI* pInstance = new CUI;

	pInstance->Set_ID(iID);

	if (FAILED(pInstance->SetUp_Components(vStartPosition)))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CUI");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CUI");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CUI::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI::Initialize()
{
    return S_OK;
}

void CUI::OnEnable()
{
}

void CUI::OnDisable()
{
}

HRESULT CUI::SetUp_Components(const _float4& vStartPosition)
{
	m_pTransform->Set_World(WORLD_POS, CFunctor::OrthoToRealPosition(vStartPosition));
	m_pTransform->Set_Scale(_float4(100.f, 100.f, 0.f, 1.f));

	Add_Component(CComponent_Factory::Create_FromPrototype(CPrototype_Factory::DEFAULT_SHADER, this));
	Add_Component(CComponent_Factory::Create_FromPrototype(CPrototype_Factory::DEFAULT_MESH_RECT, this));
	Add_Component(CComponent_Factory::Create_FromPrototype(CPrototype_Factory::DEFAULT_UI_RENDERER, this));
	Add_Component(CComponent_Factory::Create_FromPrototype(CPrototype_Factory::DEFAULT_TEXTURE, this));
	Add_Component(CComponent_Factory::Create_FromPrototype(CPrototype_Factory::DEFAULT_FADER, this));

    return S_OK;
}
