#include "CCollider_Sphere.h"

#include "Prototype_Manager.h"
#include "GameObject.h"
#include "Transform.h"

CCollider_Sphere::CCollider_Sphere(CGameObject* pOwner)
	: CCollider(pOwner)
{
}

CCollider_Sphere::~CCollider_Sphere()
{
}

CCollider_Sphere* CCollider_Sphere::Create(CGameObject* pOwner, _float fRadius, const _uint& iColIndex, _float4 vOffsetPos)
{
	CCollider_Sphere* pCollider = new CCollider_Sphere(pOwner);

	if (!pCollider)
		return nullptr;

	pCollider->Set_ColIndex(iColIndex);
	pCollider->m_tColInfo.vOffsetPos = vOffsetPos;
	pCollider->m_tColInfo.fRadius = fRadius;
	pCollider->m_eColType = CT_SPHERE;

	if (FAILED(pCollider->Initialize_Prototype()))
	{
		SAFE_DELETE(pCollider);
		Call_MsgBox(L"Failed to Initialize_Prototype : CCollider_Sphere");
		return nullptr;
	}

	return pCollider;
}

void CCollider_Sphere::Late_Tick()
{
	_float4x4 matWorld = m_pOwner->Get_Transform()->Get_WorldMatrix(MATRIX_NOSCALE);
	m_tColInfo.vFinalPos = m_tColInfo.vOffsetPos * matWorld;
}
