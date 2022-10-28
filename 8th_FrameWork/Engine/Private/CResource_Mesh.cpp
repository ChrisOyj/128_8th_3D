#include "CResource_Mesh.h"

#include "CUtility_Json.h"

CResource_Mesh::CResource_Mesh()
{
}

CResource_Mesh::~CResource_Mesh()
{
	SAFE_DELETE_ARRAY(m_tMeshDesc.pAnimVertices);
	SAFE_DELETE_ARRAY(m_tMeshDesc.pVertices);
	SAFE_DELETE_ARRAY(m_tMeshDesc.pBoneDatas);
	SAFE_DELETE_ARRAY(m_tMeshDesc.pIndices);
}

CResource_Mesh* CResource_Mesh::Create(ifstream* pReadFile, string strName)
{
	CResource_Mesh* pResource = new CResource_Mesh;

	if (FAILED(pResource->Initialize(pReadFile, strName)))
	{
		Call_MsgBox(L"Failed to Initiailize : CResource_Mesh");
		SAFE_DELETE(pResource);
	}

	return pResource;
}

HRESULT CResource_Mesh::Initialize(ifstream* pReadFile, string strName)
{
	__super::Initialize(pReadFile, strName);

	pReadFile->read((char*)&m_tMeshDesc.eModelType, sizeof(MODEL_TYPE));
	pReadFile->read((char*)&m_tMeshDesc.iNumVertices, sizeof(_uint));
	pReadFile->read((char*)&m_tMeshDesc.iNumFaces, sizeof(_uint));
	pReadFile->read((char*)&m_tMeshDesc.iMaterialIndex, sizeof(_uint));

	if (m_tMeshDesc.eModelType == TYPE_ANIM)
	{
		pReadFile->read((char*)&m_tMeshDesc.iNumBones, sizeof(_uint));

		m_tMeshDesc.pAnimVertices = new VTXANIM[m_tMeshDesc.iNumVertices];
		m_tMeshDesc.pBoneDatas = new BONE_DATA[m_tMeshDesc.iNumBones];

		pReadFile->read((char*)m_tMeshDesc.pBoneDatas, sizeof(BONE_DATA) * m_tMeshDesc.iNumBones);
		pReadFile->read((char*)m_tMeshDesc.pAnimVertices, sizeof(VTXANIM) * m_tMeshDesc.iNumVertices);
	}
	else
	{
		m_tMeshDesc.pVertices = new VTXMODEL[m_tMeshDesc.iNumVertices];
		pReadFile->read((char*)m_tMeshDesc.pVertices, sizeof(VTXMODEL) * m_tMeshDesc.iNumVertices);
	}

	m_tMeshDesc.pIndices = new FACEINDICES32[m_tMeshDesc.iNumFaces];
	pReadFile->read((char*)m_tMeshDesc.pIndices, sizeof(FACEINDICES32) * m_tMeshDesc.iNumFaces);



	pReadFile->read((char*)&m_tMeshDesc.vCenterPos, sizeof(_float4));
	pReadFile->read((char*)&m_tMeshDesc.fMaxRange, sizeof(_float));



	return S_OK;
}
