#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CDefault_UI;

class CUI_Combo
	: public CGameObject
{
	DECLARE_PROTOTYPE(CUI_Combo);

public:
	enum UI_TYPE {
		COMBO_NUMFRONT, COMBO_HIT, COMBO_KOBG, COMBO_KOTEXT, COMBO_NUMBACK, COMBO_HITS, UI_END
	};


private:
	CUI_Combo();
	virtual ~CUI_Combo();


public:
	static CUI_Combo* Create();

public:
	void	Set_ShaderResource(CShader* pShader, const char* pConstantName);

public:
	void	On_RefreshCombo();
	void	On_KO();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	//base, 아이콘들
	CDefault_UI* m_arrUI[UI_END];

	_float		m_fTimeAcc = 0.f;
	_float		m_fComboDeleteTime = 2.f;
	_bool		m_bCombo = false;

	_uint		m_iNumFrontCount = 0;
	_uint		m_iNumBackCount = 0;

	_bool		m_bSlowTime = false;
	_float		m_fSlowTimeAcc = 0.f;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

};
END

