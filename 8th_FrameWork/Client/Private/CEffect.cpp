#include "stdafx.h"
#include "CEffect.h"

#include "HIerarchyNode.h"
#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"
#include "CNavigation.h"
#include "Texture.h"

#include "Transform.h"
#include "CComponent_Factory.h"

#include "CCamera_Follow.h"
#include "CEffects_Factory.h"
#include "CUtility_Transform.h"

#include "CDefault_Effect.h"
#include "Functor.h"
#include "CCollider_Sphere.h"
#include "CRectEffects.h"

CEffect::CEffect()
{
}

CEffect::~CEffect()
{
}

CEffect* CEffect::Create_Effect_FromBinFile(string strFileKey)
{
	CEffect* pNewEffect = nullptr;

	string savePath = "../bin/effects/";
	savePath += strFileKey;
	savePath += ".bin";

	ifstream	readFile(savePath, ios::binary);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"Load 실패 ??!?!");
		return nullptr;
	}
	EFFECT_TYPE	eType = CEffect::MESH;
	readFile.read((char*)&eType, sizeof(_uint));

	switch (eType)
	{
	case Client::CEffect::MESH:
		pNewEffect = CDefault_Effect::Create(&readFile);
		if (!pNewEffect)
		{
			Call_MsgBox(L"Failed to Initialize_Prototype : CDefault_Effect");
			SAFE_DELETE(pNewEffect);
			return nullptr;
		}

		{
			if (FAILED(pNewEffect->CEffect::Initialize_Prototype()))
			{
				Call_MsgBox(L"Failed to Initialize_Prototype : CDefault_Effect");
				SAFE_DELETE(pNewEffect);
				return nullptr;
			}

			CModel* pModelCom = GET_COMPONENT_FROM(pNewEffect, CModel);
			CTexture* pTextureCom = pModelCom->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS];
			pTextureCom->Add_Texture(pNewEffect->m_wstrColorMapPath.c_str());
			pTextureCom->Set_CurTextureIndex(pTextureCom->Get_TextureSize() - 1);

			pTextureCom = pModelCom->Get_Materials().front().second.pTextures[1];
			pTextureCom->Add_Texture(pNewEffect->m_wstrMaskMapPath.c_str());
			pTextureCom->Set_CurTextureIndex(pTextureCom->Get_TextureSize() - 1);
		}

		break;

	case Client::CEffect::PARTICLE:
		pNewEffect = CRectEffects::Create(&readFile);
		if (!pNewEffect)
		{
			Call_MsgBox(L"Failed to Create : CRectEffects");
			return nullptr;
		}
		break;

	case Client::CEffect::ANIMPARTICLE:
		pNewEffect = CRectEffects::Create_Anim(&readFile);
		if (!pNewEffect)
		{
			Call_MsgBox(L"Failed to Create : CRectEffects");
			return nullptr;
		}
		break;

	case Client::CEffect::EFFECT_END:
		break;

	default:
		break;
	}

	readFile.close();
	pNewEffect->m_hcMyCode = Convert_ToHash(CFunctor::To_Wstring(strFileKey));
	

	return pNewEffect;
}

void CEffect::Self_Reset(CGameObject* pGameObject, _float4 vStartPos)
{
	m_pFollowTarget = pGameObject;
	Reset(pGameObject->Get_Transform()->Get_WorldMatrix());
}

void CEffect::Self_Reset_Turn(CGameObject* pGameObject, _float4 vStartPos)
{
	m_pFollowTarget = pGameObject;
	m_fTurnAngle = frandom(0.f, 180.f);
	Reset(vStartPos);
}

void CEffect::Reset(_float4 vStartPos)
{
	_float4 vPos = vStartPos + m_vOffsetPos;

	m_pTransform->Set_World(WORLD_POS, vPos);
	m_pTransform->Make_WorldMatrix();
	ENABLE_GAMEOBJECT(this);
}

