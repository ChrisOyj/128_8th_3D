#include "stdafx.h"
#include "CWindow_Navi.h"

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
#include "CCell.h"

#include "CMap.h"
#include "CGround.h"
#include "CMap_Sample.h"
#include "CObject_Map.h"

#include "CUtility_Transform.h"
#include "Camera.h"

#include "CUnit.h"
#include "CScript_FollowCam.h"

#include "CTerrain.h"
#include "CMesh_Terrain.h"

CWindow_Navi::CWindow_Navi()
{
}

CWindow_Navi::~CWindow_Navi()
{
}

CWindow_Navi* CWindow_Navi::Create()
{
	CWindow_Navi* pInstance = new CWindow_Navi;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Navi");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_Navi::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_Navi).name(), ImVec2(300.f, 600.f), window_flags);

	return S_OK;
}

void CWindow_Navi::Tick()
{
	if (m_bSelectStartCellMode)
	{
		if (!(KEY(C, TAP)))
			return;

		_float4 vRayPos, vRayDir;
		_float4 vOutPos;

		Compute_WorldRay(&vRayPos, &vRayDir);

		if (Is_Picked(vRayPos.XMLoad(), vRayDir.XMLoad(), m_pTerrain->m_pTerrainMesh, &vOutPos))
		{
			//OutPos로 터레인에서 인덳스 찾깅
			_uint iSize = m_pTerrain->m_pTerrainMesh->Get_NumVertices();

			//찍은곳에서 0번째꺼 x를 빼야함
			_float fLengthX = vOutPos.x - m_pTerrain->m_pTerrainMesh->Get_VerticesPos()[0].x;
			_float fLengthZ = vOutPos.z - m_pTerrain->m_pTerrainMesh->Get_VerticesPos()[0].z;

			_uint iIndexX = fLengthX / m_tPickingData.fStepSizeX;
			_uint iIndexZ = fLengthZ / m_tPickingData.fStepSizeZ;

			m_iStartIndex = iIndexX * 2 + ((m_tPickingData.iNumVerticesX-1) * 2 * iIndexZ);
		}

	}


	if (m_bSelectBlockCellMode)
	{
		if (!(KEY(V, TAP)))
			return;

		_float4 vRayPos, vRayDir;
		_float4 vOutPos;

		Compute_WorldRay(&vRayPos, &vRayDir);

		if (Is_Picked(vRayPos.XMLoad(), vRayDir.XMLoad(), m_pTerrain->m_pTerrainMesh, &vOutPos))
		{
			//OutPos로 터레인에서 인덳스 찾깅
			_uint iSize = m_pTerrain->m_pTerrainMesh->Get_NumVertices();

			//찍은곳에서 0번째꺼 x를 빼야함
			_float fLengthX = vOutPos.x - m_pTerrain->m_pTerrainMesh->Get_VerticesPos()[0].x;
			_float fLengthZ = vOutPos.z - m_pTerrain->m_pTerrainMesh->Get_VerticesPos()[0].z;

			_uint iIndexX = fLengthX / m_tPickingData.fStepSizeX;
			_uint iIndexZ = fLengthZ / m_tPickingData.fStepSizeZ;

			_int iPickedIndex = iIndexX * 2 + ((m_tPickingData.iNumVerticesX - 1) * 2 * iIndexZ);


			_uint3 Indices = m_pTerrain->m_pTerrainMesh->Get_Indices(iPickedIndex);

			_float3* pVerticesPos = m_pTerrain->m_pTerrainMesh->Get_VerticesPos();
			_float4* pVerticesColor = m_pTerrain->m_pTerrainMesh->Get_VerticesColor();

			//if (pVerticesPos[Indices._1].y <= -1.f)
			//{
			//	//이미 Blocked면
			//	pVerticesPos[Indices._1].y = vOutPos.y;
			//	pVerticesPos[Indices._2].y = vOutPos.y;
			//	pVerticesPos[Indices._3].y = vOutPos.y;

			//	pVerticesColor[Indices._1] = _float4(0.f, 1.f, 0.f, 1.f);
			//	pVerticesColor[Indices._2] = _float4(0.f, 1.f, 0.f, 1.f);
			//	pVerticesColor[Indices._3] = _float4(0.f, 1.f, 0.f, 1.f);
			//}
			//else
			{
				pVerticesPos[Indices._1].y = -1.f;
				pVerticesPos[Indices._2].y = -1.f;
				pVerticesPos[Indices._3].y = -1.f;

				pVerticesColor[Indices._1] = _float4(1.f, 0.f, 0.f, 1.f);
				pVerticesColor[Indices._2] = _float4(1.f, 0.f, 0.f, 1.f);
				pVerticesColor[Indices._3] = _float4(1.f, 0.f, 0.f, 1.f);
			}

			m_pTerrain->m_pTerrainMesh->ReMap_Vertices();
		}

	}

	if (KEY(R, HOLD))
	{
		if (!m_bSelectCellGrassMode && !m_bSelectCellGroundMode)
			return;


		_float4 vRayPos, vRayDir;
		_float4 vOutPos;

		Compute_WorldRay(&vRayPos, &vRayDir);

		if (Is_Picked(vRayPos.XMLoad(), vRayDir.XMLoad(), m_pTerrain->m_pTerrainMesh, &vOutPos))
		{
			//OutPos로 터레인에서 인덳스 찾깅
			_uint iSize = m_pTerrain->m_pTerrainMesh->Get_NumVertices();

			//찍은곳에서 0번째꺼 x를 빼야함
			_float fLengthX = vOutPos.x - m_pTerrain->m_pTerrainMesh->Get_VerticesPos()[0].x;
			_float fLengthZ = vOutPos.z - m_pTerrain->m_pTerrainMesh->Get_VerticesPos()[0].z;

			_uint iIndexX = fLengthX / m_tPickingData.fStepSizeX;
			_uint iIndexZ = fLengthZ / m_tPickingData.fStepSizeZ;

			_int iPickedIndex = iIndexX * 2 + ((m_tPickingData.iNumVerticesX - 1) * 2 * iIndexZ);
			_int iPickedIndexUp = iPickedIndex + (m_tPickingData.iNumVerticesX - 1) * 2;
			_int iPickedIndexDown = iPickedIndex - (m_tPickingData.iNumVerticesX - 1) * 2;
			_int iPickedIndexRight = iPickedIndex + 1;
			_int iPickedIndexLeft = iPickedIndex - 1;


			_uint3 Indices = m_pTerrain->m_pTerrainMesh->Get_Indices(iPickedIndex);
			_uint3 Indices2 = m_pTerrain->m_pTerrainMesh->Get_Indices(iPickedIndexUp);
			_uint3 Indices3 = m_pTerrain->m_pTerrainMesh->Get_Indices(iPickedIndexDown);
			_uint3 Indices4 = m_pTerrain->m_pTerrainMesh->Get_Indices(iPickedIndexRight);
			_uint3 Indices5 = m_pTerrain->m_pTerrainMesh->Get_Indices(iPickedIndexLeft);

			_float3* pVerticesPos = m_pTerrain->m_pTerrainMesh->Get_VerticesPos();
			_float4* pVerticesColor = m_pTerrain->m_pTerrainMesh->Get_VerticesColor();


			if (m_bSelectCellGrassMode)
			{
				pVerticesColor[Indices._1] = _float4(0.f, 0.6f, 0.f, 1.f);
				pVerticesColor[Indices._2] = _float4(0.f, 0.6f, 0.f, 1.f);
				pVerticesColor[Indices._3] = _float4(0.f, 0.6f, 0.f, 1.f);

				pVerticesColor[Indices2._1] = _float4(0.f, 0.6f, 0.f, 1.f);
				pVerticesColor[Indices2._2] = _float4(0.f, 0.6f, 0.f, 1.f);
				pVerticesColor[Indices2._3] = _float4(0.f, 0.6f, 0.f, 1.f);

				pVerticesColor[Indices3._1] = _float4(0.f, 0.6f, 0.f, 1.f);
				pVerticesColor[Indices3._2] = _float4(0.f, 0.6f, 0.f, 1.f);
				pVerticesColor[Indices3._3] = _float4(0.f, 0.6f, 0.f, 1.f);

				pVerticesColor[Indices4._1] = _float4(0.f, 0.6f, 0.f, 1.f);
				pVerticesColor[Indices4._2] = _float4(0.f, 0.6f, 0.f, 1.f);
				pVerticesColor[Indices4._3] = _float4(0.f, 0.6f, 0.f, 1.f);

				pVerticesColor[Indices5._1] = _float4(0.f, 0.6f, 0.f, 1.f);
				pVerticesColor[Indices5._2] = _float4(0.f, 0.6f, 0.f, 1.f);
				pVerticesColor[Indices5._3] = _float4(0.f, 0.6f, 0.f, 1.f);
			}
			else
			{
				pVerticesColor[Indices._1] = _float4(0.8f, 0.6f, 0.1f, 1.f);
				pVerticesColor[Indices._2] = _float4(0.8f, 0.6f, 0.1f, 1.f);
				pVerticesColor[Indices._3] = _float4(0.8f, 0.6f, 0.1f, 1.f);

				pVerticesColor[Indices2._1] = _float4(0.8f, 0.6f, 0.1f, 1.f);
				pVerticesColor[Indices2._2] = _float4(0.8f, 0.6f, 0.1f, 1.f);
				pVerticesColor[Indices2._3] = _float4(0.8f, 0.6f, 0.1f, 1.f);

				pVerticesColor[Indices3._1] = _float4(0.8f, 0.6f, 0.1f, 1.f);
				pVerticesColor[Indices3._2] = _float4(0.8f, 0.6f, 0.1f, 1.f);
				pVerticesColor[Indices3._3] = _float4(0.8f, 0.6f, 0.1f, 1.f);

				pVerticesColor[Indices4._1] = _float4(0.8f, 0.6f, 0.1f, 1.f);
				pVerticesColor[Indices4._2] = _float4(0.8f, 0.6f, 0.1f, 1.f);
				pVerticesColor[Indices4._3] = _float4(0.8f, 0.6f, 0.1f, 1.f);

				pVerticesColor[Indices5._1] = _float4(0.8f, 0.6f, 0.1f, 1.f);
				pVerticesColor[Indices5._2] = _float4(0.8f, 0.6f, 0.1f, 1.f);
				pVerticesColor[Indices5._3] = _float4(0.8f, 0.6f, 0.1f, 1.f);
			}






			m_pTerrain->m_pTerrainMesh->ReMap_Vertices();



		}
		
	}


}

