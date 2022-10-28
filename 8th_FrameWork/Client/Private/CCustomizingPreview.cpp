#include "stdafx.h"
#include "CCustomizingPreview.h"

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
#include "CAnimator.h"
#include "CBlender.h"
#include "MeshContainer.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

#include "CUtility_Transform.h"



CCustomizingPreview::CCustomizingPreview()
{
}

CCustomizingPreview::~CCustomizingPreview()
{
}

CCustomizingPreview* CCustomizingPreview::Create(wstring wstrFilePath)
{
	CCustomizingPreview* pInstance = new CCustomizingPreview;

	if (FAILED(pInstance->SetUp_Model(wstrFilePath)))
	{
		Call_MsgBox(L"Failed to SetUp_Model : CCustomizingPreview");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CCustomizingPreview");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CCustomizingPreview");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CCustomizingPreview::Add_ModelParts(wstring wstrFilePath, MODEL_PART_TYPE eType)
{
	if (FAILED(m_pModelCom->Add_Model(wstrFilePath, eType)))
		return E_FAIL;

	m_tPlayerModelData.strModelPaths[eType] = wstrFilePath;

	if (eType == MODEL_PART_OVERALL)
		m_bOverall = true;
	else if (eType == MODEL_PART_BODYLOWER || eType == MODEL_PART_BODYUPPER)
		m_bOverall = false;

	m_tPlayerModelData.bOverall = m_bOverall;

	return S_OK;
}

HRESULT CCustomizingPreview::Add_ModelParts(wstring wstrFilePath, MODEL_PART_TYPE eType, string strRefBone)
{
	if (FAILED(m_pModelCom->Add_Model(wstrFilePath, eType, strRefBone)))
		return E_FAIL;

	m_tPlayerModelData.strModelPaths[eType] = wstrFilePath;
	m_tPlayerModelData.strRefBoneName[eType] = strRefBone;

	

	return S_OK;
}

HRESULT CCustomizingPreview::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);

	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/Custom_Base.fbx");
	Add_Component(pAnimator);
		
	CTexture* pTexture = CTexture::Create(0, L"../bin/resources/textures/Materials/T_noise_Type_%d.dds", 3);
	pTexture->Set_CurTextureIndex(1);
	Add_Component(pTexture);

	return S_OK;
}

