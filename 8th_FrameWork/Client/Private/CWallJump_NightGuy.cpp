#include "stdafx.h"
#include "CWallJump_NightGuy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"
#include "CNavigation.h"

#include "CDoubleJump_NightGuy.h"
#include "CState_Manager.h"

#include "CScript_FollowCam.h"
#include "Renderer.h"
#include "CEffects_Factory.h"
#include "CEffect.h"

#include "CUtility_Transform.h"

#include "Model.h"
#include "Texture.h"
#include "Functor.h"
CWallJump_NightGuy::CWallJump_NightGuy()
{
}

CWallJump_NightGuy::~CWallJump_NightGuy()
{
}

CWallJump_NightGuy* CWallJump_NightGuy::Create()
{
	CWallJump_NightGuy* pInstance = new CWallJump_NightGuy();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWallJump_NightGuy");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWallJump_NightGuy::Initialize()
{
	m_iAnimIndex = 77;
	m_eStateType = STATE_WALLJUMP_NIGHTGUY;

	m_vecAdjState.push_back(STATE_WALLJUMPEND_NIGHTGUY);
	m_vecAdjState.push_back(STATE_DOUBLEJUMP_NIGHTGUY);

	m_fInterPolationTime = 0.1f;
	m_fStateChangableTime = 0.1f;
	return S_OK;
}

void CWallJump_NightGuy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
	CFunctor::Play_Sound(L"Eff_WallJump", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	DISABLE_COMPONENT(GET_COMPONENT_FROM(pOwner, CRenderer));

	m_vEffectUp.Normalize();
	/* Owner의 Animator Set Idle로 */
	__super::Enter(pOwner, pAnimator);
	CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
	CTransform* pTransform = pOwner->Get_Transform();
	m_fTargetY = GET_COMPONENT_FROM(pOwner, CNavigation)->Get_TargetY();

	_float4 vLook = GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurWallNormal() * -1.f;
	vLook.y = 0.f;
	vLook.Normalize();
	pTransform->Set_Look(vLook);

	/*  if (GET_COMPONENT_FROM(pOwner, CNavigation)->Is_OnWall())
	  {
		  _float4 vDir = pTransform->Get_World(WORLD_UP);
		  pPhyscisCom->Set_Dir(pTransform->Get_World(WORLD_UP));
		  pPhyscisCom->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed * 0.7f);
		  pPhyscisCom->Set_SpeedasMax();
		  pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);
		  _float4 vPos = pTransform->Get_World(WORLD_POS);
		  vPos += vDir * 1.f;
		  pTransform->Set_World(WORLD_POS, vPos);
		  pTransform->Make_WorldMatrix();
		  return;
	  }*/

	pPhyscisCom->Set_MaxSpeed(pOwner->Get_Status().fWallJumpSpeed * 1.3f);
	pPhyscisCom->Set_Dir(_float4(0.f, 1.f, 0.f, 0.f));
	pPhyscisCom->Set_Jump(pOwner->Get_Status().fJumpPower);

	CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_WALLJUMP);
}

STATE_TYPE CWallJump_NightGuy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

	pPhyscisCom->Set_Accel(50.f);

	if (pOwner->Get_Transform()->Get_World(WORLD_POS).y >= m_fTargetY)
		return m_vecAdjState.front();

	m_fEffectTimeAcc += fDT;

	if (m_fEffectTimeAcc > m_fEffectTime)
	{
		CFunctor::Play_Sound(L"Eff_WallJump", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

		m_fEffectTimeAcc = 0.f;
		CGameObject* pEffect = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"WallJump"), pOwner->Get_Transform()->Get_WorldMatrix());
		static_cast<CEffect*>(pEffect)->Set_EffectFlag(0);

		CUtility_Transform::Turn_ByAngle(pEffect->Get_Transform(), pOwner->Get_Transform()->Get_World(WORLD_LOOK), m_fEffectDegree);
		m_fEffectDegree *= -1.f;

		/* _float4 vUp = m_vEffectUp.MultiplyNormal(pOwner->Get_Transform()->Get_WorldMatrix());
		 vUp.z = 0.f;
		 pEffect->Get_Transform()->Set_Up(vUp.Normalize());
		 m_vEffectUp.x *= -1.f;*/

		CTexture* pWallJumpTexture = GET_COMPONENT_FROM(pEffect, CModel)->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS];


		CTexture* pModelTexture = GET_COMPONENT_FROM(pOwner, CModel)->Get_TextureFromParts(MODEL_PART_OVERALL, MODEL_PART_BODYUPPER);


		pWallJumpTexture->Get_vecTexture().front() = pModelTexture->Get_vecTexture().front();
		pWallJumpTexture->Set_CurTextureIndex(0);


		_float4 vOffsetPos = m_vOffsetPos.MultiplyCoord(pOwner->Get_Transform()->Get_WorldMatrix());
		pEffect->Get_Transform()->Set_World(WORLD_POS, vOffsetPos);
		pEffect->Get_Transform()->Make_WorldMatrix();
		m_vOffsetPos.x *= -1.f;

	}

	return __super::Tick(pOwner, pAnimator);
}

void CWallJump_NightGuy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	ENABLE_COMPONENT(GET_COMPONENT_FROM(pOwner, CRenderer));


	CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
	//목표에 도달했으면 앞으로 가고
	if (pOwner->Get_Transform()->Get_World(WORLD_POS).y >= m_fTargetY)
	{
		pPhyscisCom->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));
	}
	else
		//실패했으면 뒤로가
		pPhyscisCom->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK) * -1.f);


	CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);

}

STATE_TYPE CWallJump_NightGuy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Run으로 오는 조건
	1. 키눌렸을때
	*/
	if (CUser::Get_Instance()->Get_LastKey() == KEY::SPACE &&
		GET_COMPONENT_FROM(pOwner, CNavigation)->Is_OnWall()
		)
	{
		//벽의 법선벡터
		_float4 vCellNormal = GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurWallNormal();
		_float4 vDir = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
		vCellNormal.y = 0.f;
		vCellNormal.Normalize();
		_float fCosTheta = vCellNormal.Dot(vDir);

		if (fCosTheta < -0.8f)
			return m_eStateType;
	}


	return STATE_END;
}
