#include "GameObject.h"

#include "Transform.h"
#include "GameInstance.h"
#include "Physics.h"

#include "MeshRenderer.h"
#include "Collider.h"

#include "Mesh.h"
#include "ParticleSystem.h"

#pragma region Constructor, Destructor
CGameObject::CGameObject()
{
	/* GameObject must needs a Transform Component. */

	m_pTransform = CTransform::Create();

	Add_Component(m_pTransform);
}

CGameObject::CGameObject(const CGameObject & Prototype)
	: m_pParent(Prototype.m_pParent)
{
	//Deep Copy for Component
	for (auto& elem : Prototype.m_pComponents)
		Add_Component(elem->Clone());

	m_pTransform = static_cast<CTransform*>(m_pComponents.front());

	//Deep Copy for Children
	for (auto& elem : Prototype.m_pChildren)
		Add_Child(elem->Clone());
}

CGameObject::~CGameObject()
{
	Release();
}
#pragma endregion

CGameObject * CGameObject::Get_RootParent()
{
	if (!m_pParent)
		return this;
	else
	{
		return m_pParent->Get_RootParent();
	}
}

void CGameObject::Set_Enable(bool bEnable)
{
	if (m_bEnable == bEnable)
		return;

	(m_bEnable = bEnable) ? OnEnable() : OnDisable();
}

#pragma region Tick, Late_Tick
void CGameObject::Tick()
{
	//============ My Update ==============
	My_Tick();

	for (auto& elem : m_pComponents)
		elem->Tick();
	//======================================


	//=============== Children's Update ==============
	for (auto& elem : m_pChildren)
	{
		if (elem->IsValid())
			elem->Tick();
	}
	//==============================================
}

void CGameObject::Late_Tick()
{
	//============ My Update ==============
	My_LateTick();
	My_TimerTick();

	for (auto& elem : m_pComponents)
		elem->Late_Tick();
	//======================================

	//=============== Children's Update ==============
	for (auto iter = m_pChildren.begin(); iter != m_pChildren.end();)
	{
		//if Instance is dead, get rid of it from list m_pChildren
		CGameObject* pChild = *iter;

		if (pChild->IsDead())
			iter = m_pChildren.erase(iter);
		else
		{
			if (!pChild->IsDisable())
				pChild->Late_Tick();

			++iter;
		}
	}
	//==============================================

}
#pragma endregion

void CGameObject::Add_Component(CComponent* pComponent)
{
	pComponent->Set_Owner(this);
	m_pComponents.push_back(pComponent);
}

void CGameObject::Add_Child(CGameObject* pChild)
{
	pChild->Set_Parent(this);
	m_pChildren.push_back(pChild);
}

void CGameObject::Start_Components()
{
	for (auto& pComp : m_pComponents)
		pComp->Start();

	for (auto& pChild : m_pChildren)
		pChild->Start_Components();
}

void CGameObject::My_TimerTick()
{
	for (auto TimerIter = m_TimerEvents.begin(); TimerIter != m_TimerEvents.end();)
	{
		TimerIter->fCurTime -= fDT;

		if (TimerIter->fCurTime <= 0.f)
		{
			OnTimerEvent(TimerIter->iEventNum);

			if (TimerIter->bLoop)
			{
				TimerIter->fCurTime = TimerIter->fOriginTime;
				++TimerIter;
				continue;
			}
			else
			{
				TimerIter = m_TimerEvents.erase(TimerIter);
				continue;
			}
		}

		++TimerIter;
	}
}

void CGameObject::Add_Timer(const _float & fTime, const _uint & iEventNum, _bool bLoop)
{
	TIMER_EVENT	tTimerEvent;
	tTimerEvent.fOriginTime = fTime;
	tTimerEvent.fCurTime = tTimerEvent.fOriginTime;
	tTimerEvent.iEventNum = iEventNum;
	tTimerEvent.bLoop = bLoop;

	m_TimerEvents.push_back(tTimerEvent);
}

