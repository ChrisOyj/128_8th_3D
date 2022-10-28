#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)

class CUnit;

class CWindmill
	: public CEffect
{
	DECLARE_PROTOTYPE(CWindmill);

private:
	CWindmill();
	virtual ~CWindmill();

public:
	static CWindmill* Create();

public:
	void		On_Shoot(CGameObject*	pFollowTarget, _float4 vTargetPos = _float4(0.f, 0.f, 0.f, 1.f));
	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	_bool		m_bShoot = false;
	_bool		m_bOnWall = false;

	CGameObject* m_pEffect = nullptr;

	_float		m_fCreateTime = 0.15f;
	_float		m_fCreateAcc = 0.f;

	_float		m_fSoundAcc = 0.f;
	_float		m_fSoundTime = 0.3f;


private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

};

END