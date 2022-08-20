#include "..\Public\Model.h"

#include "MeshContainer.h"
#include "HierarchyNode.h"
#include "Animation.h"
#include "Texture.h"
#include "CShader.h"


CModel::CModel(_uint iGroupIdx)
	: CComponent(iGroupIdx)
{

}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_MeshContainers(rhs.m_MeshContainers)	
	, m_iNumMeshContainers(rhs.m_iNumMeshContainers)
	, m_Materials(rhs.m_Materials)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Animations(rhs.m_Animations)
	, m_bCloned(true)
{
	/*for (_uint i = 0; i < rhs.m_iNumMaterials; ++i)
	{
		MODEL_MATERIAL	tMaterial;
		MODEL_MATERIAL	rhsMaterial = rhs.m_Materials[i];

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			tMaterial.pTextures[j] = nullptr;

			if (rhsMaterial.pTextures[j] != nullptr)
				tMaterial.pTextures[j] = rhsMaterial.pTextures[j]->Clone();
		}

		m_Materials.push_back(tMaterial);
	}

	for (_uint i = 0; i < rhs.m_)*/
}

CModel::~CModel()
{
	Release();
}

CModel* CModel::Create(_uint iGroupIdx, MODEL_TYPE eType, const char* pModelFilePath, _float4x4 TransformMatrix)
{
	CModel* pInstance = new CModel(iGroupIdx);

	if (FAILED(pInstance->SetUp_Model(eType, pModelFilePath, TransformMatrix)))
	{
		Call_MsgBox(L"Failed to SetUp_Model : CModel");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CModel");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}


HRESULT CModel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CModel::Initialize()
{
	return S_OK;
}

void CModel::Tick()
{
}

void CModel::Late_Tick()
{
}

HRESULT CModel::Play_Animation(_float fTimeDelta)
{
	if (m_iCurrentAnimationIndex >= m_iNumAnimations)
		return E_FAIL;

	m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrices(fTimeDelta);



	return S_OK;
}

HRESULT CModel::Render(_uint iMeshContainerIndex)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;	
		
	m_MeshContainers[iMeshContainerIndex]->Render();	

	return S_OK;
}

void CModel::Release()
{
	if (true == m_bCloned)
		return;
		
	for (auto& pAnimation : m_Animations)
		SAFE_DELETE(pAnimation);
	m_Animations.clear();

	for (auto& pHierarchyNode : m_HierarchyNodes)
		SAFE_DELETE(pHierarchyNode);
	m_HierarchyNodes.clear();

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pTextures)
			SAFE_DELETE(pTexture);
	}
	m_Materials.clear();

	for (auto& pMeshContainer : m_MeshContainers)
		SAFE_DELETE(pMeshContainer);

	m_MeshContainers.clear();

	m_Importer.FreeScene();
}

CHierarchyNode* CModel::Find_HierarcyNode(const char* pBoneName)
{
	auto	iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CHierarchyNode* pNode)
		{
			return !strcmp(pBoneName, pNode->Get_Name());
		});

	if (iter == m_HierarchyNodes.end())
		return nullptr;

	return *iter;


}

HRESULT CModel::Bind_SRV(CShader * pShader, const char * pConstantName, _uint iMeshContainerIndex, aiTextureType eType)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	_uint		iMaterialIndex = m_MeshContainers[iMeshContainerIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	if (m_Materials[iMaterialIndex].pTextures[eType] == nullptr)
		return S_OK;

	return m_Materials[iMaterialIndex].pTextures[eType]->Set_ShaderResourceView(pShader, pConstantName);	
}

HRESULT CModel::SetUp_Model(MODEL_TYPE eType, const char* pModelFilePath, _float4x4 TransformMatrix)
{
	m_eMODEL_TYPE = eType;

	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix.XMLoad());

	if (TYPE_NONANIM == eType)
		m_pAIScene = m_Importer.ReadFile(pModelFilePath, aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
	else
		m_pAIScene = m_Importer.ReadFile(pModelFilePath, aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);



	if (FAILED(Create_Materials(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Create_HierarchyNodes(m_pAIScene->mRootNode, nullptr, 0)))
		return E_FAIL;

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
		{
			return pSour->Get_Depth() < pDest->Get_Depth();
		});

	if (FAILED(Create_MeshContainers()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Create_MeshContainers()
{
	m_iNumMeshContainers = m_pAIScene->mNumMeshes;

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(0, m_eMODEL_TYPE, m_pAIScene->mMeshes[i], this, m_TransformMatrix);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_MeshContainers.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Create_Materials(const char* pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MODEL_MATERIAL			Material;
		ZeroMemory(&Material, sizeof(MODEL_MATERIAL));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			char			szFullPath[MAX_PATH] = "";

			aiString		strPath;
						
			if (FAILED(m_pAIScene->mMaterials[i]->GetTexture(aiTextureType(j), 0, &strPath)))
				continue;

			char			szFileName[MAX_PATH] = "";
			char			szExt[MAX_PATH] = "";
			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			string strFileNames = szFileName;
			strFileNames += szExt;
			//strFileNames += ".tga";
			wstring wstrFileName(strFileNames.begin(), strFileNames.end());
			wstring wstrPath = L"../bin/resources/textures/ModelTextures/";

			wstrPath += wstrFileName;

			Material.pTextures[j] = CTexture::Create(0, wstrPath.c_str(), 1);
			if (nullptr == Material.pTextures[j])
				return E_FAIL;

			/*_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);		

			_tchar		szTextureFilePath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szTextureFilePath, MAX_PATH);


			Material.pTextures[j] = CTexture::Create(0, wstrPath.c_str(), 1);
			if (nullptr == Material.pTextures[j])
				return E_FAIL;*/
		}

		m_Materials.push_back(Material);		
	}

	return S_OK;
}

HRESULT CModel::Create_HierarchyNodes(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth)
{
	CHierarchyNode*		pHierarchyNode = CHierarchyNode::Create(pAINode, pParent, iDepth);
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_HierarchyNodes.push_back(pHierarchyNode);

	for (_uint i = 0; i < pAINode->mNumChildren; ++i)
		Create_HierarchyNodes(pAINode->mChildren[i], pHierarchyNode, iDepth + 1);

	return S_OK;
}

HRESULT CModel::Create_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation*		pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this);

		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);		
	}	

	return S_OK;
}

