#include "..\Public\MeshContainer.h"
#include "HIerarchyNode.h"
#include "Model.h"

CMeshContainer::CMeshContainer(_uint iGroupIndex)
	: CMesh(iGroupIndex)
{
}

CMeshContainer::CMeshContainer(const CMeshContainer & rhs)
	: CMesh(rhs)
{

}

CMeshContainer::~CMeshContainer()
{
	Release();
}

CMeshContainer* CMeshContainer::Create(_uint iGroupIdx, MODEL_TYPE eType, const aiMesh* pAIMesh, class CModel* pModel, _float4x4 TransformMatrix)
{
	CMeshContainer* pInstance = new CMeshContainer(iGroupIdx);

	if (FAILED(pInstance->SetUp_MeshContainer(eType, pAIMesh, pModel, TransformMatrix)))
	{
		Call_MsgBox(L"Failed to SetUp_MeshContainer : CMeshContainer");
		SAFE_DELETE(pInstance);
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CMeshContainer");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CMeshContainer::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CMeshContainer::Initialize()
{
	return S_OK;
}

void CMeshContainer::Release()
{
	m_Bones.clear();
}

HRESULT CMeshContainer::Ready_VertexBuffer_NonAnim(const aiMesh* pAIMesh, _float4x4 TransformMatrix)
{
	m_iStride = sizeof(VTXMODEL);
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXMODEL*			pVertices = new VTXMODEL[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), TransformMatrix.XMLoad()));

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));	
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), TransformMatrix.XMLoad())));

		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));			
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::Ready_VertexBuffer_Anim(const aiMesh* pAIMesh, class CModel* pModel)
{
	m_iStride = sizeof(VTXANIM);
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXANIM*			pVertices = new VTXANIM[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}

	m_iNumBones = pAIMesh->mNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone*		pAIBone = pAIMesh->mBones[i];

		_float4x4	OffSetMatrix;

		memcpy(&OffSetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));		

		CHierarchyNode*		pHierarchyNode = pModel->Find_HierarcyNode(pAIBone->mName.data);
		if (nullptr == pHierarchyNode)
			return E_FAIL;

		pHierarchyNode->Set_OffsetMatrix(OffSetMatrix.Transpose());

		m_Bones.push_back(pHierarchyNode);		

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
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::SetUp_MeshContainer(MODEL_TYPE eType, const aiMesh* pAIMesh, CModel* pModel, _float4x4 TransformMatrix)
{
	m_iMaterialIndex = pAIMesh->mMaterialIndex;


#pragma region VERTEXBUFFER

	HRESULT		hr = 0;

	if (TYPE_NONANIM == eType)
		hr = Ready_VertexBuffer_NonAnim(pAIMesh, TransformMatrix);
	else
		hr = Ready_VertexBuffer_Anim(pAIMesh, pModel);

	if (FAILED(hr))
		return E_FAIL;
#pragma endregion

#pragma region INDEXBUFFER
	m_iIndicesStride = sizeof(FACEINDICES32);
	m_iNumPrimitive = pAIMesh->mNumFaces;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[0];
		pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[1];
		pIndices[i]._3 = pAIMesh->mFaces[i].mIndices[2];
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}
