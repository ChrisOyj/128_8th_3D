#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CKOLine
	: public CEffect
{
	DECLARE_PROTOTYPE(CKOLine);

private:
	CKOLine();
	virtual ~CKOLine();

public:
	static CKOLine* Create(wstring wstrPath, _float4 vLook);


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	virtual void My_LateTick() override;

private:
	HRESULT	SetUp_Line(wstring wstrPath, _float4 vLook);

};

END