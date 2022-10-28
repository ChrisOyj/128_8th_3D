#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
class CCell;
END

BEGIN(Client)

class CUnit;

class CRasenShurikenBomb
	: public CEffect
{
	DECLARE_PROTOTYPE(CRasenShurikenBomb);

private:
	CRasenShurikenBomb();
	virtual ~CRasenShurikenBomb();

public:
	static CRasenShurikenBomb* Create();

	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);

public:
	void	Set_Cell(CCell* pCell) { m_pCurCell = pCell; }

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	_float		m_fBoomAcc = 0.f;
	_float		m_fBomTime = 2.6f;


	_float		m_fColAcc = 0.f;
	_float		m_fColTime = 0.07f;

	_bool		m_bBombed = false;

	list<CGameObject*>	m_EffectsList;

	CCell* m_pCurCell = nullptr;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;


};

END