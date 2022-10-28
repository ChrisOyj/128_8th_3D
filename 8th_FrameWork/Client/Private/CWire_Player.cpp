#include "stdafx.h"
#include "CWire_Player.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "Transform.h"
#include "CCamera_Free.h"
#include "Physics.h"

#include "CUser.h"

#include "CScript_FollowCam.h"

#include "CWire.h"
#include "CWireKunai.h"

#include "CTerrain.h"
#include "CMesh.h"
#include "Model.h"
#include "HIerarchyNode.h"

#include "CRaijinKunai.h"
#include "CEffects_Factory.h"
#include "Functor.h"

CWire_Player::CWire_Player()
{
}

CWire_Player::~CWire_Player()
{
}

CWire_Player* CWire_Player::Create()
{
    CWire_Player* pInstance = new CWire_Player();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWire_Player");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CWire_Player::Initialize()
{
    m_iAnimIndex = 79;
    m_eStateType = STATE_WIRE_PLAYER;

    m_vecAdjState.push_back(STATE_WIREDASH_PLAYER);

    m_fInterPolationTime = 0.2f;
    m_fStateChangableTime = 0.8f;
    m_fDelayTime = 0.43f;

    return S_OK;
}

void CWire_Player::Enter(CUnit* pOwner, CAnimator* pAnimator)
{
    CUser::Get_Instance()->Turn_WireMode();

    m_bCoolTime = false;

    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
    pPhyscisCom->Mul_Speed(0.1f);
    pPhyscisCom->Set_Jump(0.f);
    pPhyscisCom->Get_Physics().fGravity = 0.f;

    m_vDir = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);

    _float4 vLook = m_vDir.Normalize();
    vLook.y = 0.f;
    pOwner->Get_Transform()->Set_LerpLook(vLook, m_fDelayTime);
    pOwner->Get_Transform()->Set_Y(pOwner->Get_Transform()->Get_World(WORLD_POS).y + 0.3f);
    pOwner->Get_Transform()->Make_WorldMatrix();
    CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_WIRE);

    /* Picking */
    m_vTargetPos = _float4(0.f, 0.f, 0.f, 0.f);
    m_bWireSuccess = true;
    m_vTargetPos = Picking_Terrain();

    if (m_vTargetPos.Is_Zero())
    {
        //피킹 실패
        m_bWireSuccess = false;
        m_vTargetPos = pOwner->Get_Transform()->Get_World(WORLD_POS) + m_vDir * m_fMaxWireDistance;
    }

    __super::Enter(pOwner, pAnimator);

}

STATE_TYPE CWire_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (!m_bWireSuccess)
    {
        if (pAnimator->Is_CurAnimFinished())
        {
            CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
            CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);
            pPhyscisCom->Get_Physics().fGravity = 9.8f;
            pPhyscisCom->Set_Jump(0.f);

            return STATE_FALL_PLAYER;
        }
    }
    else if (m_fTimeAcc >= m_fStateChangableTime)
    {
        if (pOwner->Is_NinzaSword())
        {
            CFunctor::Play_Sound(L"Eff_Raisin", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

            CEffects_Factory::Get_Instance()->Create_ScreenEffects(pOwner);
            CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RaijinEffect", pOwner, m_vTargetPos);

            pOwner->Get_Transform()->Set_World(WORLD_POS, m_vTargetPos);
            pOwner->Get_Transform()->Make_WorldMatrix();

            CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();
            CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::CAMERA_LERP_DEFAULT);
            pPhyscisCom->Get_Physics().fGravity = 9.8f;
            pPhyscisCom->Set_Jump(0.f);

            CEffects_Factory::Get_Instance()->Create_MultiEffects(L"RaijinEffect", pOwner, m_vTargetPos);

            return STATE_FALL_PLAYER;
        }
    }


    return __super::Tick(pOwner, pAnimator);
}

void CWire_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
   

    pOwner->Get_PhysicsCom()->Get_Physics().fGravity = 9.8f;


    _float4 vLook = m_vDir;
    vLook.y = 0.f;
    pOwner->Get_Transform()->Set_Look(vLook);

    CPhysics* pPhyscisCom = pOwner->Get_PhysicsCom();

    pPhyscisCom->Set_Dir(m_vDir);
}

STATE_TYPE CWire_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{


    return STATE_END;
}

