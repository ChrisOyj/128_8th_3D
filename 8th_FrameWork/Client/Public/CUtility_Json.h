#pragma once
#include "Client_Defines.h"


BEGIN(Client)

class CUtility_Json
{
public:
    static HRESULT		Load_Json(const _tchar* _strPath, json* pOut);
    static HRESULT		Save_Json(const _tchar* _strPath, json _json);

public:
    template<typename T>
    static HRESULT      Save_Struct(const char* strName, T tStruct, json* pJson)
    {
        _byte  Dest2[sizeof(T)];
        memcpy(&Dest2, &tStruct, sizeof(T));
        pJson->emplace(strName, Dest2);

        return S_OK;
    }

    template<typename T>
    static T            Get_StructFromJson(json _json)
    {
        _byte    Dest[sizeof(T)];

        for (size_t i = 0; i < sizeof(T); ++i)
        {
            Dest[i] = _json[i];
        }

        T tReturnValue;
        memcpy(&tReturnValue, &Dest, sizeof(T));

        return tReturnValue;
    }

public:
    static _float4      Get_VectorFromJson(json _json);
    static _float4x4    Get_MatrixFromJson(json _json);

    static json		    Convert_ToJson(_float4 vValue);
    static json		    Convert_ToJson(_float4x4 matValue);

public:
    static wstring Complete_Path(const _uint& iID);
};

END