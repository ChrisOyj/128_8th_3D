#pragma once
#include "CState.h"

BEGIN(Engine)
class CCell;
END

BEGIN(Client)
class CWire_Player
	: public CState
{
	DECLARE_STATE(CWire_Player);
private:
	CWire_Player();
	virtual ~CWire_Player();

public:
	static CWire_Player* Create();

public:
	void	Set_Terrain(class CTerrain* pTerrain) { m_pTerrain = pTerrain; }

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	_float4	m_vDir;
	_float4	m_vTargetPos;

	CCell* m_pTargetCell = nullptr;

	_float	m_fMaxWireDistance = 30.f;

	class CTerrain* m_pTerrain = nullptr;

	_bool	m_bWireSuccess = false;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		TickExecute(CUnit* pOwner, CAnimator* pAnimator);

private:
	_float4		Picking_Terrain();
};

END