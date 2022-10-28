#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CDefault_UI;

class CUI_Skill
	: public CGameObject
{
	DECLARE_PROTOTYPE(CUI_Skill);

public:
	enum UI_TYPE { SKILL_BASE1, SKILL_BASE2, SKILL_BASE3, SKILL_BASE4,
		SKILL_EFFECT1, SKILL_EFFECT2, SKILL_EFFECT3, SKILL_EFFECT4, SKILL_EFFECTSP,
		SKILL_ICON1, SKILL_ICON2, SKILL_ICON3, SKILL_ICON4, SKILL_ICONSP, STYLEBASE, STYLETEXT, UI_END};

	enum SKILL_TYPE {DODGE, THROW, SKILL1, SKILL2, SKILLSP, SKILL_END};

	enum STYLE_TYPE {HAND, NINZASWORD, WATER, NIGHTGUY , STYLE_END};

private:
	CUI_Skill();
	virtual ~CUI_Skill();


public:
	static CUI_Skill* Create();

public:
	void	Set_ShaderResource(CShader* pShader, const char* pConstantName);

public:
	void		On_StyleChange(_uint iStyle);


public:
	void		Set_SkillCoolTime(_uint iStyle, SKILL_TYPE eType, _float fCoolTime);
	_bool		Can_UseSkill(SKILL_TYPE eType);
	void		On_UseSkill(SKILL_TYPE eType);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	//base, 아이콘들
	CDefault_UI* m_arrUI[UI_END];

	_float		m_fSkillCoolTime[STYLE_END][SKILL_END] = {};
	_float		m_fCurSkillCoolTime[STYLE_END][SKILL_END] = {0.f, 0.f, 0.f, 0.f, 0.f};

	_uint		m_iCurStyle = 0;

private:
	virtual void My_Tick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

};
END

