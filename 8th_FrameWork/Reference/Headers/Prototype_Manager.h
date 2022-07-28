#pragma once
#include "Engine_Defines.h"

//���� �������� ������Ʈ �����ÿ��� �� Ŭ���ʿ��� �̸� Add_Prototype�� ���ִ� ������ �ʿ���!!
BEGIN(Engine)

class CGameObject;
class CComponent;

class CPrototype_Manager final
{
	DECLARE_SINGLETON(CPrototype_Manager);

private:
	CPrototype_Manager();
	virtual ~CPrototype_Manager();

public:
	CGameObject* Clone_GameObject(const _uint& _iID);
	CComponent* Clone_Component(const _uint& _iID);

	HRESULT Add_GameObject_Prototype(const _uint& _iID, CGameObject* pGameObject);
	HRESULT Add_Component_Prototype(const _uint& _iID, CComponent* pComponent);

private:
	map<_uint, CGameObject*>	m_GameObject_Prototypes;
	map<_uint, CComponent*>		m_Component_Prototypes;

private:
	CGameObject*		Find_GameObject_Prototype(const _uint& _iID);
	CComponent*			Find_Component_Prototype(const _uint& _iID);

private:
	void			Release();

	friend class CLevel_Manager;
	void			Clear() { Release(); }

};

END