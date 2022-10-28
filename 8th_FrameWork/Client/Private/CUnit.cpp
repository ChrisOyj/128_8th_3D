#include "stdafx.h"
#include "CUnit.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "CModel_Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "Physics.h"
#include "CMesh_Rect.h"
#include "CMesh_Terrain.h"
#include "Model.h"
#include "CAnimator.h"
#include "CBlender.h"
#include "CModel_Renderer.h"
#include "MeshContainer.h"
#include "CCollider_Sphere.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

#include "CState.h"

#include "CState_Manager.h"

#include "CNavigation.h"
#include "CCell.h"
#include "CKOLine.h"
#include "CUser.h"
#include "CScript_FollowCam.h"

#include "CEffects_Factory.h"

#include "CTrailBuffer.h"
#include "CTrailEffect.h"

#include "CBoneChakra.h"

CUnit::CUnit()
{
}

CUnit::~CUnit()
{
	SAFE_DELETE(m_pCurState);
}

void CUnit::Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
	Set_Passes(VTXANIM_PASS_HIT);
	GET_COMPONENT(CShader)->CallBack_SetRawValues +=
		bind(&CUnit::Set_ShaderResource, this, placeholders::_1, "g_fWhite");

	m_fHitWhite = 1.f;

	m_bHit = true;
}

void CUnit::Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eColType)
{
	if (eColType == COL_WALL)
	{
		_float4 vCurPos = m_pTransform->Get_World(WORLD_POS);
		_float fSpeed = m_pPhysics->Get_Physics().fSpeed;
		_float4 vDir = vCurPos - pOtherObj->Get_Transform()->Get_World(WORLD_POS);
		vCurPos += vDir.Normalize() * fSpeed * fDT;
		m_pTransform->Set_World(WORLD_POS, vCurPos);
		m_pTransform->Make_WorldMatrix();
	}
}

void CUnit::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	pShader->Set_RawValue(pConstantName, &m_fHitWhite, sizeof(_float));

}

void CUnit::Set_Passes(VTXANIM_PASS_TYPE ePassType)
{
	vector<pair<_uint, class CMeshContainer*>>& vecMeshCont = m_pModelCom->Get_MeshContainers();
	for (auto& pMC : vecMeshCont)
	{
		pMC.second->Set_CurPass(ePassType);
	}
}

void CUnit::On_PlusHp(_float fHp)
{
	m_tUnitStatus.fHP += fHp;

	if (m_tUnitStatus.fHP <= 0.f)
	{
		m_tUnitStatus.fHP = 0.f;
		On_Die();
	}
	else if (m_tUnitStatus.fHP >= m_tUnitStatus.fMaxHP)
	{
		m_tUnitStatus.fHP = m_tUnitStatus.fMaxHP;
	}
}

void CUnit::On_Die()
{
	Enter_State(STATE_DEATH_ENEMY);

	for (_uint i = 0; i < PARTS_END; ++i)
		DISABLE_COMPONENT(m_pColliders[i]);

	//CAMERA_LERP(CAMERA_LERP_KO);
	if (m_tUnitStatus.fMaxHP > 200.f)
		CUser::Get_Instance()->On_BossDeath();
	else
		CUser::Get_Instance()->On_KO();

	_float4 vPos = m_pTransform->Get_World(WORLD_POS);
	vPos -= m_pTransform->Get_World(WORLD_LOOK) * 1.5f;
	vPos.y += 1.5f;

	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Line_0"), vPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Line_1"), vPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Line_2"), vPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Line_3"), vPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Line_4"), vPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Line_5"), vPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Line_6"), vPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Line_7"), vPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Line_8"), vPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Line_9"), vPos);
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"KOEffect"), vPos);

	vPos.y += 0.3f;
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"KOTEXT"), vPos);


	list<CGameObject*>& EnemyList = GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY);

	_bool bClear = true;
	for (auto& elem : EnemyList)
	{
		if (elem == this)
			continue;

		//한놈이라도 살아있을경우
		if (static_cast<CUnit*>(elem)->Get_Status().fHP > 0.f)
		{
			bClear = false;
			break;
		}
	}

	if (bClear)
	{
		if (m_tUnitStatus.fMaxHP < 200.f)
			CUser::Get_Instance()->On_ClearIntro();
	}


}

void CUnit::Switch_Bone(MODEL_PART_TYPE eType, string strBoneName)
{
	m_pModelCom->Switch_MeshContainerBone(eType, strBoneName);
}

void CUnit::Enter_State(STATE_TYPE eType)
{
	if (m_pCurState)
		m_pCurState->Exit(this, m_pAnimator);
	SAFE_DELETE(m_pCurState);
	m_eCurState = eType;
	m_pCurState = CState_Manager::Get_Instance()->Get_State(eType)->Clone();
	m_pCurState->Enter(this, m_pAnimator);
}

