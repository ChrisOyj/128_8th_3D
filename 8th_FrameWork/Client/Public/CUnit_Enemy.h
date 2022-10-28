#pragma once
#include "CUnit.h"

BEGIN(Engine)
class CCell;
END

BEGIN(Client)
class CUnit_Enemy
	: public CUnit
{
	DECLARE_PROTOTYPE(CUnit_Enemy);

private:
	CUnit_Enemy();
	virtual ~CUnit_Enemy();

public:
	static CUnit_Enemy* Create(UNIT_MODEL_DATA& tModelData, CCell* pStartCell);

public:
	virtual void	Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos) override;
	virtual void	Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eColType) override;

public:
	void	On_StageBegin();

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

public:
	virtual void My_Tick() override;

private:
	class CEnemyHPBar* m_pHpBar = nullptr;

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

};

END