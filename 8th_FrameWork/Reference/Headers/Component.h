#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CComponent abstract
{
	friend class CEvent_Manager;
	friend class CGameObject;
	friend class CPrototype_Manager;


protected:
	CComponent();
	virtual ~CComponent();

public:
	virtual CComponent* Clone() PURE;

public:
	CGameObject*	Get_Owner() { return m_pOwner; }
	void	Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }

	_bool				Is_Valid() { return (m_bAlive && m_bEnable) ? (true) : (false); }
	_bool				Is_Dead() { return !m_bAlive; }
	_bool				Is_Disable() { return !m_bEnable; }

public:
	virtual void	Start() {}
	virtual void	Tick() PURE;
	virtual void	Late_Tick() PURE;

protected:
	CGameObject*	m_pOwner = nullptr;

protected:
	virtual	HRESULT	Initialize_Prototype() PURE;
	virtual	HRESULT	Initialize() PURE;
	
	virtual	void	OnEnable() PURE;
	virtual	void	OnDisable() PURE;

	virtual	void	Release() PURE;

private:
	_bool				m_bEnable = true;
	_bool				m_bAlive = true;

private:
	void	Set_Dead() { m_bAlive = false; }
	void	Set_Enable(_bool bEnable);

};

END