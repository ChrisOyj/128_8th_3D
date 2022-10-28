#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)

class CUnit;

class CKunai
	: public CEffect
{
	DECLARE_PROTOTYPE(CKunai);

private:
	CKunai();
	virtual ~CKunai();

public:
	static CKunai* Create();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

public:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	class CTrailEffect* m_pTrailEffect = nullptr;


};

END