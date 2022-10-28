#pragma once
#include "CEffect.h"

BEGIN(Client)

class CEffect_Text final
	: public CEffect
{
	DECLARE_PROTOTYPE(CEffect_Text);

private:
	CEffect_Text();
	virtual ~CEffect_Text();

public:
	static CEffect_Text* Create(wstring wstrFBXPath, _hashcode hcCode);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Start() override;

private:
	_float4		m_vStartScale;
	_float4		m_vTargetScale;
	_float4		m_vOverScale;

public:
	virtual void My_Tick() override;
	virtual void My_LateTick() override {};
	virtual void OnEnable();
};

END