#pragma once
#include "Engine_Defines.h"

/* This Manager has all Renderer Instances and execute all*/

BEGIN(Engine)

class CRenderer;
class CMeshRenderer;

class ENGINE_DLL CRender_Manager
{
	DECLARE_SINGLETON(CRender_Manager);

private:
	CRender_Manager();
	virtual ~CRender_Manager() = default;

public:
	HRESULT		Add_Renderer(RENDER_GROUP eGroup, CRenderer* pRenderer);

public:
	HRESULT		Render();

private:
	list<CRenderer*>							m_Renderers[RENDER_END];
	list<pair<_float, CRenderer*>>				m_pAlphaRenderers;

	

private:
	HRESULT		Render_Group(RENDER_GROUP eGroup);
	void		Sort_AlphaList();
	void		Sort_UIGroup();

	
};

END