void CWire_Player::OnExecute(CUnit* pOwner, CAnimator* pAnimator)
{
    CFunctor::Play_Sound(L"Eff_WireThrow", CHANNEL_VOICE, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Voice_Man_CJ", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));

        CHierarchyNode* pRightHand = GET_COMPONENT_FROM(pOwner, CModel)->Find_HierarchyNode("RightHand");

        _float4 vHandPos;
        _matrix matRightHand = pRightHand->Get_CombinedMatrix();

        matRightHand = XMMatrixMultiply(matRightHand, DEFAULT_TRANS_MATRIX);

        _float4x4 matBone;
        XMStoreFloat4x4(&matBone, matRightHand);

        _float4x4 matWorld = pOwner->Get_Transform()->Get_WorldMatrix();

        //콤바인 위치 * 월드매트릭스
        vHandPos = (*((_float4*)&matBone.m[WORLD_POS]));
        vHandPos = vHandPos.MultiplyCoord(matWorld);

        CGameObject* pKunai = nullptr;

        if (!pOwner->Is_NinzaSword())
        {
           pKunai = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CWireKunai), vHandPos, m_vTargetPos);

        }
        else
           pKunai = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CRaijinKunai), vHandPos, m_vTargetPos);


        if (!m_bWireSuccess)
            return;

        
        if (!pOwner->Is_NinzaSword())
        {
            CUser::Get_Instance()->Reset_Wire(pKunai);
        }
        else
        {
            //와이어있다는뜻
            static_cast<CRaijinKunai*>(pKunai)->On_Success();
        }

}

void CWire_Player::TickExecute(CUnit* pOwner, CAnimator* pAnimator)
{
}

_float4 CWire_Player::Picking_Terrain()
{
    POINT pt{};

    ::GetCursorPos(&pt);
    ::ScreenToClient(g_hWnd, &pt);

    _float2 fPT = { (_float)pt.x, (_float)pt.y };

    _float4 vProjPos =
    {
        fPT.x / (g_iWinCX * 0.5f) - 1.f,
        fPT.y / -(g_iWinCY * 0.5f) + 1.f,
        0.f, 1.f
    };

    /* To View */
    _float4x4	ProjMatrixInv = CGameInstance::Get_Instance()->Get_CurProjMatrix();
    ProjMatrixInv.Inverse();

    _float4 vViewPos = vProjPos.MultiplyCoord(ProjMatrixInv);

    _float4 v4RayDir = vViewPos.Normalize();
    _float4 v4RayPos = { 0.f, 0.f, 0.f, 1.f };

    /* To World */
    _float4x4	ViewMatrixInv = CGameInstance::Get_Instance()->Get_CurViewMatrix();
    ViewMatrixInv.Inverse();

    v4RayDir = v4RayDir.MultiplyNormal(ViewMatrixInv);
    v4RayPos = v4RayPos.MultiplyCoord(ViewMatrixInv);

    v4RayDir.Normalize();
    _float4 vWorldRayDir = _float4(v4RayDir.x, v4RayDir.y, v4RayDir.z, 1.f);
    _float4 vWorldRayPos = _float4(v4RayPos.x, v4RayPos.y, v4RayPos.z, 0.f);



    // ======================= //
    _float4 vFinalPos;


    CMesh* pMesh = GET_COMPONENT_FROM(m_pTerrain, CMesh);
    CTransform* pTransform = m_pTerrain->Get_Transform();
    _float4x4	matWorld = pTransform->Get_WorldMatrix();
    _float4x4   matWorldInv = matWorld;
    matWorldInv.Inverse();

    _vector			vRayPos, vRayDir;

    vRayPos = vWorldRayPos.MultiplyCoord(matWorldInv).XMLoad();
    vRayDir = vWorldRayDir.MultiplyNormal(matWorldInv).XMLoad();
        

    _uint			iNumFaces = pMesh->Get_NumPrimitive();
    const _float3* pVerticesPos = pMesh->Get_VerticesPos();
    _uint			iIndexSize = pMesh->Get_IndexSize();

    _float		fDist, fMin = 9999.f;

    for (_uint i = 0; i < iNumFaces; ++i)
    {
        _uint3		iIndices = pMesh->Get_Indices(i);

        _vector		vVec0 = XMLoadFloat3(&pVerticesPos[iIndices._1]);
        GXMVECTOR	vVec1 = XMLoadFloat3(&pVerticesPos[iIndices._2]);
        HXMVECTOR	vVec2 = XMLoadFloat3(&pVerticesPos[iIndices._3]);

        

        if (true == TriangleTests::Intersects(vRayPos, vRayDir, vVec0, vVec1, vVec2, fDist))
        {
            _float4 V1, V2;
            _float4 vPickedPos;
            _float4x4 worldMat = matWorld;

            V1 = (vVec0 - vVec1);
            V2 = (vVec2 - vVec1);

            vPickedPos = vRayPos + XMVector3Normalize(vRayDir) * fDist;
            _float fDistance = (vWorldRayPos - vPickedPos).Length();

            if (fDistance >= m_fMaxWireDistance)
            {
                continue;
            }

            if (fMin > fDistance)
            {
                vFinalPos = vPickedPos;
                fMin = fDistance;
            }
        }
    }

    if (fMin != 9999.f)
    {
        return vFinalPos;
    }

    return _float4(0.f, 0.f, 0.f, 0.f);
}
