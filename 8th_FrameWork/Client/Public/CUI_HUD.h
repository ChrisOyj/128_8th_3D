#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CDefault_UI;

class CUI_HUD
	: public CGameObject
{
	DECLARE_PROTOTYPE(CUI_HUD);

public:
	enum UI_TYPE {
		HUD_BASE, HUD_HPGREEN, HUD_HPRED, HUD_MINUSEFFECT, UI_END
	};


private:
	CUI_HUD();
	virtual ~CUI_HUD();


public:
	static CUI_HUD* Create();

public:
	void	Set_ShaderResource(CShader* pShader, const char* pConstantName);

public:
	void	Start_HpLerp(_float fCurHp, _float fMaxHp);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	//base, 아이콘들
	CDefault_UI* m_arrUI[UI_END];

	_bool		m_bHPLerp = false;
	_float		m_fMaxHp = 0.f;
	_float		m_fTimeAcc = 0.f;
	_float		m_fLerpTime = 0.3f;

	_float		m_fOriginHP = 0.f;
	_float		m_fTargetHP = 0.f;

	_float		m_fCurHp = 0.f;

	_float4		m_vOriginBarScale;
	_float4		m_vOriginBarPos;

	_float		m_fCurRatio = 0.f;


private:
	virtual void My_Tick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

};
END

