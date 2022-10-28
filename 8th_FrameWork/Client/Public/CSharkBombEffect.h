#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)

class CUnit;

class CSharkBombEffect
	: public CEffect
{
	DECLARE_PROTOTYPE(CSharkBombEffect);

private:
	CSharkBombEffect();
	virtual ~CSharkBombEffect();

public:
	static CSharkBombEffect* Create();

	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	_float		m_fParticleCreateTime = 0.1f;
	_float		m_fPCAcc = 0.f;

	list<CGameObject*>	m_EffectsList;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	class CTrailEffect* m_pTrailEffect = nullptr;


};

END