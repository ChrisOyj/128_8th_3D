#include "..\Public\Transform.h"
#include "GameObject.h"

#include "GameInstance.h"

#include "CShader.h"

CTransform::CTransform(_uint iGroupID)
	: CComponent(iGroupID)
{
	ZeroMemory(&m_tTransform, sizeof(TRANSFORM));
	
}


CTransform::~CTransform()
{
}

CTransform* CTransform::Create(_uint iGroupID)
{
	CTransform* pTransform = new CTransform(iGroupID);

	if (FAILED(pTransform->Initialize_Prototype()))
	{
		SAFE_DELETE(pTransform);
		Call_MsgBox(L"Failed to Initialize_Prototype : CTransform");
		return nullptr;
	}

	return pTransform;
}

_float4x4	CTransform::Get_WorldMatrix(const _byte& matrixFlag)
{
	_float4x4	WorldMat = m_tTransform.matWorld;

	if (matrixFlag & MATRIX_NOSCALE)
	{
		(*((_float4*)&WorldMat.m[WORLD_RIGHT])).Normalize();
		(*((_float4*)&WorldMat.m[WORLD_UP])).Normalize();
		(*((_float4*)&WorldMat.m[WORLD_LOOK])).Normalize();
	}

	if (matrixFlag & MATRIX_NOTURN)
	{
		(*((_float4*)&WorldMat.m[WORLD_RIGHT])) = _float4(1.f, 0.f, 0.f);
		(*((_float4*)&WorldMat.m[WORLD_UP])) = _float4(0.f, 1.f, 0.f);
		(*((_float4*)&WorldMat.m[WORLD_LOOK])) = _float4(0.f, 0.f, 1.f);
	}

	if (matrixFlag & MATRIX_TYPEENTITY)
	{
		WorldMat.Identity();
	}

	return WorldMat;
}


_float4 CTransform::Get_World(WORLD eType)
{
	_float4 vResult = *((_float4*)&m_tTransform.matWorld.m[eType]);

	return vResult;
}

_float4 CTransform::Get_MyWorld(WORLD eType)
{
	_float4 vResult = *((_float4*)&m_tTransform.matMyWorld.m[eType]);

	return vResult;
}

void CTransform::Set_World(WORLD eType, const _float4 & vCol)
{
	*((_float4*)&m_tTransform.matMyWorld.m[eType]) = vCol;
}

void CTransform::Set_RealWorld(WORLD eType, const _float4 & vCol)
{
	*((_float4*)&m_tTransform.matWorld.m[eType]) = vCol;

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

	(*((_float4*)&m_tTransform.matMyWorld.m[WORLD_RIGHT])).Normalize() *= m_tTransform.vScale.x;
	(*((_float4*)&m_tTransform.matMyWorld.m[WORLD_UP])).Normalize() *= m_tTransform.vScale.y;
	(*((_float4*)&m_tTransform.matMyWorld.m[WORLD_LOOK])).Normalize() *= m_tTransform.vScale.z;
}


void CTransform::Set_Y(const _float& fY)
{
	(*((_float4*)&m_tTransform.matMyWorld.m[WORLD_POS])).y *= m_tTransform.vScale.z;
}

void CTransform::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	_float4x4		WorldMatrixTP = m_tTransform.matWorld;
	WorldMatrixTP.Transpose();
	pShader->Set_RawValue(pConstantName, &WorldMatrixTP, sizeof(_float4x4));
}

void CTransform::OnCollisionEnter(CGameObject* pGameObject, const _uint& iColType, _float4 vColPoint)
{
}


HRESULT CTransform::Initialize_Prototype()
{
	m_tTransform.vScale = _float4(1.f, 1.f, 1.f, 1.f);
	m_tTransform.matMyWorld.Identity();
	m_tTransform.matWorld.Identity();

	return S_OK;
}

HRESULT CTransform::Initialize()
{
	return S_OK;
}

void CTransform::Start()
{
	__super::Start();

	

}

void CTransform::Tick()
{
	m_pOwner->CallBack_CollisionEnter +=
		bind(&CTransform::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);
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
	__super::OnEnable();

	m_pOwner->CallBack_CollisionEnter +=
		bind(&CTransform::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);

	CShader* pShader = m_pOwner->Get_Component<CShader>()[0];

	pShader->CallBack_SetRawValues +=
		bind(&CTransform::Set_ShaderResource, this, placeholders::_1, "matWorld");
}

void CTransform::OnDisable()
{
	//pShader->CallBack_SetRawValues -= bind(&CTransform::Set_ShaderResource, this, placeholders::_1, "matWorld");

	m_pOwner->CallBack_CollisionEnter -=
		bind(&CTransform::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3);

	CShader* pShader = m_pOwner->Get_Component<CShader>()[0];

	pShader->CallBack_SetRawValues -=
		bind(&CTransform::Set_ShaderResource, this, placeholders::_1, "matWorld");
}

void CTransform::Make_WorldMatrix()
{
	_float4x4	parentMat;
	CGameObject* pParent = m_pOwner->Get_Parent();

	if (pParent)
		parentMat = pParent->Get_Transform()->Get_WorldMatrix(m_cParentFlag);

	m_tTransform.matWorld = m_tTransform.matMyWorld * parentMat;
}
