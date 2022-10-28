#pragma once
#include "CResource.h"

BEGIN(Engine)

struct MATERIAL_DESC
{
	_float4	vMixColor;
	string	TexturePaths[AI_TEXTURE_TYPE_MAX];
};

class CResource_Material
	: public CResource
{
private:
	CResource_Material();
	virtual ~CResource_Material();

public:
	static CResource_Material* Create(ifstream* pReadFile, string strName);

public:
	MATERIAL_DESC&	Get_MatDesc() {
		return m_tMatDesc;
	};

public:
	// CResource을(를) 통해 상속됨
	virtual HRESULT Initialize(ifstream* pReadFile, string strName) override;

private:
	MATERIAL_DESC	m_tMatDesc;

private:
	_float4 Check_FileName(string strName);

};

END