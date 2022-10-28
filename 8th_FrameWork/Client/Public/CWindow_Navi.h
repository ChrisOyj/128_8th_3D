#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
class CMesh;
END

BEGIN(Client)

class CUnit;
class CTerrain;

struct PICKINGDATA
{
	_int	iNumVerticesX = 0;
	_int	iNumVerticesZ = 0;
	_float	fStepSizeX = 0.f;
	_float	fStepSizeZ = 0.f;
};

class CWindow_Navi
	: public CImGui_Window
{

private:
	CWindow_Navi();
	virtual ~CWindow_Navi();

public:
	static CWindow_Navi* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	PICKINGDATA	m_tPickingData;
	CTerrain* m_pTerrain = nullptr;

	_int	m_iNumVerticesX = 0;
	_int	m_iNumVerticesZ = 0;
	_int	m_iStartIndex = 0;

	string	m_strTerrainName;

	_bool	m_bSelectStartCellMode = false;
	_bool	m_bSelectBlockCellMode = false;

	_bool	m_bSelectCellGrassMode = false;
	_bool	m_bSelectCellGroundMode = false;

private:
	CTerrain*	Create_Terrain(_uint iNumVerticesX, _uint iNumVerticesZ);
	void		Shoot_Ray();

private:
	_bool	Is_Picked(_vector vRayStartPos, _vector vRayDir, CMesh* pMesh, _float4* pOut);
	void	Compute_WorldRay(_float4* pOutPos, _float4* pOutDir);

};
END