void CEffect::Reset(CGameObject* pFollowTarget)
{
	m_pFollowTarget = pFollowTarget;
	ENABLE_GAMEOBJECT(this);
}

void CEffect::Reset(_float4 vStartPos, _float4 vTargetPosition)
{
	m_vTargetPosition = vTargetPosition;
	Reset(vStartPos);
}

void CEffect::Reset(_float4 vStartPos, _float4x4 matTrans)
{
	m_matTrans = matTrans;
	Reset(vStartPos);
}

void CEffect::Reset(_float4x4 matWorld)
{
	m_pTransform->Get_Transform().matMyWorld = matWorld;
	_float4 vOffsetPos = m_vOffsetPos.MultiplyCoord(matWorld);
	m_pTransform->Set_World(WORLD_POS, vOffsetPos);
	
	m_pTransform->Make_WorldMatrix();
	ENABLE_GAMEOBJECT(this);

}

void CEffect::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
	if (!(m_bEffectFlag & EFFECT_COLLIDER))
		DISABLE_GAMEOBJECT(this);
}

void CEffect::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	pShader->Set_RawValue("g_fUVPlusX", &m_fCurUVPlusX, sizeof(_float));
	pShader->Set_RawValue("g_fUVPlusY", &m_fCurUVPlusY, sizeof(_float));
}

void CEffect::Set_ShaderResourceAlpha(CShader* pShader, const char* pConstantName)
{
	pShader->Set_RawValue(pConstantName, &m_fAlpha, sizeof(_float));
}

void CEffect::Set_ShaderResourceFlag(CShader* pShader, const char* pConstantName)
{
	pShader->Set_RawValue(pConstantName, &m_vEffectFlag, sizeof(_float4));
}

void CEffect::Set_ShaderResourceGlowFlag(CShader* pShader, const char* pConstantName)
{
	pShader->Set_RawValue(pConstantName, &m_vGlowFlag, sizeof(_float4));
}

void CEffect::Set_ColliderOn(_float fRadius, COL_GROUP_CLIENT eColType)
{
	ENABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));
	GET_COMPONENT(CCollider_Sphere)->Get_ColInfo().fRadius = fRadius;
	GET_COMPONENT(CCollider_Sphere)->Set_ColIndex(eColType);
	m_bColliderOn = true;
	m_fColliderAcc = 0.f;
}


HRESULT CEffect::Start()
{
	__super::Start();

	GET_COMPONENT(CModel)->Set_ShaderPassToAll(m_iPassType);

	if (m_eShaderType == SHADER_VTXEFFECTS)
	{
		if (m_iPassType == VTXEFFECT_PASS_DISTORTION)
		{
			GET_COMPONENT(CRenderer)->Set_RenderGroup(RENDER_DISTORTION);
		}
	}

	SHADER_BINDING(CEffect, "g_fUVPlus");

	GET_COMPONENT(CModel)->Set_ShaderFlag(m_vEffectFlag);
	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CEffect::Set_ShaderResourceAlpha, this, placeholders::_1, "g_fAlpha");
	GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CEffect::Set_ShaderResourceGlowFlag, this, placeholders::_1, "g_vGlowFlag");


	CallBack_CollisionEnter += bind(&CEffect::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);




	return S_OK;
}

HRESULT CEffect::Initialize_Prototype()
{
	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, m_eShaderType,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_ALPHA, m_iPassType
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);
	Add_Component(CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, m_wstrPath.c_str(), m_matTrans));

	if (m_bEffectFlag & EFFECT_COLLIDER)
	{
		CCollider_Sphere* pCollider = CComponent_Factory::Clone_Component<CCollider_Sphere>(this);
		pCollider->Get_ColInfo().fRadius = m_vFadeInTargetScale.x;
		Add_Component(pCollider);
	}
	
	//pCollider->Initialize();

	return S_OK;
}

HRESULT CEffect::Initialize()
{
	m_pTransform->Set_Scale(m_vStartScale);

	return S_OK;
}

