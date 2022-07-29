#include "CMesh.h"

CMesh::CMesh(CGameObject* pOwner)
    : CComponent(pOwner)
{
}

CMesh::CMesh(const CMesh& _origin)
    : CComponent(_origin)
{
}

CMesh::~CMesh()
{
}

void CMesh::Tick()
{
}

void CMesh::Late_Tick()
{
}

HRESULT CMesh::Render()
{
    return E_NOTIMPL;
}

void CMesh::Release()
{
}
