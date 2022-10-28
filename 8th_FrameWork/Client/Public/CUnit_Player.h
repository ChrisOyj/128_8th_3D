#pragma once
#include "CUnit.h"

BEGIN(Engine)
class CCell;
END

BEGIN(Client)
class CUnit_Player
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_Player);

private:
	CUnit_Player();
	virtual ~CUnit_Player();

public:
	static CUnit_Player* Create(UNIT_MODEL_DATA& tModelData, CCell* pStartCell);

public:
	void	On_FixedTarget(_bool bActivate);
	_bool	Is_LockedTarget() { return m_bFixedTarget; }

	virtual void	On_PlusHp(_float fHp) override;
	virtual void	On_Die() override;

	void	TurnOn_TrailBuffer();
	void	TurnOff_TrailBuffer();

	void	TurnOn_AllTrailBuffer();
	void	TurnOff_AllTrailBuffer();

	void	Set_EightGatesEffect(CGameObject* pEffect) { m_pEightGatesEffect = pEffect; }
	CGameObject* Get_EightGatesEffect() { return m_pEightGatesEffect; }

public:
	virtual void	Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos) override;
	virtual void	Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eColType) override;
	virtual void	Unit_CollisionExit(CGameObject* pOtherObj, const _uint& eColType) {};

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

public:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	list<class CTrailEffect*>	m_TrailEffects;
	//class CTrailEffect* m_pTrailEffect2 = nullptr;
	_bool		m_bFixedTarget = false;

	CGameObject* m_pEightGatesEffect = nullptr;


};

END