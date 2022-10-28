#include "CCollider_Sphere.h"

#include "GameInstance.h"
#include "DebugDraw.h"
#include "Prototype_Manager.h"
#include "GameObject.h"
#include "Transform.h"

#include "HIerarchyNode.h"

CCollider_Sphere::CCollider_Sphere(_uint iGroupID)
	: CCollider(iGroupID)
{
}

CCollider_Sphere::CCollider_Sphere(const CCollider_Sphere& origin)
	: CCollider(origin)
	, m_tColInfo(origin.m_tColInfo)
{
	m_pSphere_Original = new BoundingSphere(_float3(0.f, 0.f, 0.f), m_tColInfo.fRadius);
	m_pSphere = new BoundingSphere(*m_pSphere_Original);
}

CCollider_Sphere::~CCollider_Sphere()
{
	Safe_Delete(m_pSphere_Original);
	Safe_Delete(m_pSphere);

}

CCollider_Sphere* CCollider_Sphere::Create(_uint iGroupID, _float fRadius, const _uint& iColIndex, _float4 vOffsetPos, _float4x4 matTransformation, CHierarchyNode* pRefBone)
{
	CCollider_Sphere* pCollider = new CCollider_Sphere(iGroupID);

	if (!pCollider)
		return nullptr;

	pCollider->Set_ColIndex(iColIndex);
	pCollider->m_tColInfo.vOffsetPos = vOffsetPos;
	pCollider->m_tColInfo.fRadius = fRadius;
	pCollider->m_eColType = CT_SPHERE;
	pCollider->m_matTransformation = matTransformation;
	pCollider->m_pRefBone = pRefBone;
	pCollider->m_pSphere_Original = new BoundingSphere(_float3(0.f, 0.f, 0.f), pCollider->m_tColInfo.fRadius);
	pCollider->m_pSphere = new BoundingSphere(*pCollider->m_pSphere_Original);
	if (FAILED(pCollider->Initialize_Prototype()))
	{
		SAFE_DELETE(pCollider);
		Call_MsgBox(L"Failed to Initialize_Prototype : CCollider_Sphere");
		return nullptr;
	}

	return pCollider;
}

HRESULT CCollider_Sphere::Initialize()
{
	__super::Initialize();

	

	return S_OK;
}

void CCollider_Sphere::Late_Tick()
{
	//owner의 월드매트릭스, 뼈의 매트릭스, transformation 까지 곱해줘야함. ㅋ;
	
	_float4x4 vFinalMatrix;
	vFinalMatrix.Identity();

	_float4 vFinalPos = _float4(0.f, 0.f, 0.f);
	_float4x4 matWorld = m_pOwner->Get_Transform()->Get_WorldMatrix();

	_float4x4 matWorldNoTrans = m_pOwner->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS);
	_float4 vOffsetPos = m_tColInfo.vOffsetPos.MultiplyCoord(matWorldNoTrans);

	//참조 뼈가 있을 경우엔 뼈 행렬 * 트랜스폼 * 월드행렬
	if (m_pRefBone)
	{
		_float4x4 matBone = m_pRefBone->Get_CombinedMatrix();
		matBone *= m_matTransformation;


		//콤바인 위치 * 월드매트릭스
		vFinalPos = (*((_float4*)&matBone.m[WORLD_POS]));
		vFinalPos = vFinalPos.MultiplyCoord(matWorld);
		vFinalPos += vOffsetPos;
	}
	else // 아닌경우 그냥 월드포스 _ 오프세솦스
	{
		vFinalPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS) + vOffsetPos;
	}

	m_tColInfo.vFinalPos = vFinalPos;


	(*((_float4*)&vFinalMatrix.m[WORLD_RIGHT])) *= m_tColInfo.fRadius;
	(*((_float4*)&vFinalMatrix.m[WORLD_UP])) *= m_tColInfo.fRadius;
	(*((_float4*)&vFinalMatrix.m[WORLD_LOOK])) *= m_tColInfo.fRadius;
	(*((_float4*)&vFinalMatrix.m[WORLD_POS])) = m_tColInfo.vFinalPos;
	m_pSphere_Original->Transform(*m_pSphere, vFinalMatrix.XMLoad());

}

#ifdef _DEBUG
HRESULT CCollider_Sphere::Render()
{
	DEVICE_CONTEXT->GSSetShader(nullptr, nullptr, 0);

	DEVICE_CONTEXT->IASetInputLayout(m_pInputLayout.Get());

	m_pEffect->SetWorld(XMMatrixIdentity());


	m_pEffect->SetView(GAMEINSTANCE->Get_CurViewMatrix().XMLoad());
	m_pEffect->SetProjection(GAMEINSTANCE->Get_CurProjMatrix().XMLoad());
	m_pEffect->Apply(PDEVICE_CONTEXT);

	_vector		vColor = m_bCol == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);

	m_pBatch->Begin();

	DX::Draw(m_pBatch, *m_pSphere, vColor);

	m_pBatch->End();

	return S_OK;
}
#endif