HRESULT CCustomizingPreview::Initialize()
{
	m_pModelCom = GET_COMPONENT(CModel);
	m_pAnimator = GET_COMPONENT(CAnimator);

	if (!m_pModelCom)
		return E_FAIL;

	if (!m_pAnimator)
		return E_FAIL;


	if (FAILED(Add_ModelParts(L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_ShortSpikyHair_07.fbx", MODEL_PART_HEADGEAR)))
		return E_FAIL;

	if (FAILED(Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodyupper/SK_CHR_Man_Trainer_01.fbx", MODEL_PART_BODYUPPER)))
		return E_FAIL;

	if (FAILED(Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodylower/SK_CHR_Man_LeafPants_01.fbx", MODEL_PART_BODYLOWER)))
		return E_FAIL;

	if (FAILED(Add_ModelParts(L"../bin/resources/meshes/weapons/SM_WEP_Sword_01.fbx", MODEL_PART_RWEAPON, "Center_Position_Sword")))
		return E_FAIL;

	if (FAILED(Add_ModelParts(L"../bin/resources/meshes/weapons/SM_WEP_Sword_01_Sheath.fbx", MODEL_PART_WEAPONSUB, "Center_Position_Sword")))
		return E_FAIL;

	/*if (FAILED(Add_ModelParts(L"../bin/resources/meshes/weapons/SM_WEP_Sword_01.fbx", MODEL_PART_LWEAPON, "Attach_Kunai")))
		return E_FAIL;*/

	/*if (FAILED(Add_ModelParts(L"../bin/resources/meshes/weapons/SM_Gloves_Cat.fbx", MODEL_PART_RWEAPON, "RightHand")))
		return E_FAIL;*/

	//R_Hand_Weapon_cnt_tr
	//Attach_Sword
	//Center_Position_Sword

	/*if (FAILED(m_pModelCom->Add_Model(L"../bin/resources/meshes/weapons/SM_WEP_KnuckleknifeAsma.fbx", MODEL_PART_WEAPON, "RightHandThumb1")))
		return E_FAIL;

	if (FAILED(m_pModelCom->Add_Model(L"../bin/resources/meshes/weapons/SM_WEP_KnuckleknifeAsma.fbx", MODEL_PART_WEAPON, "LeftHandIndex2")))
		return E_FAIL;

	if (FAILED(m_pModelCom->Add_Model(L"../bin/resources/meshes/weapons/SM_WEP_KnuckleknifeAsma.fbx", MODEL_PART_WEAPON, "RightHandIndex1")))
		return E_FAIL;

	if (FAILED(m_pModelCom->Add_Model(L"../bin/resources/meshes/weapons/SM_WEP_KnuckleknifeAsma.fbx", MODEL_PART_WEAPON, "LeftHandIndex3")))
		return E_FAIL;*/


	m_vecVaildAnimIndices.push_back(22);
	m_vecVaildAnimIndices.push_back(46);
	m_vecVaildAnimIndices.push_back(41);
	m_vecVaildAnimIndices.push_back(12);
	m_vecVaildAnimIndices.push_back(61);
	m_vecVaildAnimIndices.push_back(36);
	m_vecVaildAnimIndices.push_back(71);

	

	return S_OK;
}

HRESULT CCustomizingPreview::Start()
{
	__super::Start();

	vector<pair<_uint, CMeshContainer*>>& vecMC = GET_COMPONENT(CModel)->Get_MeshContainers();
	vector<pair<_uint, MODEL_MATERIAL>>& vecMaterials = m_pModelCom->Get_Materials();

	for (auto& elem : vecMC)
	{
		elem.second->Set_CurPass(VTXANIM_PASS_PREVIEW);
		if (elem.first == 0)
			elem.second->Set_CurPass(VTXANIM_PASS_PREVIEWFACE);

		if (vecMaterials[elem.second->Get_MaterialIndex()].second.strName == "MI_CHR_Hachigane_Konoha_01" ||
			vecMaterials[elem.second->Get_MaterialIndex()].second.strName == "MI_WEP_Sword_01" ||
			vecMaterials[elem.second->Get_MaterialIndex()].second.strName == "MI_WEP_Kurosawa")
			elem.second->Set_ShaderFlag(SH_LIGHT_DEFAULT);

		if (elem.first == 0)
			elem.second->Set_ShaderFlag(SH_LIGHT_NOOUTLINE);
		//elem.second->Set_CurPass(0);
	}


	m_pTransform->Set_World(WORLD_POS, _float4(0.f, -0.9f, 1.5f));
	m_pTransform->Set_Look(_float4(0.f, 0.f, -1.f));

	m_pAnimator->Set_CurAnimIndex(ANIM_BASE, m_vecVaildAnimIndices[m_iCurVaildAnimIndex]);



    for (auto& elem : vecMaterials)
    {
		if (elem.second.strName == "MI_CHR_Eyes_WThinL_01" ||
			elem.second.strName == "MI_CHR_Eyes_WThinR_01")
		{
			for (_uint i = 0; i < 6; ++i)
			{
				_tchar szBuff[MAX_STR] = L"";
				swprintf_s(szBuff, L"../bin/resources/textures/animModelTextures/Eyes/Eye_%d.dds", i);
				elem.second.pTextures[1]->Add_Texture(szBuff);
			}
		}
    }
	

	return S_OK;
}

HRESULT CCustomizingPreview::SetUp_Model(wstring wstrFilePath)
{
	_float4x4			TransformMatrix;
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationZ(XMConvertToRadians(180.0f));

	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, wstrFilePath, TransformMatrix);

	if (!pModel)
		return E_FAIL;

	m_tPlayerModelData.strModelPaths[MODEL_PART_FACE] = wstrFilePath;


	pModel->Initialize();

	Add_Component(pModel);

	return S_OK;
}

void CCustomizingPreview::My_Tick()
{
	if (GetFocus() != g_hWnd)
		return;

	if (KEY(UP, TAP))
	{
		if (m_iCurVaildAnimIndex > 0)
		{
			m_pAnimator->Set_CurAnimIndex(ANIM_BASE, m_vecVaildAnimIndices[--m_iCurVaildAnimIndex]);
		}
	}

	else if (KEY(DOWN, TAP))
	{
		if (m_iCurVaildAnimIndex < m_vecVaildAnimIndices.size()-1)
		{
			m_pAnimator->Set_CurAnimIndex(ANIM_BASE, m_vecVaildAnimIndices[++m_iCurVaildAnimIndex]);
		}
	}


	if (KEY(LBUTTON, HOLD))
	{
		CTransform* pTransform = m_pTransform;

		//마우스 이동에 따라 회전
		_float fTurnSpeedX = ((_float)MOUSE_MOVE(MMS_X)) * MOUSE_DPI_X * 2.f;

		CUtility_Transform::Turn_ByAngle(pTransform, _float4(0.f, 1.f, 0.f, 0.f), -fTurnSpeedX);
	}

	else if (KEY(RBUTTON, HOLD))
	{
		_float fTurnSpeedY = ((_float)MOUSE_MOVE(MMS_Y)) * MOUSE_DPI_Y * 0.1f;

		_float4 vScale = m_pTransform->Get_Scale();
		_float4 vPos = m_pTransform->Get_World(WORLD_POS);

		if (fTurnSpeedY == 0.f)
			return;

		vScale += fTurnSpeedY;

		if (vScale.x > 2.f)
			vScale = 2.f;
		else if (vScale.x < 1.f)
			vScale = 1.f;
		else
			vPos.y -= fTurnSpeedY * 2.f;


		m_pTransform->Set_Scale(vScale);
		m_pTransform->Set_World(WORLD_POS, vPos);

		/*_float4 vPos = m_pTransform->Get_World(WORLD_POS);

		vPos.z += fTurnSpeedY;

		if (vPos.z < 0.5f)
			vPos.z = 0.5f;
		else if (vPos.z > 3.5f)
			vPos.z = 3.5f;

		m_pTransform->Set_World(WORLD_POS, vPos);*/
	}

}

void CCustomizingPreview::My_LateTick()
{
	GET_COMPONENT(CTexture)->Set_ShaderResourceView(GET_COMPONENT(CShader), "g_BlendTexture");
}
