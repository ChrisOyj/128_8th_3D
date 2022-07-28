#include "..\Public\Transform.h"
#include "GameObject.h"

#include "GameInstance.h"

CTransform::CTransform(CGameObject* pOwner)
	: CComponent(pOwner)
{
	ZeroMemory(&m_tTransform, sizeof(TRANSFORM));
	m_tTransform.vScale = { 1.f, 1.f, 1.f };
	m_tTransform.matMyWorld.Identity();
	m_tTransform.matWorld.Identity();
}


CTransform::~CTransform()
{
}

CTransform* CTransform::Create(CGameObject* pOwner)
{
	return new CTransform(pOwner);
}

_float4x4	CTransform::Get_WorldMatrix(const _byte& matrixFlag)
{
	_float4x4	WorldMat = m_tTransform.matWorld;

	if (matrixFlag & MATRIX_NOSCALE)
	{
		(*((_float4*)&WorldMat.r[WORLD_RIGHT])).Normalize();
		(*((_float4*)&WorldMat.r[WORLD_UP])).Normalize();
		(*((_float4*)&WorldMat.r[WORLD_LOOK])).Normalize();
	}

	if (matrixFlag & MATRIX_NOTURN)
	{
		(*((_float4*)&WorldMat.r[WORLD_RIGHT])) = _float4(1.f, 0.f, 0.f);
		(*((_float4*)&WorldMat.r[WORLD_UP])) = _float4(0.f, 1.f, 0.f);
		(*((_float4*)&WorldMat.r[WORLD_LOOK])) = _float4(0.f, 0.f, 1.f);
	}

	if (matrixFlag & MATRIX_TYPEENTITY)
	{
		WorldMat.Identity();
	}

	return WorldMat;
}


_float4 CTransform::Get_World(WORLD eType)
{
	_float4 vResult = *((_float4*)&m_tTransform.matWorld.r[eType]);

	return vResult;
}

_float4 CTransform::Get_MyWorld(WORLD eType)
{
	_float4 vResult = *((_float4*)&m_tTransform.matMyWorld.r[eType]);

	return vResult;
}

void CTransform::Set_World(WORLD eType, const _float4 & vCol)
{
	*((_float4*)&m_tTransform.matMyWorld.r[eType]) = vCol;
}

void CTransform::Set_RealWorld(WORLD eType, const _float4 & vCol)
{
	*((_float4*)&m_tTransform.matWorld.r[eType]) = vCol;

}

void CTransform::Set_MyMatrix(_float4x4 matWorld)
{
	m_tTransform.matMyWorld = matWorld;
}

void CTransform::Set_Look(const _float4& vLook)
{
	

	_float4 _vLook = vLook;
	Set_World(WORLD_LOOK, _vLook.Normalize());

	_float4 vUp = { 0.f, 1.f, 0.f };
	if ((vLook.y < 1.1f && vLook.y > 0.9f) ||
		(vLook.y > -1.1f && vLook.y < -0.9f)
		)
		vUp = _float4(0.f, 0.f, 1.f);

	vUp.Normalize();
	_float4 vRight = vUp.Cross(vLook);
	Set_World(WORLD_RIGHT, vRight.Normalize());
	
	vUp = _vLook.Cross(vRight);
	Set_World(WORLD_UP, vUp.Normalize());
}

void CTransform::Set_Rect()
{

	_float4	vRight;
	_float4 vUp;
	_float4	vLook;

	vRight = Get_World(WORLD_RIGHT);
	vUp =	Get_World(WORLD_UP);
	vLook = Get_World(WORLD_LOOK);

	Set_World(WORLD_RIGHT,	vRight  * -1);
	Set_World(WORLD_UP,		vUp);
	Set_World(WORLD_LOOK,	vLook * -1);
}

void	CTransform::Set_Scale(const _float4& vScale)
{
	m_tTransform.vScale = vScale;

	(*((_float4*)&m_tTransform.matMyWorld.r[WORLD_RIGHT])).Normalize() *= m_tTransform.vScale.x;
	(*((_float4*)&m_tTransform.matMyWorld.r[WORLD_UP])).Normalize() *= m_tTransform.vScale.y;
	(*((_float4*)&m_tTransform.matMyWorld.r[WORLD_LOOK])).Normalize() *= m_tTransform.vScale.z;
}


void CTransform::Set_Y(const _float& fY)
{
	(*((_float4*)&m_tTransform.matMyWorld.r[WORLD_POS])).y *= m_tTransform.vScale.z;
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
}

void CTransform::Late_Tick()
{
	Make_WorldMatrix();
}

void CTransform::Release()
{
}

void CTransform::OnEnable()
{
}

void CTransform::OnDisable()
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
