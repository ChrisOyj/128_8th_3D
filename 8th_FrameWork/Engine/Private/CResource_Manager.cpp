#include "CResource_Manager.h"

#include "GameInstance.h"
#include "CUtility_Json.h"
#include "CUtility_File.h"

#include "CResource_Material.h"
#include "CResource_Bone.h"
#include "CResource_Mesh.h"
#include "CResource_Animation.h"

#include "Channel.h"

IMPLEMENT_SINGLETON(CResource_Manager);

CResource_Manager::CResource_Manager()
{
}

CResource_Manager::~CResource_Manager()
{
	Release();
}

MODEL_DATA* CResource_Manager::Get_ModelData(wstring wstrFilePath, MODEL_TYPE eType)
{
	MODEL_DATA* pModelData = Find_ModelData(wstrFilePath);

	if (!pModelData)
	{
		pModelData = Make_ModelData(wstrFilePath, eType);
	}

	return pModelData;
}

CResource* CResource_Manager::Get_Resource(wstring wstrResourceKey)
{
	CResource* pResource = Find_Resource(wstrResourceKey);

	return pResource;
}

MODEL_DATA* CResource_Manager::Find_ModelData(wstring wstrFilePath)
{
	auto iter = m_mapModelData.find(Convert_ToHash(wstrFilePath));

	if (iter == m_mapModelData.end())
		return nullptr;

	return iter->second;
}

MODEL_DATA* CResource_Manager::Make_ModelData(wstring wstrFilePath, MODEL_TYPE eType)
{
	MODEL_DATA* pModelData = nullptr;

	//dat파일 열기
	wstring newFilePath;

	_int iFind = (_int)wstrFilePath.rfind(L".");
	newFilePath = wstrFilePath.substr(0, iFind);
	newFilePath += L".dat";

	pModelData = Open_DatFile(newFilePath);

	if (!pModelData)
	{
		pModelData = Create_DatFile(wstrFilePath, eType);

		if (!pModelData)
		{
			wstrFilePath += L" : Error on Path";
			Call_MsgBox(wstrFilePath.c_str());
		}
	}

	m_mapModelData.emplace(Convert_ToHash(wstrFilePath.c_str()), pModelData);

	return pModelData;
}

MODEL_DATA* CResource_Manager::Open_DatFile(wstring wstrDatFilePath)
{
	MODEL_DATA* pModelData = new MODEL_DATA;

	ifstream	readFile(wstrDatFilePath, ios::binary);

	if (!readFile.is_open())
	{
		SAFE_DELETE(pModelData);
		return nullptr;
	}

	MODEL_TYPE	eType = TYPE_END;
	readFile.read((char*)&eType, sizeof(MODEL_TYPE));

	if (eType == TYPE_ONLYANIMATION)
	{
		if (FAILED(Fill_ModelData(&readFile, RES_ANIMATION, pModelData)))
		{
			SAFE_DELETE(pModelData);
			return nullptr;
		}
	}
	else
	{
		if (FAILED(Fill_ModelData(&readFile, RES_MATERIAL, pModelData)))
		{
			SAFE_DELETE(pModelData);
			return nullptr;
		}

		if (FAILED(Fill_ModelData(&readFile, RES_BONE, pModelData)))
		{
			SAFE_DELETE(pModelData);
			return nullptr;
		}

		if (FAILED(Fill_ModelData(&readFile, RES_MESH, pModelData)))
		{
			SAFE_DELETE(pModelData);
			return nullptr;
		}
	}

	return pModelData;
}

