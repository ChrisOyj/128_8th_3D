#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)

class CUnit;

class CBoneChakra
	: public CEffect
{
	DECLARE_PROTOTYPE(CBoneChakra);

private:
	CBoneChakra();
	virtual ~CBoneChakra();

public:
	static CBoneChakra* Create();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

public:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	CGameObject* m_pEffect = nullptr;
	class CTrailEffect* m_pTrailEffect = nullptr;


private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;



};

END