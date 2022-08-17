#pragma once
#include "Client_Defines.h"

class CFunctor
{
public:
	static _float4	Get_MousePos(void);

	static _float4	OrthoToRealPosition(const _float4& vPosition);
	static _float4	RealToOrthoPosition(const _float4& vPosition);

	static _float	Get_DegreeFromDot(const _float4& V1, const _float4& V2);

	static string	To_String(wstring wstrText);
	static wstring	To_Wstring(string wstrText);
};