void CUnit::Enable_Collider(COLLIDER_PARTS eIndex, _bool bEnable)
{
	if (eIndex >= PARTS_END)
		return;

	if (bEnable)
		ENABLE_COMPONENT(m_pColliders[eIndex]);
	else
		DISABLE_COMPONENT(m_pColliders[eIndex]);
}

CCollider_Sphere* CUnit::Get_Collider(COLLIDER_PARTS eIndex)
{
	return static_cast<CCollider_Sphere*>(m_pColliders[eIndex]);
}

void CUnit::Disable_AllColliders()
{
	DISABLE_COMPONENT(m_pColliders[BODY]);
	DISABLE_COMPONENT(m_pColliders[HITBOX]);
}

void CUnit::Enable_BodyColliders()
{
	ENABLE_COMPONENT(m_pColliders[BODY]);
	ENABLE_COMPONENT(m_pColliders[HITBOX]);
}

void CUnit::Create_FootTrails()
{
	CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CBoneChakra), this, GET_COMPONENT_FROM(this, CModel)->Find_HierarchyNode("RightFoot"));
	CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CBoneChakra), this, GET_COMPONENT_FROM(this, CModel)->Find_HierarchyNode("LeftFoot"));
}

void CUnit::Enable_FootTrails(_bool bBool)
{
	if (bBool)
	{
		if (!m_pLeftFootTrail)
			m_pLeftFootTrail = static_cast<CBoneChakra*>(
				CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CBoneChakra), this, GET_COMPONENT_FROM(this, CModel)->Find_HierarchyNode("LeftFoot")));
		else
			ENABLE_GAMEOBJECT(m_pLeftFootTrail);

		if (!m_pRightFootTrail)
			m_pRightFootTrail = static_cast<CBoneChakra*>
			(CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CBoneChakra), this, GET_COMPONENT_FROM(this, CModel)->Find_HierarchyNode("RightFoot")));
		else
			ENABLE_GAMEOBJECT(m_pRightFootTrail);

	}
	else
	{
		if (m_pLeftFootTrail)
			DISABLE_GAMEOBJECT(m_pLeftFootTrail);

		if (m_pRightFootTrail)
			DISABLE_GAMEOBJECT(m_pRightFootTrail);
	}


}

HRESULT CUnit::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXANIMMODEL,
		VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);

	CAnimator* pAnimator = CAnimator::Create(CP_BEFORE_RENDERER, L"../bin/resources/animations/Custom_Base.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/Custom_Attack_Hand.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/Custom_Attack_Kunai.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/Custom_Attack_NinzaSword.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/Custom_Attack_BigSword.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/Custom_Throw.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/Custom_Beaten.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/Custom_Guard.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/Custom_Skill_Attack.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/Custom_Skill_Shoot.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/CustomMan_Boruto_etc_Win_Type01_Loop.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/CustomMan_etc_Emotion_D32_Loop.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/CustomMan_etc_Emotion_etc_Win_Type01_Loop.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/CustomMan_etc_Emotion_Jiraiya_etc_Win_Type01_Loop.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/CustomMan_etc_Emotion_Pose_Type01_Loop.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/CustomMan_etc_Appearance.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/CustomMan_etc_Emotion_Sports_Type03_Loop.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/CustomMan_etc_Emotion_Dance_Type08_Loop.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/Custom_Lomin.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/Custom_Skill_Defend.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/Custom_EightGates.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/Custom_Koto.fbx");
	pAnimator->Add_Animations(L"../bin/resources/animations/CustomMan_Ninjutsu_Aerial_D21NJ3_Start.fbx");
	Add_Component(pAnimator);

	Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));

	_float fRadiusSize = 0.5f;

	CCollider_Sphere* pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().vOffsetPos.y = 0.4f;
	pCollider->Get_ColInfo().fRadius = 0.3f;
	Add_Component(pCollider);
	m_pColliders[BODY] = pCollider;

	pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().vOffsetPos.y = 0.7f;
	pCollider->Get_ColInfo().fRadius = 1.2f;
	Add_Component(pCollider);
	m_pColliders[HITBOX] = pCollider;

	pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().vOffsetPos.y = 0.4f;
	pCollider->Get_ColInfo().fRadius = 0.3f;
	Add_Component(pCollider);
	m_pColliders[REALBODY] = pCollider;

	pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = fRadiusSize;
	pCollider->Set_RefBone(GET_COMPONENT(CModel)->Find_HierarchyNode("RightArm"));
	Add_Component(pCollider);
	m_pColliders[RIGHTHAND] = pCollider;

	pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = fRadiusSize;
	pCollider->Set_RefBone(GET_COMPONENT(CModel)->Find_HierarchyNode("LeftArm"));
	Add_Component(pCollider);
	m_pColliders[LEFTHAND] = pCollider;

	pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = fRadiusSize * 1.5f;
	pCollider->Set_RefBone(GET_COMPONENT(CModel)->Find_HierarchyNode("RightLeg"));
	Add_Component(pCollider);
	m_pColliders[RIGHTFOOT] = pCollider;

	pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = fRadiusSize * 1.5f;
	pCollider->Set_RefBone(GET_COMPONENT(CModel)->Find_HierarchyNode("LeftLeg"));
	Add_Component(pCollider);
	m_pColliders[LEFTFOOT] = pCollider;


	pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().fRadius = fRadiusSize * 1.5f;
	pCollider->Set_RefBone(GET_COMPONENT(CModel)->Find_HierarchyNode("R_Hand_Weapon_cnt_tr"));
	Add_Component(pCollider);
	m_pColliders[NINZASWORD] = pCollider;


	pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
	pCollider->Get_ColInfo().vOffsetPos.y = 0.5f;
	pCollider->Get_ColInfo().vOffsetPos.z = 0.5f;
	pCollider->Get_ColInfo().fRadius = fRadiusSize * 5.f;
	Add_Component(pCollider);
	m_pColliders[SKILLRANGE] = pCollider;


	/* CBlender* pBlender = CBlender::Create(
		 L"../bin/resources/textures/animmodeltextures/T_CHR_Face_Byakugan_M.dds",
		 L"../bin/resources/textures/animmodeltextures/T_CHR_FP_Naruto2.dds");

	 Add_Component(pBlender);*/

	return S_OK;
}

