#include "..\Public\HIerarchyNode.h"
#include "HIerarchyNode.h"

CHierarchyNode::CHierarchyNode()
{
}

CHierarchyNode::~CHierarchyNode()
{
	Release();
}

CHierarchyNode* CHierarchyNode::Create(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth)
{
	CHierarchyNode* pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Initialize(pAINode, pParent, iDepth)))
	{
		Call_MsgBox(L"Failed to Created : CHierarchyNode");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CHierarchyNode::Initialize(aiNode * pAINode, CHierarchyNode* pParent, _uint iDepth)
{
	m_iDepth = iDepth;

	m_pParent = pParent;

	strcpy_s(m_szName, pAINode->mName.data);

	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_TransformationMatrix, 
		XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

void CHierarchyNode::Release()
{
	
}

