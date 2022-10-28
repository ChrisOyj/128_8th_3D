#include "..\Public\Model.h"

#include "MeshContainer.h"
#include "CInstanceMesh.h"
#include "HierarchyNode.h"
#include "Animation.h"
#include "Texture.h"
#include "CShader.h"
#include "Model.h"

#include "GameInstance.h"

#include "CResource_Material.h"
#include "CResource_Mesh.h"
#include "CResource_Bone.h"
#include "CResource_Animation.h"

#include "Transform.h"
#include "GameObject.h"

CModel::CModel(_uint iGroupIdx)
	: CComponent(iGroupIdx)
{

}

CModel::CModel(const CModel& rhs)
	: CComponent(rhs)
	, m_Materials(rhs.m_Materials)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_iNumMeshContainers(rhs.m_iNumMeshContainers)
	, m_bCloned(true)
	, m_bCulling(rhs.m_bCulling)
	, m_TransformMatrix(rhs.m_TransformMatrix)
	, m_eMODEL_TYPE(rhs.m_eMODEL_TYPE)
{
	for (auto& elem : rhs.m_MeshContainers)
	{
		CMeshContainer* pNewContainer = elem.second->Clone();
		m_MeshContainers.push_back(make_pair(elem.first, pNewContainer));
	}

	for (auto& elem : rhs.m_vecHierarchyNodes)
	{
		CHierarchyNode* pCloneNode = elem.second->Clone();
		pCloneNode->ReFind_Parent(this);
		m_vecHierarchyNodes.push_back(make_pair(elem.first, pCloneNode));
	}

	SetUp_MeshContainersPtr();
}

CModel::~CModel()
{
	Release();
}

