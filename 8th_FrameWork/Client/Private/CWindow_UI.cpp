#include "stdafx.h"
#include "CWindow_UI.h"

#include "GameInstance.h"

#include "Transform.h"
#include "Physics.h"
#include "Texture.h"
#include "CShader.h"
#include "CFader.h"
#include "Renderer.h"

#include "Functor.h"

#include "CGameObject_Factory.h"
#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

#include "CUtility_Json.h"

#include "CDefault_UI.h"

#define GET_UICOMPONENT(classname) static_cast<classname*>(m_vecUI[iIndex].pUI->Get_Component<classname>().front())


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
	SetUp_ImGuiDESC(typeid(CWindow_UI).name(), ImVec2(400.f, 600.f), window_flags);


	return S_OK;
}

void CWindow_UI::Tick()
{
}

HRESULT CWindow_UI::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	if (ImGui::BeginTabBar("Tab"))
	{
		if (ImGui::BeginTabItem("UI_Setting"))
		{
			Show_UITab();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("UI_Loader"))
		{
			Show_UIList();

			ImGui::EndTabItem();
		}


		ImGui::EndTabBar();
	}


	__super::End();

	return S_OK;
}

HRESULT CWindow_UI::Save_Level()
{
	

	return S_OK;
}

HRESULT CWindow_UI::Save_UI(_uint iIndex)
{
	CDefault_UI* pUI = m_vecUI[m_iCurrentIdx].pUI;

	if (pUI->Get_FileKey() == L"0")
		return E_FAIL;

	json	UIJson;
	UIJson.emplace("matWorld", pUI->Get_Transform()->Get_Transform().matMyWorld.m);
	UIJson.emplace("vScale", pUI->Get_Transform()->Get_Transform().vScale.XMLoad().m128_f32);
	

	UIJson.emplace("iCurPass", GET_COMPONENT_FROM(pUI, CRenderer)->Get_Pass());
	UIJson.emplace("iCurTextureIndex", GET_COMPONENT_FROM(pUI, CTexture)->Get_CurTextureIndex());

	FADEDESC	tFadeDesc = GET_COMPONENT_FROM(pUI, CFader)->Get_FadeDesc();
	CUtility_Json::Save_Struct("tFadeDesc", tFadeDesc, &UIJson);

	PHYSICS	tPhysics = GET_COMPONENT_FROM(pUI, CPhysics)->Get_Physics();
	CUtility_Json::Save_Struct("tPhysics", tPhysics, &UIJson);

	vector<TEXTUREDESC>& vecTexture = GET_COMPONENT_FROM(pUI, CTexture)->Get_vecTexture();

	vector<wstring> vecFilePath;
	for (_uint i = 0; i < vecTexture.size(); ++i)
	{
		if (i == 0 &&
			vecTexture[i].strFilePath == L"../bin/resources/textures/ui/Jusin_0.png")
			continue;

		vecFilePath.push_back(vecTexture[i].strFilePath);
	}

	UIJson.emplace("strTextureFilePath", vecFilePath);

	if (FAILED(CUtility_Json::Save_Json(CUtility_Json::Complete_Path(pUI->Get_FileKey()).c_str(), UIJson)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWindow_UI::Load_Levels()
{
	m_vecUI.clear();

	list<CGameObject*>& UIList = CGameInstance::Get_Instance()->Get_ObjGroup(GROUP_UI);

	for (auto& elem : UIList)
	{
		CDefault_UI* pUI = dynamic_cast<CDefault_UI*>(elem);
		if (!pUI)
			continue;

		UI_ITEM	tItem;
		tItem.pUI = pUI;
		tItem.bSelected = false;

		m_vecUI.push_back(tItem);
	}

	/*for (filesystem::directory_iterator FileIter("../bin/Json");
		FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		_int iFind = (_int)strFullPath.rfind("\\") + 1;
		string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

		_int iFind2 = (_int)strFileName.find(".");
		strFileName = strFileName.substr(0, iFind2);

		_uint iFileNum = stoi(strFileName);

		if (Safe_CheckID(iFileNum, ID_LEVEL))
		{
			LEVEL_ITEM tItem;
			tItem.iLevelID = iFileNum;
			m_vecLevel.push_back(tItem);
		}
	}*/


	return S_OK;
}

HRESULT CWindow_UI::Load_UI()
{


	return S_OK;
}

void CWindow_UI::Show_LevelTab()
{
	
}

void CWindow_UI::Show_UITab()
{
	if (ImGui::Button("CREATE_UI"))
	{
		Create_Ortho();
	}

	if (ImGui::Button("SAVE_UI"))
	{
		if (FAILED(Save_UI(m_iCurrentIdx)))
		{
			Call_MsgBox(L"Failed to Save_UI : CWindow_UI");
			return;
		}
		else
		{
			Call_MsgBox(L"Save ¼º°ø");
		}
	}
	ImGui::SameLine();

	if (ImGui::Button("Refresh_List"))
	{
		Load_Levels();
	}

	if (ImGui::Button("ENABLE_UI"))
	{
		if (m_iCurrentIdx < m_vecUI.size())
		{
			ENABLE_GAMEOBJECT(m_vecUI[m_iCurrentIdx].pUI);
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("DISABLE_UI"))
	{
		if (m_iCurrentIdx < m_vecUI.size())
		{
			DISABLE_GAMEOBJECT(m_vecUI[m_iCurrentIdx].pUI);
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("DELETE_UI"))
	{
		if (m_iCurrentIdx < m_vecUI.size())
		{
			DELETE_GAMEOBJECT(m_vecUI[m_iCurrentIdx].pUI);

			auto iter = m_vecUI.begin();
			for (_uint i = 0; i < m_iCurrentIdx; ++i)
				++iter;
			iter = m_vecUI.erase(iter);

			m_iCurrentIdx = 9999;
		}
	}

	if (ImGui::Button("CLONE_UI"))
	{
		if (m_iCurrentIdx < m_vecUI.size())
		{
			CDefault_UI* pUI = m_vecUI[m_iCurrentIdx].pUI->Clone();
			CREATE_GAMEOBJECT(pUI, GROUP_UI);
			m_vecUI[m_iCurrentIdx].bSelected = false;

			UI_ITEM	tItem;
			tItem.pUI = pUI;
			tItem.bSelected = false;
			strcpy_s(tItem.szBuf, m_vecUI[m_iCurrentIdx].szBuf);
			m_vecUI.push_back(tItem);

			m_iCurrentIdx = (_int)m_vecUI.size() - 1;
			m_vecUI[m_iCurrentIdx].bSelected = true;
		}
	}



	if (m_iCurrentIdx != 9999)
	{
		if (ImGui::InputText("UI_NAME", m_vecUI[m_iCurrentIdx].szBuf, sizeof(m_vecUI[m_iCurrentIdx].szBuf), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			string strKey = m_vecUI[m_iCurrentIdx].szBuf;
			wstring wstrKey(strKey.begin(), strKey.end());
			m_vecUI[m_iCurrentIdx].pUI->Set_FileKey(wstrKey);
		}
	}

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
}

void CWindow_UI::Show_UIList()
{
	if (ImGui::Button("Load_Files"))
	{
		m_vecUIFile.clear();

		for (filesystem::directory_iterator FileIter("../bin/Json");
			FileIter != filesystem::end(FileIter); ++FileIter)
		{
			const filesystem::directory_entry& entry = *FileIter;

			wstring wstrPath = entry.path().relative_path();
			string strFullPath;
			strFullPath.assign(wstrPath.begin(), wstrPath.end());

			_int iFind = (_int)strFullPath.rfind("\\") + 1;
			string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

			_int iFind2 = (_int)strFileName.find(".");
			strFileName = strFileName.substr(0, iFind2);

			UI_FILE tFile;
			tFile.bSelected = false;
			tFile.strFileName = strFileName;

			m_vecUIFile.push_back(tFile);
		}
	}

	if (ImGui::Button("Create_UI"))
	{
		if (m_iCurrentFileIdx == 9999)
			return;

		CREATE_GAMEOBJECT(
			CDefault_UI::Clone_WithJson(CFunctor::To_Wstring(m_vecUIFile[m_iCurrentFileIdx].strFileName)),
			GROUP_UI);
	}


	if (ImGui::BeginListBox("ALL_UI_LIST", ImVec2(0, 300.f)))
	{
		for (_uint i = 0; i < m_vecUIFile.size(); ++i)
		{
			string strFileKey = m_vecUIFile[i].strFileName;

			if (ImGui::Selectable(strFileKey.c_str(), m_vecUIFile[i].bSelected))
			{
				for (_uint j = 0; j < m_vecUIFile.size(); ++j)
					m_vecUIFile[j].bSelected = false;

				m_iCurrentFileIdx = i;
				m_vecUIFile[i].bSelected = true;
			}

			if (m_vecUIFile[i].bSelected)
			{
				ImGui::SetItemDefaultFocus();
			}

		}

		ImGui::EndListBox();


	}
}

void CWindow_UI::Create_Ortho()
{
	CDefault_UI* pUI = CGameObject_Factory::Clone_GameObject<CDefault_UI>();
	CREATE_GAMEOBJECT(pUI, GROUP_UI);

	UI_ITEM	tItem;
	tItem.pUI = pUI;
	tItem.bSelected = false;
	ZeroMemory(tItem.szBuf, sizeof(tItem.szBuf));

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
			wstring wstrFileKey = m_vecUI[i].pUI->Get_FileKey();
			string strFileKey;
			strFileKey.assign(wstrFileKey.begin(), wstrFileKey.end());

			if (ImGui::Selectable(strFileKey.c_str(), m_vecUI[i].bSelected))
			{
				for (_uint j = 0; j < m_vecUI.size(); ++j)
					m_vecUI[j].bSelected = false;

				m_iCurrentIdx = i;
				m_vecUI[i].bSelected = true;
			}

			if (m_vecUI[i].bSelected)
			{
				ImGui::SetItemDefaultFocus();
				strcpy_s(m_vecUI[i].szBuf, strFileKey.c_str());
				//sprintf(m_vecUI[i].szBuf, strFileKey.c_str());
			}

			if (m_vecUI[i].pUI->Is_Disable())
			{
				ImGui::SameLine(0.f, 70.f);
				ImGui::Text("disable");
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
	static bool inputs_step = true;


	CTransform* pTransform = m_vecUI[iIndex].pUI->Get_Transform();
	_float4 vPos = pTransform->Get_MyWorld(WORLD_POS);
	_float4 vOrthoPos = CFunctor::RealToOrthoPosition(vPos);


	ImGui::Text("- Position -");
	ImGui::SliderFloat("Pos:X", &vOrthoPos.x, 0.f, (_float)g_iWinCX);
	ImGui::SameLine();
	ImGui::InputFloat("Pos:X,", &vOrthoPos.x);
	ImGui::SliderFloat("Pos:Y", &vOrthoPos.y, 0.f, (_float)g_iWinCY);
	ImGui::SameLine();
	ImGui::InputFloat("Pos:Y,", &vOrthoPos.y);
	ImGui::InputFloat("Pos:Z,", &vOrthoPos.z);

	_float4 vScale = pTransform->Get_Scale();
	ImGui::Text("- Scale -");
	ImGui::SliderFloat("Scale:X", &vScale.x, 0.f, (_float)g_iWinCX);
	ImGui::SameLine();
	ImGui::InputFloat("Scale:X,", &vScale.x);
	ImGui::SliderFloat("Scale:Y", &vScale.y, 0.f, (_float)g_iWinCY);
	ImGui::SameLine();
	ImGui::InputFloat("Scale:Y,", &vScale.y);

	_float4 vRight = pTransform->Get_MyWorld(WORLD_RIGHT).Normalize();
	_float4 vNormalRight = _float4(1.f, 0.f, 0.f, 0.f);

	_float fDegree = CFunctor::Get_DegreeFromDot(vRight, vNormalRight);

	ImGui::Text("- Z_Rotation -");
	ImGui::SliderFloat("Degree", &fDegree, -180.f, 180.f);
	//ImGui::SameLine();
	//ImGui::InputFloat("Degree", &fDegree);

	_float4x4 matRot;
	matRot = XMMatrixRotationAxis(_float4(0.f, 0.f, 1.f, 0.f).XMLoad(), ToRadian(fDegree));

	_float4 vNormalUp = _float4(0.f, 1.f, 0.f, 0.f);

	vNormalRight = vNormalRight.MultiplyNormal(matRot);
	vNormalUp = vNormalUp.MultiplyNormal(matRot);

	if (KEY(UP, TAP))
	{
		vOrthoPos.y -= 1.f;
	}
	if (KEY(DOWN, TAP))
	{
		vOrthoPos.y += 1.f;
	}
	if (KEY(LEFT, TAP))
	{
		vOrthoPos.x -= 1.f;
	}
	if(KEY(RIGHT, TAP))
	{
		vOrthoPos.x += 1.f;
	}


	vPos = CFunctor::OrthoToRealPosition(vOrthoPos);
	pTransform->Set_World(WORLD_POS, vPos);
	pTransform->Set_World(WORLD_RIGHT, vNormalRight);
	pTransform->Set_World(WORLD_UP, vNormalUp);
	pTransform->Set_Scale(vScale);


}

void CWindow_UI::Show_Physics(_uint iIndex)
{
	CPhysics* pPhysics = static_cast<CPhysics*>(m_vecUI[iIndex].pUI->Get_Component<CPhysics>().front());
	PHYSICS& tPhysics = pPhysics->Get_Physics();

	ImGui::Text("- TurnAxis -");
	ImGui::InputFloat("X", &tPhysics.vTurnDir.x);
	ImGui::InputFloat("Y", &tPhysics.vTurnDir.y);
	ImGui::InputFloat("Z", &tPhysics.vTurnDir.z);

	ImGui::Text("- TurnSpeed -");
	ImGui::InputFloat("TurnSpeed", &tPhysics.fTurnSpeed);
}

void CWindow_UI::Show_Texture(_uint iIndex)
{
	CTexture* pTexture = static_cast<CTexture*>(m_vecUI[iIndex].pUI->Get_Component<CTexture>().front());
	_int iCurIdx = pTexture->Get_CurTextureIndex();
	_int iTextureSize = pTexture->Get_TextureSize();
	ImGui::Text("Current Texture Index");
	ImGui::InputInt("TextureIdx", &iCurIdx);
	if (iCurIdx >= iTextureSize)
		iCurIdx = iTextureSize - 1;
	pTexture->Set_CurTextureIndex(iCurIdx);

	if (ImGui::CollapsingHeader("- Textures List -"))
	{
		if (ImGui::Button("Add Texture_Back"))
		{
			if (FAILED(pTexture->Add_Texture(m_CurSelectedTextureFilePath.c_str())))
			{
				Call_MsgBox(L"Failed to Add_Texture : CWindow_UI");
				return;
			}

			pTexture->Set_CurTextureIndex(pTexture->Get_TextureSize() - 1);
		}

		ImGui::SameLine();

		if (ImGui::Button("Add Texture_Insert"))
		{
			if (FAILED(pTexture->Add_Texture(m_CurSelectedTextureFilePath.c_str(), true)))
			{
				Call_MsgBox(L"Failed to Add_Texture : CWindow_UI");
				return;
			}

			pTexture->Set_CurTextureIndex(pTexture->Get_TextureSize() - 1);
		}

		if (ImGui::Button("Delete Texture"))
		{
			vector<TEXTUREDESC>& vecTex = pTexture->Get_vecTexture();
			_uint iCurIdx = pTexture->Get_CurTextureIndex();
			auto iter = vecTex.begin();
			for (_uint i = 0; i < iCurIdx; ++i)
				++iter;

			iter = vecTex.erase(iter);

			if (iCurIdx == vecTex.size() - 1)
				pTexture->Set_CurTextureIndex(iCurIdx - 1);

		}

		ImGui::SameLine();

		if (ImGui::Button("Pop Texture"))
		{
			pTexture->Pop_Texture();
		}
		if (ImGui::BeginListBox("Textures List", ImVec2(300.f, 0.f)))
		{
			Read_Folder("../bin/resources/textures");

			ImGui::EndListBox();
		}
	}

	ImGui::Text("Current Texture Size : %d", pTexture->Get_TextureSize());

	if (ImGui::BeginListBox("Current Textures List"))
	{
		for (auto& elem : pTexture->Get_vecTexture())
		{
			wstring wstrPath = elem.strFilePath;
			string strFullPath;
			strFullPath.assign(wstrPath.begin(), wstrPath.end());

			_int iFind = (_int)strFullPath.rfind("/") + 1;
			_int iFind2 = (_int)strFullPath.rfind("\\") + 1;
			_int iFinder = max(iFind, iFind2);
			string strFileName = strFullPath.substr(iFinder, strFullPath.length() - iFinder);

			ImGui::Text(strFileName.c_str());
		}

		ImGui::EndListBox();

	}

	
}

void CWindow_UI::Show_Shader(_uint iIndex)
{
	CRenderer* pRenderer = GET_UICOMPONENT(CRenderer);
	_int iCurIndex = pRenderer->Get_Pass();
	ImGui::Text("- Shader Pass -");

	ImGui::InputInt("Pass_Index", &iCurIndex);

	pRenderer->Set_Pass(iCurIndex);

}

void CWindow_UI::Show_Fader(_uint iIndex)
{
	if (ImGui::Button("Enable Fader"))
	{
		ENABLE_COMPONENT(GET_UICOMPONENT(CFader));
	}
	ImGui::SameLine();
	if (ImGui::Button("Disable Fader"))
	{
		DISABLE_COMPONENT(GET_UICOMPONENT(CFader));
	}


	FADEDESC& tFadeDesc = GET_UICOMPONENT(CFader)->Get_FadeDesc();

	if (ImGui::CollapsingHeader("- FadeIn Flag -"))
	{
		const static _uint iFlagCnt = 4;
		static _bool	bFlagSelect[iFlagCnt] = {};

		if (ImGui::Selectable("IN_FADE_TIME", &bFlagSelect[0]))
		{
			if (tFadeDesc.bFadeInFlag & FADE_TIME)
			{
				bFlagSelect[0] = true;
				tFadeDesc.bFadeInFlag &= ~FADE_TIME;
			}
			else
			{
				bFlagSelect[0] = false;
				tFadeDesc.bFadeInFlag |= FADE_TIME;
			}

		}

		if (ImGui::Selectable("IN_FADE_KEY", &bFlagSelect[1]))
		{
			if (tFadeDesc.bFadeInFlag & FADE_KEY)
			{
				tFadeDesc.bFadeInFlag &= ~FADE_KEY;
			}
			else
			{
				tFadeDesc.bFadeInFlag |= FADE_KEY;
			}
		}

		if (ImGui::Selectable("IN_FADE_COL", &bFlagSelect[2]))
		{
			if (tFadeDesc.bFadeInFlag & FADE_COL)
			{
				tFadeDesc.bFadeInFlag &= ~FADE_COL;
			}
			else
			{
				tFadeDesc.bFadeInFlag |= FADE_COL;
			}
		}

		if (ImGui::Selectable("IN_FADE_TEMP", &bFlagSelect[3]))
		{
			if (tFadeDesc.bFadeInFlag & FADE_TEMP)
			{
				tFadeDesc.bFadeInFlag &= ~FADE_TEMP;
			}
			else
			{
				tFadeDesc.bFadeInFlag |= FADE_TEMP;
			}
		}

		memset(bFlagSelect, 0, sizeof(_bool) * 4);

		if (tFadeDesc.bFadeInFlag & FADE_TIME)
			bFlagSelect[0] = true;
		if (tFadeDesc.bFadeInFlag & FADE_KEY)
			bFlagSelect[1] = true;
		if (tFadeDesc.bFadeInFlag & FADE_COL)
			bFlagSelect[2] = true;
		if (tFadeDesc.bFadeInFlag & FADE_TEMP)
			bFlagSelect[3] = true;

	}

	if (ImGui::CollapsingHeader("- FadeOut Flag -"))
	{
		const static _uint iFlagCnt = 4;
		static _bool	bFlagSelect[iFlagCnt] = {};

		if (ImGui::Selectable("FADE_TIME", &bFlagSelect[0]))
		{
			if (tFadeDesc.bFadeOutFlag & FADE_TIME)
			{
				bFlagSelect[0] = true;
				tFadeDesc.bFadeOutFlag &= ~FADE_TIME;
			}
			else
			{
				bFlagSelect[0] = false;
				tFadeDesc.bFadeOutFlag |= FADE_TIME;
			}

		}

		if (ImGui::Selectable("FADE_KEY", &bFlagSelect[1]))
		{
			if (tFadeDesc.bFadeOutFlag & FADE_KEY)
			{
				tFadeDesc.bFadeOutFlag &= ~FADE_KEY;
			}
			else
			{
				tFadeDesc.bFadeOutFlag |= FADE_KEY;
			}
		}

		if (ImGui::Selectable("FADE_COL", &bFlagSelect[2]))
		{
			if (tFadeDesc.bFadeOutFlag & FADE_COL)
			{
				tFadeDesc.bFadeOutFlag &= ~FADE_COL;
			}
			else
			{
				tFadeDesc.bFadeOutFlag |= FADE_COL;
			}
		}

		if (ImGui::Selectable("FADE_TEMP", &bFlagSelect[3]))
		{
			if (tFadeDesc.bFadeOutFlag & FADE_TEMP)
			{
				tFadeDesc.bFadeOutFlag &= ~FADE_TEMP;
			}
			else
			{
				tFadeDesc.bFadeOutFlag |= FADE_TEMP;
			}
		}

		memset(bFlagSelect, 0, sizeof(_bool) * 4);

		if (tFadeDesc.bFadeOutFlag & FADE_TIME)
			bFlagSelect[0] = true;
		if (tFadeDesc.bFadeOutFlag & FADE_KEY)
			bFlagSelect[1] = true;
		if (tFadeDesc.bFadeOutFlag & FADE_COL)
			bFlagSelect[2] = true;
		if (tFadeDesc.bFadeOutFlag & FADE_TEMP)
			bFlagSelect[3] = true;

	}

	if (ImGui::CollapsingHeader("- Fade Key -"))
	{
		static _bool	bKeyInputStart = false;

		if (ImGui::Button("Input_Key"))
		{
			bKeyInputStart = true;
		}

		ImGui::SameLine();

		ImGui::Text("Current Key : %d", (_uint)tFadeDesc.eKeyType);

		if (bKeyInputStart)
		{
			_uint iIdx = Check_KeyInput();

			if (iIdx < (_uint)KEY::LAST)
			{
				bKeyInputStart = false;
				tFadeDesc.eKeyType = (KEY)iIdx;
			}
		}
	}

	if (ImGui::CollapsingHeader("- FadeOut Type -"))
	{
		static _bool	bSelect[FADEDESC::FADEOUT_END] = {};

		if (ImGui::Selectable("DELETE", &bSelect[FADEDESC::FADEOUT_DELETE]))
		{
			tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DELETE;
		}
		if (ImGui::Selectable("DISABLE", &bSelect[FADEDESC::FADEOUT_DISABLE]))
		{
			tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
		}
		if (ImGui::Selectable("NEXT_TEXTURE", &bSelect[FADEDESC::FADEOUT_NEXTTEXTURE]))
		{
			tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_NEXTTEXTURE;
		}
		if (ImGui::Selectable("RANDOM_TEXTURE", &bSelect[FADEDESC::FADEOUT_RANDOMTEXTURE]))
		{
			tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_RANDOMTEXTURE;
		}
		if (ImGui::Selectable("NONE", &bSelect[FADEDESC::FADEOUT_NONE]))
		{
			tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_NONE;
		}

		memset(bSelect, 0, sizeof(_bool)* FADEDESC::FADEOUT_END);
		bSelect[tFadeDesc.eFadeOutType] = true;

	}
	if (ImGui::CollapsingHeader("- Fade Style -"))
	{
		static _bool	bSelect[FADEDESC::FADE_STYLE_END] = {};

		if (ImGui::Selectable("Style_Default", &bSelect[FADEDESC::FADE_STYLE_DEFAULT]))
		{
			tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;
		}
		if (ImGui::Selectable("Style_Move", &bSelect[FADEDESC::FADE_STYLE_MOVE]))
		{
			tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_MOVE;
		}
		if (ImGui::Selectable("Style_ScaleUp", &bSelect[FADEDESC::FADE_STYLE_SCALEUP]))
		{
			tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_SCALEUP;
		}
		if (ImGui::Selectable("Style_ScaleDown", &bSelect[FADEDESC::FADE_STYLE_SCALEDOWN]))
		{
			tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_SCALEDOWN;
		}

		memset(bSelect, 0, sizeof(_bool) * FADEDESC::FADE_STYLE_END);
		bSelect[tFadeDesc.eFadeStyle] = true;

	}
	if (ImGui::CollapsingHeader("- Variables -"))
	{
		ImGui::SliderFloat("Alpha", &tFadeDesc.fAlpha, 0.f, 1.f);

		ImGui::InputFloat("FadeIn Time", &tFadeDesc.fFadeInTime);
		ImGui::InputFloat("FadeInStart Time", &tFadeDesc.fFadeInStartTime);

		ImGui::InputFloat("FadeOut Time", &tFadeDesc.fFadeOutTime);
		ImGui::InputFloat("FadeOutStart Time", &tFadeDesc.fFadeOutStartTime);
	}






}

_uint CWindow_UI::Check_KeyInput()
{
	_uint iReturnValue = (_uint)KEY::LAST;

	vector<CKey_Manager::tKeyInfo>& vecKeyList = CGameInstance::Get_Instance()->Get_KeyList();

	for (_uint i = 0; i < (_uint)KEY::LAST; ++i)
	{
		if (vecKeyList[i].eState == KEY_STATE::TAP)
		{
			iReturnValue = i;
			break;
		}
	}

	return iReturnValue;
}

void CWindow_UI::Read_Folder(const char* pFolderPath)
{
	for (filesystem::directory_iterator FileIter(pFolderPath);
		FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		_int iFind = (_int)strFullPath.rfind("\\") + 1;
		string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

		if (entry.is_directory())
		{
			if (ImGui::TreeNode(strFileName.c_str()))
			{
				Read_Folder(strFullPath.c_str());
				ImGui::TreePop();
			}
		}
		else
		{
			_bool bSelected = false;

			if (m_CurSelectedTextureFilePath == wstrPath)
			{
				bSelected = true;
			}

			if (ImGui::Selectable(strFileName.c_str(), bSelected))
			{
				m_CurSelectedTextureFilePath = wstrPath;

			}
		}
	}
}
