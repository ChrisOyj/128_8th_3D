#pragma once
#include "Engine_Defines.h"

#define PLANE_EPSILON	1.0f

class CZFrustum
{
	DECLARE_SINGLETON(CZFrustum)

private:
	CZFrustum();
	~CZFrustum();

public:
	_bool	Make_Plane(const _float4x4& matViewProj);
	_bool	IsInPlane(const _float3& vPos);
	_bool	IsInSphere(const _float3& vPos, const _float& fRadius);

private:
	_float3		m_vtx[8];	/// ���������� ������ ���� 8��
	_float3		m_vPos;		/// ���� ī�޶��� ������ǥ
	D3DXPLANE	m_plane[6];	/// ���������� �����ϴ� 6���� ���


};

