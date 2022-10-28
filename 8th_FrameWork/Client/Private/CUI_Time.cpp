#include "stdafx.h"
#include "CUI_Time.h"

#include "GameInstance.h"

#include "CGameObject_Factory.h"
#include "Texture.h"

#include "CDefault_UI.h"

#include "Functor.h"

CUI_Time::CUI_Time()
{
}

CUI_Time::~CUI_Time()
{
}

CUI_Time* CUI_Time::Create()
{
	CUI_Time* pInstance = new CUI_Time;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CUI_Time");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CUI_Time::Reset_Time(_int iMin, _int iSecFront, _int iSecBack)
{
	m_bStopTimer = false;
	m_fCurTimeAcc = 0.f;

	m_iCurMin = iMin;
	m_iCurSecFront = iSecFront;
	m_iCurSecBack = iSecBack;
}

HRESULT CUI_Time::Initialize_Prototype()
{

	Reset_Time(9, 5, 9);

	return S_OK;
}

HRESULT CUI_Time::Initialize()
{
	vector<CDefault_UI*>	vecUIs = CGameObject_Factory::Create_vecUI_Reverse(L"InGame_Time");

	if (vecUIs.size() != TIME_END)
		return E_FAIL;

	//이름순으로 숙숟 들어오게
	for (_uint i = 0; i < TIME_END; ++i)
	{
		m_arrUI[i] = vecUIs[i];
		CREATE_STATIC(m_arrUI[i], Convert_ToHash(m_arrUI[i]->Get_FileKey()));
	
	}



	return S_OK;
}

void CUI_Time::OnEnable()
{
	__super::OnEnable();

	for (_uint i = 0; i < TIME_END; ++i)
		ENABLE_GAMEOBJECT(m_arrUI[i]);

	Reset_Time(9, 5, 9);

}

void CUI_Time::OnDisable()
{
	__super::OnDisable();

	for (_uint i = 0; i < TIME_END; ++i)
		DISABLE_GAMEOBJECT(m_arrUI[i]);
}

void CUI_Time::My_Tick()
{
	if (m_bStopTimer)
		return;

	m_fCurTimeAcc += RealfDT;

	if (m_fCurTimeAcc >= 1.f)
	{
		m_fCurTimeAcc = 0.f;
		m_iCurSecBack--;

		if (m_iCurSecBack < 0)
		{
			m_iCurSecBack = 9;
			m_iCurSecFront--;

			if (m_iCurSecFront < 0)
			{
				m_iCurSecFront = 5;
				m_iCurMin--;

				if (m_iCurMin < 0)
				{
					m_bStopTimer = true;
				}
			}
		}

		GET_COMPONENT_FROM(m_arrUI[TIME_MIN], CTexture)->Set_CurTextureIndex(m_iCurMin);
		GET_COMPONENT_FROM(m_arrUI[TIME_SECFRONT], CTexture)->Set_CurTextureIndex(m_iCurSecFront);
		GET_COMPONENT_FROM(m_arrUI[TIME_SECBACK], CTexture)->Set_CurTextureIndex(m_iCurSecBack);
	}
}