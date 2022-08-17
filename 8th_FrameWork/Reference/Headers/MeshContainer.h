#pragma once
#include "CMesh.h"


BEGIN(Engine)

class CMeshContainer final : public CMesh
{
	DECLARE_PROTOTYPE(CMeshContainer);
	
	friend class CModel;

private:
	CMeshContainer(_uint iGroupIndex);
	CMeshContainer(const CMeshContainer& rhs);
	virtual ~CMeshContainer();

public:
	static CMeshContainer* Create(_uint iGroupIndex, MODEL_TYPE eType, const aiMesh* pAIMesh, class CModel* pModel, _float4x4 TransformMatrix);


public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	void	Release();

public:
	HRESULT Ready_VertexBuffer_NonAnim(const aiMesh* pAIMesh, _float4x4 TransformMatrix);
	HRESULT Ready_VertexBuffer_Anim(const aiMesh* pAIMesh, class CModel* pModel);

private: /* 현재ㅑ 메시컨테이너에게 적용되어야할 머테리얼 인덱스*/ 
	_uint				m_iMaterialIndex = 0;
	_uint				m_iNumBones = 0;
	vector<class CHierarchyNode*>			m_Bones;

private:
	HRESULT	SetUp_MeshContainer(MODEL_TYPE eType, const aiMesh* pAIMesh, class CModel* pModel, _float4x4 TransformMatrix);
};

END