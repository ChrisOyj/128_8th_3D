#include "stdafx.h"
#include "CWindow_UI.h"

#include "GameInstance.h"

#include "CUI.h"

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
	SetUp_ImGuiDESC(typeid(CWindow_UI).name(), ImVec2(350.f, 600.f), window_flags);


	return S_OK;
}

void CWindow_UI::Tick()
{
}

HRESULT CWindow_UI::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	if (ImGui::BeginTabBar("GROUP"))
	{
		if (ImGui::BeginTabItem("LEVEL"))
		{
			Show_LevelTab();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("UI"))
		{
			Show_UITab();

			ImGui::EndTabItem();
		}


		ImGui::EndTabBar();
	}



	__super::End();

	return S_OK;
}

HRESULT CWindow_UI::Save_Level()
{
	json LevelJson;
	vector<_uint> vecGameObjectID;
	vector<_uint> vecGroupType;

	_uint SaveLevelID = m_vecLevel[m_iCurrentLevelIdx].iLevelID;

	if (SUCCEEDED(CUtility_Json::Load_Json(CUtility_Json::Complete_Path(SaveLevelID).c_str(), &LevelJson)))
	{
		for (_uint i = 0; i < LevelJson["GameObjects"].size(); ++i)
		{
			vecGameObjectID.push_back(LevelJson["GameObjects"][i]);
			vecGroupType.push_back(GROUP_UI);
			Save_UI(i);
		}

		for (_uint i = LevelJson["GameObjects"].size(); i < m_vecUI.size(); ++i)
		{
			vecGameObjectID.push_back(m_vecUI[i].iGameObjectID);
			vecGroupType.push_back(GROUP_UI);
			Save_UI(i);
		}
	}
	else
	{
		for (_uint i = 0; i < m_vecUI.size(); ++i)
		{
			vecGameObjectID.push_back(m_vecUI[i].iGameObjectID);
			vecGroupType.push_back(GROUP_UI);
			Save_UI(i);
		}
	}

	

	json SaveJson;
	SaveJson.emplace("GameObjects", vecGameObjectID);
	SaveJson.emplace("GroupType", vecGroupType);

	if (FAILED(CUtility_Json::Save_Json(CUtility_Json::Complete_Path(SaveLevelID).c_str(), SaveJson)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWindow_UI::Save_UI(_uint iIndex)
{
	json	UIjson;
	CGameObject* pUI = m_vecUI[iIndex].pUI;
	vector<_uint>	vecComponents;
	list<CComponent*>& ComponentList = pUI->Get_ComponentsList();
	TRANSFORM& tTransform = pUI->Get_Transform()->Get_Transform();

	for (auto& pComponent : ComponentList)
	{
		if (pComponent == pUI->Get_Transform())
			continue;

		if (SUCCEEDED(CComponent_Factory::Save_Json(g_iCurComponentID, pComponent)))
		{
			vecComponents.push_back(g_iCurComponentID++);
		}
		else
		{
			vecComponents.push_back(pComponent->m_iSaveID);
		}
	}

	UIjson.emplace("GameObject_Type", OBJ_UI);
	UIjson.emplace("Component_List", vecComponents);
	UIjson.emplace("WorldMatrix", tTransform.matMyWorld.m);
	UIjson.emplace("vScale", tTransform.vScale.XMLoad().m128_f32);

	if (FAILED(CUtility_Json::Save_Json(CUtility_Json::Complete_Path(m_vecUI[iIndex].iGameObjectID).c_str(), UIjson)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWindow_UI::Load_Levels()
{
	m_vecLevel.clear();

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

		_uint iFileNum = stoi(strFileName);

		if (Safe_CheckID(iFileNum, ID_LEVEL))
		{
			LEVEL_ITEM tItem;
			tItem.iLevelID = iFileNum;
			m_vecLevel.push_back(tItem);
		}
	}


	return S_OK;
}

HRESULT CWindow_UI::Load_UI()
{
	json LevelJson;
	if (FAILED(CUtility_Json::Load_Json(CUtility_Json::Complete_Path(m_vecLevel[m_iCurrentLevelIdx].iLevelID).c_str(), &LevelJson)))
		return S_OK;

	for (_uint i = 0; i < LevelJson["GameObjects"].size(); ++i)
	{
		if (!Safe_CheckID(LevelJson["GameObjects"][i], ID_UI))
			break;

		CGameObject* pGameObject = CGameObject_Factory::Create_FromJson(LevelJson["GameObjects"][i]);

		if (!pGameObject)
			return E_FAIL;

		CREATE_GAMEOBJECT(pGameObject, GROUP_UI);
		//g_iCurUIID = LevelJson["GameObjects"][i] + 1;
		//g_iCurComponentID = pGameObject->Get_ComponentsList().back()->m_iSaveID + 1;

		UI_ITEM	tItem;
		tItem.bOrtho = false;
		tItem.bSelected = false;
		tItem.iGameObjectID = LevelJson["GameObjects"][i];
		tItem.pUI = pGameObject;

		m_vecUI.push_back(tItem);
	}

	return S_OK;
}

void CWindow_UI::Show_LevelTab()
{
	if (ImGui::Button("Add_Level"))
	{
		LEVEL_ITEM	tItem;
		tItem.bSelected = false;
		tItem.iLevelID = g_iCurLevelID++;
		m_vecLevel.push_back(tItem);
	}

	if (ImGui::BeginListBox("Level_List", ImVec2(0, 200.f)))
	{
		for (_uint i = 0; i < m_vecLevel.size(); ++i)
		{

			if (ImGui::Selectable(to_string(m_vecLevel[i].iLevelID).c_str(), m_vecLevel[i].bSelected))
			{
				for (_uint j = 0; j < m_vecLevel.size(); ++j)
					m_vecLevel[j].bSelected = false;

				m_iCurrentLevelIdx = i;
				m_iCurrentIdx = 9999;
				m_vecLevel[i].bSelected = true;

				/* Select Event */
				for (auto& elem : CGameInstance::Get_Instance()->Get_ObjGroup(GROUP_UI))
					DELETE_GAMEOBJECT(elem);


				m_vecUI.clear();

				if (FAILED(Load_UI()))
				{
					Call_MsgBox(L"Failed to Load_UI : CWindow_UI");
					return;
				}
			}

			if (m_vecLevel[i].bSelected)
			{
				ImGui::SetItemDefaultFocus();
			}

		}

		ImGui::EndListBox();
	}

	if (ImGui::Button("SAVE_LEVEL", ImVec2(120.f, 20.f)))
	{
		if (FAILED(Save_Level()))
		{
			Call_MsgBox(L"Failed to Save_Level");
		}
		else
		{
			Call_MsgBox(L"Succeeded to Save_Level");
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("LOAD_ALL_LEVELS", ImVec2(150.f, 20.f)))
	{
		if (FAILED(Load_Levels()))
		{
			Call_MsgBox(L"Failed to Load_Levels");
		}
		else
		{
			Call_MsgBox(L"Succeeded to Load_Levels");
		}
	}
}

void CWindow_UI::Show_UITab()
{
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

void CWindow_UI::Create_Ortho()
{
	CUI* pUIPrototype = CUI::Create(g_iCurUIID, _float4(g_iWinCX * 0.5f, g_iWinCY * 0.5f, 0.f));
	CGameInstance::Get_Instance()->Add_GameObject_Prototype(g_iCurUIID, pUIPrototype);

	CUI* pUI = static_cast<CUI*>(CGameObject_Factory::Create_FromPrototype(g_iCurUIID));
	CREATE_GAMEOBJECT(pUI, GROUP_UI);

	UI_ITEM	tItem;
	tItem.bSelected = false;
	tItem.iGameObjectID = g_iCurUIID++;
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
	CPhysics* pPhysics = m_vecUI[iIndex].pUI->Get_Component<CPhysics>()[0];
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
	CTexture* pTexture = m_vecUI[iIndex].pUI->Get_Component<CTexture>()[0];
	_int iCurIdx = pTexture->Get_CurTextureIndex();
	_int iTextureSize = pTexture->Get_TextureSize();
	ImGui::Text("Current Texture Index");
	ImGui::InputInt("TextureIdx", &iCurIdx);
	if (iCurIdx >= iTextureSize)
		iCurIdx = iTextureSize - 1;
	pTexture->Set_CurTextureIndex(iCurIdx);

	if (ImGui::CollapsingHeader("- Textures List -"))
	{
		if (ImGui::Button("Add Texture") || ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (FAILED(pTexture->Add_Texture(m_CurSelectedTextureFilePath.c_str())))
			{
				Call_MsgBox(L"Failed to Add_Texture : CWindow_UI");
				return;
			}

			pTexture->Set_CurTextureIndex(pTexture->Get_TextureSize() - 1);
		}

		ImGui::SameLine();

		if (ImGui::Button("Pop Texture"))
		{
			pTexture->Pop_Texture();
		}
		if (ImGui::BeginListBox("Textures List"))
		{
			Read_Folder("../bin/resources/textures");

			ImGui::EndListBox();
		}
	}

	ImGui::Text("Current Texture Size : %d", pTexture->Get_TextureSize());

	if (ImGui::BeginListBox("Textures List"))
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
	CRenderer* pRenderer = m_vecUI[iIndex].pUI->Get_Component<CRenderer>()[0];
	_int iCurIndex = pRenderer->Get_Pass();
	ImGui::Text("- Shader Pass -");

	ImGui::InputInt("Pass_Index", &iCurIndex);

	pRenderer->Set_Pass(iCurIndex);

}

void CWindow_UI::Show_Fader(_uint iIndex)
{
	if (ImGui::Button("Enable Fader"))
	{
		ENABLE_COMPONENT(m_vecUI[iIndex].pUI->Get_Component<CFader>()[0]);
	}
	ImGui::SameLine();
	if (ImGui::Button("Disable Fader"))
	{
		DISABLE_COMPONENT(m_vecUI[iIndex].pUI->Get_Component<CFader>()[0]);
	}


	FADEDESC& tFadeDesc = m_vecUI[iIndex].pUI->Get_Component<CFader>()[0]->Get_FadeDesc();

	if (ImGui::CollapsingHeader("- FadeOut Flag -"))
	{
		const static _uint iFlagCnt = 4;
		static _bool	bFlagSelect[iFlagCnt] = {};

		if (ImGui::Selectable("FADE_TIME", &bFlagSelect[0]))
		{
			if (tFadeDesc.bFadeFlag & FADE_TIME)
			{
				bFlagSelect[0] = true;
				tFadeDesc.bFadeFlag &= ~FADE_TIME;
			}
			else
			{
				bFlagSelect[0] = false;
				tFadeDesc.bFadeFlag |= FADE_TIME;
			}

		}

		if (ImGui::Selectable("FADE_KEY", &bFlagSelect[1]))
		{
			if (tFadeDesc.bFadeFlag & FADE_KEY)
			{
				tFadeDesc.bFadeFlag &= ~FADE_KEY;
			}
			else
			{
				tFadeDesc.bFadeFlag |= FADE_KEY;
			}
		}

		if (ImGui::Selectable("FADE_COL", &bFlagSelect[2]))
		{
			if (tFadeDesc.bFadeFlag & FADE_COL)
			{
				tFadeDesc.bFadeFlag &= ~FADE_COL;
			}
			else
			{
				tFadeDesc.bFadeFlag |= FADE_COL;
			}
		}

		if (ImGui::Selectable("FADE_TEMP", &bFlagSelect[3]))
		{
			if (tFadeDesc.bFadeFlag & FADE_TEMP)
			{
				tFadeDesc.bFadeFlag &= ~FADE_TEMP;
			}
			else
			{
				tFadeDesc.bFadeFlag |= FADE_TEMP;
			}
		}

		memset(bFlagSelect, 0, sizeof(_bool) * 4);

		if (tFadeDesc.bFadeFlag & FADE_TIME)
			bFlagSelect[0] = true;
		if (tFadeDesc.bFadeFlag & FADE_KEY)
			bFlagSelect[1] = true;
		if (tFadeDesc.bFadeFlag & FADE_COL)
			bFlagSelect[2] = true;
		if (tFadeDesc.bFadeFlag & FADE_TEMP)
			bFlagSelect[3] = true;

	}

	if (ImGui::CollapsingHeader("- FadeOut Key -"))
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

		memset(bSelect, 0, sizeof(_bool)* FADEDESC::FADEOUT_END);
		bSelect[tFadeDesc.eFadeOutType] = true;

	}

	if (ImGui::CollapsingHeader("- Variables -"))
	{
		ImGui::SliderFloat("Alpha", &tFadeDesc.fAlpha, 0.f, 1.f);

		ImGui::InputFloat("FadeIn Time", &tFadeDesc.fFadeInTime);

		ImGui::InputFloat("FadeOut Time", &tFadeDesc.fFadeOutTime);

		ImGui::InputFloat("FadeDelay Time", &tFadeDesc.fFadeOutStartTime);
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
