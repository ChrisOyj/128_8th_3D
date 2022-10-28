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
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

};

END