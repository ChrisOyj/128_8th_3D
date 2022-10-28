#include "stdafx.h"
#include "CUI_Combo.h"

#include "GameInstance.h"

#include "CGameObject_Factory.h"
#include "Texture.h"

#include "CDefault_UI.h"

#include "Functor.h"
#include "CShader.h"
#include "CFader.h"

#include "CUnit_Player.h"
#include "CUser.h"
#include "CScript_FollowCam.h"

#include "Transform.h"

#include "CKOLine.h"



CUI_Combo::CUI_Combo()
{
}

CUI_Combo::~CUI_Combo()
{
}

CUI_Combo* CUI_Combo::Create()
{
	CUI_Combo* pInstance = new CUI_Combo;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CUI_Combo");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CUI_Combo::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
}

void CUI_Combo::On_RefreshCombo()
{
	//이미 활성화 중이 아니었다면
	if (!m_bCombo)
	{
		GET_COMPONENT_FROM(m_arrUI[COMBO_HIT], CFader)->Re_FadeIn();
	}
	else
	{
		//2번째 힛부터는
		GET_COMPONENT_FROM(m_arrUI[COMBO_NUMBACK], CFader)->Re_FadeIn();
		GET_COMPONENT_FROM(m_arrUI[COMBO_HITS], CFader)->Re_FadeIn();

		//2번째 타였는데 아직 hit이 살아있으면
		if (m_iNumBackCount == 1)
		{
			GET_COMPONENT_FROM(m_arrUI[COMBO_HIT], CFader)->Re_FadeOut();
		}
	}


	m_bCombo = true;
	m_fTimeAcc = 0.f;
	m_iNumBackCount++;
	if (m_iNumBackCount > 9)
	{
		m_iNumBackCount = 0;
		m_iNumFrontCount++;

		if (m_iNumFrontCount > 9)
			m_iNumFrontCount = 0;
	}


	if (m_iNumFrontCount >= 1)
	{
		GET_COMPONENT_FROM(m_arrUI[COMBO_NUMFRONT], CFader)->Re_FadeIn();
	}

	GET_COMPONENT_FROM(m_arrUI[COMBO_NUMBACK], CTexture)->Set_CurTextureIndex(m_iNumBackCount);
	GET_COMPONENT_FROM(m_arrUI[COMBO_NUMFRONT], CTexture)->Set_CurTextureIndex(m_iNumFrontCount);

}

void CUI_Combo::On_KO()
{
	GET_COMPONENT_FROM(m_arrUI[COMBO_KOBG], CFader)->Re_FadeIn();
	GET_COMPONENT_FROM(m_arrUI[COMBO_KOTEXT], CFader)->Re_FadeIn();

	//GAMEINSTANCE->Set_TimeSpeed(0.1f);
	m_bSlowTime = true;
	m_fSlowTimeAcc = 0.f;



}


HRESULT CUI_Combo::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_Combo::Initialize()
{
	vector<CDefault_UI*>	vecUIs = CGameObject_Factory::Create_vecUI_Reverse(L"InGame_Combo");

	if (vecUIs.size() != UI_END)
		return E_FAIL;

	//이름순으로 숙숟 들어오게
	for (_uint i = 0; i < UI_END; ++i)
	{
		m_arrUI[i] = vecUIs[i];
		CREATE_STATIC(m_arrUI[i], Convert_ToHash(m_arrUI[i]->Get_FileKey()));
	}



	return S_OK;
}

void CUI_Combo::OnEnable()
{
	__super::OnEnable();

	for (_uint i = 0; i < UI_END; ++i)
		ENABLE_GAMEOBJECT(m_arrUI[i]);

}

void CUI_Combo::OnDisable()
{
	__super::OnDisable();

	for (_uint i = 0; i < UI_END; ++i)
		DISABLE_GAMEOBJECT(m_arrUI[i]);
}

void CUI_Combo::My_Tick()
{
	if (!m_bCombo)
		return;

	m_fTimeAcc += fDT;

	if (m_fTimeAcc >= m_fComboDeleteTime)
	{
		m_bCombo = false;
		m_fTimeAcc = 0.f;
		m_iNumBackCount = 0;
		m_iNumFrontCount = 0;
	}
}

void CUI_Combo::My_LateTick()
{
	if (!m_bSlowTime)
		return;

	m_fSlowTimeAcc += RealfDT;

	if (m_fSlowTimeAcc >= 0.4f)
	{
		m_bSlowTime = false;
		//GAMEINSTANCE->Set_TimeSpeed(1.f);
		CAMERA_LERP(CAMERA_LERP_DEFAULT);
	}
}