HRESULT CUnit::Initialize()
{
	m_pModelCom = GET_COMPONENT(CModel);
	m_pAnimator = GET_COMPONENT(CAnimator);
	m_pPhysics = GET_COMPONENT(CPhysics);

	if (!m_pModelCom)
		return E_FAIL;

	if (!m_pAnimator)
		return E_FAIL;

	if (!m_pPhysics)
		return E_FAIL;

	m_pModelCom->Set_ShaderPass(MODEL_PART_FACE, VTXANIM_PASS_FACE);

	vector<pair<_uint, MODEL_MATERIAL>>& vecMaterials = m_pModelCom->Get_Materials();
	vector<pair<_uint, CMeshContainer*>>& vecMC = m_pModelCom->Get_MeshContainers();

	for (auto& elem : vecMC)
	{
		if (vecMaterials[elem.second->Get_MaterialIndex()].second.strName == "MI_CHR_Hachigane_Konoha_01" ||
			vecMaterials[elem.second->Get_MaterialIndex()].second.strName == "MI_WEP_Sword_01" ||
			vecMaterials[elem.second->Get_MaterialIndex()].second.strName == "MI_WEP_Kurosawa")
			elem.second->Set_ShaderFlag(SH_LIGHT_DEFAULT);

		if (vecMaterials[elem.second->Get_MaterialIndex()].second.strName.find("Eye") > 0)
			elem.second->Set_ShaderFlag(SH_LIGHT_NOOUTLINE);
	}

	return S_OK;
}

HRESULT CUnit::Start()
{
	__super::Start();
	
	m_pPhysics->Get_PhysicsDetail().fCurGroundY = m_pTransform->Get_MyWorld(WORLD_POS).y;

	//m_pAnimator->Set_CurAnimIndex(ANIM_BASE, 22);
	DISABLE_COMPONENT(m_pPhysics);
	m_pPhysics->Set_NaviOn();

	


	for (_uint i = RIGHTHAND; i < PARTS_END; ++i)
		DISABLE_COMPONENT(m_pColliders[i]);

	CallBack_CollisionEnter += bind(&CUnit::Unit_CollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);
	CallBack_CollisionStay += bind(&CUnit::Unit_CollisionStay, this, placeholders::_1, placeholders::_2);
	CallBack_CollisionExit += bind(&CUnit::Unit_CollisionExit, this, placeholders::_1, placeholders::_2);

	CState_Manager::Get_Instance()->Get_State(STATE_POSE)->Set_AnimType(m_eStartPose);
	Enter_State(STATE_POSE);

	Enable_FootTrails(true);


	return S_OK;
}

void CUnit::OnEnable()
{
	__super::OnEnable();
	m_pPhysics->Get_PhysicsDetail().fCurGroundY = m_pTransform->Get_MyWorld(WORLD_POS).y;

	//m_pAnimator->Set_CurAnimIndex(ANIM_BASE, 22);
	DISABLE_COMPONENT(m_pPhysics);

	for (_uint i = RIGHTHAND; i < PARTS_END; ++i)
		DISABLE_COMPONENT(m_pColliders[i]);

	On_PlusHp(m_tUnitStatus.fMaxHP - m_tUnitStatus.fHP);


	m_pLeftFootTrail = nullptr;
	m_pRightFootTrail = nullptr;

	Enable_FootTrails(true);

}

