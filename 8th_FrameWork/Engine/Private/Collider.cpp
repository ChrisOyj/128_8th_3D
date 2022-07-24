#include "..\Public\Collider.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"

#include "Prototype_Manager.h"

_uint CCollider::g_iNextID = 0;

CCollider::CCollider()
	: m_iID(g_iNextID++)
{
	//ZeroMemory(&m_tObbInfo, sizeof(OBBINFO));
}
CCollider::CCollider(const CCollider& origin)
	: CComponent(origin)
	, m_iID(g_iNextID++)
	, m_bSphere(origin.m_bSphere)
	, m_eColType(origin.m_eColType)
	, m_tObbInfo(origin.m_tObbInfo)
{
}


CCollider::~CCollider()
{
}

CCollider * CCollider::Create(_float3 vSize, COL_TYPE eColType, _float3 vOffsetPos, bool bSphere)
{
	CCollider* pCollider = GET_CLONE(CCollider);

	if (!pCollider)
		return nullptr;

	pCollider->Set_ColType(eColType);
	pCollider->Set_Sphere(bSphere);
	pCollider->m_tObbInfo = OBBINFO(vSize, vOffsetPos);

	if (FAILED(pCollider->Initialize()))
	{
		MSG_BOX("FAILED to Initialize : CCollider");
		return nullptr;
	}

	return pCollider;
}

void CCollider::Set_ObbInfo(const _float3 & _vSize, const _float3 & _vOffsetPos)
{
	m_tObbInfo = OBBINFO(_vSize, _vOffsetPos);
}

HRESULT CCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider::Initialize()
{
	return S_OK;
}

void CCollider::Start()
{
	CCollision_Manager::Get_Instance()->Add_Collider(this);
}

void CCollider::Tick()
{
}

void CCollider::Late_Tick()
{
	m_tObbInfo.Set_float4x4(&m_pOwner->Get_Transform()->Get_WorldMatrix());

}

void CCollider::Release()
{
}
