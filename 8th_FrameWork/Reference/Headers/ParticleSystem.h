//#pragma once
//#include "Renderer.h"
//
//BEGIN(Engine)
//
//class CParticle;
//class CMesh;
//class CTexture;
//class CGameObject;
//
//struct PARTICLE
//{
//	PARTICLE(CParticle* _pParticle)
//		: pParticle(_pParticle)
//	{}
//	PARTICLE(CParticle* _pParticle, _float _fDistance)
//		: pParticle(_pParticle)
//		, fDistance(_fDistance)
//	{}
//
//	CParticle*	pParticle = nullptr;
//	_float		fDistance = 0.f;
//};
//
//enum PARTICLE_TYPE {PARTICLE_DEFAULT, PARTICLE_POP, PARTICLE_CUBE, PARTICLE_END};
//
//class ENGINE_DLL CParticleSystem final
//	: public CRenderer
//{
//	DECLARE_PROTOTYPE(CParticleSystem)
//
//private:
//	CParticleSystem();
//	CParticleSystem(const CParticleSystem& _origin);
//	virtual ~CParticleSystem();
//
//public:
//	static	CParticleSystem*	Create(
//		CGameObject* pOwner, const _tchar* pTextureFilePath,
//		PARTICLE_PASS_TYPE	ePassType, PARTICLE_TYPE eParticleType, _float4	vRandomRange_ParticlesPos,
//		_float4	vRandomRange_ParticlesDir, _float4 vRandomRange_ParticlesScale, _float4	vRandomRange_ParticlesColor
//		, _float fRandomRange_ParticlesSpeed, _float fRandomRange_ParticlesFadeOutSpeed
//		);
//
//public:
//	void	Start_ParticleSystem(
//		_float4	vParticlesPos, _float4 vParticlesDir, _float4 vParticlesScale, _float4 vParticlesColor,
//		_float fParticleGenerationCycle, _float fParticleGenerationTime, _uint iParticleGenerationCnt,
//		_float fParticleInitSpeed, _float fParticleChangeSpeed, _float fParticleFadeOutSpeed, _uint iMaxParticleCnt
//	);
//
//public:
//	void	Set_JumpPower(_float fJumpPower, _float fRandomRangeJumpPower)
//	{
//		m_fParticleJumpPower = fJumpPower;
//		m_fRandomRange_ParticleJumpPower = fRandomRangeJumpPower;
//	}
//
//	void	Set_TurnSpeed(_float fTurnSpeed, _float fRandomRangeTurnSpeed)
//	{
//		m_fParticleTurnSpeed = fTurnSpeed;
//		m_fRandomRange_ParticleTurnSpeed = fRandomRangeTurnSpeed;
//	}
//
//public:
//	// CComponent을(를) 통해 상속됨
//	virtual HRESULT Initialize_Prototype() override;
//	virtual HRESULT Initialize() override;
//	virtual void Tick() override;
//	virtual HRESULT		Render();
//
//#pragma region Member Variables
//private:
//	list<PARTICLE>			m_pParticles;
//	list<CParticle*>		m_pParticles_Collection;
//	CMesh*					m_pMesh = nullptr;
//
//	CTexture*						m_pTexture = nullptr;
//	PARTICLE_PASS_TYPE				m_ePassType = P_PASS_END;
//
//
//	/* Particle Detail */
//	PARTICLE_TYPE	m_eParticle_Type	= PARTICLE_END;
//
//	_float4					m_vParticlesPos;
//	_float4					m_vParticlesDir;
//	_float4					m_vParticlesScale;
//	_float4					m_vParticlesColor;
//
//	_float4					m_vRandomRange_ParticlesPos;
//	_float4					m_vRandomRange_ParticlesDir;
//	_float4					m_vRandomRange_ParticlesScale;
//	_float4					m_vRandomRange_ParticlesColor;
//	_float					m_fRandomRange_ParticlesSpeed;
//	_float					m_fRandomRange_ParticlesFadeOutSpeed;
//
//	_float					m_fParticleGenerationCycle;
//	_float					m_fParticleGenerationTime;
//	_uint					m_iParticleGenerationCnt;
//
//	_float					m_fDT = 0.f;
//	_float					m_fGenerationAcc = 0.f;
//	_float					m_fGenerationCycleAcc = 0.f;
//
//
//	_float					m_fParticleInitSpeed;
//	_float					m_fParticleChangeSpeed;
//	_float					m_fParticleFadeOutSpeed;
//
//	_uint					m_iMaxParticleCnt;
//	_uint					m_iCurParticlesCnt;
//
//	_float					m_fParticleJumpPower;
//	_float					m_fRandomRange_ParticleJumpPower;
//
//	_float					m_fParticleTurnSpeed;
//	_float					m_fRandomRange_ParticleTurnSpeed;
//
//#pragma endregion
//
//private:
//	void Update_ParticleSystem();
//	void Create_Particles();
//	void Update_Particles();
//	void Collect_Particles();
//
//	_float4 RandomRange(const _float4& vStandard, const _float4& vRandomRange);
//	_float4 RandomRange(const _float4& vStandard, const _float4& vRandomRange);
//
//private:
//	virtual void Release() override;
//
//};
//
//END