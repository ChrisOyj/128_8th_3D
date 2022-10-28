#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
class CCell;
END

BEGIN(Client)

class CUnit;

class CBossAura
	: public CEffect
{
	DECLARE_PROTOTYPE(CBossAura);

private:
	CBossAura();
	virtual ~CBossAura();

public:
	static CBossAura* Create();

	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);

public:
	void	On_StartFog();
	void	On_StopFog();



public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	_float		m_fCreateAcc = 0.f;
	_float		m_fCreateTime = 0.5f;


	_float		m_fParticleAcc = 0.f;
	_float		m_fParticleTime = 0.15f;

	list<CGameObject*>	m_EffectsList;

	CCell* m_pCurCell = nullptr;

	_bool	m_bFog = false;
	_float	m_fFogAlpha = 0.f;

	_bool	m_bStopFoging = false;

	class CDefault_UI* m_pKotoUI = nullptr;


	

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	void Update_Fog();


};

END