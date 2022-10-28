#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CLight_Manager final
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	CLight_Manager();
	virtual ~CLight_Manager();

public:
	const LIGHTDESC* Get_LightDesc(_uint iIndex);

public:
	HRESULT Add_Light(const LIGHTDESC& LightDesc);
	HRESULT Render_Lights(class CShader* pShader, class CMesh_Rect* pVIBuffer);
	void	Clear_Lights();
	void	Pop_Light();
	HRESULT	Load_Lights(wstring wstrPath);

public:
	void	Update_Lights();
private:
	list<class CLight*>				m_Lights;
	list<LIGHTDESC>				m_ReservedLights;
	typedef list<class CLight*>		LIGHTS;

public:
	void Release();
};

END