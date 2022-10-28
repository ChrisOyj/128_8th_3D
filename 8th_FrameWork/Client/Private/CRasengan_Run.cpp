#include "stdafx.h"
#include "CRasengan_Run.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "CUnit_Player.h"
#include "CState_Manager.h"
#include "CHit_Ground.h"
#include "CBlasted_Ground.h"
#include "CEffects_Factory.h"
#include "CNavigation.h"

#include "CEffect.h"
#include "Model.h"

#include "Functor.h"
CRasengan_Run::CRasengan_Run()
{
}

CRasengan_Run::~CRasengan_Run()
{
}

CRasengan_Run* CRasengan_Run::Create()
{
	CRasengan_Run* pInstance = new CRasengan_Run();

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CRasengan_Run");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CRasengan_Run::OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos)
{
    if (eColType == COL_BODY)
    {
        m_bExecuted = true;
    }
}

HRESULT CRasengan_Run::Initialize()
{
	m_eAnimType = ANIM_SKILL_ATTACK;
	m_iAnimIndex = 58;
	m_eStateType = STATE_RASENGAN_RUN;

	
	m_vecAdjState.push_back(STATE_DASH_PLAYER);
	m_vecAdjState.push_back(STATE_JUMP_PLAYER);

	m_fInterPolationTime = 0.05f;
	m_fDelayTime = 2.f;


	return S_OK;
}

void CRasengan_Run::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Dash", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Eff_RasenganRun", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    pOwner->CallBack_CollisionEnter += bind(&CRasengan_Run::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);


    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RasenganRun", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dash_Effects_Ground", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());

    static_cast<CEffect*>(m_EffectsList.back())->Set_RefBone(GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("RightHand"));
    static_cast<CEffect*>(m_EffectsList.back())->Set_OffsetPos(_float4(10.f, 10.f, 0.f, 1.f));
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fShortDashSpeed);
    pOwner->Get_PhysicsCom()->Set_SpeedasMax();

    CAMERA_LERP(CAMERA_LERP_DEFAULT);

	__super::Enter(pOwner, pAnimator);

}

STATE_TYPE CRasengan_Run::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bExecuted)
        return STATE_RASENGAN_ATTACK;

    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    if (pPhyscisCom->Get_Physics().bAir)
    {
        pPhyscisCom->Set_Jump();
        return STATE_FALL_PLAYER;
    }

    _float4 vUserLook = CGameInstance::Get_Instance()->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    vUserLook.Normalize();

    _float4 _vLook = vUserLook;
    _float4 vUp = pTransform->Get_World(WORLD_UP);
    _float4 vRight = vUp.Cross(_vLook);
    _vLook = vRight.Cross(vUp.Normalize());
    _vLook.Normalize();

    _float4 vDir = { 0.f, 0.f, 0.f, 0.f };
    if (KEY(A, HOLD))
    {
        vDir -= vRight;
    }
    if (KEY(D, HOLD))
    {
        vDir += vRight;
    }
    if (KEY(W, HOLD))
    {
        vDir += _vLook;
    }
    if (KEY(S, HOLD))
    {
        vDir -= _vLook;
    }
    pPhyscisCom->Set_Accel(30.f);

    if (vDir.Is_Zero())
    {
        return __super::Tick(pOwner, pAnimator);
    }

    _float4 vTargetLook = vDir.Normalize();

    _float4 vCurPos = pTransform->Get_World(WORLD_POS);
    _float4 vCurLook = pTransform->Get_World(WORLD_LOOK);

    _float fRatio = (vTargetLook - vCurLook).Length() * 4.f;

    vCurLook += (vTargetLook - vCurLook).Normalize() * fRatio * fDT;


    vRight = vUp.Cross(vCurLook);

    pTransform->Set_World(WORLD_LOOK, vCurLook);
    pTransform->Set_World(WORLD_UP, vUp);
    pTransform->Set_World(WORLD_RIGHT, vRight.Normalize());

    pPhyscisCom->Set_Dir(vCurLook);

    

    return __super::Tick(pOwner, pAnimator);

}

STATE_TYPE CRasengan_Run::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Dash으로 오는 조건
	1. 키눌렸을때
	스킬
	스킬 쿨타임도 확인하고
	스킬 챠크라 게이지도 확인하고

	*/
	if (KEY(T, NONE))
	{
		return m_eStateType;
	}


	return STATE_END;
}

void CRasengan_Run::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    m_bExecuted = true;
}

void CRasengan_Run::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->CallBack_CollisionEnter -= bind(&CRasengan_Run::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);

    for (auto& elem : m_EffectsList)
        if (!elem->Is_Disable())
            static_cast<CEffect*>(elem)->Set_FadeOut();


}