MODEL_DATA* CResource_Manager::Create_DatFile(wstring wstrFilePath, MODEL_TYPE eType)
{
	MODEL_DATA* pModelData = nullptr;
	const aiScene* pAIScene;

	//FBX를 dat파일로 저장하는 과정임
	string strFbxPath(wstrFilePath.begin(), wstrFilePath.end());
	string strDatPath(wstrFilePath.begin(), wstrFilePath.end());
	_int iFind = (_int)strDatPath.rfind(".");
	strDatPath = strDatPath.substr(0, iFind);
	strDatPath += ".dat";

	if (TYPE_NONANIM == eType)
		pAIScene = m_Importer.ReadFile(strFbxPath.c_str(), aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
	else
		pAIScene = m_Importer.ReadFile(strFbxPath.c_str(), aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (!pAIScene)
	{
		return nullptr;
	}

	ofstream	writeFile(strDatPath, ios::binary);
	writeFile.write((char*)&eType, sizeof(eType));

	if (!writeFile.is_open())
	{
		Call_MsgBox(L"Failed to wrtie File : CResource_Manager");
		return nullptr;
	}

	if (TYPE_ONLYANIMATION == eType)
	{
		writeFile.write((char*)&pAIScene->mNumAnimations, sizeof(_uint));

		for (_uint i = 1; i < pAIScene->mNumAnimations; ++i)
		{
			Save_Anim(pAIScene->mAnimations[i], &writeFile);
		}
	}
	else
	{
		// 1. 머테리얼 저장
		_uint iNumMaterials = pAIScene->mNumMaterials;
		writeFile.write((char*)&iNumMaterials, sizeof(_uint));

		for (_uint i = 0; i < iNumMaterials; ++i)
		{
			aiMaterial* pMaterial = pAIScene->mMaterials[i];
			Save_Material(pMaterial, &writeFile, eType);
		}

		// 2. 노드 정보 저장
		_uint iNumNode = 1;
		writeFile.write((char*)&iNumNode, sizeof(_uint));
		Save_Node(pAIScene->mRootNode, &writeFile);

		// 3. 메쉬 정보 저장
		_uint iNumMeshes = pAIScene->mNumMeshes;
		writeFile.write((char*)&iNumMeshes, sizeof(_uint));

		for (_uint i = 0; i < iNumMeshes; ++i)
			Save_Mesh(pAIScene->mMeshes[i], eType, &writeFile, i);
	}

	writeFile.close();

	pModelData = Open_DatFile(wstring(strDatPath.begin(), strDatPath.end()));

	return pModelData;
}

void CResource_Manager::Save_Material(const aiMaterial* pMaterial, ofstream* pWriteFile, MODEL_TYPE eType)
{
	CUtility_File::Write_Text(pWriteFile, pMaterial->GetName().data);

	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
	{
		char			szFullPath[MAX_PATH] = "";

		aiString		strPath;

		if (FAILED(pMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
		{
			CUtility_File::Write_Text(pWriteFile, "");
			continue;
		}

		char			szFileName[MAX_PATH] = "";
		char			szExt[MAX_PATH] = "";
		_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

		string strFileNames = szFileName;
		strFileNames += szExt;

		string strNewPath;

		if (eType == TYPE_NONANIM)
			strNewPath = "../bin/resources/textures/ModelTextures/";
		else
			strNewPath = "../bin/resources/textures/AnimModelTextures/";

		strNewPath += strFileNames;

		CUtility_File::Write_Text(pWriteFile, strNewPath.c_str());
	}
}

void CResource_Manager::Save_Node(const aiNode* pNode, ofstream* pWriteFile)
{
	CUtility_File::Write_Text(pWriteFile, pNode->mName.data);
	pWriteFile->write((char*)&pNode->mNumChildren, sizeof(_uint));
	pWriteFile->write((char*)&pNode->mTransformation, sizeof(_float4x4));

	for (_uint i = 0; i < pNode->mNumChildren; ++i)
	{
		Save_Node(pNode->mChildren[i], pWriteFile);
	}
}

void CResource_Manager::Save_Mesh(const aiMesh* pMesh, MODEL_TYPE eType, ofstream* pWriteFile, _uint iIdx)
{
	_uint	iNumVertices = pMesh->mNumVertices;
	_uint iNumFaces = pMesh->mNumFaces;
	_uint iMaterialIndex = pMesh->mMaterialIndex;
	string	strMeshName = pMesh->mName.data;
	strMeshName += "_";
	strMeshName += to_string(iIdx);

	string	strMaterialName;

	CUtility_File::Write_Text(pWriteFile, strMeshName.c_str());
	pWriteFile->write((char*)&eType, sizeof(eType));
	pWriteFile->write((char*)&iNumVertices, sizeof(_uint));
	pWriteFile->write((char*)&iNumFaces, sizeof(_uint));
	pWriteFile->write((char*)&iMaterialIndex, sizeof(_uint));

	if (TYPE_ANIM == eType)
	{
		VTXANIM* pVertices = new VTXANIM[iNumVertices];
		ZeroMemory(pVertices, sizeof(VTXANIM) * iNumVertices);

		for (_uint i = 0; i < iNumVertices; ++i)
		{
			memcpy(&pVertices[i].vPosition, &pMesh->mVertices[i], sizeof(_float3));
			memcpy(&pVertices[i].vNormal, &pMesh->mNormals[i], sizeof(_float3));
			XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
			memcpy(&pVertices[i].vTexUV, &pMesh->mTextureCoords[0][i], sizeof(_float2));
			memcpy(&pVertices[i].vTangent, &pMesh->mTangents[i], sizeof(_float3));
		}

		_uint iNumBones = pMesh->mNumBones;
		pWriteFile->write((char*)&iNumBones, sizeof(_uint));

		for (_uint i = 0; i < iNumBones; ++i)
		{
			aiBone* pAIBone = pMesh->mBones[i];

			/* pAIBone->mNumWeights : 이 뼈는 몇개의 정점에 영향릉 주는지 */
			for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
			{
				/* pAIBone->mWeights[j].mVertexId : 그 중에 j번째 정점의 인덱스는 뭐였는지?  */
				/* pAIBone->mWeights[j].mWeight : j번째 정점에게 적용해야할 가중치. */

				if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.x = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
				}

				else if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.y = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
				}

				else if (0 == pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z)
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.z = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
				}

				else
				{
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendIndex.w = i;
					pVertices[pAIBone->mWeights[j].mVertexId].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
				}
			}
			BONE_DATA	tData;
			memcpy(&tData.OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
			strcpy_s(tData.szName, pAIBone->mName.data);
			pWriteFile->write((char*)&tData, sizeof(BONE_DATA));

		}

		pWriteFile->write((char*)pVertices, sizeof(VTXANIM) * iNumVertices);

		delete[] pVertices;
	}
	else
	{
		VTXMODEL* pVertices = new VTXMODEL[iNumVertices];
		ZeroMemory(pVertices, sizeof(VTXMODEL) * iNumVertices);

		for (_uint i = 0; i < iNumVertices; ++i)
		{
			json	VertexJson;

			memcpy(&pVertices[i].vPosition, &pMesh->mVertices[i], sizeof(_float3));
			memcpy(&pVertices[i].vNormal, &pMesh->mNormals[i], sizeof(_float3));
			XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
			memcpy(&pVertices[i].vTexUV, &pMesh->mTextureCoords[0][i], sizeof(_float2));
			memcpy(&pVertices[i].vTangent, &pMesh->mTangents[i], sizeof(_float3));
		}

		pWriteFile->write((char*)pVertices, sizeof(VTXMODEL) * iNumVertices);

		delete[] pVertices;
	}


	FACEINDICES32* pIndices = new FACEINDICES32[iNumFaces];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * iNumFaces);

	for (_uint i = 0; i < iNumFaces; ++i)
	{
		pIndices[i]._1 = pMesh->mFaces[i].mIndices[0];
		pIndices[i]._2 = pMesh->mFaces[i].mIndices[1];
		pIndices[i]._3 = pMesh->mFaces[i].mIndices[2];
	}

	pWriteFile->write((char*)pIndices, sizeof(FACEINDICES32) * iNumFaces);


	SAFE_DELETE_ARRAY(pIndices);

	//CenterPos
	_float4 vCenterPos = _float4(0.f, 0.f, 0.f);

	_float4 vPosition;
	memcpy(&vPosition, &pMesh->mVertices[1], sizeof(_float3));
	_float fMaxX = vPosition.x, fMaxY = vPosition.y, fMaxZ = vPosition.z;
	_float fMinX = vPosition.x, fMinY = vPosition.y, fMinZ = vPosition.z;


	for (_uint i = 0; i < iNumVertices; ++i)
	{
		_float4 vPosition;
		memcpy(&vPosition, &pMesh->mVertices[i], sizeof(_float3));

		if (vPosition.x > fMaxX)
			fMaxX = vPosition.x;
		else if (vPosition.x < fMinX)
			fMinX = vPosition.x;

		if (vPosition.y > fMaxY)
			fMaxY = vPosition.y;
		else if (vPosition.y < fMinY)
			fMinY = vPosition.y;

		if (vPosition.z > fMaxZ)
			fMaxZ = vPosition.z;
		else if (vPosition.x < fMinZ)
			fMinZ = vPosition.z;
	}

	vCenterPos.x = (fMinX + fMaxX) * 0.5f;
	vCenterPos.y = (fMinY + fMaxY) * 0.5f;
	vCenterPos.z = (fMinZ + fMaxZ) * 0.5f;

	pWriteFile->write((char*)&vCenterPos, sizeof(_float4));

	//Range
	_float fMax = 0.f;
	for (_uint i = 0; i < iNumVertices; ++i)
	{
		_float4 vPosition;
		memcpy(&vPosition, &pMesh->mVertices[i], sizeof(_float3));
		vPosition.w = 1.f;

		_float fLength = (vPosition - vCenterPos).Length();

		if (fLength > fMax)
			fMax = fLength;
	}

	pWriteFile->write((char*)&fMax, sizeof(_float));

}

void CResource_Manager::Save_Anim(const aiAnimation* pAnim, ofstream* pWriteFile)
{
	_uint iNumChannels = pAnim->mNumChannels;
	_float fDuration = (_float)pAnim->mDuration;
	_float fTickPerSecond = (_float)pAnim->mTicksPerSecond;
	CUtility_File::Write_Text(pWriteFile, pAnim->mName.data);

	pWriteFile->write((char*)&fDuration, sizeof(_float));
	pWriteFile->write((char*)&fTickPerSecond, sizeof(_float));
	pWriteFile->write((char*)&iNumChannels, sizeof(_uint));


	for (_uint i = 0; i < iNumChannels; ++i)
	{
		aiNodeAnim* pAIChannel = pAnim->mChannels[i];
		CUtility_File::Write_Text(pWriteFile, pAIChannel->mNodeName.data);

		_uint iNumKeyFrames = 0;
		iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
		iNumKeyFrames = max(iNumKeyFrames, pAIChannel->mNumPositionKeys);
		pWriteFile->write((char*)&iNumKeyFrames, sizeof(_uint));

		_float3		vScale;
		_float4		vRotation;
		_float3		vPosition;

		KEYFRAME* pKeyFrame = new KEYFRAME[iNumKeyFrames];

		for (_uint j = 0; j < iNumKeyFrames; ++j)
		{

			ZeroMemory(&pKeyFrame[j], sizeof(KEYFRAME));

			if (pAIChannel->mNumScalingKeys > j)
			{
				memcpy(&vScale, &pAIChannel->mScalingKeys[j].mValue, sizeof(_float3));
				pKeyFrame[j].fTime = (_float)pAIChannel->mScalingKeys[j].mTime;
			}

			if (pAIChannel->mNumRotationKeys > j)
			{
				/*memcpy(&vRotation, &pAIChannel->mRotationKeys[i].mValue, sizeof(_float4));*/
				vRotation.x = pAIChannel->mRotationKeys[j].mValue.x;
				vRotation.y = pAIChannel->mRotationKeys[j].mValue.y;
				vRotation.z = pAIChannel->mRotationKeys[j].mValue.z;
				vRotation.w = pAIChannel->mRotationKeys[j].mValue.w;
				pKeyFrame[j].fTime = (_float)pAIChannel->mScalingKeys[j].mTime;
			}

			if (pAIChannel->mNumPositionKeys > j)
			{
				memcpy(&vPosition, &pAIChannel->mPositionKeys[j].mValue, sizeof(_float3));
				pKeyFrame[j].fTime = (_float)pAIChannel->mScalingKeys[j].mTime;
			}

			pKeyFrame[j].vScale = vScale;
			pKeyFrame[j].vRotation = vRotation;
			pKeyFrame[j].vPosition = vPosition;
		}

		pWriteFile->write((char*)pKeyFrame, sizeof(KEYFRAME) * iNumKeyFrames);
		delete[] pKeyFrame;

	}

}

void CResource_Manager::Release()
{
	for (auto& elem : m_mapModelData)
	{
		SAFE_DELETE(elem.second);
	}

	for (auto& elem : m_mapResources)
	{
		SAFE_DELETE(elem.second);
	}
}

CResource* CResource_Manager::Find_Resource(wstring strResourceKey)
{
	auto iter = m_mapResources.find(Convert_ToHash(strResourceKey));

	if (iter == m_mapResources.end())
		return nullptr;

	return iter->second;
}

void CResource_Manager::Add_Resource(wstring strResourceKey, CResource* pResource)
{
	m_mapResources.emplace(Convert_ToHash(strResourceKey), pResource);
}

CResource* CResource_Manager::Create_Resource(ifstream* pReadFile, string strName, RES_TYPE eType)
{
	CResource* pResource = nullptr;
	switch (eType)
	{
	case Engine::RES_MATERIAL:
		pResource = CResource_Material::Create(pReadFile, strName);
		break;

	case Engine::RES_BONE:
		pResource = CResource_Bone::Create(pReadFile, strName, nullptr);
		break;

	case Engine::RES_MESH:
		pResource = CResource_Mesh::Create(pReadFile, strName);
		break;

	case Engine::RES_ANIMATION:
		pResource = CResource_Animation::Create(pReadFile, strName);
		break;

	default:
		break;
	}

	return pResource;
}

HRESULT CResource_Manager::Fill_ModelData(ifstream* pReadFile, RES_TYPE eType, MODEL_DATA* pOut)
{
	/* 리소스 갯수 */
	_uint iNumResources = 0;
	pReadFile->read((char*)&iNumResources, sizeof(_uint));

	for (_uint i = 0; i < iNumResources; ++i)
	{
		string strName = CUtility_File::Read_Text(pReadFile);
		wstring wstrName(strName.begin(), strName.end());

		if (wstrName == L"RootNode")
			wstrName += to_wstring(m_iRootNodeIdx++);


		//일단 생성
		CResource* pResource = Create_Resource(pReadFile, strName, eType);

		if (!pResource)
		{
			return E_FAIL;
		}

		if (!Find_Resource(wstrName))
			Add_Resource(wstrName, pResource);
		else
		{
			SAFE_DELETE(pResource);
		}

		pOut->m_vecResourceKeys.push_back(wstrName);
		pOut->m_vecResType.push_back(eType);
	}


	return S_OK;
}
