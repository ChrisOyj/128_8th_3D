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
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	_ulong					m_iNumVerticesX, m_iNumVerticesZ;

private:
	HRESULT		SetUp_HeightMap(const _tchar* pHeightMapFilePath);
	HRESULT		SetUp_Terrain(_uint iNumVerticesX, _uint iNumVerticesZ);



};

END