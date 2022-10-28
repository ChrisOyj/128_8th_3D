#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
class CCell;
END

BEGIN(Client)

class CUnit;

class CLaughingMonkEffect
	: public CEffect
{
	DECLARE_PROTOTYPE(CLaughingMonkEffect);

private:
	CLaughingMonkEffect();
	virtual ~CLaughingMonkEffect();

public:
	static CLaughingMonkEffect* Create();

public:
	void	Set_Cell(CCell* pCell) { m_pCurCell = pCell; };

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	_bool		m_bLittleDelay = false;
	_bool		m_bOnce = false;
	_bool		m_bFinished = false;

	_float		m_fDelayTime = 1.2f;
	_float		m_fDelayAcc = 0.f;

	list<CGameObject*>	m_EffectsList;

	COL_GROUP_CLIENT	m_eColType = COL_ENEMYSKILL;

	CCell* m_pCurCell = nullptr;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;


};

END