CModel* CModel::Create(_uint iGroupIdx, MODEL_TYPE eType, wstring wstrModelFilePath, _float4x4 TransformMatrix)
{
	CModel* pInstance = new CModel(iGroupIdx);

	pInstance->m_wstrModelFilePath = wstrModelFilePath;

	if (FAILED(pInstance->SetUp_Model(eType, wstrModelFilePath, TransformMatrix, 0)))
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

CModel* CModel::Create(_uint iGroupIdx, MODEL_TYPE eType, wstring wstrModelFilePath, wstring wstrInstanceFilePath, _float4x4 TransformMatrix)
{
	CModel* pInstance = new CModel(iGroupIdx);
	pInstance->m_wstrModelFilePath = wstrModelFilePath;

	if (FAILED(pInstance->SetUp_InstancingModel(eType, wstrModelFilePath, wstrInstanceFilePath, TransformMatrix, 0)))
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

CModel* CModel::Create(_uint iGroupIdx, MODEL_TYPE eType, wstring wstrModelFilePath, _uint iNumInstance, _float4x4 TransformMatrix)
{
	CModel* pInstance = new CModel(iGroupIdx);
	pInstance->m_wstrModelFilePath = wstrModelFilePath;

	if (FAILED(pInstance->SetUp_InstancingModel(eType, wstrModelFilePath, iNumInstance, TransformMatrix, 0)))
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


wstring CModel::Get_TextureFilePath(_uint iIndex)
{
	CTexture* pTexture = m_Materials.front().second.pTextures[iIndex];

	if (pTexture)
	{
		return pTexture->Get_vecTexture()[pTexture->Get_CurTextureIndex()].strFilePath;
	}

	return wstring();
}

wstring CModel::Get_TextureFilePathFromParts(_uint iMeshPartType)
{
	for (auto& elem : m_Materials)
	{
		if (elem.first == iMeshPartType)
			return elem.second.pTextures[1]->Get_vecTexture().front().strFilePath;
	}

	return wstring();
}

CTexture* CModel::Get_TextureFromParts(_uint iMeshPartType, _uint iMeshPartType2)
{
	for (auto& elem : m_Materials)
	{
		if (elem.first == iMeshPartType || elem.first == iMeshPartType2)
			return elem.second.pTextures[1];
	}

	return nullptr;
}

void CModel::Set_NewModel(wstring wstrModelFilePath)
{
	m_wstrModelFilePath = wstrModelFilePath;
	Release();

	SetUp_Model(m_eMODEL_TYPE, m_wstrModelFilePath, m_TransformMatrix, 0);
}

void CModel::Set_NewTransformMatrix(_float4x4 TransformMatrix)
{
	m_TransformMatrix = TransformMatrix;
	Release();

	SetUp_Model(m_eMODEL_TYPE, m_wstrModelFilePath, m_TransformMatrix, 0);
}

void CModel::Set_ShaderPass(_uint iModelPart, _uint iShaderPass)
{
	for (auto& elem : m_MeshContainers)
	{
		if (elem.first == iModelPart)
			elem.second->Set_CurPass(iShaderPass);
	}
}

void CModel::Set_ShaderPassToAll(_uint iShaderPass)
{
	for (auto& elem : m_MeshContainers)
	{
		elem.second->Set_CurPass(iShaderPass);
	}
}

void CModel::Set_Owner(CGameObject* pOwner)
{
	__super::Set_Owner(pOwner);

	for (auto& elem : m_MeshContainers)
	{
		elem.second->Set_Owner(pOwner);
	}
}

void CModel::Set_ShaderFlag(_float4 vFlag)
{
	for (auto& elem : m_MeshContainers)
	{
		elem.second->Set_ShaderFlag(vFlag);
	}
}

void CModel::Set_ShaderFlag(_uint iMeshPartType, _float4 vFlag)
{
	for (auto& elem : m_MeshContainers)
	{
		if (elem.first == iMeshPartType)
			elem.second->Set_ShaderFlag(vFlag);
	}
}

void CModel::Set_OutlinePower(_float fOutline)
{
	for (auto& elem : m_MeshContainers)
	{
		elem.second->Set_OutlinePower(fOutline);
	}
}

void CModel::Set_OutlinePower(_uint iMeshPartType, _float fOutline)
{
	for (auto& elem : m_MeshContainers)
	{
		if (elem.first == iMeshPartType)
			elem.second->Set_OutlinePower(fOutline);
	}
}

void CModel::Switch_MeshContainerBone(_uint iMeshPartType, string strBoneName)
{
	CMeshContainer* pMeshCon = nullptr;

	for (auto& elem : m_MeshContainers)
	{
		if (elem.first == iMeshPartType)
		{
			pMeshCon = elem.second;
			break;
		}
			
	}

	if (pMeshCon)
	{
		if (pMeshCon->m_Bones.size() == 1)
		{
			pMeshCon->m_Bones.front() = Find_HierarchyNode(strBoneName.c_str());
		}
		
	}
}

HRESULT CModel::Add_Model(wstring wstrModelFilePath, _uint iMeshPartType)
{
	MODEL_DATA* pModelData = CGameInstance::Get_Instance()->Get_ModelData(wstrModelFilePath, m_eMODEL_TYPE);
	_uint iSize = (_uint)pModelData->m_vecResourceKeys.size();
	for (_uint i = 0; i < iSize; ++i)
	{
		CResource* pResource = CGameInstance::Get_Instance()->Get_Resource(pModelData->m_vecResourceKeys[i]);
		if (!pResource)
			return E_FAIL;

		if (FAILED(Create_ModelData(pResource, pModelData->m_vecResType[i], iMeshPartType)))
			return E_FAIL;
	}

	m_iNumMaterials = (_uint)m_Materials.size();
	m_iNumMeshContainers = (_uint)m_MeshContainers.size();

	/* 매칭시켜주기 */
	if (FAILED(SetUp_MeshContainersPtr()))
		return E_FAIL;



	

	return S_OK;
}

HRESULT CModel::Add_Model(wstring wstrModelFilePath, _uint iMeshPartType, string strBoneName)
{
	MODEL_DATA* pModelData = CGameInstance::Get_Instance()->Get_ModelData(wstrModelFilePath, m_eMODEL_TYPE);
	_uint iSize = (_uint)pModelData->m_vecResourceKeys.size();
	for (_uint i = 0; i < iSize; ++i)
	{
		CResource* pResource = CGameInstance::Get_Instance()->Get_Resource(pModelData->m_vecResourceKeys[i]);
		if (!pResource)
			return E_FAIL;

		if (FAILED(Create_ModelData(pResource, pModelData->m_vecResType[i], iMeshPartType)))
			return E_FAIL;

		if (pModelData->m_vecResType[i] == RES_MESH)
		{
			m_MeshContainers.back().second->m_strRefBoneName = strBoneName;
		}
	}

	

	m_iNumMaterials = (_uint)m_Materials.size();
	m_iNumMeshContainers = (_uint)m_MeshContainers.size();

	/* 매칭시켜주기 */
	if (FAILED(SetUp_MeshContainersPtr()))
		return E_FAIL;





	return S_OK;
}

HRESULT CModel::Enable_ModelParts(_uint iMeshPartType, _bool bEnable)
{
	for (auto& elem : m_MeshContainers)
	{
		if (elem.first == iMeshPartType)
		{
			if (bEnable)
				ENABLE_COMPONENT(elem.second);
			else
				DISABLE_COMPONENT(elem.second);
		}
	}

	return S_OK;
}

HRESULT CModel::Delete_ModelParts(_uint iMeshContainerType)
{
	for (auto iter = m_MeshContainers.begin(); iter != m_MeshContainers.end();)
	{
		if (iter->first == iMeshContainerType)
		{
			SAFE_DELETE((iter->second));
			iter = m_MeshContainers.erase(iter);
		}
		else
			++iter;

	}

	for (auto iter = m_Materials.begin(); iter != m_Materials.end();)
	{
		if (iter->first == iMeshContainerType)
		{
			for (auto& pTexture : iter->second.pTextures)
			{
				SAFE_DELETE(pTexture);
			}

			iter = m_Materials.erase(iter);
		}
		else
			++iter;

	}


	m_iNumMaterials = (_uint)m_Materials.size();
	m_iNumMeshContainers = (_uint)m_MeshContainers.size();
	return S_OK;
}

HRESULT CModel::Initialize_Prototype()
{
	m_iNumMaterials = (_uint)m_Materials.size();
	m_iNumMeshContainers = (_uint)m_MeshContainers.size();
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
	for (auto& pHierarchyNode : m_vecHierarchyNodes)
		pHierarchyNode.second->Update_CombinedTransformationMatrix();

	if (!m_bCulling)
		return;

	_float4x4 matWorld = m_pOwner->Get_Transform()->Get_WorldMatrix();
	_float4 vScale = m_pOwner->Get_Transform()->Get_Scale();
	_float fScaleLength = vScale.x;

	if (vScale.y > fScaleLength)
		fScaleLength = vScale.y;

	if (vScale.z > fScaleLength)
		fScaleLength = vScale.z;

	for (auto& pMeshContainer : m_MeshContainers)
	{
		_float4 vWorldPos = pMeshContainer.second->Get_CenterPos().MultiplyCoord(matWorld);
		_float fRange = pMeshContainer.second->Get_MaxRange();

		fRange *= fScaleLength;

		if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(vWorldPos.XMLoad(), fRange))
		{
			pMeshContainer.second->Set_Enable(true);
		}
		else
		{
			pMeshContainer.second->Set_Enable(false);
		}
	}
}

HRESULT CModel::Render(_uint iMeshContainerIndex, CShader* pShader, _uint iPassIndex, const char* pConstantBoneName)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	if (!m_MeshContainers[iMeshContainerIndex].second->Is_Valid())
		return S_OK;

	m_MeshContainers[iMeshContainerIndex].second->Set_ShaderResourceFlag(pShader, "g_vFlag");
	m_MeshContainers[iMeshContainerIndex].second->Set_ShaderResource(pShader, "g_vColor");

	if (m_eMODEL_TYPE == TYPE_ANIM)
		m_MeshContainers[iMeshContainerIndex].second->SetUp_BoneMatrices(pShader, "g_Bones");

	if (FAILED(pShader->Begin(m_MeshContainers[iMeshContainerIndex].second->Get_CurPass())))
		return E_FAIL;

	return m_MeshContainers[iMeshContainerIndex].second->Render();
}

HRESULT CModel::Bake_Shadow(_uint iMeshContainerIndex, CShader* pShader, _uint iPassIndex)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	//m_MeshContainers[iMeshContainerIndex].second->Set_ShaderResourceFlag(pShader, "g_vFlag");

	if (m_eMODEL_TYPE == TYPE_ANIM)
		m_MeshContainers[iMeshContainerIndex].second->SetUp_BoneMatrices(pShader, "g_Bones");

	if (FAILED(pShader->Begin(iPassIndex)))
		return E_FAIL;

	return m_MeshContainers[iMeshContainerIndex].second->Render();
}