HRESULT CGameObject::SetUp_Transform(_float3 vScale, _byte parentFlag)
{
	m_pTransform->Set_Scale(vScale);
	m_pTransform->Set_ParentFlag(parentFlag);

	return S_OK;
}

HRESULT CGameObject::SetUp_Collider(_float3 vSize, COL_TYPE eColType, _float3 vOffsetPos, bool bSphere)
{
	CCollider* pCollider = CCollider::Create(vSize, eColType, vOffsetPos, bSphere);

	if (!pCollider)
		return E_FAIL;

	Add_Component(pCollider);

	return S_OK;
}

CMeshRenderer* CGameObject::SetUp_Renderer(PASS_TYPE eType, const MESH_TYPE& eMeshType,
	const D3DXCOLOR& DiffuseColor, RENDERGROUP eGroup, const _float3& vSize, const _float3& vOffsetPos)
{
	CMeshRenderer* pRenderer = CMeshRenderer::Create(this, eType, eMeshType, DiffuseColor, eGroup, vSize, vOffsetPos);

	if (!pRenderer)
		return nullptr;

	Add_Component(pRenderer);

	return pRenderer;
}

CMeshRenderer* CGameObject::SetUp_Renderer(PASS_TYPE eType, CMesh * pMesh, RENDERGROUP eGroup, const _float3& vOffsetPos)
{
	CMeshRenderer* pRenderer = CMeshRenderer::Create(this, eType, pMesh, eGroup, vOffsetPos);

	if (!pRenderer)
		return nullptr;

	Add_Component(pRenderer);

	return pRenderer;
}

HRESULT CGameObject::SetUp_Renderer_Texture(TEX_TYPE eType, const _tchar * pFilePath, const _uint & iNumTextures)
{
	CMeshRenderer* pRenderer = Get_Component<CMeshRenderer>();

	if (!pRenderer)
		return E_FAIL;

	if (FAILED(pRenderer->Bake_Texture(eType, pFilePath, iNumTextures)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameObject::SetUp_Renderer_Texture(CMeshRenderer * pRenderer, TEX_TYPE eType, const _tchar * pFilePath, const _uint & iNumTextures)
{

	if (FAILED(pRenderer->Bake_Texture(eType, pFilePath, iNumTextures)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameObject::SetUp_Physics()
{
	CPhysics* pPhysics = CPhysics::Create();

	if (!pPhysics)
		return E_FAIL;

	Add_Component(pPhysics);

	return S_OK;
}

CParticleSystem* CGameObject::SetUp_ParticleSystem(const _tchar * pTextureFilePath, PARTICLE_PASS_TYPE ePassType,
	PARTICLE_TYPE eParticleType, _float3 vRandomRange_ParticlesPos,
	_float3 vRandomRange_ParticlesDir, _float3 vRandomRange_ParticlesScale, _float4 vRandomRange_ParticlesColor
	,_float fRandomRange_ParticlesSpeed, _float fRandomRange_ParticlesFadeOutSpeed)
{

	CParticleSystem*	pParticleSystem = CParticleSystem::Create(this, pTextureFilePath, ePassType,
		eParticleType, vRandomRange_ParticlesPos, vRandomRange_ParticlesDir,
		vRandomRange_ParticlesScale, vRandomRange_ParticlesColor, fRandomRange_ParticlesSpeed, fRandomRange_ParticlesFadeOutSpeed);

	if (!pParticleSystem)
		return nullptr;

	Add_Component(pParticleSystem);


	return pParticleSystem;
}

void CGameObject::Release()
{
	for (auto iter = m_pComponents.begin(); iter != m_pComponents.end(); ++iter)
		(*iter)->Destory_Instance();

	m_pComponents.clear();

	for (auto iter = m_pChildren.begin(); iter != m_pChildren.end(); ++iter)
		(*iter)->Destory_Instance();

	m_pChildren.clear();
}
