#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CGameObject;
class CUI;

class ENGINE_DLL CPicking_Manager
{
	DECLARE_SINGLETON(CPicking_Manager)

private:
	CPicking_Manager();
	~CPicking_Manager();

public:
	void	Add_Object(CGameObject* pGameObject, _float fDistanceToPlayer);
	void	Add_UI(CUI* pUI);
	void	Execute_Picking() { m_bPicking = true; }
	void	Execute_RandomPicking() { m_bRandomPicking = true; }
public:
	HRESULT	Initialize(HWND hWnd);
	void	Tick();
public:
	_bool	Is3DPicked(CGameObject* pGameObject, _float3*	pOut, _float3* pOutNormal = nullptr);
	_bool	IsOnMouseRay(CGameObject* pGameObject);
	_bool	Is2DPicked(CUI* pGameObject);

	/* Shotgun */
	_bool	IsRandom3DPicked(CGameObject* pGameObject, _float3*	pOut, _float fRange, _float3* pOutNormal = nullptr);

	void	SetRayCenterAnyTime(_bool bBool) { m_bRayCenterAnytime = bBool; }
	_bool	GetRayCenterAnyTime() { return m_bRayCenterAnytime; }

private:
	HWND	m_hWnd;
	list<pair<_float, CGameObject*>>	m_PickingObject;
	list<CUI*>							m_PickingUI;

	D3DVIEWPORT9	m_Viewport;

	_float3			m_vRayDir;
	_float3			m_vRayPos;
	_bool			m_bPicking = false;

	_bool			m_bRandomPicking = false;

	_bool			m_bRayCenterAnytime = false;

private:
	void	Sort_Lists();
	void	Compute_WorldRay();
	void	Picking();

	void	RandomPicking(_float fRange);
};

END