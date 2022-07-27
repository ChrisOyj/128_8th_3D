#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CComponent;
class CTransform;
class CCollider;
class CMesh;
class CParticleSystem;
class CMeshRenderer;
enum MESH_TYPE;
enum PASS_TYPE;

typedef struct tag_TimerEventInfo
{
	_uint iEventNum;
	_float fOriginTime;
	_float fCurTime;
	_bool	bLoop;

}TIMER_EVENT;



class ENGINE_DLL CGameObject abstract
{
protected:
	CGameObject();	
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject();

	friend class CObject_Manager;
	friend class CPrototype_Manager;

public:
	virtual CGameObject* Clone() PURE;

#pragma region GET, SET
public:/*Get, Set*/
	template<typename COMPONENT>
	COMPONENT*			Get_Component()
	{
		COMPONENT* pComponent = nullptr;
		for (auto& elem : m_pComponents)
		{
			pComponent = dynamic_cast<COMPONENT*>(elem);

			if (pComponent)
				break;
		}

		if (!pComponent)
			Call_MsgBox(L"Failed to Find Component : CGameObject");

		return pComponent;
	}
	CGameObject*		Get_Parent() { return m_pParent; }
	CGameObject*		Get_RootParent();
	CTransform*			Get_Transform() { return m_pTransform; }
	CCollider*			Get_Collider() { return m_pCollider; }
	list<CGameObject*>&	Get_Children() { return m_pChildren; }


	void				Set_Parent(CGameObject* pParent) { m_pParent = pParent; }
	void				Set_Enable(_bool bEnable);

	/* Is_Valid : Check the Instance is okay to update. */
	_bool			Is_Valid() { return (m_bAlive && m_bEnable) ? (true) : (false); }
	_bool			Is_Disable() { return !m_bEnable; }
	_bool			Is_Dead() { return !m_bAlive; }
#pragma endregion

public:
	virtual void	Tick();
	virtual void	Late_Tick();

public:
	void			Add_Component(CComponent* pComponent);
	void			Add_Child(CGameObject* pChild);

public:
	void		Add_Timer(const _float&	fTime, const _uint& iEventNum, _bool bLoop = false);

#pragma region Message_Event
public: /* Collision Event */
	virtual void		OnCollisionEnter(CGameObject* pGameObject, const _uint& iColType, _float4 vColPoint) {}
	virtual void		OnCollisionStay(CGameObject* pGameObject, const _uint& iColType) {}
	virtual void		OnCollisionExit(CGameObject* pGameObject, const _uint& iColType) {}

	virtual void		OnPickingEvent(const _float4& vPickedPos, const _float4& vPickedNormal = { 0.f,0.f,0.f }) {}
	virtual void		OnTimerEvent(const _uint& iEventNum) {}

	void				Call_CollisionEnter(CGameObject* pGameObject, const _uint& iColType, _float4 vColPoint);
	void				Call_CollisionStay(CGameObject* pGameObject, const _uint& iColType);
	void				Call_CollisionExit(CGameObject* pGameObject, const _uint& iColType);

	void				Call_PickingEvent(const _float4& vPickedPos, const _float4& vPickedNormal);
	void				Call_TimerEvent(const _uint& iEventNum);

	void				Call_Enable();
	void				Call_Disable();
#pragma endregion

public:
	void			Start_Components();


protected:
	CTransform*				m_pTransform = nullptr;
	CCollider*				m_pCollider = nullptr;
	CGameObject*			m_pParent = nullptr;

	list<CGameObject*>		m_pChildren;
	list<TIMER_EVENT>		m_TimerEvents;
	list<CComponent*>		m_pComponents;

protected:
	virtual	HRESULT	Initialize_Prototype() PURE;
	virtual	HRESULT	Initialize() PURE;
	// These will be called by Set_Enable Func.
	virtual	void	OnEnable() PURE;
	virtual	void	OnDisable() PURE;

private:
	_bool				m_bEnable = true;
	_bool				m_bAlive = true;

private:
	virtual void		Release();

private:
	/* Only Event_Manager can set this dead. */
	friend class CEvent_Manager;
	void	Set_Dead() { m_bAlive = false; }
	void	My_TimerTick();
};

END