#include "stdafx.h"
#include "CEmpty_GameObject.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

CEmpty_GameObject::CEmpty_GameObject()
{
}

CEmpty_GameObject::~CEmpty_GameObject()
{
}


HRESULT CEmpty_GameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEmpty_GameObject::Initialize()
{
	return S_OK;
}

void CEmpty_GameObject::OnEnable()
{
}

void CEmpty_GameObject::OnDisable()
{
}

