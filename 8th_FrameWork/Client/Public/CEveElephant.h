#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)

class CUnit;

class CEveElephant
	: public CEffect
{
	DECLARE_PROTOTYPE(CEveElephant);

private:
	CEveElephant();
	virtual ~CEveElephant();

public:
	static CEveElephant* Create();

	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	_float		m_fDistortionCreateTime = 0.07f;
	_float		m_fDistortionAcc = 0.f;
private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

END