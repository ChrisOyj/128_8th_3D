#include "stdafx.h"
#include "CWindow_Cinema.h"

#include "GameInstance.h"

#include "Transform.h"
#include "Physics.h"
#include "Texture.h"
#include "CShader.h"
#include "CFader.h"
#include "Renderer.h"
#include "Model.h"
#include "MeshContainer.h"
#include "CUser.h"
#include "Functor.h"

#include "CGameObject_Factory.h"
#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"
#include "CUnit_Player.h"
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

CWindow_Cinema::CWindow_Cinema()
{
}

CWindow_Cinema::~CWindow_Cinema()
{
}

CWindow_Cinema* CWindow_Cinema::Create()
{
	CWindow_Cinema* pInstance = new CWindow_Cinema;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Cinema");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CWindow_Cinema::SetUp_CinemaWindow(CCamera_Cinema* pCinemaCam)
{
	m_pCinemaCam = pCinemaCam;
	m_pCurCam = m_pCinemaCam;
}

void CWindow_Cinema::Set_SkillCam(CCamera_Cinema* pSkillCam)
{
	m_pSkillCam = pSkillCam;
	m_pCurCam = m_pSkillCam;
}

HRESULT CWindow_Cinema::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_Cinema).name(), ImVec2(600.f, 400.f), window_flags);

	return S_OK;
}

void CWindow_Cinema::Tick()
{

}

