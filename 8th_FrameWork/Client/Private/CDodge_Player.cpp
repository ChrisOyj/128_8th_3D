#include "stdafx.h"
#include "CDodge_Player.h"

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

CDodge_Player::CDodge_Player()
{
}

CDodge_Player::~CDodge_Player()
{
}

CDodge_Player* CDodge_Player::Create()
{
    CDodge_Player* pInstance = new CDodge_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CDodge_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CDodge_Player::Initialize()
{
    m_iAnimIndex = 30;
    m_eStateType = STATE_DODGE_PLAYER;

    m_vecAdjState.push_back(STATE_FALL_PLAYER);
    m_vecAdjState.push_back(STATE_LAND_PLAYER);
    m_vecAdjState.push_back(STATE_DOUBLEJUMP_PLAYER);
    m_vecAdjState.push_back(STATE_AIRDASHSTART_PLAYER);


    m_fInterPolationTime = 0.15f;

    return S_OK;
}

void CDodge_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_Dodge", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_Dodge", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);

    _float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
    vPos.y += 2.f;
    CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"DODGETEXT"), vPos);

    CUser::Get_Instance()->On_UseSkill(0);

    _float fDashSpeed = pOwner->Get_Status().fShortDashSpeed;

    CTransform* pTransform = pOwner->Get_Transform();
    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    _float4 vCurPos = pTransform->Get_World(WORLD_POS);
    vCurPos.y += 1.f;
    CGameObject* pDodgeWood = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CDodgeWood), vCurPos);
    vCurPos.y -= 1.0f;
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Smoke", pOwner, vCurPos);

    // CDodgeWood* pWood = CDodgeWood::Create();
    //CREATE_GAMEOBJECT(pWood, GROUP_EFFECT);
    //pWood->Get_Transform()->Set_World(WORLD_POS, vCurPos);

    _float4 vUserLook = CGameInstance::Get_Instance()->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    vUserLook.Normalize();

    _float4 _vLook = vUserLook;
    _float4 vUp = pTransform->Get_World(WORLD_UP);
    _float4 vRight = vUp.Cross(_vLook);
    _vLook = vRight.Cross(vUp.Normalize());
    _vLook.Normalize();

    _float4 vDir = { 0.f, 0.f, 0.f, 0.f };
    if (KEY(W, HOLD))
    {

        vDir += _vLook;
    }
    if (KEY(A, HOLD))
    {
        vDir -= vRight;


    }
    if (KEY(D, HOLD))
    {
        vDir += vRight;

    }

    if (KEY(S, HOLD))
    {
        vDir -= _vLook;
    }

    pPhyscisCom->Set_MaxSpeed(fDashSpeed);
    pPhyscisCom->Set_Speed(fDashSpeed);
    pPhyscisCom->Set_Jump(4.f);

    _float fDistance = 3.5f;
            if (vDir.Is_Zero())
            {
                fDistance = 4.5f;
        _float4 vCurLook = pTransform->Get_World(WORLD_LOOK);
        pPhyscisCom->Set_Dir(vCurLook);
        vCurLook.y += 2.f;
        vCurPos += vCurLook.Normalize() * fDistance;
        pTransform->Set_World(WORLD_POS, vCurPos);
        pTransform->Make_WorldMatrix();
        pPhyscisCom->Set_Jump(6.f);


    }
    else
    {
        _float4 vTargetLook = vDir.Normalize();

        pTransform->Set_Look(vTargetLook);

        vCurPos += vTargetLook * fDistance;
        pTransform->Set_World(WORLD_POS, vCurPos);
        pTransform->Make_WorldMatrix();
        pPhyscisCom->Set_Dir(vTargetLook);
    }

   

    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CDodge_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CDodge_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CDodge_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    


    return STATE_END;
}