HRESULT CWindow_Navi::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	//해야되는ㄱ거
	// 1. 터레인 만듥기
	// 2. 터레인 크기 조정
	// 3. 터레인 아래로 쏘기

	if (ImGui::CollapsingHeader("CREATING TERRAIN"))
	{
		if (ImGui::InputInt("NumVerticesX", &m_iNumVerticesX))
		{
		}
		if (ImGui::InputInt("NumVerticesZ", &m_iNumVerticesZ))
		{
		}

		static char szBuff[MIN_STR] = "";
		if (ImGui::InputText("TerrainName", szBuff, sizeof(szBuff), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_strTerrainName = szBuff;
		}

		if (ImGui::Button("Load_Terrain"))
		{
			if (!m_strTerrainName.empty())
			{
				if (m_pTerrain)
					DELETE_GAMEOBJECT(m_pTerrain);

				m_pTerrain = CTerrain::Create(CFunctor::To_Wstring(m_strTerrainName).c_str());
				CREATE_GAMEOBJECT(m_pTerrain, GROUP_DEFAULT);

				m_iNumVerticesX = (m_pTerrain->m_iNumCellsX / 2) + 1;
				m_iNumVerticesZ = (m_pTerrain->m_iNumCellsZ / 2) + 1;

				m_tPickingData.iNumVerticesX = m_iNumVerticesX;
				m_tPickingData.iNumVerticesX = m_iNumVerticesZ;

				_float3* pVerticesPos = m_pTerrain->m_pTerrainMesh->Get_VerticesPos();
				m_tPickingData.fStepSizeX = pVerticesPos[1].x - pVerticesPos[0].x;
				m_tPickingData.fStepSizeZ = pVerticesPos[m_iNumVerticesX].z - pVerticesPos[0].z;

				m_iStartIndex = m_pTerrain->Get_StartCell()->Get_Index();
			}

			
		}

		if (ImGui::Button("Create_Terrain"))
		{
			Create_Terrain(m_iNumVerticesX, m_iNumVerticesZ);
		}
	}


	if (m_pTerrain)
	{
		if (ImGui::InputInt("iStartIndex", &m_iStartIndex))
		{
		}

		if (ImGui::RadioButton("BlockCell(V)", m_bSelectBlockCellMode))
		{
			m_bSelectBlockCellMode = !m_bSelectBlockCellMode;
		}

		ImGui::SameLine();

		if (ImGui::RadioButton("SelectStartCell(C)", m_bSelectStartCellMode))
		{
			m_bSelectStartCellMode = !m_bSelectStartCellMode;
		}

		if (ImGui::RadioButton("SelectGrassCell", m_bSelectCellGrassMode))
		{
			m_bSelectCellGrassMode = !m_bSelectCellGrassMode;
		}
		ImGui::SameLine();

		if (ImGui::RadioButton("SelectGroundCell", m_bSelectCellGroundMode))
		{
			m_bSelectCellGroundMode = !m_bSelectCellGroundMode;
		}


		if (ImGui::Button("Shoot_Ray"))
		{
			Shoot_Ray();
		}

		ImGui::SameLine();

		if (ImGui::Button("Save_CurTerrain"))
		{
			string savePath = "../bin/terrain/";
			savePath += m_strTerrainName;
			savePath += ".dat";
			ofstream	writeFile(savePath, ios::binary);

			if (!writeFile.is_open())
			{
				Call_MsgBox(L"SSave 실패 ??!?!");
				return E_FAIL;
			}

			writeFile.write((char*)&m_iNumVerticesX, sizeof(_uint));
			writeFile.write((char*)&m_iNumVerticesZ, sizeof(_uint));
			writeFile.write((char*)m_pTerrain->m_pTerrainMesh->Get_VerticesPos(), sizeof(_float3) * m_pTerrain->m_pTerrainMesh->Get_NumVertices());
			writeFile.write((char*)m_pTerrain->m_pTerrainMesh->Get_VerticesColor(), sizeof(_float4) * m_pTerrain->m_pTerrainMesh->Get_NumVertices());
			writeFile.write((char*)&m_iStartIndex, sizeof(_uint));

			writeFile.close();
			Call_MsgBox(L"Save 성공");

		}


		CTransform* pTerrainTransform = m_pTerrain->Get_Transform();
		_float4 vPos = pTerrainTransform->Get_World(WORLD_POS);
		_float4 vScale = pTerrainTransform->Get_Scale();

		_vector	vCurPos = vPos.XMLoad();
		_vector	vCurScale = vScale.XMLoad();

		if (ImGui::InputFloat3("TerrainWorldPos", vCurPos.m128_f32))
		{
			XMStoreFloat4(&vPos, vCurPos);
			pTerrainTransform->Set_World(WORLD_POS, vPos);
		}

		if (ImGui::InputFloat3("TerrainScale", vCurScale.m128_f32))
		{
			XMStoreFloat4(&vScale, vCurScale);
			pTerrainTransform->Set_Scale(vScale);
		}

	}



	__super::End();

	return S_OK;
}