HRESULT CWindow_Cinema::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	_float fStep = 0.5f;

	if (ImGui::Button("FreeCam(F4)") || KEY(F4, TAP))
	{
		GAMEINSTANCE->Change_Camera(L"Free");
	}
	ImGui::SameLine();
	if (ImGui::Button("CinemaCam(F5)") || KEY(F5, TAP))
	{
		GAMEINSTANCE->Change_Camera(L"Cinema");
		m_pCurCam = m_pCinemaCam;
	}
	ImGui::SameLine();
	if (ImGui::Button("SkillCam(F6)") || KEY(F6, TAP))
	{
		GAMEINSTANCE->Change_Camera(L"Skill");
		m_pCurCam = m_pSkillCam;
	}

	static _int iCinemaType = 0;
	if (ImGui::InputInt("CurrentType", &iCinemaType))
	{
		if (!(iCinemaType < 0 || iCinemaType >= CCamera_Cinema::CINEMA_END))
			m_pCurCam->m_eCurrentType = (CCamera_Cinema::CINEMA_TYPE)iCinemaType;

		/*if (iCinemaType == 6)
		{

			m_pCurCam->m_pFollowTarget = GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY).front();
			m_pCurCam->m_pParent = GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY).front();

		}
		else
		{
			m_pCurCam->m_pFollowTarget = PLAYER;
			m_pCurCam->m_pParent = PLAYER;

		}*/
	}


	if (ImGui::Button("RePlay_CINEMA(1)") || KEY(NUM1, TAP))
	{
		m_pCurCam->m_bTesting = true;
		m_pCurCam->Start_Cinema(m_pCurCam->m_eCurrentType);
	}

	ImGui::SameLine();

	if (ImGui::Button("Stop(O)") || KEY(O, TAP))
	{
		m_pCurCam->m_bNodeFinish = true;
	}

	ImGui::SameLine();
	if (ImGui::Button("Play(P)") || KEY(P, TAP))
	{
		m_pCurCam->m_bNodeFinish = false;

	}


	if (ImGui::CollapsingHeader("CINEMA"))
	{
		_float4 vCurCamPos = GAMEINSTANCE->Get_ViewPos();
		_float4 vCurCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);

		static char szBuff[MIN_STR] = "";
		if (ImGui::InputText("CinemaName", szBuff, sizeof(szBuff), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_strCinemaName = szBuff;
		}
		if (ImGui::Button("SAVE_CINEMA"))
		{
			m_pCurCam->On_SaveCinema(m_strCinemaName);
			Call_MsgBox(L"Save성공");
		}

		ImGui::SameLine();
		if (ImGui::Button("LOAD_CINEMA"))
		{
			m_pCurCam->On_LoadCinema(m_strCinemaName, m_pCurCam->m_eCurrentType);
		}
		if (ImGui::Button("Save_This_Node(TAB)") || KEY(TAB, TAP))
		{
			CAMERA_NODE	tNode;
			if (!m_pCurCam->m_vecCamNode[m_pCurCam->m_eCurrentType].empty())
				tNode = m_pCurCam->m_vecCamNode[m_pCurCam->m_eCurrentType].back();

			//월드행렬 역행렬 곱해줘서 로컬로 만든 후 저장시켜야함
			if (m_pCurCam->m_eCurrentType >= CCamera_Cinema::CINEMA_KAMUI)
			{
				_float4x4 matWorldInv = m_pCurCam->m_pFollowTarget->Get_Transform()->Get_WorldMatrix().Inverse();
				tNode.vLook = vCurCamLook.MultiplyNormal(matWorldInv).Normalize();
				tNode.vPosition = vCurCamPos.MultiplyCoord(matWorldInv);
			}
			else
			{
				tNode.vLook = vCurCamLook;
				tNode.vPosition = vCurCamPos;
			}

			m_pCurCam->m_vecCamNode[m_pCurCam->m_eCurrentType].push_back(tNode);
			m_pCurCam->m_fOriginAngle[m_pCurCam->m_eCurrentType].push_back(0.f);
			m_pCurCam->m_fTargetAngle[m_pCurCam->m_eCurrentType].push_back(0.f);
			m_pCurCam->m_iCurrentIndex = m_pCurCam->m_vecCamNode[m_pCurCam->m_eCurrentType].size() -1;

			
		}

		ImGui::SameLine();

		if (ImGui::Button("Pop_Node(DELETE)") || KEY(DELETEKEY, TAP))
		{
			m_pCurCam->m_vecCamNode[m_pCurCam->m_eCurrentType].pop_back();
			m_pCurCam->m_iCurrentIndex = 0;
		}

		ImGui::SameLine();

		if (ImGui::Button("DELETE_ALL"))
		{
			m_pCurCam->m_vecCamNode[m_pCurCam->m_eCurrentType].clear();
			m_pCurCam->m_iCurrentIndex = 0;
		}

		ImGui::Text("Current Node Info");
		string strTemp = "Current Node Size : ";
		strTemp += to_string((_int)m_pCurCam->m_vecCamNode[m_pCurCam->m_eCurrentType].size());
		ImGui::Text(strTemp.c_str());

		_int iCurIndex = m_pCurCam->m_iCurrentIndex;

		if (ImGui::InputInt("CurrentIndex", &iCurIndex))
		{
			if (!(iCurIndex < 0 || iCurIndex >= m_pCurCam->m_vecCamNode[m_pCurCam->m_eCurrentType].size()))
				m_pCurCam->m_iCurrentIndex = iCurIndex;

		}

		if (!m_pCurCam->m_vecCamNode[m_pCurCam->m_eCurrentType].empty())
		{
			CAMERA_NODE& tCurrentNode = m_pCurCam->m_vecCamNode[m_pCurCam->m_eCurrentType][m_pCurCam->m_iCurrentIndex];

			_float	vVector[3] = {};
			memcpy(vVector, &tCurrentNode.vPosition, sizeof(_float3));

			if (ImGui::InputFloat3("vPosition", vVector))
			{
				memcpy(&tCurrentNode.vPosition, vVector, sizeof(_float3));
				tCurrentNode.vPosition.w = 1.f;

			}
			memcpy(vVector, &tCurrentNode.vLook, sizeof(_float3));

			if (ImGui::InputFloat3("vLook", vVector))
			{
				memcpy(&tCurrentNode.vLook, vVector, sizeof(_float3));
				tCurrentNode.vLook.w = 0.f;
			}

			_int iNodeType = (_int)tCurrentNode.eNodeType;

			if (ImGui::InputInt("NodeType(1:TELEPORT)", &iNodeType))
			{
				if (iNodeType < CAMERA_NODE::NODE_END)
					tCurrentNode.eNodeType = (CAMERA_NODE::NODETYPE)iNodeType;
			}

			_int iLerpType = (_int)tCurrentNode.eLerpType;

			if (ImGui::InputInt("LerpType(1:FASTER)", &iLerpType))
			{
				if (iLerpType < CAMERA_NODE::LERP_END)
					tCurrentNode.eLerpType = (CAMERA_NODE::LERPTYPE)iLerpType;
			}

			if (ImGui::InputFloat("ChangeSpeed", &tCurrentNode.fChangeSpeed, fStep))
			{
			}

			if (ImGui::InputFloat("MinSpeed", &tCurrentNode.fMinSpeed, fStep))
			{
			}

			if (ImGui::InputFloat("MaxSpeed", &tCurrentNode.fMaxSpeed, fStep))
			{
			}

			if (ImGui::InputFloat("DelayTime", &tCurrentNode.fDelayTime, fStep))
			{
			}

			if (ImGui::InputFloat("OriginAngle", &m_pCurCam->m_fOriginAngle[m_pCurCam->m_eCurrentType][m_pCurCam->m_iCurrentIndex], fStep))
			{
			}

			if (ImGui::InputFloat("TargetAngle", &m_pCurCam->m_fTargetAngle[m_pCurCam->m_eCurrentType][m_pCurCam->m_iCurrentIndex], fStep))
			{
			}
		}

		

			
	}




	__super::End();

	return S_OK;
}
