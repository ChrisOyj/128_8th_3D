#include "..\Public\Renderer.h"
#include "GameInstance.h"
#include "GameObject.h"

CRenderer::CRenderer()
	: m_eRenderGroup(RENDER_END)
{
}

CRenderer::CRenderer(const CRenderer& _origin)
	: CComponent(_origin)
	, m_eRenderGroup(_origin.m_eRenderGroup)
{
}


CRenderer::~CRenderer()
{
	Release();
}

void CRenderer::Late_Tick()
{
	CRender_Manager::Get_Instance()->Add_Renderer(m_eRenderGroup, this);
}

void CRenderer::Release()
{
}