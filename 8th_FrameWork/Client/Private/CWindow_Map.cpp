#include "stdafx.h"
#include "CWindow_Map.h"

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
#include "CInstancingMap.h"

CWindow_Map::CWindow_Map()
{
}

CWindow_Map::~CWindow_Map()
{
}

CWindow_Map* CWindow_Map::Create()
{
	CWindow_Map* pInstance = new CWindow_Map;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Map");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_Map::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	m_RootTreeNode.strFolderPath = "../bin/resources";
	m_RootTreeNode.strFileName = "meshes";
	m_RootTreeNode.strFullPath = "../bin/resources/meshes";

	Read_Folder("../bin/resources/meshes", m_RootTreeNode);


	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_Map).name(), ImVec2(400.f, 630.f), window_flags);

	return S_OK;
}

void CWindow_Map::Tick()
{
	if (m_bLight)
	{
		if (KEY(C, TAP))
		{
			if (m_vecCurMeshes.empty())
			{
				list<CGameObject*>& DecoList = GAMEINSTANCE->Get_ObjGroup(GROUP_DECORATION);
				for (auto& elem : DecoList)
				{
					MESH_ITEM	tItem;
					tItem.bSelected = false;
					tItem.pGameObject = elem;
					tItem.eClassType = TOOL_MAP_DEFAULT;
					tItem.wstrFilePath = GET_COMPONENT_FROM(elem, CModel)->Get_ModelFilePath();

					_int iFind = (_int)tItem.wstrFilePath.rfind(L"/") + 1;
					wstring strFileName = tItem.wstrFilePath.substr(iFind, tItem.wstrFilePath.length() - iFind);

					tItem.strName = CFunctor::To_String(strFileName);
					m_vecCurMeshes.push_back(tItem);
				}
			}



			CGameInstance::Get_Instance()->Compute_WorldRay();
			_float4 vViewPos = CGameInstance::Get_Instance()->Get_ViewPos();
			_float4 vPickedPos, vPickedNormal;
			_float fDist, fMin = 9999.f;
			_float4 vFinalPickedPos, vFinalPickedNormal;

			for (auto& elem : m_vecCurMeshes)
			{
				vector<pair<_uint, CMeshContainer*>>& vecMeshContainers = GET_COMPONENT_FROM(elem.pGameObject, CModel)->Get_MeshContainers();

				for (auto& pMesh : vecMeshContainers)
				{
					if (CGameInstance::Get_Instance()->Is_Picked(pMesh.second, &vPickedPos, &vPickedNormal))
					{
						fDist = (vPickedPos - vViewPos).Length();
						if (fMin > fDist)
						{
							fMin = fDist;
							vFinalPickedPos = vPickedPos;
							vFinalPickedNormal = vPickedNormal;
						}
					}
				}
			}


			if (fMin != 9999.f)
			{
				m_tCurLightDesc.vPosition = vFinalPickedPos;
			}
		}

		if (KEY(V, TAP))
		{
			m_vecLights.push_back(m_tCurLightDesc);
			GAMEINSTANCE->Add_Light(m_tCurLightDesc);
		}

		return;
	}










	if (m_bInstancing)
	{
		m_fInstancingTimeAcc += fDT;
	}

	if (m_bInstancing && KEY(V, HOLD) && m_fInstancingTimeAcc >= 0.1f)
	{
		m_fInstancingTimeAcc = 0.f;

		if (m_vecSelectedMeshFilePath.empty())
			return;

		// ============= Tab =============

		CGameInstance::Get_Instance()->Compute_WorldRay();
		_float4 vViewPos = CGameInstance::Get_Instance()->Get_ViewPos();
		_float4 vPickedPos, vPickedNormal;
		_float fDist, fMin = 9999.f;
		_float4 vFinalPickedPos, vFinalPickedNormal;

		for (auto& elem : m_vecCurMeshes)
		{
			vector<pair<_uint, CMeshContainer*>>& vecMeshContainers = GET_COMPONENT_FROM(elem.pGameObject, CModel)->Get_MeshContainers();

			for (auto& pMesh : vecMeshContainers)
			{
				if (CGameInstance::Get_Instance()->Is_Picked(pMesh.second, &vPickedPos, &vPickedNormal))
				{
					fDist = (vPickedPos - vViewPos).Length();
					if (fMin > fDist)
					{
						fMin = fDist;
						vFinalPickedPos = vPickedPos;
						vFinalPickedNormal = vPickedNormal;
					}
				}
			}
		}


		if (fMin == 9999.f)
		{
			return;
		}

		// ============= C ===============


		_float4 vPos = vFinalPickedPos;
		_float4 vLook;
		vLook.x = frandom(-1.f, 1.f);
		vLook.y = 0.f;
		vLook.z = frandom(-1.f, 1.f);
		vLook.w = 0.f;
		vLook.Normalize();



		Create_Selected_Meshes(TOOL_MAP_DEFAULT, CFunctor::To_Wstring(m_CurSelectedMeshFilePath));

		m_vecCurMeshes.back().pGameObject->Get_Transform()->Set_World(WORLD_POS, vPos);
		m_vecCurMeshes.back().pGameObject->Get_Transform()->Set_Look(vLook);

		if (m_bInstancing)
		{
			CTransform* pTransform = m_vecCurMeshes.back().pGameObject->Get_Transform();

			VTXINSTANCE	tVtxInstance;
			tVtxInstance.vRight = pTransform->Get_MyWorld(WORLD_RIGHT);
			tVtxInstance.vUp = pTransform->Get_MyWorld(WORLD_UP);
			tVtxInstance.vLook = pTransform->Get_MyWorld(WORLD_LOOK);
			tVtxInstance.vTranslation = pTransform->Get_MyWorld(WORLD_POS);

			m_vecVtxInstance.push_back(tVtxInstance);
		}
		return;
	}





	if (m_pSampleObject == nullptr)
		return;

	if (KEY(LBUTTON, TAP))
	{
		CGameInstance::Get_Instance()->Compute_WorldRay();
		_float4 vViewPos = CGameInstance::Get_Instance()->Get_ViewPos();
		_float4 vPickedPos, vPickedNormal;
		_float fDist, fMin = 9999.f;
		_float4 vFinalPickedPos, vFinalPickedNormal;

		for (auto& elem : m_vecCurMeshes)
		{
			vector<pair<_uint, CMeshContainer*>>& vecMeshContainers = GET_COMPONENT_FROM(elem.pGameObject, CModel)->Get_MeshContainers();

			for (auto& pMesh : vecMeshContainers)
			{
				if (CGameInstance::Get_Instance()->Is_Picked(pMesh.second, &vPickedPos, &vPickedNormal))
				{
					fDist = (vPickedPos - vViewPos).Length();
					if (fMin > fDist)
					{
						fMin = fDist;
						vFinalPickedPos = vPickedPos;
						vFinalPickedNormal = vPickedNormal;
					}
				}
			}
		}


		if (fMin != 9999.f)
		{
			//m_pSampleObject->Get_Transform()->Set_World(WORLD_POS, vFinalPickedPos);
			m_pSampleObject->Set_OriginPos(vFinalPickedPos);
		}
	}
	

	if (KEY(C, TAP))
	{
		_float4 vPos = m_pSampleObject->Get_Transform()->Get_World(WORLD_POS);
		_float4 vLook = m_pSampleObject->Get_Transform()->Get_World(WORLD_LOOK);

		DELETE_GAMEOBJECT(m_pSampleObject);
		m_pSampleObject = nullptr;
		Create_Selected_Meshes(TOOL_MAP_DEFAULT, CFunctor::To_Wstring(m_CurSelectedMeshFilePath));

		m_vecCurMeshes.back().pGameObject->Get_Transform()->Set_World(WORLD_POS, vPos);
		m_vecCurMeshes.back().pGameObject->Get_Transform()->Set_Look(vLook);

		if (m_bInstancing)
		{
			CTransform* pTransform = m_vecCurMeshes.back().pGameObject->Get_Transform();

			VTXINSTANCE	tVtxInstance;
			tVtxInstance.vRight = pTransform->Get_MyWorld(WORLD_RIGHT);
			tVtxInstance.vUp = pTransform->Get_MyWorld(WORLD_UP);
			tVtxInstance.vLook = pTransform->Get_MyWorld(WORLD_LOOK);
			tVtxInstance.vTranslation = pTransform->Get_MyWorld(WORLD_POS);

			m_vecVtxInstance.push_back(tVtxInstance);
		}
		return;
	}

	_float4 vUp = { 0.f, 1.f, 0.f };
	_float fAngle = 180.f * fDT;
	CTransform* pTransform = m_pSampleObject->Get_Transform();
	if (KEY(Q, HOLD))
	{
		CUtility_Transform::Turn_ByAngle(pTransform, vUp, fAngle);
	}
	else if (KEY(E, HOLD))
	{
		CUtility_Transform::Turn_ByAngle(pTransform, vUp, -fAngle);
	}

	if (KEY(R, TAP))
	{
		CUtility_Transform::Turn_ByAngle(pTransform, vUp, 0.05f);
	}
	else if (KEY(T, TAP))
	{
		CUtility_Transform::Turn_ByAngle(pTransform, vUp, -0.05f);
	}
	
	_float4 vCurPos = m_pSampleObject->Get_OffsetPos();
	_float fSpeed = 50.f * fDT;
	CTransform* pCamTransform = CGameInstance::Get_Instance()->Get_CurCam()->Get_Transform();
	if (KEY(I, HOLD))
	{
		_float4 vDir = pCamTransform->Get_World(WORLD_LOOK);
		vDir.y = 0.f;
		vCurPos += vDir.Normalize() * fSpeed;
	}
	if (KEY(K, HOLD))
	{
		_float4 vDir = pCamTransform->Get_World(WORLD_LOOK);
		vDir.y = 0.f;
		vCurPos -= vDir.Normalize() * fSpeed;
	}
	if (KEY(J, HOLD))
	{
		_float4 vDir = pCamTransform->Get_World(WORLD_RIGHT);
		vDir.y = 0.f;
		vCurPos -= vDir.Normalize() * fSpeed;
	}
	if (KEY(L, HOLD))
	{
		_float4 vDir = pCamTransform->Get_World(WORLD_RIGHT);
		vDir.y = 0.f;
		vCurPos += vDir.Normalize() * fSpeed;
	}

	if (KEY(U, HOLD))
	{
		_float4 vDir = _float4(0.f, 1.f, 0.f, 0.f);
		vCurPos += vDir * fSpeed;
	}
	else if (KEY(O, HOLD))
	{
		_float4 vDir = _float4(0.f, 1.f, 0.f, 0.f);
		vCurPos -= vDir * fSpeed;
	}
	vCurPos.w = 1.f;
	m_pSampleObject->Set_OffsetPos(vCurPos);
	

}

