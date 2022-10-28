#pragma once
#include "CMesh.h"

BEGIN(Engine)

class ENGINE_DLL CMesh_Cube final : public CMesh
{
	DECLARE_PROTOTYPE(CMesh_Cube);

protected:
	CMesh_Cube(_uint iGroupIDX);
	virtual ~CMesh_Cube();

public:
	virtual _uint3 Get_Indices(_uint iIndex) const override {
		return _uint3(((FACEINDICES16*)m_pIndices)[iIndex]._1,
			((FACEINDICES16*)m_pIndices)[iIndex]._2,
			((FACEINDICES16*)m_pIndices)[iIndex]._3);
	}

public:
	static CMesh_Cube* Create(_uint iGroupIDX);
	static CMesh_Cube* Create_DecalBox(_uint iGroupIDX);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	virtual HRESULT	SetUp_DecalBox();


};

END