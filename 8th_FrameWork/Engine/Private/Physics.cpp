#include "..\Public\Physics.h"

#include "Transform.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Prototype_Manager.h"

CPhysics::CPhysics(_uint iGroupID)
	: CComponent(iGroupID)
{
	ZeroMemory(&m_tPhysics, sizeof(PHYSICS));
}


CPhysics::~CPhysics()
{
}

CPhysics * CPhysics::Create(_uint iGroupID)
{
	CPhysics* pPhysics = new CPhysics(iGroupID);

	if (FAILED(pPhysics->Initialize_Prototype()))
	{
		SAFE_DELETE(pPhysics);
		Call_MsgBox(L"Failed to Initialize_Prototype : CPhysics");
		return nullptr;
	}

	return pPhysics;
}

void CPhysics::Set_Jump(const _float& fJumpPower)
{
	m_tPhysics.bAir = true;
	m_tPhysics.fAcc = 0.f;
	m_tPhysics.fOriginY = m_pOwner->Get_Transform()->Get_MyWorld(WORLD_POS).y;
	m_tPhysicsDetail.fPrevY = m_tPhysics.fOriginY;

	m_tPhysics.fJumpPower = fJumpPower;
}

HRESULT CPhysics::Initialize_Prototype()
{
	m_tPhysicsDetail.fFriction = 30.f;
	m_tPhysicsDetail.fAirFriction = 1.f;
	m_tPhysicsDetail.fMaxSpeed = 15.f;

	m_tPhysics.fGravity = 9.8f;
	m_tPhysics.fPlusAcc = 1.5f;

	return S_OK;
}

HRESULT CPhysics::Initialize()
{
	return S_OK;
}

void CPhysics::Start()
{
	__super::Start();
	m_pOwner->Get_Transform()->Make_WorldMatrix();
}

void CPhysics::Tick()
{
	Turn();

	if (m_bNaviOn)
		return;
	

	_float4 vCurPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
	vCurPos += m_tPhysics.vDir * Calculate_Speed() * fDT;
	vCurPos.y += Calculate_FreeFall();

	if (m_tPhysics.fSpeed <= 0.f)
		return;

	m_pOwner->Get_Transform()->Set_World(WORLD_POS, vCurPos);
}

void CPhysics::Late_Tick()
{
	m_tPhysicsDetail.fAccel = 0.f;
}

void CPhysics::OnEnable()
{
	__super::OnEnable();
}

void CPhysics::OnDisable()
{
	__super::OnDisable();
}

_float CPhysics::Calculate_Speed()
{
	_float fTimeDelta = fDT;

	if (m_tPhysicsDetail.fAccel <= 0.f)
	{
		if (m_tPhysics.fSpeed > 0.f)
			m_tPhysics.fSpeed -= ((m_tPhysics.bAir) ? m_tPhysicsDetail.fAirFriction : m_tPhysicsDetail.fFriction) * m_tPhysicsDetail.fFrictionRatio * fTimeDelta;
		else
		{
			m_tPhysics.fSpeed = 0.f;
		}
	}
	else
	{
		m_tPhysics.fSpeed += m_tPhysicsDetail.fAccel * fTimeDelta;

		if (m_tPhysics.fSpeed > m_tPhysicsDetail.fMaxSpeed)
		{
			m_tPhysics.fSpeed = m_tPhysicsDetail.fMaxSpeed;
		}
	}

	return m_tPhysics.fSpeed;
}

_float CPhysics::Calculate_FreeFall()
{
	_float fFreeFallY = 0.f;
	_float fFreeFallPower = 0.f;

	if (m_tPhysics.bAir)
	{
		m_tPhysics.fAcc += m_tPhysics.fPlusAcc * fDT;

		fFreeFallY = m_tPhysics.fOriginY + ((m_tPhysics.fJumpPower * m_tPhysics.fAcc) - (m_tPhysics.fGravity * 0.5f * m_tPhysics.fAcc * m_tPhysics.fAcc));

		//이동량 = 현재 Y - 이전 Y
		fFreeFallPower = fFreeFallY - m_tPhysicsDetail.fPrevY;

		m_tPhysicsDetail.fPrevY = fFreeFallY;

	}


	if (fFreeFallPower <= -0.2f)
		fFreeFallPower = -0.2f;

	return fFreeFallPower;
}

_float CPhysics::Check_Air(_float fCurY)
{
	if (m_tPhysics.bAir)
	{
		if (fCurY <= m_tPhysicsDetail.fCurGroundY)
		{
			m_tPhysics.bAir = false;
			return m_tPhysicsDetail.fCurGroundY;
		}

		return -10.f;
	}

	return m_tPhysicsDetail.fCurGroundY;
}

void CPhysics::Release()
{
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
	RotationMatrix = XMMatrixRotationAxis(m_tPhysics.vTurnDir.XMLoad(), m_tPhysics.fTurnSpeed * fDT);

	vRight = vRight.MultiplyNormal(RotationMatrix);
	vUp = vUp.MultiplyNormal(RotationMatrix);
	vLook = vLook.MultiplyNormal(RotationMatrix);

	pTransform->Set_World(WORLD_RIGHT, vRight.Normalize());
	pTransform->Set_World(WORLD_UP, vUp.Normalize());
	pTransform->Set_World(WORLD_LOOK, vLook.Normalize());
}