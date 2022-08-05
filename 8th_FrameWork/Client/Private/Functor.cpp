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

_bool CFunctor::Check_GameObject_Prototype_Exist(const _uint& iID)
{
	if (CGameInstance::Get_Instance()->Find_GameObject_Prototype(iID))
		return true;

	return false;
}

_bool CFunctor::Check_Component_Prototype_Exist(const _uint& iID)
{
	if (CGameInstance::Get_Instance()->Find_Component_Prototype(iID))
		return true;

	return false;
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
