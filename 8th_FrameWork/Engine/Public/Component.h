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
	CComponent(_uint iGroupID);
	virtual ~CComponent();

public:
	virtual CComponent* Clone() PURE;

public:
	CGameObject*	Get_Owner() { return m_pOwner; }
	void			Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }
	_uint			Get_GroupID() { return m_iGroupID; }
	void			Set_GroupID(const _uint& iGroupID) { m_iGroupID = iGroupID; }

	_uint			Get_ComponentID() { return m_iComponentID; }
	void			Set_ComponentID(_uint iID) { m_iComponentID = iID; }

	_bool				Is_Valid() { return (m_bAlive && m_bEnable) ? (true) : (false); }
	_bool				Is_Dead() { return !m_bAlive; }
	_bool				Is_Disable() { return !m_bEnable; }


public:
	virtual	HRESULT	Initialize_Prototype() PURE;
	virtual	HRESULT	Initialize() PURE;
	virtual void	Start() { OnEnable(); }
	virtual void	Tick() PURE;
	virtual void	Late_Tick() PURE;

public:
	virtual void		OnCollisionEnter(CGameObject* pGameObject, const _uint& iColType, _float4 vColPoint) {}
	virtual void		OnCollisionStay(CGameObject* pGameObject, const _uint& iColType) {}
	virtual void		OnCollisionExit(CGameObject* pGameObject, const _uint& iColType) {}

	virtual void		OnPickingEvent(const _float4& vPickedPos, const _float4& vPickedNormal = { 0.f,0.f,0.f }) {}
	virtual void		OnTimerEvent(const _uint& iEventNum) {}

public:
	virtual void		Set_ShaderResource(class CShader* pShader, const char* pConstantName) {}


protected:
	CGameObject*	m_pOwner = nullptr;
	_uint			m_iGroupID = 0;
	_uint			m_iComponentID = 0;

protected:
	virtual	void	OnEnable();
	virtual	void	OnDisable();

	virtual	void	Release() PURE;

private:
	_bool				m_bEnable = true;
	_bool				m_bAlive = true;

private:
	void	Set_Dead() { m_bAlive = false; }
	void	Set_Enable(_bool bEnable);
	void	Destroy_Instance() { delete this; }


};

END