void CEffect::Update_FollowTarget()
{
	_float4 vLook = m_pFollowTarget->Get_Transform()->Get_World(WORLD_LOOK);
	_float4 vPos = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);

	_float4 vOffsetPos = m_vOffsetPos.MultiplyCoord(m_pFollowTarget->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS));
	vPos += vOffsetPos;

	m_pTransform->Set_Look(vLook);
	m_pTransform->Set_World(WORLD_POS, vPos);

	/*if (m_fTurnAngle > 0.f)
	{
		CUtility_Transform::Turn_ByAngle(m_pTransform, vLook, m_fTurnAngle);
	}*/
	m_pTransform->Make_WorldMatrix();
}

void CEffect::Update_TargetPos()
{
	On_Target();
}

void CEffect::On_Target()
{
	_float fDistance = (m_vTargetPosition - m_pTransform->Get_World(WORLD_POS)).Length();

	if (fDistance <= 0.2f)
	{
		m_eCurFadeType = FADEOUT;
		GET_COMPONENT(CPhysics)->Set_Speed(0.f);
	}
	else
		GET_COMPONENT(CPhysics)->Set_Accel(100.f);

}

void CEffect::Stick_RefBone()
{
	_float4 vBonePos = m_vOffsetPos;
	_matrix matBone = m_pRefBone->Get_CombinedMatrix();

	matBone = XMMatrixMultiply(matBone, DEFAULT_TRANS_MATRIX);

	_float4x4 BoneMatrix;
	XMStoreFloat4x4(&BoneMatrix, matBone);

	_float4x4 matWorld = m_pFollowTarget->Get_Transform()->Get_WorldMatrix();

	//콤바인 위치 * 월드매트릭스
	vBonePos = vBonePos.MultiplyCoord(BoneMatrix);
	vBonePos = vBonePos.MultiplyCoord(matWorld);


	_float4 vBoneLook = (*((_float4*)&BoneMatrix.m[WORLD_LOOK]));
	vBoneLook = vBoneLook.MultiplyNormal(matWorld);

	//vBonePos = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);

	m_pTransform->Set_World(WORLD_POS, vBonePos);
	m_pTransform->Set_Look(vBoneLook);
	m_pTransform->Make_WorldMatrix();
}

void CEffect::My_Tick()
{
	if (m_bColliderOn)
	{
		m_fColliderAcc += fDT;
		if (m_fColliderAcc > m_fColliderMaxTime)
		{
			m_bColliderOn = false;
			m_fColliderAcc = 0.f;
			DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));
		}
	}

	if (m_bEffectFlag & EFFECT_TARGETPOS)
	{
		On_Target();
	}
}

void CEffect::My_LateTick()
{

	if (m_bEffectFlag & EFFECT_REFBONE)
	{
		Stick_RefBone();
	}

	//Follow


	else if (m_bEffectFlag & EFFECT_FOLLOWTARGET)
	{
		Update_FollowTarget();
	}

	if (m_fTurnSpeed > 0.f)
		Update_Turn();

	//UV
	m_fCurUVPlusX += m_fUVSpeedX * fDT;
	m_fCurUVPlusY += m_fUVSpeedY * fDT;

	Update_Disable();

	if (fabs(m_fCurUVPlusX) >= 1.f)
		m_fCurUVPlusX = 0.f;
	if (fabs(m_fCurUVPlusY) >= 1.f)
		m_fCurUVPlusY = 0.f;

	Update_Fade();

	if (m_bEffectFlag & EFFECT_BILLBOARD)
	{
		_float4 vLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
		m_pTransform->Set_Look(vLook * -1.f);
		m_pTransform->Make_WorldMatrix();
	}
}

