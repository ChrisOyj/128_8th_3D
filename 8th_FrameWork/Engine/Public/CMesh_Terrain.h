#pragma once
#include "CMesh.h"

BEGIN(Engine)

class ENGINE_DLL CMesh_Terrain final : public CMesh
{
	DECLARE_PROTOTYPE(CMesh_Terrain);

protected:
	CMesh_Terrain(_uint iGroupIdx);
	virtual ~CMesh_Terrain();

public:
	static CMesh_Terrain* Create(_uint iGroupIdx, const _tchar* pHeightMapFilePath);
	static CMesh_Terrain* Create(_uint iGroupIdx, _uint iNumVerticesX, _uint iNumVerticesZ);

public:
	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(((FACEINDICES32*)m_pIndices)[iIndex]._1,
			((FACEINDICES32*)m_pIndices)[iIndex]._2,
			((FACEINDICES32*)m_pIndices)[iIndex]._3);
	}

	_float4* Get_VerticesColor() { return m_pVerticesColor; }

public:
	void	Map_Vertex(_uint  iIndex, _float4 vPosition, _float4 vColor);
	void	ReMap_Vertices();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	_ulong					m_iNumVerticesX, m_iNumVerticesZ;
	_float4*				m_pVerticesColor = nullptr;

private:
	HRESULT		SetUp_HeightMap(const _tchar* pHeightMapFilePath);
	HRESULT		SetUp_Terrain(_uint iNumVerticesX, _uint iNumVerticesZ);



};

END