CTerrain* CWindow_Navi::Create_Terrain(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	if (m_pTerrain)
		DELETE_GAMEOBJECT(m_pTerrain);

	m_pTerrain = CTerrain::Create(iNumVerticesX, iNumVerticesZ);

	CREATE_GAMEOBJECT(m_pTerrain, GROUP_DEFAULT);

	m_tPickingData.iNumVerticesX = m_iNumVerticesX;
	m_tPickingData.iNumVerticesZ = m_iNumVerticesZ;
	m_tPickingData.fStepSizeX = m_pTerrain->Get_Transform()->Get_Scale().x;
	m_tPickingData.fStepSizeZ = m_pTerrain->Get_Transform()->Get_Scale().z;

	return m_pTerrain;
}

void CWindow_Navi::Shoot_Ray()
{
	_float4x4 TerrainWorldMatrix = m_pTerrain->Get_Transform()->Get_WorldMatrix();
	m_tPickingData.iNumVerticesX = m_iNumVerticesX;
	m_tPickingData.iNumVerticesZ = m_iNumVerticesZ;
	m_tPickingData.fStepSizeX = m_pTerrain->Get_Transform()->Get_Scale().x;
	m_tPickingData.fStepSizeZ = m_pTerrain->Get_Transform()->Get_Scale().z;

	// 터레인의 정점다 받아와야함.
	_uint	iNumTerrainVertices = m_pTerrain->m_pTerrainMesh->Get_NumVertices();
	_float3* pTerrainVerticesPos = m_pTerrain->m_pTerrainMesh->Get_VerticesPos();
	_float4* pTerrainVerticesColor = m_pTerrain->m_pTerrainMesh->Get_VerticesColor();
	 
	// 생성되어있는 메쉬들 다 가져옴
	list<CGameObject*>& decoObjects = CGameInstance::Get_Instance()->Get_ObjGroup(GROUP_DECORATION);


	// ray방향은 아래로

	for (_uint i = 0; i < iNumTerrainVertices; ++i)
	{
		_float4 vNewPos = {0.f, 0.f, 0.f, 0.f};
		_float	fMinDistance = 9999.f;

		_vector vWorldTerrainVertexPos = XMVector3TransformCoord(
			XMLoadFloat3(&pTerrainVerticesPos[i]),
			TerrainWorldMatrix.XMLoad()
		);

		for (auto& elem : decoObjects)
		{
			CModel* pModel = GET_COMPONENT_FROM(elem, CModel);
			if (!pModel)
				continue;

			vector<pair<_uint, class CMeshContainer*>>& vecMeshes = pModel->Get_MeshContainers();

			for (auto& meshPair : vecMeshes)
			{
				_float4 vOutPos;

				if (Is_Picked(vWorldTerrainVertexPos, {0.f, -1.f, 0.f, 0.f}, meshPair.second, &vOutPos))
				{

					_float4 vTempWorldVertexPos;
					XMStoreFloat4(&vTempWorldVertexPos, vWorldTerrainVertexPos);
					_float fDistance = (vOutPos - vTempWorldVertexPos).Length();

					if (fMinDistance > fDistance)
					{
						vNewPos = vOutPos;
						fMinDistance = fDistance;
					}

				}
				
			}

		}

		// 터레인 좌표 갱신은 월드 기준으로 줘버림.
		if (fMinDistance != 9999.f)
		{
			pTerrainVerticesPos[i] = _float3(vNewPos.XMLoad().m128_f32);
		}
		else // 해당 정점에 암것도 읎음
		{
			vNewPos;
			XMStoreFloat4(&vNewPos, vWorldTerrainVertexPos);
			vNewPos.y = -1.f;
			pTerrainVerticesPos[i] = _float3(vNewPos.XMLoad().m128_f32);

		}
			
	}

	// 다 끝나면 터레인은 0좌표로 감. 스케일도 1로맞춰
	m_pTerrain->Get_Transform()->Set_World(WORLD_POS, _float4(0.f, 0.f, 0.f, 1.f));
	m_pTerrain->Get_Transform()->Set_Scale(_float4(1.f, 1.f, 1.f, 1.f));


	_uint iIndex = 0;
	_uint iNumFaces = m_pTerrain->m_pTerrainMesh->Get_NumPrimitive();
	_uint			iIndexSize = m_pTerrain->m_pTerrainMesh->Get_IndexSize();

	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_uint3		iIndices = m_pTerrain->m_pTerrainMesh->Get_Indices(i);

		_float3 vPointA = pTerrainVerticesPos[iIndices._1];
		_float3 vPointB = pTerrainVerticesPos[iIndices._2];
		_float3 vPointC = pTerrainVerticesPos[iIndices._3];

		_byte   bCellFlag = 0;

		//법선벡터
		_vector V1, V2;
		_float4 vCellNormal;

		V1 = (XMLoadFloat3(&vPointA) - XMLoadFloat3(&vPointB));
		V2 = (XMLoadFloat3(&vPointC) - XMLoadFloat3(&vPointB));

		vCellNormal = XMVector3Cross(V1, V2);
		vCellNormal.Normalize();

		if (fabs(vCellNormal.y) < 0.3f)
		{
			pTerrainVerticesColor[iIndices._1] = _float4(0.f, 0.f, 1.f, 1.f);
			pTerrainVerticesColor[iIndices._2] = _float4(0.f, 0.f, 1.f, 1.f);
			pTerrainVerticesColor[iIndices._3] = _float4(0.f, 0.f, 1.f, 1.f);
		}



		for (_uint k = 0; k < 3; ++k)
		{
			_float3 vCurComparePoint;
			switch (k)
			{
			case 0:
				vCurComparePoint = vPointA;
				break;
			case 1:
				vCurComparePoint = vPointB;
				break;
			default:
				vCurComparePoint = vPointC;
				break;
			}

			if (vCurComparePoint.y == -1.f)
			{
				pTerrainVerticesColor[iIndices._1] = _float4(1.f, 0.f, 0.f, 1.f);
				pTerrainVerticesColor[iIndices._2] = _float4(1.f, 0.f, 0.f, 1.f);
				pTerrainVerticesColor[iIndices._3] = _float4(1.f, 0.f, 0.f, 1.f);
				break;
			}
		}



	}

	m_pTerrain->m_pTerrainMesh->ReMap_Vertices();
}

