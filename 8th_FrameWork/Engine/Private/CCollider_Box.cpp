#include "CCollider_Box.h"

#include "Prototype_Manager.h"
#include "GameObject.h"
#include "Transform.h"

CCollider_Box::CCollider_Box(CGameObject* pOwner)
	: CCollider(pOwner)
{
}

CCollider_Box::~CCollider_Box()
{
}

CCollider_Box* CCollider_Box::Create(CGameObject* pOwner, _float4 vSize, const _uint& iColIndex, _float4 vOffsetPos)
{
	CCollider_Box* pCollider = new CCollider_Box(pOwner);

	if (!pCollider)
		return nullptr;

	pCollider->Set_ColIndex(iColIndex);
	pCollider->m_tColInfo = COL_INFO_BOX(vSize, vOffsetPos);
	pCollider->m_eColType = CT_BOX;

	if (FAILED(pCollider->Initialize_Prototype()))
	{
		SAFE_DELETE(pCollider);
		Call_MsgBox(L"Failed to Initialize_Prototype : CCollider_Box");
		return nullptr;
	}

	return pCollider;
}

void CCollider_Box::Set_BoxInfo(const _float4& _vSize, const _float4& _vOffsetPos)
{
	m_tColInfo = COL_INFO_BOX(_vSize, _vOffsetPos);
}

void CCollider_Box::Late_Tick()
{
	//m_tColInfo.Set_float4x4(&m_pOwner->Get_Transform()->Get_WorldMatrix());

}
