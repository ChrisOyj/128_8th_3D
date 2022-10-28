#include "..\Public\CFrustum_Manager.h"

#include "GameInstance.h"

IMPLEMENT_SINGLETON(CFrustum_Manager)

CFrustum_Manager::CFrustum_Manager()
{
}

CFrustum_Manager::~CFrustum_Manager()
{
}

HRESULT CFrustum_Manager::Initialize()
{
	m_vOriginalPoints[0] = _float4(-1.f, 1.f, 0.f, 1.f);
	m_vOriginalPoints[1] = _float4(1.f, 1.f, 0.f, 1.f);
	m_vOriginalPoints[2] = _float4(1.f, -1.f, 0.f, 1.f);
	m_vOriginalPoints[3] = _float4(-1.f, -1.f, 0.f, 1.f);

	m_vOriginalPoints[4] = _float4(-1.f, 1.f, 1.f, 1.f);
	m_vOriginalPoints[5] = _float4(1.f, 1.f, 1.f, 1.f);
	m_vOriginalPoints[6] = _float4(1.f, -1.f, 1.f, 1.f);
	m_vOriginalPoints[7] = _float4(-1.f, -1.f, 1.f, 1.f);

	return S_OK;
}

void CFrustum_Manager::Update()
{
	_matrix			ViewMatrixInv = GAMEINSTANCE->Get_CurViewMatrix().Inverse().XMLoad();
	_matrix			ProjMatrixInv = GAMEINSTANCE->Get_CurProjMatrix().Inverse().XMLoad();


	for (_uint i = 0; i < 8; ++i)
	{
		m_vWorldPoints[i] = XMVector3TransformCoord(XMLoadFloat4(&m_vOriginalPoints[i]), ProjMatrixInv);
		m_vWorldPoints[i] = XMVector3TransformCoord(m_vWorldPoints[i], ViewMatrixInv);
	}

	XMStoreFloat4(&m_vWorldPlane[0], XMPlaneFromPoints(m_vWorldPoints[1], m_vWorldPoints[5], m_vWorldPoints[6]));
	XMStoreFloat4(&m_vWorldPlane[1], XMPlaneFromPoints(m_vWorldPoints[4], m_vWorldPoints[0], m_vWorldPoints[3]));
	XMStoreFloat4(&m_vWorldPlane[2], XMPlaneFromPoints(m_vWorldPoints[4], m_vWorldPoints[5], m_vWorldPoints[1]));
	XMStoreFloat4(&m_vWorldPlane[3], XMPlaneFromPoints(m_vWorldPoints[3], m_vWorldPoints[2], m_vWorldPoints[6]));
	XMStoreFloat4(&m_vWorldPlane[4], XMPlaneFromPoints(m_vWorldPoints[5], m_vWorldPoints[4], m_vWorldPoints[7]));
	XMStoreFloat4(&m_vWorldPlane[5], XMPlaneFromPoints(m_vWorldPoints[0], m_vWorldPoints[1], m_vWorldPoints[2]));


}

_bool CFrustum_Manager::isIn_Frustum_InWorldSpace(_vector vWorldPoint, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vWorldPlane[i]), vWorldPoint)))
		{
			return false;
		}
		/*ax + by + cz + d

		a b c d
		x y z 1*/
	}

	return true;
}

_bool CFrustum_Manager::isIn_Frustum_InLocalSpace(_vector vLocalPoint, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vLocalPlane[i]), vLocalPoint)))
		{
			return false;
		}
	}

	return true;
}

void CFrustum_Manager::Transform_ToLocalSpace(_matrix WorldMatrixInv)
{
	_vector		vLocalPoints[8];

	/* 여덟개 정점을 모두 로컬 스페이스로 변경했다. */
	for (_uint i = 0; i < 8; ++i)
		vLocalPoints[i] = XMVector3TransformCoord(m_vWorldPoints[i], WorldMatrixInv);


	/* 로컬 스페이스 상의 평면 여섯개룰 만드낟. */

	XMStoreFloat4(&m_vLocalPlane[0], XMPlaneFromPoints(vLocalPoints[1], vLocalPoints[5], vLocalPoints[6]));
	XMStoreFloat4(&m_vLocalPlane[1], XMPlaneFromPoints(vLocalPoints[4], vLocalPoints[0], vLocalPoints[3]));
	XMStoreFloat4(&m_vLocalPlane[2], XMPlaneFromPoints(vLocalPoints[4], vLocalPoints[5], vLocalPoints[1]));
	XMStoreFloat4(&m_vLocalPlane[3], XMPlaneFromPoints(vLocalPoints[3], vLocalPoints[2], vLocalPoints[6]));
	XMStoreFloat4(&m_vLocalPlane[4], XMPlaneFromPoints(vLocalPoints[5], vLocalPoints[4], vLocalPoints[7]));
	XMStoreFloat4(&m_vLocalPlane[5], XMPlaneFromPoints(vLocalPoints[0], vLocalPoints[1], vLocalPoints[2]));
}