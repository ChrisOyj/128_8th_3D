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
	template<typename T>
	vector<T*>			Get_Component()
	{
		vector<T*> vecComponents;

		for (auto& elem : m_pComponents)
		{
			T* pComponent = dynamic_cast<T*>(elem);

			if (pComponent)
				vecComponents.push_back(pComponent);
		}

		//if (vecComponents.empty())
			//Call_MsgBox(L"Failed to Find Component : CGameObject");

		return vecComponents;
	}

	CGameObject*		Get_Parent() { return m_pParent; }
	CGameObject*		Get_RootParent();
	CTransform*			Get_Transform() { return m_pTransform; }
	CCollider*			Get_Collider() { return m_pCollider; }
	list<CGameObject*>&	Get_Children() { return m_pChildren; }
	_uint				Get_ID() { return m_iID; }

	void				Set_Parent(CGameObject* pParent) { m_pParent = pParent; }
	void				Set_Enable(_bool bEnable);
	void				Set_ID(const _uint& iID) { m_iID = iID; }

	/* Is_Valid : Check the Instance is okay to update. */
	_bool			Is_Valid() { return (m_bAlive && m_bEnable) ? (true) : (false); }
	_bool			Is_Disable() { return !m_bEnable; }
	_bool			Is_Dead() { return !m_bAlive; }
#pragma endregion

public:
	virtual	HRESULT	Initialize_Prototype() PURE;
	virtual	HRESULT	Initialize() PURE;
	virtual void	Tick();
	virtual void	Late_Tick();

public:
	void			Add_Component(CComponent* pComponent);
	void			Add_Child(CGameObject* pChild);
	void			Start_Components();

#pragma region Message_Event

public:
	CDelegate<CGameObject*, const _uint&, _float4>	CallBack_CollisionEnter;
	CDelegate<CGameObject*, const _uint&>			CallBack_CollisionStay;
	CDelegate<CGameObject*, const _uint&>			CallBack_CollisionExit;
	CDelegate<const _float4&, const _float4&>		CallBack_PickingEvent;
	CDelegate<const _uint&>							CallBack_TimerEvent;

public: /* Event */
	void				Call_Enable();
	void				Call_Disable();
	void				Call_Dead();
#pragma endregion

protected:
	CTransform*				m_pTransform = nullptr;
	CCollider*				m_pCollider = nullptr;
	CGameObject*			m_pParent = nullptr;

	list<CGameObject*>		m_pChildren;
	list<CComponent*>		m_pComponents;

	_uint					m_iID = 0;

protected:
	// These will be called by Set_Enable Func.
	virtual	void	OnEnable();
	virtual	void	OnDisable();

private:
	_bool				m_bEnable = true;
	_bool				m_bAlive = true;

	

private:
	virtual void		Release();

private:
	/* Only Event_Manager can set this dead. */
	friend class CEvent_Manager;
	void	Set_Dead() { m_bAlive = false; }
	void	Destroy_Instance() { delete this; }
};

END