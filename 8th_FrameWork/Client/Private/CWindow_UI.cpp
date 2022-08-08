#include "stdafx.h"
#include "CWindow_UI.h"

#include "GameInstance.h"

#include "CUI.h"

#include "Transform.h"
#include "Physics.h"
#include "Texture.h"
#include "CShader.h"
#include "CFader.h"

#include "Functor.h"

#include "CGameObject_Factory.h"

CWindow_UI::CWindow_UI()
{
}

CWindow_UI::~CWindow_UI()
{
}

CWindow_UI* CWindow_UI::Create()
{
	CWindow_UI* pInstance = new CWindow_UI;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_UI");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_UI::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_UI).name(), ImVec2(300.f, 600.f), window_flags);

	return S_OK;
}

void CWindow_UI::Tick()
{
}

HRESULT CWindow_UI::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	if (ImGui::Button("CREATE ORTHO"))
	{
		Create_Ortho();
	}

	ImGui::SameLine();

	if (ImGui::Button("CREATE PERSPECTIVE"))
	{
		Create_Perspective();
	}

	ImGui::NewLine();

	if (ImGui::BeginTabBar("ui_list"))
	{
		if (ImGui::BeginTabItem("UI_LIST"))
		{
			Show_ListBox();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	if (m_iCurrentIdx != 9999)
	{
		ImGui::NewLine();

		if (ImGui::BeginTabBar("components"))
		{
			if (ImGui::BeginTabItem("Transform"))
			{
				Show_Transform(m_iCurrentIdx);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Physics"))
			{
				Show_Physics(m_iCurrentIdx);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Texture"))
			{
				Show_Texture(m_iCurrentIdx);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Shader"))
			{
				Show_Shader(m_iCurrentIdx);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Fader"))
			{
				Show_Fader(m_iCurrentIdx);

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
	




	__super::End();

	return S_OK;
}

void CWindow_UI::Create_Ortho()
{
	CUI* pUIPrototype = CUI::Create(m_iUIID, _float4(g_iWinCX * 0.5f, g_iWinCY * 0.5f, 0.f));
	CGameInstance::Get_Instance()->Add_GameObject_Prototype(m_iUIID, pUIPrototype);

	CUI* pUI = static_cast<CUI*>(CGameObject_Factory::Create_FromPrototype(m_iUIID));
	CREATE_GAMEOBJECT(pUI, GROUP_UI);

	UI_ITEM	tItem;
	tItem.bSelected = false;
	tItem.iGameObjectID = m_iUIID++;
	tItem.pUI = pUI;
	tItem.bOrtho = true;

	m_vecUI.push_back(tItem);
}

void CWindow_UI::Create_Perspective()
{
}

void CWindow_UI::Show_ListBox()
{
	if (ImGui::BeginListBox("UI_LIST", ImVec2(0, 200.f)))
	{
		for (_uint i = 0; i < m_vecUI.size(); ++i)
		{
			if (ImGui::Selectable(to_string(m_vecUI[i].iGameObjectID).c_str(), m_vecUI[i].bSelected))
			{
				for (_uint j = 0; j < m_vecUI.size(); ++j)
					m_vecUI[j].bSelected = false;

				m_iCurrentIdx = i;
				m_vecUI[i].bSelected = true;
			}

			if (m_vecUI[i].bSelected)
			{
				ImGui::SetItemDefaultFocus();
			}

		}

		ImGui::EndListBox();


	}
}

void CWindow_UI::Show_Components(_uint iIndex)
{
	if (ImGui::BeginListBox("COMPONENTS_LIST", ImVec2(0, 200.f)))
	{
		if (ImGui::Selectable("Transform", m_vecUI[iIndex].bSelected))
		{
			ImGui::OpenPopup("Transform");
		}
		Show_Transform(iIndex);


		ImGui::EndListBox();
	}
}

void CWindow_UI::Show_Transform(_uint iIndex)
{
	CTransform* pTransform = m_vecUI[iIndex].pUI->Get_Transform();
	_float4 vPos = pTransform->Get_MyWorld(WORLD_POS);
	_float4 vOrthoPos = CFunctor::RealToOrthoPosition(vPos);
	ImGui::Text("- Position -");
	ImGui::SliderFloat("Pos:X", &vOrthoPos.x, 0.f, (_float)g_iWinCX);
	ImGui::SameLine();
	ImGui::
	ImGui::SliderFloat("Pos:Y", &vOrthoPos.y, 0.f, (_float)g_iWinCY);

	_float4 vScale = pTransform->Get_Scale();
	ImGui::Text("- Scale -");
	ImGui::SliderFloat("Scale:X", &vScale.x, 0.f, (_float)g_iWinCX);
	ImGui::SliderFloat("Scale:Y", &vScale.y, 0.f, (_float)g_iWinCY);


	_float4 vRight = pTransform->Get_MyWorld(WORLD_RIGHT).Normalize();
	_float4 vNormalRight = _float4(1.f, 0.f, 0.f, 0.f);

	_float fDegree = CFunctor::Get_DegreeFromDot(vRight, vNormalRight);

	ImGui::Text("- Z_Rotation -");
	ImGui::SliderFloat("Degree", &fDegree, 0.f, 360.f);

	_float4x4 matRot;
	matRot = XMMatrixRotationAxis(_float4(0.f, 0.f, 1.f, 0.f).XMLoad(), ToRadian(fDegree));

	_float4 vNormalUp = _float4(0.f, 1.f, 0.f, 0.f);

	vNormalRight *= matRot;
	vNormalUp *= matRot;

	vPos = CFunctor::OrthoToRealPosition(vOrthoPos);
	pTransform->Set_World(WORLD_POS, vPos);
	pTransform->Set_World(WORLD_RIGHT, vNormalRight);
	pTransform->Set_World(WORLD_UP, vNormalUp);
	pTransform->Set_Scale(vScale);
	
}

void CWindow_UI::Show_Physics(_uint iIndex)
{
	ImGui::Text("Hi");
}

void CWindow_UI::Show_Texture(_uint iIndex)
{
	ImGui::Text("Hi");
}

void CWindow_UI::Show_Shader(_uint iIndex)
{
	ImGui::Text("Hi");
}

void CWindow_UI::Show_Fader(_uint iIndex)
{
	ImGui::Text("Hi");
}
