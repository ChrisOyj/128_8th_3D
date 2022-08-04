#pragma once
#include "Client_Defines.h"


BEGIN(Client)

class CUtility_Json
{
public:
    static HRESULT		Load_Json(const _tchar* _strPath, json* pOut);
    static HRESULT		Save_Json(const _tchar* _strPath, json _json);

public:
    static _float4      Get_VectorFromJson(json _json);
    static _float4x4    Get_MatrixFromJson(json _json);

    static json		    Convert_ToJson(_float4 vValue);
    static json		    Convert_ToJson(_float4x4 matValue);

public:
    static wstring Complete_Path(const _uint& iID);
};

END