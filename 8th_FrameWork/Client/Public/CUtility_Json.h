#pragma once
#include "Client_Defines.h"


BEGIN(Client)

class CUtility_Json
{
public:
    static json    Load_Json(const _tchar* _strPath);
    static void    Save_Json(const _tchar* _strPath, json _json);

public:
    static _float4 Get_VectorFromJson(json _json, const char* strName);

public:
    static const _tchar* Complete_Path(const _uint& iID);
};

END