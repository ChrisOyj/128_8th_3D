#include "stdafx.h"
#include "CUI_HUD.h"

#include "GameInstance.h"

#include "CGameObject_Factory.h"
#include "Texture.h"

#include "CDefault_UI.h"

#include "Functor.h"
#include "CShader.h"
#include "CFader.h"
#include "CUnit_Player.h"
#include "CUser.h"

#include "Transform.h"



CUI_HUD::CUI_HUD()
{
}

CUI_HUD::~CUI_HUD()
{
}

CUI_HUD* CUI_HUD::Create()
{
	CUI_HUD* pInstance = new CUI_HUD;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CUI_HUD");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CUI_HUD::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	CGameObject* pOwner = pShader->Get_Owner();

	_uint iCurIndex = HUD_HPGREEN;

	for (; iCurIndex < UI_END; ++iCurIndex)
	{
		if (pOwner == m_arrUI[iCurIndex])
			break;
	}
	_float4 vColor = _float4(0.60392f, 0.7725f, 0.1137f, 1.f);

	


	if (iCurIndex == HUD_HPRED)
		vColor = _float4(0.53725f, 0.f, 0.f);
	else if (m_fCurRatio < 0.45f)
	{
		//개피일 때
		vColor.x = 1.f * (1.f - m_fCurRatio);
		vColor.y = 1.f * (m_fCurRatio);
	}

	pShader->Set_RawValue(pConstantName, &vColor, sizeof(_float4));
}

void CUI_HUD::Start_HpLerp(_float fCurHp, _float fMaxHp)
{
	m_fOriginHP = m_fCurHp;
	m_fTargetHP = fCurHp;
	m_fMaxHp = fMaxHp;
	m_fTimeAcc = 0.f;

	m_bHPLerp = true;

	GET_COMPONENT_FROM(m_arrUI[HUD_MINUSEFFECT], CFader)->Re_FadeIn();

}


HRESULT CUI_HUD::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_HUD::Initialize()
{
	vector<CDefault_UI*>	vecUIs = CGameObject_Factory::Create_vecUI_Reverse(L"InGame_SP");

	if (vecUIs.size() != UI_END)
		return E_FAIL;

	//이름순으로 숙숟 들어오게
	for (_uint i = 0; i < UI_END; ++i)
	{
		m_arrUI[i] = vecUIs[i];
		CREATE_STATIC(m_arrUI[i], Convert_ToHash(m_arrUI[i]->Get_FileKey()));

		if (i >= HUD_HPGREEN)
			GET_COMPONENT_FROM(m_arrUI[i], CShader)->CallBack_SetRawValues +=
			bind(&CUI_HUD::Set_ShaderResource, this, placeholders::_1, "g_vColor");

	}

	m_vOriginBarScale = m_arrUI[HUD_HPGREEN]->Get_Transform()->Get_Scale();
	m_vOriginBarPos = m_arrUI[HUD_HPGREEN]->Get_Transform()->Get_MyWorld(WORLD_POS);




	return S_OK;
}

void CUI_HUD::OnEnable()
{
	__super::OnEnable();

	for (_uint i = 0; i < UI_END; ++i)
		ENABLE_GAMEOBJECT(m_arrUI[i]);

}

void CUI_HUD::OnDisable()
{
	__super::OnDisable();

	for (_uint i = 0; i < UI_END; ++i)
		DISABLE_GAMEOBJECT(m_arrUI[i]);
}

void CUI_HUD::My_Tick()
{
	if (!m_bHPLerp)
		return;

	_float fTimeDelta = RealfDT;

	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc >= m_fLerpTime)
	{
		m_bHPLerp = false;
		m_fTimeAcc = m_fLerpTime;
	}

	//체력 비율에 맞춰 Green 줄여야함
	_float fRatio = m_fTimeAcc / m_fLerpTime;

	m_fCurHp = CFunctor::Lerp_Float(m_fOriginHP, m_fTargetHP, sqrtf(fRatio));

	m_fCurRatio = m_fCurHp / m_fMaxHp;

	_float4 vNewScale = m_vOriginBarScale;
	vNewScale.x *= m_fCurRatio;

	m_arrUI[HUD_HPGREEN]->Get_Transform()->Set_Scale(vNewScale);

	//둘 크기의 차이의 절반만큼 이동
	_float fMoveX = (m_vOriginBarScale.x - vNewScale.x) * 0.31f;

	_float4 vPos = m_vOriginBarPos;
	vPos.x -= fMoveX;
	m_arrUI[HUD_HPGREEN]->Get_Transform()->Set_World(WORLD_POS, vPos);

}