void CEffect::OnDisable()
{
	__super::OnDisable();
	CEffects_Factory::Get_Instance()->Collect_Effect(m_hcMyCode, this);
	m_fTimeAcc = 0.f;
	m_fCurUVPlusX = 0.f;
	m_fCurUVPlusY = 0.f;
	m_fFadeTimeAcc = 0.f;
	m_fTurnAngle = 0.f;

	if (m_eDisableType == FADE)
		m_fAlpha = 0.f;
	m_eCurFadeType = FADEINREADY;
	/*if (m_fUVSpeed > 0.f)
		SHADER_BINDINGOUT(CEffect, "g_fUVPlus");*/
}

void CEffect::Update_Disable()
{
	switch (m_eDisableType)
	{
	case Client::CEffect::UV:
		if (fabs(m_fCurUVPlusX) >= 1.f || fabs(m_fCurUVPlusY) >= 1.f)
			DISABLE_GAMEOBJECT(this);

		break;
	case Client::CEffect::WALL:
		if (GET_COMPONENT(CNavigation)->Is_OnWall())
			DISABLE_GAMEOBJECT(this);


		break;
	case Client::CEffect::NONE:
		break;
	case Client::CEffect::DISABLE_END:
		break;
	default:
		break;
	}
}

void CEffect::Update_Turn()
{
	m_fTurnAngle += m_fTurnSpeed * fDT;

	//내 회전각에 타겟의 월드 곱하던가 머하던가
	_float4 vTurnDir = m_vTurnDir;

	if (m_bEffectFlag & EFFECT_FOLLOWTARGET)
		vTurnDir = m_vTurnDir.MultiplyNormal(m_pFollowTarget->Get_Transform()->Get_WorldMatrix());


	CUtility_Transform::Turn_ByAngle(m_pTransform, vTurnDir.Normalize(), m_fTurnAngle);

	m_pTransform->Make_WorldMatrix();
}

void CEffect::Update_Fade()
{
	if (m_eDisableType != FADE)
		return;

	m_fFadeTimeAcc += fDT;

	switch (m_eCurFadeType)
	{
	case FADEINREADY:
		if (m_fFadeTimeAcc >= m_fFadeInStartTime)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = FADEIN;
			m_pTransform->Set_Scale(m_vStartScale);
			m_fAlpha = 0.f;
			m_fCurUVPlusX = 0.f;
			m_fCurUVPlusY = 0.f;
		}
		else
		{
			m_pTransform->Set_Scale(m_vStartScale);
			m_fAlpha = 0.f;
		}


		break;

	case FADEIN:
		if (m_fFadeTimeAcc >= m_fFadeInTime)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = FADEOUTREADY;
			m_pTransform->Set_Scale(m_vFadeInTargetScale);
			m_fAlpha = m_fTargetAlpha;
		}
		else
		{
			_float fRatio = m_fFadeTimeAcc / m_fFadeInTime;
			_float4 vScale =
				XMVectorLerp(m_vStartScale.XMLoad(), m_vFadeInTargetScale.XMLoad(), fRatio);

			m_fAlpha = m_fTargetAlpha * fRatio;
			m_pTransform->Set_Scale(vScale);

		}

		break;

	case FADEOUTREADY:
		if (m_fFadeTimeAcc >= m_fFadeOutStartTime)
		{
			m_fFadeTimeAcc = 0.f;
			m_eCurFadeType = FADEOUT;
		}

		break;

	case FADEOUT:
		if (m_fFadeTimeAcc >= m_fFadeOutTime)
		{
			DISABLE_GAMEOBJECT(this);
		}
		else
		{
			_float fRatio = m_fFadeTimeAcc / m_fFadeOutTime;
			_float4 vScale =
				XMVectorLerp(m_vFadeInTargetScale.XMLoad(), m_vFadeOutTargetScale.XMLoad(), sqrtf(fRatio));

			m_fAlpha = m_fTargetAlpha * (1.f - fRatio);
			m_pTransform->Set_Scale(vScale);

		}
		 
		break;

	default:
		break;
	}

	m_pTransform->Make_WorldMatrix();


}
