#include "stdafx.h"
#include "CUI_Skill.h"

#include "GameInstance.h"

#include "CGameObject_Factory.h"
#include "Texture.h"

#include "CDefault_UI.h"
#include	"CFader.h"
#include "Functor.h"
#include "CShader.h"

CUI_Skill::CUI_Skill()
{
}

CUI_Skill::~CUI_Skill()
{
}

CUI_Skill* CUI_Skill::Create()
{
	CUI_Skill* pInstance = new CUI_Skill;

	if (FAILED(pInstance->Initialize_Prototype())) 
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CUI_Skill");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CUI_Skill::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	CGameObject* pOwner = pShader->Get_Owner();

	_uint i = SKILL_ICON1;

	for (; i < STYLEBASE; ++i)
	{
		if (pOwner == m_arrUI[i])
			break;
	}

	_uint iCurIndex = i - SKILL_ICON1;

	_float fRatio = m_fCurSkillCoolTime[m_iCurStyle][iCurIndex] / m_fSkillCoolTime[m_iCurStyle][iCurIndex];
	//-0.5에서 0.5로 해야함
	fRatio -= 0.5f;

	pShader->Set_RawValue(pConstantName, &fRatio, sizeof(_float));
}

void CUI_Skill::On_StyleChange(_uint iStyle)
{
	GAMEINSTANCE->Play_Sound(L"SkillCool", (CHANNEL_GROUP)CHANNEL_UI);
	
	m_iCurStyle = iStyle;
	GET_COMPONENT_FROM(m_arrUI[STYLEBASE], CTexture)->Set_CurTextureIndex(iStyle);
	GET_COMPONENT_FROM(m_arrUI[STYLETEXT], CTexture)->Set_CurTextureIndex(iStyle);

	GET_COMPONENT_FROM(m_arrUI[SKILL_ICON2], CTexture)->Set_CurTextureIndex(iStyle);

	if (iStyle == 0)
	{
		//나선수리검
		GET_COMPONENT_FROM(m_arrUI[SKILL_ICON3], CTexture)->Set_CurTextureIndex(12);
		//나선환
		GET_COMPONENT_FROM(m_arrUI[SKILL_ICON4], CTexture)->Set_CurTextureIndex(0);
		//팔문둔갑
		GET_COMPONENT_FROM(m_arrUI[SKILL_ICONSP], CTexture)->Set_CurTextureIndex(21);
		//용 22

	}
	else if (iStyle == 1)
	{
		//호화구술
		GET_COMPONENT_FROM(m_arrUI[SKILL_ICON3], CTexture)->Set_CurTextureIndex(3);
		//치도리
		GET_COMPONENT_FROM(m_arrUI[SKILL_ICON4], CTexture)->Set_CurTextureIndex(1);
		//카무잉
		GET_COMPONENT_FROM(m_arrUI[SKILL_ICONSP], CTexture)->Set_CurTextureIndex(2);

	}
	else if (iStyle == 2) // Water
	{
		//상어
		GET_COMPONENT_FROM(m_arrUI[SKILL_ICON3], CTexture)->Set_CurTextureIndex(21);
		//물감옥
		GET_COMPONENT_FROM(m_arrUI[SKILL_ICON4], CTexture)->Set_CurTextureIndex(18);
		//대폭포
		GET_COMPONENT_FROM(m_arrUI[SKILL_ICONSP], CTexture)->Set_CurTextureIndex(24);
	}
	else if (iStyle == 3) // Night Guy
	{
		GET_COMPONENT_FROM(m_arrUI[SKILL_ICONSP], CTexture)->Set_CurTextureIndex(22);

		GET_COMPONENT_FROM(m_arrUI[STYLEBASE], CTexture)->Set_CurTextureIndex(0);
		GET_COMPONENT_FROM(m_arrUI[STYLETEXT], CTexture)->Set_CurTextureIndex(0);

		GET_COMPONENT_FROM(m_arrUI[SKILL_ICON2], CTexture)->Set_CurTextureIndex(0);
	}


	GET_COMPONENT_FROM(m_arrUI[STYLEBASE], CFader)->Re_FadeIn();
	GET_COMPONENT_FROM(m_arrUI[STYLETEXT], CFader)->Re_FadeIn();

	for (_uint i = 0; i < 5; ++i)
	{
		if (Can_UseSkill((SKILL_TYPE)i))
			GET_COMPONENT_FROM(m_arrUI[SKILL_EFFECT1 + i], CFader)->Force_KeyInput();
	}

}

void CUI_Skill::Set_SkillCoolTime(_uint iStyle,  SKILL_TYPE eType, _float fCoolTime)
{
	m_fSkillCoolTime[iStyle][eType] = fCoolTime;
}

_bool CUI_Skill::Can_UseSkill(SKILL_TYPE eType)
{
	if (m_fCurSkillCoolTime[m_iCurStyle][eType] <= 0.f)
		return true;


	return false;
}

void CUI_Skill::On_UseSkill(SKILL_TYPE eType)
{
	m_fCurSkillCoolTime[m_iCurStyle][eType] = m_fSkillCoolTime[m_iCurStyle][eType];
}

HRESULT CUI_Skill::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_Skill::Initialize()
{
	vector<CDefault_UI*>	vecUIs = CGameObject_Factory::Create_vecUI_Reverse(L"InGame_Skill");

	if (vecUIs.size() != UI_END)
		return E_FAIL;

	//이름순으로 숙숟 들어오게
	for (_uint i = 0; i < UI_END; ++i)
	{
		m_arrUI[i] = vecUIs[i];
		CREATE_STATIC(m_arrUI[i], Convert_ToHash(m_arrUI[i]->Get_FileKey()));

		if (i >= SKILL_ICON1 && i < STYLEBASE)
			GET_COMPONENT_FROM(m_arrUI[i], CShader)->CallBack_SetRawValues +=
			bind(&CUI_Skill::Set_ShaderResource, this, placeholders::_1, "g_fProgressY");

	}

	GET_COMPONENT_FROM(m_arrUI[SKILL_ICON2], CTexture)->Set_CurTextureIndex(2);



	return S_OK;
}

void CUI_Skill::OnEnable()
{
	__super::OnEnable();

	for (_uint i = 0; i < UI_END; ++i)
		ENABLE_GAMEOBJECT(m_arrUI[i]);

}

void CUI_Skill::OnDisable()
{
	__super::OnDisable();

	for (_uint i = 0; i < UI_END; ++i)
		DISABLE_GAMEOBJECT(m_arrUI[i]);
}

void CUI_Skill::My_Tick()
{
	_float fTimeDelta = RealfDT;
	for (_uint i = 0; i < STYLE_END; ++i)
	{
		for (_uint j = 0; j < SKILL_END; ++j)
		{
			if (m_fCurSkillCoolTime[i][j] > 0.f)
			{
				m_fCurSkillCoolTime[i][j] -= fTimeDelta;

				if (m_fCurSkillCoolTime[i][j] <= 0.f)
				{
					GAMEINSTANCE->Play_Sound(L"SkillCool", (CHANNEL_GROUP)CHANNEL_UI);

					GET_COMPONENT_FROM(m_arrUI[SKILL_EFFECT1 + j], CFader)->Force_KeyInput();
					m_fCurSkillCoolTime[i][j] = 0.f;
				}
			}
		}
	}
	
}