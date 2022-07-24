#include "..\Public\Particle_Cube.h"

#include "Transform.h"

CParticle_Cube::CParticle_Cube()
{
}


CParticle_Cube::~CParticle_Cube()
{
}

CParticle_Cube * CParticle_Cube::Create(CParticleSystem * pSystem)
{
	CParticle_Cube * pParticle = new CParticle_Cube;
	pParticle->m_pSystem = pSystem;
	return pParticle;
}

void CParticle_Cube::SetUp(_float3 vPos, _float3 vDir, _float3 vScale, _float4 vColor, _float fSpeed, _float fChangeSpeed, _float fFadeOutSpeed, _float fJumpPower, _float fTurnSpeed)
{
	__super::SetUp(vPos, vDir, vScale, vColor, fSpeed, fChangeSpeed, fFadeOutSpeed);
	m_fOriginY = vPos.y;
	m_fJumpPower = fJumpPower;
	m_fTurnSpeed = fTurnSpeed;
	m_fOriginTurnSpeed = fTurnSpeed;

	(*((_float3*)&m_matWorld.m[WORLD_LOOK])) = vDir;

	_float3 _vLook = vDir;
	_float3 vUp = { 0.f, 1.f, 0.f };
	_float3 vRight = vUp.Cross(_vLook);
	(*((_float3*)&m_matWorld.m[WORLD_RIGHT])) = vRight;

	vUp = _vLook.Cross(vRight);
	(*((_float3*)&m_matWorld.m[WORLD_UP])) = vUp;
}

_bool CParticle_Cube::Tick(const _float & fTimeDelta)
{
	m_vColor.w -= m_fFadeOutSpeed * fTimeDelta;

	if (m_vColor.w < 0.1f)
		return false;

	m_fSpeed += m_fChangeSpeed * fTimeDelta;

	m_vPos += m_fSpeed * m_vDir * fTimeDelta;

	m_fTurnSpeed = (m_fSpeed / m_fOriginSpeed) * 3.f;

	//m_fTurnSpeed -= m_fTurnChangeSpeed * fTimeDelta;
	if (m_fTurnSpeed < 0.f)
		m_fTurnSpeed = 0.f;

	m_fJumpAcc += m_fJumpPlusAcc * fTimeDelta;

	float fY = m_fOriginY + ((m_fJumpPower * m_fJumpAcc) - (9.8f * 0.5f * m_fJumpAcc * m_fJumpAcc));
	m_vPos.y = fY;

	if (m_vPos.y < 0.5f)
		m_vPos.y = 0.5f;

	if (m_fSpeed < 0.f)
	{
		return false;
	}
	

	_float3		vRight = (*((_float3*)&m_matWorld.m[WORLD_RIGHT]));
	_float3		vUp = (*((_float3*)&m_matWorld.m[WORLD_UP]));
	_float3		vLook = (*((_float3*)&m_matWorld.m[WORLD_LOOK]));

	_float4x4	RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vRight, m_fTurnSpeed * fTimeDelta);

	vRight.Multiply(RotationMatrix, true);
	vUp.Multiply(RotationMatrix, true);
	vLook.Multiply(RotationMatrix, true);

	(*((_float3*)&m_matWorld.m[WORLD_RIGHT])) = vRight;
	(*((_float3*)&m_matWorld.m[WORLD_UP])) = vUp;
	(*((_float3*)&m_matWorld.m[WORLD_LOOK])) = vLook;



	Make_WorldMatrix();


	return true;
}

void CParticle_Cube::Make_WorldMatrix()
{
	(*(_float3*)(&m_matWorld.m[0])).Normalize() *= m_vScale.x;
	(*(_float3*)(&m_matWorld.m[1])).Normalize() *= m_vScale.y;
	(*(_float3*)(&m_matWorld.m[2])).Normalize() *= m_vScale.z;
	*(_float3*)(&m_matWorld.m[3]) = m_vPos;
}
