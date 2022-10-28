#include "stdafx.h"
#include "CGround.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "CModel_Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "CMesh_Rect.h"
#include "CMesh_Terrain.h"
#include "Model.h"
#include "CBlender.h"
#include "MeshContainer.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"



CGround::CGround()
{
}

CGround::~CGround()
{
}

CGround* CGround::Create(wstring wstrFilePath)
{
    CGround* pInstance = new CGround;

    if (FAILED(pInstance->SetUp_Model(wstrFilePath)))
    {
        Call_MsgBox(L"Failed to SetUp_Model : CGround");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CGround");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CGround::Initialize_Prototype()
{
    CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
        VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
    pShader->Initialize();
    Add_Component(pShader);

    CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_GROUND
        , _float4(0.f, 0.f, 0.f, 1.f));

    Add_Component<CRenderer>(pRenderer);
   
    CBlender* pBlender = CBlender::Create(
        L"../bin/resources/textures/modeltextures/T_ENV_LKNVLD_Mask_01_M.dds",
        L"../bin/resources/textures/modeltextures/T_ENV_KNFRST_Ground_01_BC.dds");

    Add_Component(pBlender);

    return S_OK;
}

HRESULT CGround::Initialize()
{
    return S_OK;
}

HRESULT CGround::Start()
{
	__super::Start();

	vector<pair<_uint, CMeshContainer*>>& vecMC = GET_COMPONENT(CModel)->Get_MeshContainers();

	for (auto& elem : vecMC)
	{
        if (elem.second->Get_MaterialName() == "MI_ENV_KNVLLG_GroundSoil_A")
            elem.second->Set_CurPass(VTXMODEL_PASS_GROUND);
        else if (elem.second->Get_MaterialName() == "MI_ENV_TCHEXA_GroundSoil_A")
            elem.second->Set_CurPass(VTXMODEL_PASS_GROUNDEXAM);
        else
            elem.second->Set_CurPass(VTXMODEL_PASS_BUILDING);
	}


	return S_OK;
}

void CGround::My_LateTick()
{
   /* vector<pair<_uint, CMeshContainer*>>& vecMC = GET_COMPONENT(CModel)->Get_MeshContainers();

    _float4 vCamPos = CGameInstance::Get_Instance()->Get_ViewPos();
    _float fDistance = (vCamPos - m_pTransform->Get_World(WORLD_POS)).Length();

    for (auto& elem : vecMC)
    {
        CGameInstance::Get_Instance()->Regist_Mesh(elem.second, fDistance);
    }*/
}

HRESULT CGround::SetUp_Model(wstring wstrFilePath)
{
    _float4x4			TransformMatrix;
    TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(0.0f));

    CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, wstrFilePath, TransformMatrix);

    if (!pModel)
        return E_FAIL;

    pModel->Initialize();
    Add_Component(pModel);

    return S_OK;
}
