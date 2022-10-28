#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)

class CUnit;

class CBossKunai
	: public CEffect
{
	DECLARE_PROTOTYPE(CBossKunai);

private:
	CBossKunai();
	virtual ~CBossKunai();

public:
	static CBossKunai* Create();

	void	Set_ColType(COL_GROUP_CLIENT eType) { m_eColType = eType; }

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	_float4	m_vOnLook;
	_bool	m_bOnTarget = false;
	_bool	m_bOnWall = false;
	_float4 m_vOriginLook;
	COL_GROUP_CLIENT	m_eColType = COL_ENEMYSKILL;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;
	class CTrailEffect* m_pTrailEffect = nullptr;

};

END