void CModel::Release()
{
	for (auto& pHierarchyNode : m_vecHierarchyNodes)
		SAFE_DELETE(pHierarchyNode.second);
	m_vecHierarchyNodes.clear();

	for (auto& pMeshContainer : m_MeshContainers)
		SAFE_DELETE(pMeshContainer.second);

	m_MeshContainers.clear();

	if (true == m_bCloned)
		return;

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			SAFE_DELETE(Material.second.pTextures[i]);
		}
	}

	m_Materials.clear();
}

void CModel::Start()
{
	__super::Start();

	for (auto& elem : m_MeshContainers)
	{
		elem.second->Set_Owner(m_pOwner);
	}
}

CHierarchyNode* CModel::Find_HierarchyNode(const char* pBoneName)
{
	auto	iter = find_if(m_vecHierarchyNodes.begin(), m_vecHierarchyNodes.end(), [&](pair<_uint, CHierarchyNode*> pNode)
		{
			return !strcmp(pBoneName, pNode.second->Get_Name());
		});

	if (iter == m_vecHierarchyNodes.end())
		return nullptr;

	return iter->second;
}

_uint CModel::Find_MaterialIndex(const char* pMatName)
{
	_uint iIdx = 0;

	for (_uint i = 0; i < m_Materials.size(); ++i)
	{
		if (m_Materials[i].second.strName == pMatName)
		{
			return i;
		}
	}

	return 0;
}

