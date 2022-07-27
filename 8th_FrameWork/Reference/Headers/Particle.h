//#pragma once
//#include "Engine_Defines.h"
//
//BEGIN(Engine)
//
//class CParticleSystem;
//
//class CParticle abstract
//{
//protected:
//	friend class CParticleSystem;
//	CParticle();
//	~CParticle();
//
//public:
//	_float4x4	Get_WorldMatrix() { return m_matWorld; }
//	_float4 Get_Color() { return m_vColor; }
//	_float4 Get_Pos() { return m_vPos; }
//	
//
//public:
//	virtual void	SetUp(_float4 vPos, _float4 vDir, _float4 vScale, _float4 vColor, _float fSpeed, _float fChangeSpeed, _float fFadeOutSpeed);
//	virtual void	Recycle(const _float4& vPos, const _float4& vDir) {
//		m_vPos = vPos;
//		m_vDir = vDir;
//		m_vDir.Normalize();
//		m_vColor.w = 1.f;
//		m_fSpeed = m_fOriginSpeed;
//	}
//
//public:
//	virtual _bool Tick(const _float& fTimeDelta) PURE;
//
//protected:
//	_float4x4	m_matWorld;
//	CParticleSystem*	m_pSystem = nullptr;
//
//	_float4		m_vPos;
//	_float4		m_vDir;
//	_float4		m_vScale;
//	_float4		m_vColor;
//	_float		m_fSpeed;
//	_float		m_fOriginSpeed;
//	_float		m_fChangeSpeed;
//	_float		m_fFadeOutSpeed;
//
//	_float		m_fAcc;
//
//protected:
//	virtual void	Make_WorldMatrix();
//
//
//};
//
//END