void CUnit::OnDisable()
{
	__super::OnDisable();


	Enable_FootTrails(false);

}

HRESULT CUnit::SetUp_Model(UNIT_MODEL_DATA tData)
{
	m_tModelData = tData;
	_float4x4			TransformMatrix;
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationZ(XMConvertToRadians(180.0f));

	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_ANIM, tData.strModelPaths[MODEL_PART_FACE], TransformMatrix);


	if (!pModel)
		return E_FAIL;

	for (_uint i = 1; i < MODEL_PART_END; ++i)
	{
		if (tData.strModelPaths[i].empty())
			continue;

		if (tData.bOverall)
		{
			if (i == MODEL_PART_BODYUPPER || i == MODEL_PART_BODYLOWER)
				continue;
		}
		else
		{
			if (i == MODEL_PART_OVERALL)
				continue;
		}

		if (!tData.strRefBoneName[i].empty())
		{
			if (FAILED(pModel->Add_Model(tData.strModelPaths[i], i, tData.strRefBoneName[i])))
				return E_FAIL;
		}
		else
		{
			if (FAILED(pModel->Add_Model(tData.strModelPaths[i], i)))
				return E_FAIL;
		}



		if (i == MODEL_PART_BODYLOWERSUB ||
			i == MODEL_PART_BODYUPPERSUB ||
			i == MODEL_PART_HEADGEARSUB ||
			i == MODEL_PART_OVERALLSUB)
			pModel->Enable_ModelParts(i, false);
	}


	vector<pair<_uint, MODEL_MATERIAL>>& vecMaterials = pModel->Get_Materials();

	for (auto& elem : vecMaterials)
	{
		if (elem.second.strName == "MI_CHR_Eyes_WThinL_01" ||
			elem.second.strName == "MI_CHR_Eyes_WThinR_01")
		{
			for (_uint i = 0; i < EYE_END; ++i)
			{
				_tchar szBuff[MAX_STR] = L"";
				swprintf_s(szBuff, L"../bin/resources/textures/animModelTextures/Eyes/Eye_%d.dds", i);
				elem.second.pTextures[1]->Add_Texture(szBuff);
			}

			elem.second.pTextures[1]->Set_CurTextureIndex(tData.iEyeIndex);
		}
	}



	pModel->Initialize();

	Add_Component(pModel);

	m_eCurState = STATE_POSE;
	m_eStartPose = tData.eStartPose;

	return S_OK;
}

HRESULT CUnit::SetUp_Navigation(CCell* pStartCell)
{
	CNavigation* pNaviCom = CNavigation::Create(CP_NAVIGATION, pStartCell, m_pPhysics);

	if (!pNaviCom)
		return E_FAIL;

	Add_Component(pNaviCom);
	pNaviCom->Set_Epsilon(1.f);

	_float4 vStartPos;
	XMStoreFloat4(&vStartPos, pStartCell->Get_Point(CCell::POINT_A));
	vStartPos.w = 1.f;
	vStartPos.x += 0.1f;
	vStartPos.z += 0.1f;
	//vStartPos.y = -100.f;
	m_pTransform->Set_World(WORLD_POS, vStartPos);

	//pNaviCom->Set_ExamArena(true, vStartPos);
	return S_OK;
}

void CUnit::My_Tick()
{
	if (!m_pCurState)
	{
		Call_MsgBox(L"State is Nullptr : CUnit");
		return;
	}

	STATE_TYPE eNewState = STATE_END;
	eNewState = m_pCurState->Tick(this, m_pAnimator);

	if (STATE_END != eNewState)
	{
		//새로운게 플레이어고
#ifdef _DEBUG
		if (eNewState < STATE_POSE)
		{
			//기존에 에너미였다면
			if (m_eCurState >= STATE_IDLE_ENEMY)
			{
				int i = 0;
			}
		}

		if (!m_bControlable)
			return;
#endif // _DEBUG

		Enter_State(eNewState);
	}

	//상태패턴

	/*
	가능한 상태가 정해져있고
	매 프레임마다
	해당 상태로 넘어가는 조건을 체크해주기
	조건은? 싯팔 어카냐 걍 다 만들자 할만해.
	*/
}

void CUnit::My_LateTick()
{
	if (!m_bHit)
		return;

	m_fHitWhite -= 6.f * fDT;

	if (m_fHitWhite <= 0.f)
	{
		m_bHit = false;
		Set_Passes(VTXANIM_PASS_DEFAULT);

		GET_COMPONENT(CShader)->CallBack_SetRawValues -=
			bind(&CUnit::Set_ShaderResource, this, placeholders::_1, "g_fWhite");

		m_fHitWhite = 1.f;
	}

}
