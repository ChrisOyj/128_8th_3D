#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)

class CUnit;

class CRaijinKunai
	: public CEffect
{
	DECLARE_PROTOTYPE(CRaijinKunai);

private:
	CRaijinKunai();
	virtual ~CRaijinKunai();

public:
	static CRaijinKunai* Create();

public:
	void	On_Success() { m_bSuccess = true; }

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	_bool		m_bSuccess = false;
	_bool		m_bSound = false;

private:
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	class CTrailEffect* m_pTrailEffect = nullptr;

};

END