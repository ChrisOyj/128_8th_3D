#include "..\Public\Collider.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"

#include "Collision_Manager.h"
#include "Prototype_Manager.h"

_uint	CCollider::g_iNextID = 0;


CCollider::CCollider(CGameObject* pOwner)
	: CComponent(pOwner)
	, m_iID(g_iNextID++)
{
	//ZeroMemory(&m_tObbInfo, sizeof(OBBINFO));
}

CCollider::CCollider(const CCollider& origin)
	: CComponent(origin)
	, m_iID(g_iNextID++)
{
}

CCollider::~CCollider()
{
}



HRESULT CCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider::Initialize()
{
	return S_OK;
}

void CCollider::Start()
{
	CCollision_Manager::Get_Instance()->Regist_Collider(this, m_iColIndex);
}

void CCollider::Tick()
{
}

void CCollider::Late_Tick()
{

}

void CCollider::Release()
{
}

void CCollider::OnEnable()
{
}

void CCollider::OnDisable()
{
}
