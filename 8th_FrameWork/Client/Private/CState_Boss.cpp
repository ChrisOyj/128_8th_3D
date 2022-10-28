#include "stdafx.h"
#include "CState_Boss.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CState_Manager.h"
#include "CDoubleJump_Player.h"

#include "CNavigation.h"

#include "CUser.h"
#include "CScript_FollowCam.h"
#include "CUnit_Player.h"

#include "CWindow_Boss.h"
#include "ImGui_Manager.h"

#include "CEffects_Factory.h"
#include "CCamera_Cinema.h"

#include "CBossAura.h"

#include "Functor.h"

CState_Boss::CState_Boss()
{
}

CState_Boss::~CState_Boss()
{
}

CState_Boss* CState_Boss::Create()
{
    CState_Boss* pInstance = new CState_Boss();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Boss");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CState_Boss::Initialize()
{
    m_fInterPolationTime = 0.2f;
    m_arrBossPhase[CRYING].iAnimType = ANIM_BOSS;
    m_arrBossPhase[CRYING].iAnimIndex = 1;
    m_arrBossPhase[CRYING].fAnimSpeed = 1.f;
    m_arrBossPhase[CRYING].fDuration = 6.3f;
    m_arrBossPhase[CRYING].fInterPolationTime = 0.f;

    m_arrBossPhase[CHARGESTART].iAnimType = ANIM_BASE;
    m_arrBossPhase[CHARGESTART].iAnimIndex = 18;
    m_arrBossPhase[CHARGESTART].fAnimSpeed = 0.1f;
    m_arrBossPhase[CHARGESTART].fDuration = 0.f;  // 0이면 anim finish 까지
    m_arrBossPhase[CHARGESTART].fInterPolationTime = 0.4f;

    m_arrBossPhase[CHARGELOOP].iAnimType = ANIM_BASE;
    m_arrBossPhase[CHARGELOOP].iAnimIndex = 17;
    m_arrBossPhase[CHARGELOOP].fAnimSpeed = 1.f;
    m_arrBossPhase[CHARGELOOP].fDuration = 2.f;
    m_arrBossPhase[CHARGELOOP].fInterPolationTime = 0.05f;

    m_arrBossPhase[CHARGEEND].iAnimType = ANIM_BASE;
    m_arrBossPhase[CHARGEEND].iAnimIndex = 16;
    m_arrBossPhase[CHARGEEND].fAnimSpeed = 1.f;
    m_arrBossPhase[CHARGEEND].fDuration = 0.f;
    m_arrBossPhase[CHARGEEND].fInterPolationTime = 0.f;

    m_arrBossPhase[FALL].iAnimType = ANIM_BASE;
    m_arrBossPhase[FALL].iAnimIndex = 30;
    m_arrBossPhase[FALL].fAnimSpeed = 1.f;
    m_arrBossPhase[FALL].fDuration = 6.f;
    m_arrBossPhase[FALL].fInterPolationTime = 0.f;

    m_arrBossPhase[LAND].iAnimType = ANIM_BASE;
    m_arrBossPhase[LAND].iAnimIndex = 37;
    m_arrBossPhase[LAND].fAnimSpeed = 0.5f;
    m_arrBossPhase[LAND].fDuration = 0.f;
    m_arrBossPhase[LAND].fInterPolationTime = 0.05f;

    m_arrBossPhase[ANGRY].iAnimType = ANIM_BOSS;
    m_arrBossPhase[ANGRY].iAnimIndex = 4;
    m_arrBossPhase[ANGRY].fAnimSpeed = 1.f;
    m_arrBossPhase[ANGRY].fDuration = 10.f;
    m_arrBossPhase[ANGRY].fInterPolationTime = 0.3f;


    return S_OK;
}

void CState_Boss::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!m_bFirst)
    {
        m_bFirst = true;
        m_vOriginPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
        _float4 vPlayerPos = PLAYER->Get_Transform()->Get_World(WORLD_POS);
        _float4 vDir = vPlayerPos - m_vOriginPos;
        vDir.y = 0.f;
        pOwner->Get_Transform()->Set_Look(vDir);
#ifdef _DEBUG
        CImGui_Manager::Get_Instance()->SetUp_BossWindow(this);
#endif // _DEBUG

    }

    m_iAnimIndex = m_arrBossPhase[m_eCurPhase].iAnimIndex;
    m_eAnimType = (ANIM_TYPE)m_arrBossPhase[m_eCurPhase].iAnimType;
    m_fInterPolationTime = m_arrBossPhase[m_eCurPhase].fInterPolationTime;
    m_fDelayTime = m_arrBossPhase[m_eCurPhase].fDuration;

    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator);



    switch (m_eCurPhase)
    {
    case Client::CState_Boss::CRYING:
        CFunctor::Play_Sound(L"Env_StageIntro", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

        break;
    case Client::CState_Boss::CHARGESTART:

        break;
    case Client::CState_Boss::CHARGELOOP:
        CFunctor::Play_Sound(L"Eff_ChakraJumpStart", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
        CFunctor::Play_Sound(L"Eff_CJBossCharge", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));


        m_pChargeEffect = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"CJ_LoopEffect_0"), pOwner->Get_Transform()->Get_World(WORLD_POS));
        {
            LIGHTDESC			LightDesc;

            LightDesc.eType = tagLightDesc::TYPE_POINT;

            LightDesc.fRange = 4.f;

            LightDesc.vDiffuse = _float4(0.1f, 0.2f, 0.6f, 1.f);
            LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
            LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
            LightDesc.pOwner = m_pChargeEffect;
            LightDesc.vOffset.y = 0.5f;

            GAMEINSTANCE->Add_Light(LightDesc);
        }
        
        break;
    case Client::CState_Boss::CHARGEEND:
        CFunctor::Play_Sound(L"Eff_ChakraJumpEnd", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
        CFunctor::Play_Sound(L"Voice_Boss_StrongAttack", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));

        static_cast<CCamera_Cinema*>(GAMEINSTANCE->Get_CurCam())->Start_ShakingCamera(0.08f, 0.3f);

        DISABLE_GAMEOBJECT(m_pChargeEffect);
        pOwner->Get_PhysicsCom()->Set_Jump(m_fJumpPower);
        pOwner->Get_PhysicsCom()->Set_Dir(pOwner->Get_Transform()->Get_World(WORLD_LOOK));
        pOwner->Get_PhysicsCom()->Set_Speed(m_fSpeed);
        //CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);
        break;
    case Client::CState_Boss::FALL:

        break;
    case Client::CState_Boss::LAND:
    {
        GAMEINSTANCE->Play_BGM(L"BGM_Boss");
        CFunctor::Play_Sound(L"Eff_WallBreak", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));


        static_cast<CCamera_Cinema*>(GAMEINSTANCE->Get_CurCam())->Start_ShakingCamera(0.06f, 0.4f);

        CGameObject* pAura = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CBossAura), pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
        CUser::Get_Instance()->Set_BossAura(static_cast<CBossAura*>(pAura));

        CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"GroundBreak_B"), pOwner->Get_Transform()->Get_World(WORLD_POS));
        CEffects_Factory::Get_Instance()->Create_LandingEffects(pOwner->Get_Transform()->Get_World(WORLD_POS), GET_COMPONENT_FROM(pOwner, CNavigation)->Get_CurCell());
        CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"GroundSmoke", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BossAuraLoop", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
    }
        

        break;
    case Client::CState_Boss::ANGRY:

        break;
    case Client::CState_Boss::PHASE_END:
        break;
    default:
        break;
    }

}