_bool CWindow_Navi::Is_Picked(_vector vRayStartPos, _vector _vRayDirs, CMesh* pMesh, _float4* pOut)
{
	CTransform* pTransform = pMesh->Get_Owner()->Get_Transform();
	_float4x4	matWorld = pTransform->Get_WorldMatrix();
	_matrix		WorldMatrixInv = matWorld.Inverse().XMLoad();
	matWorld.Inverse();

	_vector			vRayPos, vRayDir;

	vRayPos = XMVector3TransformCoord(vRayStartPos, WorldMatrixInv);
	vRayDir = XMVector3Normalize(XMVector3TransformNormal(_vRayDirs, WorldMatrixInv));

	_uint			iNumFaces = pMesh->Get_NumPrimitive();
	const _float3* pVerticesPos = pMesh->Get_VerticesPos();
	_uint			iIndexSize = pMesh->Get_IndexSize();

	_float		fDist, fMin = 9999.f;

	for (_uint i = 0; i < iNumFaces; ++i)
	{
		_uint3		iIndices = pMesh->Get_Indices(i);

		_vector		vVec0 = XMLoadFloat3(&pVerticesPos[iIndices._1]);
		GXMVECTOR	vVec1 = XMLoadFloat3(&pVerticesPos[iIndices._2]);
		HXMVECTOR	vVec2 = XMLoadFloat3(&pVerticesPos[iIndices._3]);

		if (true == TriangleTests::Intersects(vRayPos, vRayDir, vVec0, vVec1, vVec2, fDist))
		{
			_float4 V1, V2;
			_float4 vOutNormal, vPickedPos;
			_float4x4 worldMat = matWorld;

			V1 = (vVec0 - vVec1);
			V2 = (vVec2 - vVec1);

			vOutNormal = XMVector3Cross(V1.XMLoad(), V2.XMLoad());
			vOutNormal = vOutNormal.MultiplyNormal(worldMat);
			vOutNormal.Normalize();
			vPickedPos = vRayPos + XMVector3Normalize(vRayDir) * fDist;

			_float4 vWorldRayStartPos;
			XMStoreFloat4(&vWorldRayStartPos, vRayStartPos);

			_float fDistance = (vWorldRayStartPos - vPickedPos).Length();

			if (fMin > fDistance)
			{
				*pOut = vPickedPos;
				fMin = fDistance;
			}
		}
	}

	if (fMin != 9999.f)
	{
		return true;
	}

	return false;
}

