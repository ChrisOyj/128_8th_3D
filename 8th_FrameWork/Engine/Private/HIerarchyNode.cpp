#include "..\Public\HIerarchyNode.h"
#include "HIerarchyNode.h"

#include "CResource_Bone.h"
#include "Model.h"

CHierarchyNode::CHierarchyNode()
{
}

CHierarchyNode::~CHierarchyNode()
{
	Release();
}

CHierarchyNode* CHierarchyNode::Create(CResource_Bone* pResource, CHierarchyNode* pParent, _uint iDepth)
{
	CHierarchyNode* pInstance = new CHierarchyNode();

	if (FAILED(pInstance->Initialize(pResource, pParent, iDepth)))
	{
		Call_MsgBox(L"Failed to Created : CHierarchyNode");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CHierarchyNode::Add_NewChild(CHierarchyNode* pNode)
{
	if (!pNode->m_pParent)
		return E_FAIL;

	for (_uint i = 0; i < m_pChildrenNodes.size(); ++i)
	{
		if (!strcmp(m_pChildrenNodes[i]->Get_Name(), pNode->Get_Name()))
		{
			return E_FAIL;
		}
	}

	m_pChildrenNodes.push_back(pNode);


	return S_OK;
}

void CHierarchyNode::ReFind_Parent(CModel* pModel)
{
	if (m_pParent)
	{
		m_pParent = pModel->Find_HierarchyNode(m_pParent->Get_Name());
	}
}

void CHierarchyNode::Get_AllNodes(vector<CHierarchyNode*>& vecNodes)
{
	vecNodes.push_back(this);

	for (auto& elem : m_pChildrenNodes)
	{
		elem->Get_AllNodes(vecNodes);
	}
}


HRESULT CHierarchyNode::Initialize(CResource_Bone* pResource, CHierarchyNode* pParent, _uint iDepth)
{
	m_iDepth = iDepth;
	m_pParent = pParent;

	strcpy_s(m_szName, pResource->Get_Name().c_str());

	m_TransformationMatrix = pResource->Get_TransformationMatrix();

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_TransformationMatrix,
		XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	vector<CResource_Bone*>& vecChildrenBones = pResource->Get_ChildrenBones();

	for (_uint i = 0; i < pResource->Get_NumChildren(); ++i)
	{
		CHierarchyNode* pNode = CHierarchyNode::Create(vecChildrenBones[i], this, m_iDepth + 1);

		if (!pNode)
			return E_FAIL;

		m_pChildrenNodes.push_back(pNode);
	}

	return S_OK;
}

void CHierarchyNode::Update_CombinedTransformationMatrix()
{
	if (nullptr == m_pParent)
		m_CombinedTransformationMatrix = m_TransformationMatrix;

	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
}

void CHierarchyNode::Release()
{
	
}