void CModel::Sort_HierarchyNode()
{
	sort(m_vecHierarchyNodes.begin(), m_vecHierarchyNodes.end(), [](pair<_uint, CHierarchyNode*> pSour, pair < _uint, CHierarchyNode*> pDest)
		{
			return pSour.second->Get_Depth() < pDest.second->Get_Depth();
		});
}

HRESULT CModel::Bind_SRV(CShader* pShader, const char* pConstantName, _uint iMeshContainerIndex, aiTextureType eType)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	_uint		iMaterialIndex = m_MeshContainers[iMeshContainerIndex].second->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	if (m_Materials[iMaterialIndex].second.pTextures[eType] == nullptr)
		return S_OK;

	return m_Materials[iMaterialIndex].second.pTextures[eType]->Set_ShaderResourceView(pShader, pConstantName);
}

HRESULT CModel::SetUp_Model(MODEL_TYPE eType, wstring wstrModelFilePath, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	m_wstrModelFilePath = wstrModelFilePath;
	m_eMODEL_TYPE = eType;
	if (m_eMODEL_TYPE == TYPE_NONANIM)
		m_bCulling = true;
	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix.XMLoad());

	MODEL_DATA* pModel_Data = CGameInstance::Get_Instance()->Get_ModelData(wstrModelFilePath, eType);

	if (!pModel_Data)
		return E_FAIL;

	_uint iSize = (_uint)pModel_Data->m_vecResourceKeys.size();

	for (_uint i = 0; i < iSize; ++i)
	{
		CResource* pResource = CGameInstance::Get_Instance()->Get_Resource(pModel_Data->m_vecResourceKeys[i]);
		if (!pResource)
			return E_FAIL;
		Create_ModelData(pResource, pModel_Data->m_vecResType[i], iMeshPartType);
	}

	if (FAILED(SetUp_MeshContainersPtr()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::SetUp_InstancingModel(MODEL_TYPE eType, wstring wstrModelFilePath, wstring wstrInstanceFilePath, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	m_eMODEL_TYPE = eType;
	m_bCulling = false;
	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix.XMLoad());

	MODEL_DATA* pModel_Data = CGameInstance::Get_Instance()->Get_ModelData(wstrModelFilePath, eType);

	if (!pModel_Data)
		return E_FAIL;

	_uint iSize = (_uint)pModel_Data->m_vecResourceKeys.size();

	for (_uint i = 0; i < iSize; ++i)
	{
		CResource* pResource = CGameInstance::Get_Instance()->Get_Resource(pModel_Data->m_vecResourceKeys[i]);
		if (!pResource)
			return E_FAIL;

		if (pModel_Data->m_vecResType[i] == RES_MESH)
		{
			Create_InstancingMesh(static_cast<CResource_Mesh*>(pResource), wstrInstanceFilePath, TransformMatrix, iMeshPartType);
		}
		else
		{
			Create_ModelData(pResource, pModel_Data->m_vecResType[i], iMeshPartType);
		}

	}

	if (FAILED(SetUp_MeshContainersPtr()))
		return E_FAIL;

	return S_OK;
}


HRESULT CModel::SetUp_InstancingModel(MODEL_TYPE eType, wstring wstrModelFilePath, _uint iNumInstance, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	m_eMODEL_TYPE = eType;
	m_bCulling = false;
	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix.XMLoad());

	MODEL_DATA* pModel_Data = CGameInstance::Get_Instance()->Get_ModelData(wstrModelFilePath, eType);

	if (!pModel_Data)
		return E_FAIL;

	_uint iSize = (_uint)pModel_Data->m_vecResourceKeys.size();

	for (_uint i = 0; i < iSize; ++i)
	{
		CResource* pResource = CGameInstance::Get_Instance()->Get_Resource(pModel_Data->m_vecResourceKeys[i]);
		if (!pResource)
			return E_FAIL;

		if (pModel_Data->m_vecResType[i] == RES_MESH)
		{
			Create_InstancingMesh(static_cast<CResource_Mesh*>(pResource), iNumInstance, TransformMatrix, iMeshPartType);
		}
		else
		{
			Create_ModelData(pResource, pModel_Data->m_vecResType[i], iMeshPartType);
		}

	}

	if (FAILED(SetUp_MeshContainersPtr()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::SetUp_MeshContainersPtr()
{
	for (auto& elem : m_MeshContainers)
	{
		if (FAILED(elem.second->SetUp_Ptr(this)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Create_ModelData(CResource* pResource, _uint iResType, _uint iMeshPartType)
{
	switch (iResType)
	{
	case RES_ANIMATION:
	{
		return E_FAIL;
	}
	break;

	case RES_MATERIAL:
	{
		CResource_Material* pResource_Material = dynamic_cast<CResource_Material*>(pResource);
		if (!pResource_Material)
			return E_FAIL;

		if (FAILED(Create_Material(pResource_Material, iMeshPartType)))
			return E_FAIL;
	}
	break;

	case RES_BONE:
	{
		CResource_Bone* pResource_Bone = dynamic_cast<CResource_Bone*>(pResource);
		if (!pResource_Bone)
			return E_FAIL;

		if (FAILED(Create_HierarchyNode(pResource_Bone, nullptr, 0, iMeshPartType)))
			return E_FAIL;
	}
	break;

	case RES_MESH:
	{
		CResource_Mesh* pResource_Mesh = dynamic_cast<CResource_Mesh*>(pResource);
		if (!pResource_Mesh)
			return E_FAIL;

		if (FAILED(Create_MeshContainer(pResource_Mesh, m_TransformMatrix, iMeshPartType)))
			return E_FAIL;
	}
	break;


	default:
		break;
	}

	return S_OK;
}

HRESULT CModel::Create_Material(CResource_Material* pResource, _uint iMeshPartType)
{
	MATERIAL_DESC& tMatDesc = pResource->Get_MatDesc();
	MODEL_MATERIAL			Material;

	Material.strName = pResource->Get_Name();
	Material.vColor = tMatDesc.vMixColor;

	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
	{
		if (tMatDesc.TexturePaths[j].empty())
		{
			Material.pTextures[j] = nullptr;
			continue;
		}

		Material.pTextures[j] = CTexture::Create(0, wstring(tMatDesc.TexturePaths[j].begin(), tMatDesc.TexturePaths[j].end()).c_str(), 1);

		if (nullptr == Material.pTextures[j])
			return E_FAIL;
	}

	m_Materials.push_back(make_pair(iMeshPartType, Material));

	return S_OK;
}

HRESULT CModel::Create_MeshContainer(CResource_Mesh* pResource, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	MESH_DESC& tMeshDesc = pResource->Get_MeshDesc();

	CMeshContainer* pMeshContainer = CMeshContainer::Create(pResource, TransformMatrix);

	if (nullptr == pMeshContainer)
		return E_FAIL;

	//여기서 받은 인덱스... 흐음..
	// 내 파트타입이랑 같은 인덱스를 찾으면 댈듯? 그럼 안댐.

	_uint iMatFind = 0;
	for (_uint i = 0; i < m_Materials.size(); ++i)
	{
		if (m_Materials[i].first != iMeshPartType)
			continue;

		if (iMatFind == pMeshContainer->m_iMaterialIndex)
		{
			pMeshContainer->m_MaterialName = m_Materials[i].second.strName;
			break;
		}

		iMatFind++;

	}

	m_MeshContainers.push_back(make_pair(iMeshPartType, pMeshContainer));
	pMeshContainer->Set_Owner(m_pOwner);

	return S_OK;
}

HRESULT CModel::Create_InstancingMesh(CResource_Mesh* pResource, wstring wstrInstanceFilePath, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	MESH_DESC& tMeshDesc = pResource->Get_MeshDesc();

	CInstanceMesh* pMeshContainer = CInstanceMesh::Create(wstrInstanceFilePath, pResource, TransformMatrix);

	if (nullptr == pMeshContainer)
		return E_FAIL;

	//여기서 받은 인덱스... 흐음..
	// 내 파트타입이랑 같은 인덱스를 찾으면 댈듯? 그럼 안댐.

	_uint iMatFind = 0;
	for (_uint i = 0; i < m_Materials.size(); ++i)
	{
		if (m_Materials[i].first != iMeshPartType)
			continue;

		if (iMatFind == pMeshContainer->m_iMaterialIndex)
		{
			pMeshContainer->m_MaterialName = m_Materials[i].second.strName;
			break;
		}

		iMatFind++;

	}

	m_MeshContainers.push_back(make_pair(iMeshPartType, pMeshContainer));
	pMeshContainer->Set_Owner(m_pOwner);

	return S_OK;
}

HRESULT CModel::Create_InstancingMesh(CResource_Mesh* pResource, _uint iNumInstance, _float4x4 TransformMatrix, _uint iMeshPartType)
{
	MESH_DESC& tMeshDesc = pResource->Get_MeshDesc();

	CInstanceMesh* pMeshContainer = CInstanceMesh::Create(iNumInstance, pResource, TransformMatrix);

	if (nullptr == pMeshContainer)
		return E_FAIL;

	//여기서 받은 인덱스... 흐음..
	// 내 파트타입이랑 같은 인덱스를 찾으면 댈듯? 그럼 안댐.

	_uint iMatFind = 0;
	for (_uint i = 0; i < m_Materials.size(); ++i)
	{
		if (m_Materials[i].first != iMeshPartType)
			continue;

		if (iMatFind == pMeshContainer->m_iMaterialIndex)
		{
			pMeshContainer->m_MaterialName = m_Materials[i].second.strName;
			break;
		}

		iMatFind++;

	}

	m_MeshContainers.push_back(make_pair(iMeshPartType, pMeshContainer));
	pMeshContainer->Set_Owner(m_pOwner);

	return S_OK;
}

HRESULT CModel::Create_HierarchyNode(CResource_Bone* pResource, CHierarchyNode* pParent, _uint iDepth, _uint iMeshPartType)
{
	//1. 일단 모두 만듬
	CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(pResource, pParent, iDepth);

	if (nullptr == pHierarchyNode)
		return E_FAIL;

	//2. 만들어진거 모두 가져옴
	vector<CHierarchyNode*>	vecNodes;
	pHierarchyNode->Get_AllNodes(vecNodes);

	sort(vecNodes.begin(), vecNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)
		{
			return pSour->Get_Depth() < pDest->Get_Depth();
		});

	if (m_vecHierarchyNodes.empty())
	{
		for (_uint i = 0; i < vecNodes.size(); ++i)
		{
			m_vecHierarchyNodes.push_back(make_pair(iMeshPartType, vecNodes[i]));
		}
		return S_OK;

	}


	for (_uint i = 0; i < vecNodes.size(); ++i)
	{
		_bool bExist = false;

		for (_uint j = 0; j < m_vecHierarchyNodes.size(); ++j)
		{
			// 이미 있는지 검색
			if (!strcmp(m_vecHierarchyNodes[j].second->Get_Name(), vecNodes[i]->Get_Name()))
			{
				//이미 있으면 그녀석이랑 나를 대체해야함.

				//박힌돌
				//m_vecHierarchyNodes[j].second;

				////굴러온돌
				//vecNodes[i];

				////1. 이녀석의 부모한테서 새로 연결
				////2. 원래의 자식들을 새로 부모로
				//CHierarchyNode* pNewNode = vecNodes[i]->Clone();

				//for (auto& pChildNode : m_vecHierarchyNodes[j].second->m_pChildrenNodes)
				//{
				//	pChildNode->m_pParent = pNewNode;
				//}

				//pNewNode->m_pChildrenNodes.clear();
				//pNewNode->m_pChildrenNodes = m_vecHierarchyNodes[j].second->m_pChildrenNodes;\
				//
				//pNewNode->m_pParent = m_vecHierarchyNodes[j].second->m_pParent;

				//if (pNewNode->m_pParent)
				//	pNewNode->m_iDepth = pNewNode->m_pParent->m_iDepth + 1;

				//SAFE_DELETE(m_vecHierarchyNodes[j].second);
				//m_vecHierarchyNodes[j].second = pNewNode;

				bExist = true;
				break;
			}
		}

		if (bExist)
			continue;

		// 처음 보는 녀석이다.

		if (vecNodes[i]->m_pParent)
		{
			CHierarchyNode* pRealParentNode = Find_HierarchyNode(vecNodes[i]->m_pParent->Get_Name());
			CHierarchyNode* pNewNode = vecNodes[i]->Clone();
			pNewNode->m_pChildrenNodes.clear();
			pNewNode->m_pParent = pRealParentNode;
			pNewNode->m_iDepth = pRealParentNode->m_iDepth+1;
			pRealParentNode->Add_NewChild(vecNodes[i]);

			m_vecHierarchyNodes.push_back(make_pair(iMeshPartType, pNewNode));
		}
		else
		{
			return E_FAIL;
		}

	}

	for (_uint i = 0; i < vecNodes.size(); ++i)
		SAFE_DELETE(vecNodes[i]);


	Sort_HierarchyNode();

	return S_OK;
}