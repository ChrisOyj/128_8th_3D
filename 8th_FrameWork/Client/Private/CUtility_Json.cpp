#include "stdafx.h"
#include "CUtility_Json.h"



HRESULT CUtility_Json::Load_Json(const _tchar* _strPath, json* pOut)
{
	json JsonTemp;

	ifstream fin;

	fin.open(_strPath);

	if (fin.is_open())
	{
		fin >> (JsonTemp);
	}
	else
	{
		return E_FAIL;
	}

	fin.close();
	*pOut = JsonTemp;

	return S_OK;
}

HRESULT CUtility_Json::Save_Json(const _tchar* _strPath, json _json)
{
	ofstream fout;

	fout.open(_strPath);

	if (fout.is_open())
	{
		fout << _json << endl;
	}
	else
		return E_FAIL;

	fout.close();

	return S_OK;
}



_float4 CUtility_Json::Get_VectorFromJson(json _json)
{
	if (_json.empty())
		return _float4(0.f, 0.f, 0.f, 1.f);

	return _float4(_json[0], _json[1], _json[2], _json[3]);
}

_float4x4 CUtility_Json::Get_MatrixFromJson(json _json)
{
	_float4x4 matResult;

	if (_json.empty())
	{
		matResult.Identity();
	}
	else
	{
		for (int i = 0; i < 4; ++i)
			(*((_float4*)&matResult.m[i])) = Get_VectorFromJson(_json[i]);
	}

	return matResult;
}

json CUtility_Json::Convert_ToJson(_float4 vValue)
{
	json jsonTemp;
	jsonTemp.push_back(vValue.x);
	jsonTemp.push_back(vValue.y);
	jsonTemp.push_back(vValue.z);
	jsonTemp.push_back(vValue.w);
	return jsonTemp;
}

json CUtility_Json::Convert_ToJson(_float4x4 matValue)
{
	json jsonTemp;
	jsonTemp.push_back(matValue.m[0]);
	jsonTemp.push_back(matValue.m[1]);
	jsonTemp.push_back(matValue.m[2]);
	jsonTemp.push_back(matValue.m[3]);

	return jsonTemp;
}

wstring CUtility_Json::Complete_Path(const _uint& iID)
{
	wstring strFullPath = L"../Bin/Json/";
	strFullPath += to_wstring(iID);
	strFullPath += L".json";


	return strFullPath;
}


