#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
class CCell;
END

BEGIN(Client)

class CUnit;

class CKamuiEffect
	: public CEffect
{
	DECLARE_PROTOTYPE(CKamuiEffect);

private:
	CKamuiEffect();
	virtual ~CKamuiEffect();

public:
	static CKamuiEffect* Create();

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
	_float		m_fBomTime = 1.2f;


	_float		m_fColAcc = 0.f;
	_float		m_fColTime = 0.07f;

	_bool		m_bDisAppear = false;
	_bool		m_bOnce = false;
	_float		m_fDisappearTime = 0.6f;

	list<CGameObject*>	m_EffectsList;
	list<CGameObject*>	m_EnemyList;

	CCell* m_pCurCell = nullptr;

	_uint		m_iDisappearCnt = 0;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;


};

END