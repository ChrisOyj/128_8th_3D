#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CCamera;

class ENGINE_DLL CCamera_Manager final
	: public CManager
{
	DECLARE_SINGLETON(CCamera_Manager);

private:
	CCamera_Manager();
	virtual ~CCamera_Manager();

public:
	_float4x4	Get_View() { return m_tView.matView; }
	_float4x4	Get_Proj() { return m_tProj.matProj; }
	_float4x4	Get_ViewProj() { return m_tView.matView * m_tProj.matProj; }
	_float4x4	Get_BillBoard();
	_float4x4	Get_OrthoView() { return m_matOrthoView; }
	_float4x4	Get_OrthoProj() { return m_matOrthoProj; }

	CCamera* Get_CurCam() { return m_pCurCam; }
	_float3 Get_ViewPos();

public:
	HRESULT	Initialize(const GRAPHICDESC& GraphicDesc);
	void	Tick();
	void	Late_Tick();
	
public:
	CCamera*	Change_Camera(wstring strKey);
	void		Add_Camera(wstring strKey, CCamera* pCamera);

private:
	VIEW_TRANSFORM			m_tView;
	PROJ_TRANSFORM			m_tProj;
	_float					m_fAspect = 0.f;


	CCamera*				m_pCurCam = nullptr;
	map<wstring, CCamera*>	m_mapCam;

private:
	_float4x4										m_matOrthoView;
	_float4x4										m_matOrthoProj;

private:
	void	SetUp_Camera();
	void	Make_ViewMatrix();
	void	Make_ProjMatrix();
};

END