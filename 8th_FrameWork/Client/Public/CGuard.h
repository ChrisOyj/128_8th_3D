#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CGuard
	: public CEffect
{
	DECLARE_PROTOTYPE(CGuard);

private:
	CGuard();
	virtual ~CGuard();

public:
	static CGuard* Create();


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

};

END