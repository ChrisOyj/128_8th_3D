#pragma once
#include "Engine_Defines.h"

/* This Manager has all Renderer Instances and execute all*/

BEGIN(Engine)

class CRenderer;
class CMeshRenderer;

class ENGINE_DLL CRender_Manager final : public CManager
{
	DECLARE_SINGLETON(CRender_Manager);

private:
	CRender_Manager();
	virtual ~CRender_Manager() = default;

public:
	void		Set_CurLightDir(_float3 vLightDir);

public:
	HRESULT		Add_Renderer(RENDERGROUP eGroup, CRenderer* pRenderer);

public:
	HRESULT		Render();

private:
	list<CRenderer*>							m_Renderers[RENDER_END];
	list<pair<_float, CMeshRenderer*>>			m_AlphaRenderers;

	

private:
	HRESULT		Render_Group(RENDERGROUP eGroup);
	void		Sort_AlphaList();

	
};

END