STATE_TYPE CState_Boss::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    STATE_TYPE eType = STATE_END;

    /*if (KEY(NUM1, TAP))
    {
        m_eCurPhase = CRYING;
        Enter(pOwner, pAnimator);
        pOwner->Get_Transform()->Set_World(WORLD_POS, m_vOriginPos);
        pOwner->Get_Transform()->Make_WorldMatrix();
        return eType;

    }*/

    if (m_eCurPhase == ANGRY)
        return eType;

    m_fTimeAcc += fDT;

    //d
    if (m_fDelayTime <= 0.f)
    {
        if (pAnimator->Is_CurAnimFinished())
        {
            //다음으로
            _int iCurPhase = m_eCurPhase;
            iCurPhase++;
            m_eCurPhase = (PHASE)iCurPhase;
            Enter(pOwner, pAnimator);
            return eType;
        }
    }
    else
    {
        if (m_eCurPhase == CRYING)
        {
            _float fBGMTime = 1.f;


            if (m_fTimeAcc >= fBGMTime)
            {
                GAMEINSTANCE->Stop_Sound((CHANNEL_GROUP)CHANNEL_BGM);
                GAMEINSTANCE->Set_ChannelVolume((CHANNEL_GROUP)CHANNEL_BGM, 0.3f);
            }
            else
            {

                _float fVolume = 0.3f * ((fBGMTime - m_fTimeAcc) / fBGMTime);

                GAMEINSTANCE->Set_ChannelVolume((CHANNEL_GROUP)CHANNEL_BGM, fVolume);
            }


        }


        if (m_eCurPhase == FALL)
        {
            if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
            {
                //다음으로
                _int iCurPhase = m_eCurPhase;
                iCurPhase++;
                m_eCurPhase = (PHASE)iCurPhase;
                Enter(pOwner, pAnimator);
                return eType;
            }
        }

        if (m_eCurPhase == CHARGELOOP)
        {
            


            if (m_fTimeAcc >= 1.f)
            {
                if (!m_bOnce)
                {
                    m_bOnce = true;
                    CFunctor::Play_Sound(L"Eff_ChakraJumpMaxCharge", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
                    CFunctor::Play_Sound(L"Eff_ChakraJumpLoop", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

                    CEffects_Factory::Get_Instance()->Create_MultiEffects((L"CJ_MaxCharge"), pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
                }

            }
        }

        if (m_fTimeAcc >= m_fDelayTime)
        {
            //다음으로
            _int iCurPhase = m_eCurPhase;
            iCurPhase++;
            m_eCurPhase = (PHASE)iCurPhase;
            Enter(pOwner, pAnimator);
            return eType;
        }
    }

    return eType;


}

void CState_Boss::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CState_Boss::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!pOwner->Get_PhysicsCom()->Get_Physics().bAir)
        return m_eStateType;


    return STATE_END;
}