HRESULT CWindow_Map::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	if (KEY(DELETEKEY, TAP))
	{
		if (!m_vecCurMeshes.empty())
		{
			DELETE_GAMEOBJECT(m_vecCurMeshes.back().pGameObject);
			m_vecCurMeshes.pop_back();
			m_iCurrentIdx = 9999;
		}
	}

	if (ImGui::BeginTabBar("Tab"))
	{
		if (ImGui::BeginTabItem("Map_Setting"))
		{
			Show_MapList();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Map_Loader"))
		{
			Show_MapLoader();

			ImGui::EndTabItem();
		}


		ImGui::EndTabBar();
	}


	__super::End();

	return S_OK;
}

void CWindow_Map::Show_MapList()
{
	if (ImGui::Button("ENABLE"))
	{
		if (m_iCurrentIdx == 9999)
			return;

		ENABLE_GAMEOBJECT(m_vecCurMeshes[m_iCurrentIdx].pGameObject);
	}

	ImGui::SameLine();

	if (ImGui::Button("DISABLE"))
	{
		if (m_iCurrentIdx == 9999)
			return;

		DISABLE_GAMEOBJECT(m_vecCurMeshes[m_iCurrentIdx].pGameObject);
	}

	ImGui::SameLine();

	if (ImGui::Button("DELETE"))
	{
		if (m_iCurrentIdx == 9999)
			return;

		DELETE_GAMEOBJECT(m_vecCurMeshes[m_iCurrentIdx].pGameObject);

		auto iter = m_vecCurMeshes.begin();

		for (_uint i = 0; i < m_iCurrentIdx; ++i)
		{
			++iter;
		}

		iter = m_vecCurMeshes.erase(iter);
	}

	

	if (ImGui::Button("ENABLE_ALL"))
	{
		list<CGameObject*>& objList = CGameInstance::Get_Instance()->Get_ObjGroup(GROUP_DECORATION);
		for (auto& elem : objList)
			ENABLE_GAMEOBJECT(elem);
	}

	ImGui::SameLine();

	if (ImGui::Button("DISABLE_ALL"))
	{
		list<CGameObject*>& objList = CGameInstance::Get_Instance()->Get_ObjGroup(GROUP_DECORATION);
		for (auto& elem : objList)
			DISABLE_GAMEOBJECT(elem);
	}

	ImGui::SameLine();

	if (ImGui::Button("DELETE_ALL"))
	{
		m_iCurrentIdx = 9999;
		list<CGameObject*>& objList = CGameInstance::Get_Instance()->Get_ObjGroup(GROUP_DECORATION);
		for (auto& elem : objList)
			DELETE_GAMEOBJECT(elem);

		m_vecCurMeshes.clear();
	}

	if (ImGui::InputText("MAP_NAME", m_szMapName, sizeof(m_szMapName), ImGuiInputTextFlags_EnterReturnsTrue))
	{
	}

	if (ImGui::Button("SAVE_MAP"))
	{
		Save_CurMap();
	}


	if (ImGui::InputText("INSTANCING_NAME", m_szInstancingName, sizeof(m_szInstancingName), ImGuiInputTextFlags_EnterReturnsTrue))
	{
	}

	if (ImGui::Button("SAVE_INSTANCING_DATA"))
	{
		Save_InstancingData();


	}
	ImGui::SameLine();

	if (ImGui::Button("LOAD_INSTANCING_DATA"))
	{
		Load_InstancingData();


	}

	if (ImGui::BeginListBox("Meshes_List", ImVec2(400.f, 350.f)))
	{
		for (_uint i = 0; i < m_vecCurMeshes.size(); ++i)
		{
			if (ImGui::Selectable(m_vecCurMeshes[i].strName.c_str(), m_vecCurMeshes[i].bSelected))
			{
				for (_uint j = 0; j < m_vecCurMeshes.size(); ++j)
					m_vecCurMeshes[j].bSelected = false;

				m_iCurrentIdx = i;
				m_vecCurMeshes[i].bSelected = true;
			}

			if (m_vecCurMeshes[i].bSelected)
			{
				ImGui::SetItemDefaultFocus();
			}

			if (m_vecCurMeshes[i].pGameObject->Is_Disable())
			{
				ImGui::SameLine(0.f, 70.f);
				ImGui::Text("disable");
			}
		}


		ImGui::EndListBox();
	}

	if (ImGui::Button("CCW"))
	{
		Change_Passes(VTXMODEL_PASS_DEFAULT);
	}

	ImGui::SameLine();

	if (ImGui::Button("CW"))
	{
		Change_Passes(VTXMODEL_PASS_CW);
	}

	ImGui::SameLine();

	if (ImGui::Button("NONE"))
	{
		Change_Passes(VTXMODEL_PASS_NONE);
	}

}

