#include "stdafx.h"
#include "CUtility_Json.h"

json CUtility_Json::Load_Json(const _tchar* _strPath)
{
	json ProjJson;

	ifstream fin;

	fin.open(_strPath);

	if (fin.is_open())
	{
		fin >> (ProjJson);
	}

	fin.close();


	return ProjJson;
}

void CUtility_Json::Save_Json(const _tchar* _strPath, json _json)
{
	ofstream fout;

	fout.open(_strPath);

	if (fout.is_open())
	{
		fout << _json << std::endl;
	}

	fout.close();
}

_float4 CUtility_Json::Get_VectorFromJson(json _json, const char* strName)
{
	return _float4(_json[strName][0], _json[strName][1], _json[strName][2], _json[strName][3]);
}

const _tchar* CUtility_Json::Complete_Path(const _uint& iID)
{
	wstring strFullPath = L"../Bin/Json/";
	strFullPath += to_wstring(iID);
	strFullPath += L".json";


	return strFullPath.c_str();
}


