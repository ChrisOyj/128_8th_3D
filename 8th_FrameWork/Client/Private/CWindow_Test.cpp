#include "stdafx.h"
#include "CWindow_Test.h"

#include "GameInstance.h"

#include "Transform.h"
#include "Physics.h"
#include "Texture.h"
#include "CShader.h"
#include "CFader.h"
#include "Renderer.h"
#include "Model.h"
#include "MeshContainer.h"
#include "CEffect.h"
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

CWindow_Test::CWindow_Test()
{
}

CWindow_Test::~CWindow_Test()
{
}

CWindow_Test* CWindow_Test::Create()
{
	CWindow_Test* pInstance = new CWindow_Test;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Test");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CWindow_Test::SetUp_TestWindow(CUnit* pPlayer, CScript_FollowCam* pFollowCamCom)
{
	m_pPlayer = pPlayer;
	m_pFollowCamCom = pFollowCamCom;
}

HRESULT CWindow_Test::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_Test).name(), ImVec2(600.f, 400.f), window_flags);

	return S_OK;
}

void CWindow_Test::Tick()
{

}

HRESULT CWindow_Test::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	_float fStep = 0.5f;
	if (ImGui::CollapsingHeader("Dragon"))
	{
		_float4 vOffset = m_pDragon->m_vOffsetPos;
		if (ImGui::InputFloat("DragonOffsetX", &vOffset.x, fStep, 0.f, "%.3f"))
		{
		}
		if (ImGui::InputFloat("DragonOffsetY", &vOffset.y, fStep, 0.f, "%.3f"))
		{
		}
		if (ImGui::InputFloat("DragonOffsetZ", &vOffset.z, fStep, 0.f, "%.3f"))
		{
		}
		m_pDragon->m_vOffsetPos = vOffset;


		vOffset = m_pDragon->Get_Transform()->Get_World(WORLD_POS);
		if (ImGui::InputFloat("DragonWorldPosX", &vOffset.x, fStep, 0.f, "%.3f"))
		{
		}
		if (ImGui::InputFloat("DragonWorldPosY", &vOffset.y, fStep, 0.f, "%.3f"))
		{
		}
		if (ImGui::InputFloat("DragonWorldPosZ", &vOffset.z, fStep, 0.f, "%.3f"))
		{
		}
		m_pDragon->Get_Transform()->Set_World(WORLD_POS, vOffset);


		static _float4 vTurn = _float4(0.f, 0.f, 0.f, 0.f);
		if (ImGui::InputFloat("TurnX", &vTurn.x, fStep, 0.f, "%.3f"))
		{
		}
		if (ImGui::InputFloat("TurnY", &vTurn.y, fStep, 0.f, "%.3f"))
		{
		}
		if (ImGui::InputFloat("TurnZ", &vTurn.z, fStep, 0.f, "%.3f"))
		{
		}

		static _float fTurnAngle = 0.f;
		if (ImGui::InputFloat("TurnAngle", &fTurnAngle, fStep, 0.f, "%.3f"))
		{
		}

		if (!vTurn.Is_Zero())
			CUtility_Transform::Turn_ByAngle(m_pDragon->Get_Transform(), vTurn.Normalize(), fTurnAngle);
	}


	if (ImGui::CollapsingHeader("Test"))
	{
		_float4 vPos = m_pPlayer->Get_Transform()->Get_MyWorld(WORLD_POS);
		if (ImGui::InputFloat("PlayerWorldPosX", &vPos.x, fStep, 0.f, "%.3f"))
		{
		}
		if (ImGui::InputFloat("PlayerWorldPosY", &vPos.y, fStep, 0.f, "%.3f"))
		{
		}
		if (ImGui::InputFloat("PlayerWorldPosZ", &vPos.z, fStep, 0.f, "%.3f"))
		{
		}
		m_pPlayer->Get_Transform()->Set_World(WORLD_POS, vPos);
	}


	if (ImGui::CollapsingHeader("Player_Status"))
	{
		//À¯´Ö ÀÌ¼Ó
		if (ImGui::InputFloat("RunSpeed", &m_pPlayer->Get_Status().fRunSpeed, fStep, 0.f, "%.3f"))
		{

		}
		//À¯´Ö Á¡ÇÁ·Â
		if (ImGui::InputFloat("JumpPower", &m_pPlayer->Get_Status().fJumpPower, fStep, 0.f, "%.3f"))
		{

		}
		if (ImGui::InputFloat("DashSpeed", &m_pPlayer->Get_Status().fDashSpeed, fStep, 0.f, "%.3f"))
		{

		}
		if (ImGui::InputFloat("WalkSpeed", &m_pPlayer->Get_Status().fWalkSpeed, fStep, 0.f, "%.3f"))
		{

		}
		if (ImGui::InputFloat("WallJumpSpeed", &m_pPlayer->Get_Status().fWallJumpSpeed, fStep, 0.f, "%.3f"))
		{

		}
	}

	if (ImGui::CollapsingHeader("CamInfo"))
	{
		if (ImGui::Button("REFRESH"))
		{
			m_pFollowCamCom->Initialize_Prototype();
			m_pFollowCamCom->Initialize();
		}

		if (ImGui::InputFloat("DEFAULT_FOV", &m_pFollowCamCom->Get_DefaultDesc().fDefaultFOV, 0.1f, 0.f, "%.3f"))
		{
		}
		if (ImGui::InputFloat("DEFAULT_LerpTime", &m_pFollowCamCom->Get_DefaultDesc().fDefaultLerpTime, fStep, 0.f, "%.3f"))
		{
		}
		_float vFloat[4] = {};
		memcpy(vFloat, &m_pFollowCamCom->Get_DefaultDesc().vDefaultOffset, sizeof(_float4));
		if (ImGui::InputFloat3("Default_Offset", vFloat))
		{
		}
		static _int iIndex = 0;

		if (ImGui::InputInt("Index", &iIndex))
		{
		}

		//offset
		if (ImGui::InputFloat("Offset x", &m_pFollowCamCom->Get_Desc(iIndex).vTargetOffset.x, fStep, 0.f, "%.3f"))
		{
		}
		if (ImGui::InputFloat("Offset y", &m_pFollowCamCom->Get_Desc(iIndex).vTargetOffset.y, fStep, 0.f, "%.3f"))
		{
		}
		if (ImGui::InputFloat("Offset z", &m_pFollowCamCom->Get_Desc(iIndex).vTargetOffset.z, fStep, 0.f, "%.3f"))
		{
		}
		if (ImGui::InputFloat("CamFOV", &m_pFollowCamCom->Get_Desc(iIndex).fTargetFOV, fStep, 0.f, "%.3f"))
		{
		}
	}
	__super::End();

	return S_OK;
}
