#pragma once
#include "CResource.h"

BEGIN(Engine)

struct MESH_DESC
{
	_uint				iMaterialIndex = 0;
	_uint				iNumVertices = 0;
	_uint				iNumFaces = 0;

	VTXMODEL*			pVertices = nullptr;

	MODEL_TYPE			eModelType = TYPE_NONANIM;

	_float4				vCenterPos;
	_float				fMaxRange = 0.f;


/*For Anim*/
	VTXANIM*		pAnimVertices = nullptr;
	_uint			iNumBones = 0;
	BONE_DATA*		pBoneDatas = nullptr;

	FACEINDICES32*	pIndices = nullptr;

};

class CResource_Mesh
	: public CResource
{
private:
	CResource_Mesh();
	virtual ~CResource_Mesh();

public:
	static CResource_Mesh* Create(ifstream* pReadFile, string strName);

public:
	MESH_DESC& Get_MeshDesc() { return m_tMeshDesc; }

public:
	// CResource을(를) 통해 상속됨
	virtual HRESULT Initialize(ifstream* pReadFile, string strName) override;

private:
	MESH_DESC	m_tMeshDesc;
};

END