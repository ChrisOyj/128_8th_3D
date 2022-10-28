#pragma once
#include "CEffect.h"

BEGIN(Client)

class CEightGatesDragon final
	: public CEffect
{
	DECLARE_PROTOTYPE(CEightGatesDragon);

private:
	CEightGatesDragon();
	virtual ~CEightGatesDragon();

public:
	static CEightGatesDragon* Create();

public:
	void	Start_Dragon();



public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Start() override;

public:
	virtual void My_LateTick() override;

public:
	virtual void OnEnable();
	virtual void OnDisable();

private:
	CGameObject* m_pEffect = nullptr;
};

END