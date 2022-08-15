#include "stdafx.h"
#include "..\Public\Functor.h"

#include "GameInstance.h"

_float4 CFunctor::Get_MousePos(void)
{
	POINT pt{};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return _float4(float(pt.x), float(pt.y), 0.f, 1.f);
}

_float4 CFunctor::OrthoToRealPosition(const _float4& vPosition)
{
	_float4 vScreenPosition = _float4(
		vPosition.x - (g_iWinCX * 0.5f),
		-vPosition.y + (g_iWinCY * 0.5f),
		vPosition.z,
		1.f);

	return vScreenPosition;
}

_float4 CFunctor::RealToOrthoPosition(const _float4& vPosition)
{
	_float4 vScreenPosition = _float4(
		vPosition.x + (g_iWinCX * 0.5f),
		-vPosition.y + (g_iWinCY * 0.5f),
		vPosition.z,
		1.f);

	return vScreenPosition;
}

_float CFunctor::Get_DegreeFromDot(const _float4& V1, const _float4& V2)
{
	_float4	vLeft = V1;
	_float4	vRight = V2;

	vLeft.Normalize();
	vRight.Normalize();

	_float fCosTheta = vLeft.Dot(V2);
	_float fTheta = acosf(fCosTheta);

	if (vLeft.y < 0)
		fTheta *= -1.f;

	_float fDegree = XMConvertToDegrees(fTheta);

	return fDegree;
}
