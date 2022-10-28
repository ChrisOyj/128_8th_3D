#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CMesh_Terrain;
class CCell;
END

BEGIN(Client)
class CTerrain
	: public CGameObject
{
	DECLARE_PROTOTYPE(CTerrain);
	
	friend class CWindow_Navi;

private:
	CTerrain();
	virtual ~CTerrain();

public:
	static CTerrain* Create(_uint iNumVerticesX, _uint iNumVerticesZ);
	static CTerrain* Create(const _tchar* pFilePath);

public:
	CCell* Get_Cell(_uint iIndex) { if (iIndex > m_vecCells.size()) return nullptr; return m_vecCells[iIndex]; }
	CCell* Get_StartCell() { return m_vecCells[m_iStartIndex]; }
	CCell* Get_CenterCell();
	CCell* Find_Cell(_float4 vPosition);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	CMesh_Terrain*	m_pTerrainMesh = nullptr;
	vector<CCell*>	m_vecCells;
	_uint			m_iStartIndex = 0;

	_int			m_iNumCellsX = 0;
	_int			m_iNumCellsZ = 0;

private:
	HRESULT	SetUp_TerrainMesh(_uint iNumVerticesX, _uint iNumVerticesZ);
	HRESULT	SetUp_TerrainMesh(const _tchar* pFilePath);

};
END
