#include "stdafx.h"
#include "CCamera_Follow.h"

#include "GameInstance.h"

#include "CScript_FollowCam.h"

#include "CNavigation.h"
#include "CScript_CamNavi.h"
#include "Physics.h"
#include "Transform.h"
#include "CUnit.h"
#include "CComponent_Factory.h"

#include "CCell.h"

CCamera_Follow::CCamera_Follow()
{
}

CCamera_Follow::~CCamera_Follow()
{
}

CCamera_Follow* CCamera_Follow::Create(CGameObject* pTarget, CCell* pStartCell)
{
	CCamera_Follow* pInstance = new CCamera_Follow;

	if (FAILED(pInstance->SetUp_Components(pTarget, pStartCell)))
	{
		Call_MsgBox(L"Failed to SetUp_Components : CCamera_Follow");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CCamera_Follow");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CCamera_Follow::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CCamera_Follow::Initialize()
{
	return GET_COMPONENT(CScript_FollowCam)->Initialize();
}

HRESULT CCamera_Follow::Start()
{
	m_pNaviCom = GET_COMPONENT(CNavigation);
	m_pFollowScript = GET_COMPONENT(CScript_FollowCam);

	if (!m_pNaviCom)
		return E_FAIL;

	if (!m_pFollowScript)
		return E_FAIL;

	return __super::Start();
}

HRESULT CCamera_Follow::SetUp_Components(CGameObject* pTarget, CCell* pStartCell)
{
	Add_Component(CScript_FollowCam::Create(CP_BEFORE_NAVIGATION, pTarget));
	CPhysics* pPhyscisCom = CComponent_Factory::Clone_Component<CPhysics>(this);
	Add_Component(pPhyscisCom);

	//CUnit* pPlayer = dynamic_cast<CUnit*>(pTarget);

	//if (!pPlayer)
		//return E_FAIL;

	//CNavigation* pPlayerNavi = GET_COMPONENT_FROM(pPlayer, CNavigation);


	//CNavigation* pNaviCom = CScript_CamNavi::Create(CP_NAVIGATION, pStartCell, pPhyscisCom, pTarget);
	CNavigation* pNaviCom = CNavigation::Create(CP_NAVIGATION, pStartCell, pPhyscisCom);

	if (!pNaviCom)
		return E_FAIL;

	Add_Component(pNaviCom);

	_float4 vStartPos;
	XMStoreFloat4(&vStartPos, pStartCell->Get_Point(CCell::POINT_A));
	vStartPos.w = 1.f;
	vStartPos.x += 0.1f;
	vStartPos.z += 0.1f;

	m_pTransform->Set_World(WORLD_POS, vStartPos);
	m_pTransform->Make_WorldMatrix();
	return S_OK;
}

void CCamera_Follow::Late_Tick()
{
	/*_bool	bOnWall = m_pNaviCom->Is_OnWall();

	if (bOnWall)
		m_pFollowScript->m_fCurrentDistance -= 10.f * fDT;*/



}
