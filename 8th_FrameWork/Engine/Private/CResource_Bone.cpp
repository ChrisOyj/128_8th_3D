#include "CResource_Bone.h"

#include "CUtility_File.h"

CResource_Bone::CResource_Bone()
{
}

CResource_Bone::~CResource_Bone()
{
	for (auto& elem : m_ChildrenBones)
	{
		SAFE_DELETE(elem);
	}

}

CResource_Bone* CResource_Bone::Create(ifstream* pReadFile, string strName, CResource_Bone* pParent)
{
	CResource_Bone* pResource = new CResource_Bone;

	pResource->m_pParentBone = pParent;

	if (FAILED(pResource->Initialize(pReadFile, strName)))
	{
		Call_MsgBox(L"Failed to Initiailize : CResource_Bone");
		SAFE_DELETE(pResource);
	}

	return pResource;
}

HRESULT CResource_Bone::Initialize(ifstream* pReadFile, string strName)
{
	__super::Initialize(pReadFile, strName);

	pReadFile->read((char*)&m_iNumChildren, sizeof(_uint));
	pReadFile->read((char*)&m_matTransformation, sizeof(_float4x4));

	m_ChildrenBones.reserve(m_iNumChildren);

	for (_uint i = 0; i < m_iNumChildren; ++i)
	{
		CResource_Bone* pChildBone = CResource_Bone::Create(pReadFile, CUtility_File::Read_Text(pReadFile), this);

		if (!pChildBone)
			return E_FAIL;

		m_ChildrenBones.push_back(pChildBone);
	}

	return S_OK;
}
