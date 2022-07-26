#include "..\Public\Particle.h"

#include "Camera_Manager.h"

CParticle::CParticle()
{
}


CParticle::~CParticle()
{
}

void CParticle::SetUp(_float4 vPos, _float4 vDir, _float4 vScale, _float4 vColor, _float fSpeed, _float fChangeSpeed, _float fFadeOutSpeed)
{
	m_vPos = vPos;
	m_vDir = vDir.Normalize();
	m_vScale = vScale;
	m_vColor = vColor;
	m_fOriginSpeed = fSpeed;
	m_fSpeed = fSpeed;
	m_fChangeSpeed = fChangeSpeed;
	m_fFadeOutSpeed = fFadeOutSpeed;
}

void CParticle::Make_WorldMatrix()
{
	m_matWorld = CCamera_Manager::Get_Instance()->Get_BillBoard();
	
	*(_float4*)(&m_matWorld.m[0]) *= m_vScale.x;
	*(_float4*)(&m_matWorld.m[1]) *= m_vScale.y;
	*(_float4*)(&m_matWorld.m[2]) *= m_vScale.z;
	*(_float4*)(&m_matWorld.m[3]) = m_vPos;
}


