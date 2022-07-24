#pragma once
#include "Obj.h"

BEGIN(Engine)

class CComponent;
class CTransform;
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
	: public CObj
{
protected:
	CGameObject();	
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject();

	friend class CObject_Manager;

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
#ifdef _DEBUG
		if (!pComponent)
			MSG_BOX("Failed to Find Component : CGameObject");
#endif

		return pComponent;
	}
	CGameObject*		Get_Parent() { return m_pParent; }
	CGameObject*		Get_RootParent();
	CTransform*			Get_Transform() { return m_pTransform; }
	list<CGameObject*>&		Get_Children() { return m_pChildren; }


	void				Set_Parent(CGameObject* pParent) { m_pParent = pParent; }
	void				Set_Enable(bool bEnable);

	/* IsValid : Check the Instance is okay to update. */
	bool			IsValid() { return (m_bAlive && m_bEnable) ? (true) : (false); }
	bool			IsDisable() { return !m_bEnable; }
	bool			IsDead() { return !m_bAlive; }
#pragma endregion

public:
	virtual void	Tick() ;
	virtual void	Late_Tick() ;

public:
	void			Add_Component(CComponent* pComponent);
	void			Add_Child(CGameObject* pChild);

public:
	void		Add_Timer(const _float&	fTime, const _uint& iEventNum, _bool bLoop = false);

#pragma region Collision_Event
public: /* Collision Event */
	virtual void		OnCollisionEnter(CGameObject* pGameObject, COL_TYPE eColType) {}
	virtual void		OnCollisionStay(CGameObject* pGameObject, COL_TYPE eColType) {}
	virtual void		OnCollisionExit(CGameObject* pGameObject, COL_TYPE eColType) {}

	virtual void		OnPickingEvent(const _float3& vPickedPos, const _float3& vPickedNormal = { 0.f,0.f,0.f }) {}
	virtual void		OnTimerEvent(const _uint& iEventNum) {}
#pragma endregion

public:
	void			Start_Components();


protected:
	CTransform*				m_pTransform = nullptr;
	CGameObject*			m_pParent = nullptr;
	list<CGameObject*>		m_pChildren;
	list<TIMER_EVENT>	m_TimerEvents;
	list<CComponent*>	m_pComponents;

protected:
	// Override these for your own code.
	virtual void My_Tick() {}
	virtual void My_LateTick() {}
	void			My_TimerTick();



protected:
	// These will be called by Set_Enable Func.
	virtual	void	OnEnable() PURE;
	virtual	void	OnDisable() PURE;

	// Setup Components
	HRESULT SetUp_Transform(_float3 vScale = _float3(1.f, 1.f, 1.f), _byte parentFlag = 0);
	HRESULT SetUp_Collider(_float3 vSize, COL_TYPE eColType, _float3 vOffsetPos = _float3(0.f, 0.f, 0.f), bool bSphere = false);
	CMeshRenderer* SetUp_Renderer(PASS_TYPE eType, const MESH_TYPE& eMeshType, const D3DXCOLOR& DiffuseColor, RENDERGROUP	eGroup = RENDER_NONALPHABLEND, const _float3& vSize = _float3(1.f, 1.f, 1.f), const _float3& vOffsetPos = _float3(0.f, 0.f, 0.f));
	CMeshRenderer* SetUp_Renderer(PASS_TYPE eType, CMesh* pMesh, RENDERGROUP	eGroup = RENDER_NONALPHABLEND, const _float3& vOffsetPos = _float3(0.f, 0.f, 0.f));
	HRESULT SetUp_Renderer_Texture(TEX_TYPE eType, const _tchar* pFilePath, const _uint& iNumTextures);
	HRESULT SetUp_Renderer_Texture(CMeshRenderer* pRenderer, TEX_TYPE eType, const _tchar* pFilePath, const _uint& iNumTextures);
	HRESULT SetUp_Physics();
	CParticleSystem* SetUp_ParticleSystem(
		const _tchar* pTextureFilePath,	enum PARTICLE_PASS_TYPE	ePassType, 
		enum PARTICLE_TYPE eParticleType, _float3	vRandomRange_ParticlesPos,
		_float3	vRandomRange_ParticlesDir, _float3 vRandomRange_ParticlesScale, _float4	vRandomRange_ParticlesColor,
		_float fRandomRange_ParticlesSpeed, _float fRandomRange_ParticlesFadeOutSpeed
	);

private:
	_bool				m_bEnable = true;
	_bool				m_bAlive = true;

private:
	virtual void		Release();

private:
	/* Only Event_Manager can set this dead. */
	friend class CEvent_Manager;
	void	Set_Dead() { m_bAlive = false; }

	//void				Destory_Instance() { delete this; }
};

END