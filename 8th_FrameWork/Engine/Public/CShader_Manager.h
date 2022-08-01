#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CShader_Manager
{
	DECLARE_SINGLETON(CShader_Manager);

private:
	CShader_Manager();
	~CShader_Manager();

public:
	HRESULT		Add_Effect(ComPtr<ID3DX11Effect>	pEffect);
	HRESULT		Set_RawValue(const _uint& iIndex, const char* pConstantName, void* pData, _uint iDataSize);

private:
	vector<ComPtr<ID3DX11Effect>>	m_vecEffects;

};

END