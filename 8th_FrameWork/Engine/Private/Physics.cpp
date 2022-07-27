#include "..\Public\Physics.h"

#include "Transform.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Prototype_Manager.h"

CPhysics::CPhysics()
{
	ZeroMemory(&m_tPhysics, sizeof(PHYSICS));
}


CPhysics::~CPhysics()
{
}

CPhysics * CPhysics::Create( )
{
	CPhysics* pPhysics = CLONE_COMPONENT(CPhysics);
	
	if (!pPhysics)
		return nullptr;

	if (FAILED(pPhysics->Initialize()))
	{
		Call_MsgBox(L"FAILED to Initialize : CPhysics");
		return nullptr;
	}


	return pPhysics;
}

void CPhysics::Set_Jump(const _float& fJumpPower)
{
	m_tPhysics.bAir = true;
	m_tPhysics.fAcc = 0.f;
	m_tPhysics.fOriginY = m_pOwner->Get_Transform()->Get_MyWorld(WORLD_POS).y;

	m_tPhysics.fJumpPower = fJumpPower;
}

HRESULT CPhysics::Initialize_Prototype()
{
	m_tPhysics.fGravity = 9.8f;
	m_tPhysics.fPlusAcc = 4.f;

	return S_OK;
}

HRESULT CPhysics::Initialize()
{
	return S_OK;
}

void CPhysics::Tick()
{
	Move();
	Turn();
	Free_Fall();
}

void CPhysics::Late_Tick()
{
}

void CPhysics::OnEnable()
{
}

void CPhysics::OnDisable()
{
}

void CPhysics::Release()
{
}

void CPhysics::Move()
{
	if (m_tPhysics.fSpeed == 0.f)
		return;

	_float4 vPos = m_pOwner->Get_Transform()->Get_MyWorld(WORLD_POS);
	vPos += m_tPhysics.vDir * m_tPhysics.fSpeed * fDT;
	m_pOwner->Get_Transform()->Set_World(WORLD_POS, vPos);
}

void CPhysics::Free_Fall()
{
	if (m_tPhysics.bAir)
	{
		if (m_pOwner->Get_Transform()->Get_World(WORLD_POS).y < 0.f)
		{
			m_pOwner->Get_Transform()->Set_Y(0.f);
			m_tPhysics.bAir = false;
			return;
		}
		m_tPhysics.fAcc += m_tPhysics.fPlusAcc * fDT;

		float fY = m_tPhysics.fOriginY + ((m_tPhysics.fJumpPower * m_tPhysics.fAcc) - (m_tPhysics.fGravity * 0.5f * m_tPhysics.fAcc * m_tPhysics.fAcc));
		m_pOwner->Get_Transform()->Set_Y(fY);
	}
	else
	{
	}
}

void CPhysics::Turn()
{
	if (m_tPhysics.vTurnDir.Is_Zero())
		return;

	CTransform*	pTransform = m_pOwner->Get_Transform();

	_float4		vRight = pTransform->Get_MyWorld(WORLD_RIGHT).Normalize();
	_float4		vUp = pTransform->Get_MyWorld(WORLD_UP).Normalize();
	_float4		vLook = pTransform->Get_MyWorld(WORLD_LOOK).Normalize();
	
	_float4x4	RotationMatrix;
	RotationMatrix = XMMatrixRotationAxis(m_tPhysics.vTurnDir.XMVector(), m_tPhysics.fTurnSpeed * fDT);

	vRight *= RotationMatrix;
	vUp *= RotationMatrix;
	vLook *= RotationMatrix;

	pTransform->Set_World(WORLD_RIGHT, vRight);
	pTransform->Set_World(WORLD_UP, vUp);
	pTransform->Set_World(WORLD_LOOK, vLook);
}