#include "stdafx.h"
#include "CScript_Freecam.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Transform.h"

#include "CUtility_Transform.h"

CScript_Freecam::CScript_Freecam(_uint iGroupIdx)
	: CScriptable(iGroupIdx)
{
}

CScript_Freecam::~CScript_Freecam()
{
}

CScript_Freecam* CScript_Freecam::Create(_uint iGroupIdx)
{
	CScript_Freecam* pInstance = new CScript_Freecam(iGroupIdx);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Create CScript_Freecam");
		delete pInstance;
		return nullptr;
	}

	return pInstance;
}

HRESULT CScript_Freecam::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScript_Freecam::Initialize()
{
	return S_OK;
}

void CScript_Freecam::Tick()
{
	if (!m_bUnityLevel && KEY(TAB, TAP))
		m_bTurnOn = !m_bTurnOn;

	if (!m_bTurnOn)
		return;


	if (KEY(RBUTTON, HOLD))
		OnMouseMove();

	Key_Input();
}

void CScript_Freecam::Late_Tick()
{
}

void CScript_Freecam::Release()
{
}

void CScript_Freecam::Key_Input()
{
	CTransform* pTransform = m_pOwner->Get_Transform();

	_float4 vPos = m_pOwner->Get_Transform()->Get_MyWorld(WORLD_POS);
	_float4 vDir = _float4(0.f, 0.f, 0.f, 0.f);

	_float fSpeed = 5.f;

	if (KEY(W, HOLD))
	{
		vDir += pTransform->Get_MyWorld(WORLD_LOOK);
	}
	if (KEY(A, HOLD))
	{
		vDir -= pTransform->Get_MyWorld(WORLD_RIGHT);
	}
	if (KEY(S, HOLD))
	{
		vDir -= pTransform->Get_MyWorld(WORLD_LOOK);
	}
	if (KEY(D, HOLD))
	{
		vDir += pTransform->Get_MyWorld(WORLD_RIGHT);
	}
	if (KEY(SPACE, HOLD))
	{
		vDir += _float4(0.f, 1.f, 0.f, 0.f);
	}
	if (KEY(CTRL, HOLD))
	{
		vDir -= _float4(0.f, 1.f, 0.f, 0.f);
	}
	if (KEY(LSHIFT, HOLD))
	{
		fSpeed = 40.f;
	}
	if (KEY(Z, HOLD))
	{
		fSpeed = 1.f;
	}

	vPos += vDir * fSpeed * fDT;
	vPos.w = 1.f;

	pTransform->Set_World(WORLD_POS, vPos);
}

void CScript_Freecam::OnMouseMove()
{
	if (GetFocus() != g_hWnd)
		return;

	CTransform* pTransform = m_pOwner->Get_Transform();

	//마우스 이동에 따라 회전
	_float fTurnSpeedX = ((_float)MOUSE_MOVE(MMS_X)) * MOUSE_DPI_X;
	_float fTurnSpeedY = ((_float)MOUSE_MOVE(MMS_Y)) * MOUSE_DPI_Y;

	CUtility_Transform::Turn_ByAngle(pTransform, pTransform->Get_MyWorld(WORLD_RIGHT), fTurnSpeedY);
	CUtility_Transform::Turn_ByAngle(pTransform, _float4(0.f, 1.f, 0.f, 0.f), fTurnSpeedX);

	/* Mouse Wheel */
	//_long lWheelmMove = MOUSE_MOVE(MMS_WHEEL);
	//m_tProj.fFOV -= (_float)lWheelmMove * fDT * 0.5f;
}
