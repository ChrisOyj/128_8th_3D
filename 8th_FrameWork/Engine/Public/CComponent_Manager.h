#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CComponent;
class CGameObject;

class CComponent_Manager
{
	DECLARE_SINGLETON(CComponent_Manager);

private:
	CComponent_Manager();
	~CComponent_Manager();

public:
	HRESULT	Regist_Component(CComponent* pComponent, const _uint& iGroupIndex);
	void	Clear_All();

public:
	void	Tick();
	void	Late_Tick();

private:
	list <CComponent*>		m_Components[COM_GROUP_END];

};
END
