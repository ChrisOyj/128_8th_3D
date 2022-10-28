#include "stdafx.h"
#include "CDodge_Enemy.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"
#include "CScript_FollowCam.h"

#include "CEffects_Factory.h"
#include "CDodgeWood.h"
#include "Functor.h"
CDodge_Enemy::CDodge_Enemy()
{
}

CDodge_Enemy::~CDodge_Enemy()
{
}

CDodge_Enemy* CDodge_Enemy::Create()
{
	CDodge_Enemy* pInstance = new CDodge_Enemy();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CDodge_Enemy");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CDodge_Enemy::Initialize()
{
	m_iAnimIndex = 30;
	m_eStateType = STATE_DODGE_ENEMY;

	m_vecAdjState.push_back(STATE_FALL_ENEMY);
	m_vecAdjState.push_back(STATE_LAND_ENEMY);
	m_vecAdjState.push_back(STATE_HANDAERIALATTACK_4_ENEMY);


	m_fInterPolationTime = 0.15f;
	m_fStateChangableTime = 0.4f;

	return S_OK;
}

void CDodge_Enemy::Enter(CUnit* pOwner, CAnimator* pAnimator)
{

	CFunctor::Play_Sound(L"Eff_Dodge", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

	_float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
	vPos.y += 2.5f;
	CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"DODGETEXT"), vPos);
	_float fDashSpeed = pOwner->Get_Status().fShortDashSpeed;

	CTransform* pTransform = pOwner->Get_Transform();
	CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
	_float4 vCurPos = pTransform->Get_World(WORLD_POS);
	vCurPos.y += 1.f;
	CGameObject* pDodgeWood = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CDodgeWood), vCurPos);
	vCurPos.y -= 1.0f;
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Smoke", pOwner, vCurPos);


	pPhyscisCom->Set_MaxSpeed(fDashSpeed);
	pPhyscisCom->Set_Speed(fDashSpeed);
	pPhyscisCom->Set_Jump(4.f);

	_float fDistance = 3.5f;

	_float4 vCurLook = pTransform->Get_World(WORLD_LOOK);
	vCurLook.x += 0.3f;
	vCurLook.Normalize();
	vCurPos += vCurLook * fDistance;
	pTransform->Set_World(WORLD_POS, vCurPos);
	pTransform->Make_WorldMatrix();

	pPhyscisCom->Set_Dir(vCurLook);


	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CDodge_Enemy::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Tick(pOwner, pAnimator);
}

void CDodge_Enemy::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CDodge_Enemy::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{



	return STATE_END;
}