void CWindow_Navi::Compute_WorldRay(_float4* pOutPos, _float4* pOutDir)
{
	POINT pt{};

	::GetCursorPos(&pt);
	::ScreenToClient(g_hWnd, &pt);

	_float2 fPT = { (_float)pt.x, (_float)pt.y };

	_float4 vProjPos =
	{
		fPT.x / (g_iWinCX * 0.5f) - 1.f,
		fPT.y / -(g_iWinCY * 0.5f) + 1.f,
		0.f, 1.f
	};

	/* To View */
	_float4x4	ProjMatrixInv = CGameInstance::Get_Instance()->Get_CurProjMatrix();
	ProjMatrixInv.Inverse();

	_float4 vViewPos = vProjPos.MultiplyCoord(ProjMatrixInv);

	_float4 vRayDir = vViewPos.Normalize();
	_float4 vRayPos = { 0.f, 0.f, 0.f, 1.f };

	/* To World */
	_float4x4	ViewMatrixInv = CGameInstance::Get_Instance()->Get_CurViewMatrix();
	ViewMatrixInv.Inverse();

	vRayDir = vRayDir.MultiplyNormal(ViewMatrixInv);
	vRayPos = vRayPos.MultiplyCoord(ViewMatrixInv);

	vRayDir.Normalize();
	*pOutDir = _float4(vRayDir.x, vRayDir.y, vRayDir.z, 1.f);
	*pOutPos = _float4(vRayPos.x, vRayPos.y, vRayPos.z, 0.f);
}
