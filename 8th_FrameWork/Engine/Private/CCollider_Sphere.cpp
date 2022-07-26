#include "CCollider_Sphere.h"

#include "Prototype_Manager.h"
#include "GameObject.h"
#include "Transform.h"

CCollider_Sphere::CCollider_Sphere()
{
}

CCollider_Sphere::~CCollider_Sphere()
{
}

CCollider_Sphere* CCollider_Sphere::Create(_float fRadius, const _uint& iColIndex, _float4 vOffsetPos)
{
	CCollider_Sphere* pCollider = CLONE_COMPONENT(CCollider_Sphere);

	if (!pCollider)
		return nullptr;

	pCollider->Set_ColIndex(iColIndex);
	pCollider->m_tColInfo.vOffsetPos = vOffsetPos;
	pCollider->m_tColInfo.fRadius = fRadius;
	pCollider->m_eColType = CT_SPHERE;

	if (FAILED(pCollider->Initialize()))
	{
		Call_MsgBox(L"FAILED to Initialize : CCollider_Sphere");
		return nullptr;
	}

	return pCollider;
}

void CCollider_Sphere::Late_Tick()
{
	_float4x4 matWorld = m_pOwner->Get_Transform()->Get_WorldMatrix(MATRIX_NOSCALE);
	m_tColInfo.vFinalPos = m_tColInfo.vOffsetPos * matWorld;
}
