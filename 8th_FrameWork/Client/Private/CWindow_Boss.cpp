#include "stdafx.h"
#include "CWindow_Boss.h"

#include "GameInstance.h"

#include "Transform.h"
#include "Physics.h"
#include "Texture.h"
#include "CShader.h"
#include "CFader.h"
#include "Renderer.h"
#include "Model.h"
#include "MeshContainer.h"

#include "Functor.h"

#include "CGameObject_Factory.h"
#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

#include "CUtility_Json.h"

#include "CMap.h"
#include "CGround.h"
#include "CMap_Sample.h"
#include "CObject_Map.h"

#include "CUtility_Transform.h"
#include "Camera.h"

#include "CUnit.h"
#include "CScript_FollowCam.h"
#include "CCamera_Cinema.h"

#include "CState_Manager.h"
#include "CState_Boss.h"

CWindow_Boss::CWindow_Boss()
{
}

CWindow_Boss::~CWindow_Boss()
{
}

CWindow_Boss* CWindow_Boss::Create()
{
	CWindow_Boss* pInstance = new CWindow_Boss;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Boss");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_Boss::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_Boss).name(), ImVec2(360.f, 300.f), window_flags);

	m_pStateBoss = GET_STATE(STATE_BOSS, CState_Boss);

	return S_OK;
}

void CWindow_Boss::Tick()
{
}

HRESULT CWindow_Boss::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	if (!m_pStateBoss)
	{
		__super::End();
		return S_OK;

	}

	static _int iCurIndex = 0;
	if (ImGui::InputInt("TargetIndex", &iCurIndex))
	{
		if (iCurIndex < 0)
			iCurIndex = 0;

		if (iCurIndex > 6)
			iCurIndex = 6;
	}

	string strTemp = "Current Phase : ";
	switch (iCurIndex)
	{
	case 0:
		strTemp += "CRYING";
		break;
	case 1:
		strTemp += "CHARGE_START";
		break;
	case 2:
		strTemp += "CHARGE_LOOP";
		break;
	case 3:
		strTemp += "CHARGE_END";
		break;
	case 4:
		strTemp += "FALL";
		break;
	case 5:
		strTemp += "LAND";
		break;
	case 6:
		strTemp += "ANGRY";
		break;

	default:
		break;
	}
	ImGui::Text(strTemp.c_str());

	CState_Boss::BOSSPHASE& tPhase = m_pStateBoss->m_arrBossPhase[iCurIndex];

	if (ImGui::InputFloat("Duration", &tPhase.fDuration))
	{
	}
	if (ImGui::InputFloat("InterPolationTime", &tPhase.fInterPolationTime))
	{
	}
	if (ImGui::InputFloat("AnimSpeed", &tPhase.fAnimSpeed))
	{ }

	if (ImGui::InputFloat("JumpPower", &m_pStateBoss->m_fJumpPower))
	{
	}
	if (ImGui::InputFloat("Speed", &m_pStateBoss->m_fSpeed))
	{
	}




	__super::End();

	return S_OK;
}
