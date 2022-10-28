#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)

class CUnit;

class CRasenShurikenEffect
	: public CEffect
{
	DECLARE_PROTOTYPE(CRasenShurikenEffect);

private:
	CRasenShurikenEffect();
	virtual ~CRasenShurikenEffect();

public:
	static CRasenShurikenEffect* Create();

public:
	void		On_Shoot(CGameObject* pFollowTarget, _float4 vTargetPos = _float4(0.f, 0.f, 0.f, 1.f));

	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	_bool		m_bShoot = false;

	_float		m_fParticleCreateTime = 0.1f;
	_float		m_fPCAcc = 0.f;

	_float		m_fDistortionCreateTime = 0.2f;
	_float		m_fDistortionAcc = 0.f;

	list<CGameObject*>	m_EffectsList;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	class CTrailEffect* m_pTrailEffect = nullptr;


};

END