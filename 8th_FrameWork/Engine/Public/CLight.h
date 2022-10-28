#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CLight final
{
private:
	CLight();
	virtual ~CLight();

	friend class CLight_Manager;

public:
	static CLight* Create(const LIGHTDESC& LightDesc);
	void Release();

public:
	const LIGHTDESC* Get_LightDesc() const {
		return &m_LightDesc;
	}

public:
	HRESULT Initialize(const LIGHTDESC& LightDesc);
	HRESULT Render(class CShader* pShader, class CMesh_Rect* pVIBuffer);

private:
	LIGHTDESC					m_LightDesc;


};

END