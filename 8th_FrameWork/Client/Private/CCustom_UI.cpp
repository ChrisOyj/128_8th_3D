#include "stdafx.h"
#include "CCustom_UI.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "CMesh_Rect.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

CCustom_UI::CCustom_UI()
{
}

CCustom_UI::~CCustom_UI()
{
}

HRESULT CCustom_UI::Initialize_Prototype()
{
	Add_Component(CComponent_Factory::Clone_Component<CShader>(this));
	Add_Component<CMesh>(CComponent_Factory::Clone_Component<CMesh_Rect>(this));
	Add_Component(CComponent_Factory::Clone_Component<CRenderer>(this));

	return S_OK;
}

HRESULT CCustom_UI::Initialize()
{
	return S_OK;
}

void CCustom_UI::OnEnable()
{
}

void CCustom_UI::OnDisable()
{
}
