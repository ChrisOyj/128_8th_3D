#pragma once
#include "CEffect.h"

BEGIN(Client)

class CWoodWall final
	: public CEffect
{
	DECLARE_PROTOTYPE(CWoodWall);

private:
	CWoodWall();
	virtual ~CWoodWall();

public:
	static CWoodWall* Create();

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	_bool		m_bOnce = false;

public:
	virtual void My_LateTick() override;

public:
	virtual void OnEnable();
	virtual void OnDisable();
};

END