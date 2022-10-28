#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
class CCell;
END

BEGIN(Client)

class CUnit;

class CEightGatesEffects
	: public CEffect
{
	DECLARE_PROTOTYPE(CEightGatesEffects);

private:
	CEightGatesEffects();
	virtual ~CEightGatesEffects();

public:
	static CEightGatesEffects* Create();

	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);

public:
	void	On_UseNightGuy() { m_fFadeTimeAcc = 0.f; }
	void	Set_Dragon(CGameObject* pDragon) { m_EffectsList.push_back(pDragon); }

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	_float		m_fCreateAcc = 0.f;
	_float		m_fCreateTime = 0.45f;


	_float		m_fParticleAcc = 0.f;
	_float		m_fParticleTime = 0.06f;


	list<CGameObject*>	m_EffectsList;

	CCell* m_pCurCell = nullptr;


private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;


};

END