void CWindow_Map::Show_MapLoader()
{
	if (ImGui::CollapsingHeader("JSON_LOADER"))
	{
		if (ImGui::InputText("MAP_NAME", m_szMapName, sizeof(m_szMapName), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			json MapJson;
			string strFileName = m_szMapName;
			wstring wstrPath = CUtility_Json::Complete_Path(CFunctor::To_Wstring(strFileName));
			if (FAILED(CUtility_Json::Load_Json(wstrPath.c_str(), &MapJson)))
			{
				Call_MsgBox(L"Failed to Load_Json : CWindow_Map");
				return;
			}

			for (auto iter = MapJson.begin(); iter != MapJson.end(); iter++)
			{
				CGameObject* pGameObject = nullptr;

				json ObjJson = *iter;

				TOOL_MAP_TYPE eType = ObjJson["eClassType"];
				wstring wstrFilePath = ObjJson["wstrFilePath"];

				if (eType == TOOL_MAP_PROP)
				{
					m_bProp = true;
					eType = TOOL_MAP_DEFAULT;
				}
				else
					m_bProp = false;

				Create_Selected_Meshes(eType, wstrFilePath);

				m_vecCurMeshes.back().pGameObject->Get_Transform()->Get_Transform().matMyWorld =
					CUtility_Json::Get_MatrixFromJson(ObjJson["worldMatrix"]);
			}

		}
	}


	if (ImGui::CollapsingHeader("FBX_LOADER"))
	{
#pragma region RadioButton
		if (ImGui::RadioButton("Default", m_bCurMapType[TOOL_MAP_DEFAULT]))
		{
			for (_uint i = 0; i < TOOL_MAP_END; ++i)
			{
				m_bCurMapType[i] = false;
			}

			m_bCurMapType[TOOL_MAP_DEFAULT] = true;
		}

		ImGui::SameLine();

		if (ImGui::RadioButton("Ground", m_bCurMapType[TOOL_MAP_GROUND]))
		{
			for (_uint i = 0; i < TOOL_MAP_END; ++i)
			{
				m_bCurMapType[i] = false;
			}

			m_bCurMapType[TOOL_MAP_GROUND] = true;
		}

		ImGui::SameLine();

		if (ImGui::RadioButton("PREVIEW", m_bCurMapType[TOOL_MAP_PROP]))
		{
			for (_uint i = 0; i < TOOL_MAP_END; ++i)
			{
				m_bCurMapType[i] = false;
			}

			m_bCurMapType[TOOL_MAP_PROP] = true;
		}

		ImGui::SameLine();

		if (ImGui::RadioButton("Prop", m_bProp))
		{
			m_bProp = !m_bProp;
		}

		if (ImGui::RadioButton("Instancing", m_bInstancing))
		{
			m_bInstancing = !m_bInstancing;
		}

		ImGui::SameLine();

		if (ImGui::Button("Pop Instancing"))
		{
			if (!m_vecVtxInstance.empty())
			{
				m_vecVtxInstance.pop_back();

				if (!m_vecCurMeshes.empty())
				{
					DELETE_GAMEOBJECT(m_vecCurMeshes.back().pGameObject);
					m_vecCurMeshes.pop_back();
					m_iCurrentIdx = 9999;
				}
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Clear Instancing"))
		{
			m_vecVtxInstance.clear();
		}

		if (ImGui::RadioButton("Lights", m_bLight))
		{
			m_bLight = !m_bLight;
		}
		ImGui::SameLine();
		if (ImGui::Button("Save Lights"))
		{
			Save_Lights();
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear Lights"))
		{
			m_vecLights.clear();
			GAMEINSTANCE->Clear_Lights();

			LIGHTDESC			LightDesc;

			LightDesc.eType = tagLightDesc::TYPE_POINT;
			LightDesc.vPosition = _float4(200.f, 400.f, -200.f, 1.f);
			LightDesc.fRange = 1000.f;
			LightDesc.vDiffuse = _float4(0.2f, 0.2f, 0.4f, 1.f);
			LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.4f, 1.f);
			LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

			GAMEINSTANCE->Add_Light(LightDesc);
		}
		ImGui::SameLine();

		if (ImGui::Button("Pop Light"))
		{
			m_vecLights.pop_back();
			GAMEINSTANCE->Pop_Light();
		}
#pragma endregion

		if (ImGui::Button("Create_Selected_Mesh") || KEY(TAB, TAP))
		{
			if (m_vecSelectedMeshFilePath.empty())
				return;

			TOOL_MAP_TYPE eType = TOOL_MAP_END;

			for (_uint i = 0; i < TOOL_MAP_END; ++i)
			{
				if (m_bCurMapType[i])
				{
					eType = (TOOL_MAP_TYPE)i;
					break;
				}
			}

			for (auto& strPath : m_vecSelectedMeshFilePath)
			{
				Create_Selected_Meshes(eType, CFunctor::To_Wstring(strPath));
			}

		}

		if (m_bLight)
		{
			if (ImGui::InputFloat("fRange", &m_tCurLightDesc.fRange))
			{ }

			_float LightDiffuse[3] = { m_tCurLightDesc.vDiffuse .x, m_tCurLightDesc.vDiffuse.y, m_tCurLightDesc.vDiffuse.z};
			if (ImGui::InputFloat3("LightDiffuse", LightDiffuse))
			{
				m_tCurLightDesc.vDiffuse.x = LightDiffuse[0];
				m_tCurLightDesc.vDiffuse.y = LightDiffuse[1];
				m_tCurLightDesc.vDiffuse.z = LightDiffuse[2];
			}

		}



		if (ImGui::BeginListBox("FBX_Files_List", ImVec2(400.f, 450.f)))
		{
			Show_TreeData(m_RootTreeNode);

			ImGui::EndListBox();
		}
	}
}

void CWindow_Map::Read_Folder(const char* pFolderPath, TREE_DATA& tRootTree)
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

		TREE_DATA	tTreeData;
		tTreeData.strFullPath = strFullPath;
		tTreeData.strFileName = strFileName;
		tTreeData.strFolderPath = pFolderPath;
		if (entry.is_directory())
		{
			Read_Folder(strFullPath.c_str(), tTreeData);
		}
		else
		{
			_int iFindExt = (int)strFileName.rfind(".") + 1;
			string strExtName = strFileName.substr(iFindExt, strFileName.length() - iFindExt);

			if (strExtName == "dat")
				continue;
		}

		tRootTree.vecChildren.push_back(tTreeData);

	}
}

void CWindow_Map::Show_TreeData(TREE_DATA& tTree)
{
	if (!tTree.vecChildren.empty())
	{
		if (ImGui::TreeNode(tTree.strFileName.c_str()))
		{
			for (auto& tTreeData : tTree.vecChildren)
			{
				Show_TreeData(tTreeData);
			}

			ImGui::TreePop();
		}
		
	}
	else
	{
		_bool bSelected = false;

		if (m_CurSelectedMeshFilePath == tTree.strFullPath)
		{
			bSelected = true;
		}

		for (auto& strPath : m_vecSelectedMeshFilePath)
		{
			if (strPath == tTree.strFullPath)
				bSelected = true;
		}

		if (ImGui::Selectable(tTree.strFileName.c_str(), bSelected))
		{
			m_vecSelectedMeshFilePath.clear();

			string prevFilePath = m_CurSelectedMeshFilePath;
			m_CurSelectedMeshFilePath = tTree.strFullPath;

			if (KEY(LSHIFT, HOLD))
			{
				// 1. 새로운 이터레이터로 prevPath 위치로 가야함
				filesystem::directory_iterator shiftFileIter(tTree.strFolderPath);
				string strshiftFullPath;

				wstring wstrPath = shiftFileIter->path().relative_path();
				strshiftFullPath = CFunctor::To_String(wstrPath);

				while (strshiftFullPath != prevFilePath)
				{
					shiftFileIter++;
					wstrPath = shiftFileIter->path().relative_path();
					strshiftFullPath = CFunctor::To_String(wstrPath);
				}

				// 2. prevPath부터 curPath까지 모두 선택

				for (; strshiftFullPath != m_CurSelectedMeshFilePath; ++shiftFileIter)
				{
					if (shiftFileIter == filesystem::end(shiftFileIter))
						break;

					

					const filesystem::directory_entry& shiftentry = *shiftFileIter;

					wstring wstrshiftPath = shiftentry.path().relative_path();
					strshiftFullPath = strshiftFullPath.assign(wstrshiftPath.begin(), wstrshiftPath.end());

					_int iFindExt = (int)strshiftFullPath.rfind(".") + 1;
					string strExtName = strshiftFullPath.substr(iFindExt, strshiftFullPath.length() - iFindExt);

					if (strExtName == "dat")
						continue;

					m_vecSelectedMeshFilePath.push_back(strshiftFullPath);
				}

			}
			else
			{
				m_vecSelectedMeshFilePath.push_back(m_CurSelectedMeshFilePath);
			}
		}
	}
}
void CWindow_Map::Change_Passes(VTXMODEL_PASS_TYPE eType)
{
	for (auto& elemITEM : m_vecCurMeshes)
	{
		GET_COMPONENT_FROM(elemITEM.pGameObject, CRenderer)->Set_Pass(eType);
	}
}

void CWindow_Map::Create_Selected_Meshes(TOOL_MAP_TYPE eType, wstring pFilePath)
{
	CGameObject* pGameObject = nullptr;

	switch (eType)
	{
	case Client::TOOL_MAP_DEFAULT:
		pGameObject = CMap::Create(pFilePath);
		break;
	case Client::TOOL_MAP_GROUND:
		pGameObject = CGround::Create(pFilePath);
		break;
	case Client::TOOL_MAP_PROP:
		m_pSampleObject = CMap_Sample::Create(pFilePath);
		CREATE_GAMEOBJECT(m_pSampleObject, GROUP_DECORATION);
		return;
	case Client::TOOL_MAP_END:
		break;
	default:
		break;
	}

	MESH_ITEM	tItem;
	tItem.bSelected = false;
	tItem.pGameObject = pGameObject;
	tItem.eClassType = eType;
	if (m_bProp) tItem.eClassType = TOOL_MAP_PROP;
	tItem.wstrFilePath = pFilePath;

	_int iFind = (_int)pFilePath.rfind(L"\\") + 1;
	wstring strFileName = pFilePath.substr(iFind, pFilePath.length() - iFind);

	tItem.strName = CFunctor::To_String(strFileName);
	m_vecCurMeshes.push_back(tItem);
	CREATE_GAMEOBJECT(pGameObject, GROUP_DECORATION);
	
}

void CWindow_Map::Save_CurMap()
{
	json MapJson;
	for (auto& elem : m_vecCurMeshes)
	{
		json SaveJson;
		SaveJson.emplace("eClassType", elem.eClassType);
		SaveJson.emplace("wstrFilePath", elem.wstrFilePath);
		SaveJson.emplace("worldMatrix", elem.pGameObject->Get_Transform()->Get_WorldMatrix().m);
		MapJson.push_back(SaveJson);
	}
	string strName = m_szMapName;

	if (SUCCEEDED(CUtility_Json::Save_Json(CUtility_Json::Complete_Path(CFunctor::To_Wstring(strName)).c_str(), MapJson)))
		Call_MsgBox(L"Save 성공");
	else
		Call_MsgBox(L"!!!!!!!!!!!! Save 실패 !!!!!!!!!!");

}

void CWindow_Map::Save_InstancingData()
{
	string strFilePath = "../bin/Instancing/";
	strFilePath += m_szInstancingName;
	strFilePath += ".dat";

	ofstream	writeFile(strFilePath, ios::binary);

	if (!writeFile.is_open())
	{
		Call_MsgBox(L"Failed to wrtie File : CWindow_Map");
		return;
	}


	_uint iNumInstance = m_vecVtxInstance.size();
	writeFile.write((char*)&iNumInstance, sizeof(_uint));

	VTXINSTANCE* pInstance = new VTXINSTANCE[iNumInstance];

	for (_uint i = 0; i < iNumInstance; ++i)
	{
		pInstance[i] = m_vecVtxInstance[i];
	}

	writeFile.write((char*)pInstance, sizeof(VTXINSTANCE) * iNumInstance);

	writeFile.close();

	Call_MsgBox(L"Instancing Save 성공!");

	SAFE_DELETE_ARRAY(pInstance);

}

void CWindow_Map::Save_Lights()
{
	string strFilePath = "../bin/Lights/";
	strFilePath += m_szInstancingName;
	strFilePath += ".bin";

	ofstream	writeFile(strFilePath, ios::binary);

	if (!writeFile.is_open())
	{
		Call_MsgBox(L"Failed to wrtie File : CWindow_Map");
		return;
	}


	_uint iNumLights = m_vecLights.size();
	writeFile.write((char*)&iNumLights, sizeof(_uint));

	for (_uint i = 0; i < iNumLights; ++i)
	{
		writeFile.write((char*)&m_vecLights[i], sizeof(LIGHTDESC));
	}

	writeFile.close();

	Call_MsgBox(L"Instancing Save 성공!");

}

void CWindow_Map::Load_InstancingData()
{
	wstring wstrPath = CFunctor::To_Wstring(m_vecSelectedMeshFilePath.back());
	string strName = m_szInstancingName;

	CREATE_GAMEOBJECT(CInstancingMap::Create(wstrPath, CFunctor::To_Wstring(strName)), GROUP_DECORATION);
}
