#include "stdafx.h"
#include "CMap_Sample.h"

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
#include "MeshContainer.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"



CMap_Sample::CMap_Sample()
{
}

CMap_Sample::~CMap_Sample()
{
}

CMap_Sample* CMap_Sample::Create(wstring wstrFilePath)
{
    CMap_Sample* pInstance = new CMap_Sample;

    if (FAILED(pInstance->SetUp_Model(wstrFilePath)))
    {
        Call_MsgBox(L"Failed to SetUp_Model : CMap_Sample");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CMap_Sample");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CMap_Sample::Initialize_Prototype()
{
    CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
        VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
    pShader->Initialize();
    Add_Component(pShader);

    CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_ALPHA, VTXMODEL_PASS_SAMPLE
        , _float4(0.f, 0.f, 0.f, 1.f));

    Add_Component<CRenderer>(pRenderer);

    m_vOffsetPos = _float4( 0.f, 0.f, 0.f );
    m_vPickedPos = _float4( 0.f, 0.f, 0.f );

    return S_OK;
}

HRESULT CMap_Sample::Initialize()
{
    return S_OK;
}

HRESULT CMap_Sample::Start()
{
    __super::Start();

    vector<pair<_uint, CMeshContainer*>>& vecMC = GET_COMPONENT(CModel)->Get_MeshContainers();

    for (auto& elem : vecMC)
    {
        elem.second->Set_CurPass(VTXMODEL_PASS_SAMPLE);
    }

    return S_OK;
}

HRESULT CMap_Sample::SetUp_Model(wstring wstrFilePath)
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

void CMap_Sample::My_LateTick()
{
    _float4 vPos = m_vPickedPos + m_vOffsetPos;
    m_pTransform->Set_World(WORLD_POS, vPos);

    m_pTransform->Make_WorldMatrix();
}
