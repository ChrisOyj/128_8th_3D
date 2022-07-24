#include "..\Public\Transform.h"
#include "GameObject.h"

#include "GameInstance.h"
#include "Prototype_Manager.h"

CTransform::CTransform()
{
	ZeroMemory(&m_tTransform, sizeof(TRANSFORM));
	m_tTransform.vScale = { 1.f, 1.f, 1.f };
	m_tTransform.matMyWorld.Identity();
	m_tTransform.matWorld.Identity();
}


CTransform::~CTransform()
{
}

CTransform * CTransform::Create()
{
	CTransform* pTransform = GET_CLONE(CTransform);
	
	if (!pTransform)
		return nullptr;

	if (FAILED(pTransform->Initialize()))
	{
		MSG_BOX("FAILED to Initialize : CTransform");
		return nullptr;
	}

	return pTransform;
}

_float4x4	CTransform::Get_WorldMatrix(const _byte& matrixFlag)
{
	_float4x4	WorldMat = m_tTransform.matWorld;

	if (matrixFlag & MATRIX_NOSCALE)
	{
		(*((_float3*)&WorldMat.m[WORLD_RIGHT])).Normalize();
		(*((_float3*)&WorldMat.m[WORLD_UP])).Normalize();
		(*((_float3*)&WorldMat.m[WORLD_LOOK])).Normalize();
	}

	if (matrixFlag & MATRIX_NOTURN)
	{
		(*((_float3*)&WorldMat.m[WORLD_RIGHT])) = _float3(1.f, 0.f, 0.f);
		(*((_float3*)&WorldMat.m[WORLD_UP])) = _float3(0.f, 1.f, 0.f);
		(*((_float3*)&WorldMat.m[WORLD_LOOK])) = _float3(0.f, 0.f, 1.f);
	}

	if (matrixFlag & MATRIX_IDENTITY)
	{
		WorldMat.Identity();
	}

	return WorldMat;
}


_float3 CTransform::Get_World(WORLD eType)
{
	_float3 vResult = *((_float3*)&m_tTransform.matWorld.m[eType]);

	return (eType == WORLD_POS) ? vResult : vResult.Normalize();
}

_float3 CTransform::Get_MyWorld(WORLD eType)
{
	_float3 vResult = *((_float3*)&m_tTransform.matMyWorld.m[eType]);

	return (eType == WORLD_POS) ? vResult : vResult.Normalize();
}

void CTransform::Set_World(WORLD eType, const _float3 & vCol)
{
	*((_float3*)&m_tTransform.matMyWorld.m[eType]) = vCol;
}

void CTransform::Set_RealWorld(WORLD eType, const _float3 & vCol)
{
	*((_float3*)&m_tTransform.matWorld.m[eType]) = vCol;

}

void CTransform::Set_MyMatrix(_float4x4 matWorld)
{
	m_tTransform.matMyWorld = matWorld;
}

void CTransform::Set_Look(const _float3& vLook)
{
	

	_float3 _vLook = vLook;
	Set_World(WORLD_LOOK, _vLook.Normalize());

	_float3 vUp = { 0.f, 1.f, 0.f };
	if ((vLook.y < 1.1f && vLook.y > 0.9f) ||
		(vLook.y > -1.1f && vLook.y < -0.9f)
		)
		vUp = _float3(0.f, 0.f, 1.f);

	vUp.Normalize();
	_float3 vRight = vUp.Cross(vLook);
	Set_World(WORLD_RIGHT, vRight.Normalize());
	
	vUp = _vLook.Cross(vRight);
	Set_World(WORLD_UP, vUp.Normalize());
}

void CTransform::Set_Rect()
{

	_float3	vRight;
	_float3 vUp;
	_float3	vLook;

	vRight = Get_World(WORLD_RIGHT);
	vUp =	Get_World(WORLD_UP);
	vLook = Get_World(WORLD_LOOK);

	Set_World(WORLD_RIGHT,	vRight  * -1);
	Set_World(WORLD_UP,		vUp);
	Set_World(WORLD_LOOK,	vLook * -1);
}

void	CTransform::Set_Scale(const _float3& vScale)
{
	m_tTransform.vScale = vScale;
}


void CTransform::Set_Y(const _float& fY)
{
	m_tTransform.matMyWorld._42 = fY;
}


HRESULT CTransform::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTransform::Initialize()
{
	return S_OK;
}

void CTransform::Tick()
{
	(*((_float3*)&m_tTransform.matMyWorld.m[WORLD_RIGHT])).Normalize();
	(*((_float3*)&m_tTransform.matMyWorld.m[WORLD_UP])).Normalize();
	(*((_float3*)&m_tTransform.matMyWorld.m[WORLD_LOOK])).Normalize();
}

void CTransform::Late_Tick()
{
	(*((_float3*)&m_tTransform.matMyWorld.m[WORLD_RIGHT])) *= m_tTransform.vScale.x;
	(*((_float3*)&m_tTransform.matMyWorld.m[WORLD_UP])) *= m_tTransform.vScale.y;
	(*((_float3*)&m_tTransform.matMyWorld.m[WORLD_LOOK])) *= m_tTransform.vScale.z;

	Make_WorldMatrix();
}

void CTransform::Release()
{
}

void CTransform::Make_WorldMatrix()
{
	_float4x4	parentMat;
	CGameObject* pParent = m_pOwner->Get_Parent();

	if (pParent)
		parentMat = pParent->Get_Transform()->Get_WorldMatrix(m_cParentFlag);

	m_tTransform.matWorld = m_tTransform.matMyWorld * parentMat;
}
