#include "..\Public\Collider.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"

#include "Collision_Manager.h"
#include "Prototype_Manager.h"

#include "DebugDraw.h"


_uint	CCollider::g_iNextID = 0;


CCollider::CCollider(_uint iGroupID)
	: CComponent(iGroupID)
	, m_iID(g_iNextID++)
{
	//ZeroMemory(&m_tObbInfo, sizeof(OBBINFO));
}

CCollider::CCollider(const CCollider& origin)
	: CComponent(origin)
	, m_iID(g_iNextID++)
	, m_isCloned(true)
	, m_eColType(origin.m_eColType)
	, m_matTransformation(origin.m_matTransformation)
#ifdef _DEBUG
	, m_pEffect(origin.m_pEffect)
	, m_pBatch(origin.m_pBatch)
	, m_pInputLayout(origin.m_pInputLayout)
#endif
{
}

CCollider::~CCollider()
{
#ifdef _DEBUG
	if (false == m_isCloned)
	{
		Safe_Delete(m_pEffect);
		Safe_Delete(m_pBatch);
	}
#endif
}

void CCollider::Set_Col(_bool bCol)
{
	if (bCol)
		++m_iColCount;
	else
		--m_iColCount;

	if (m_iColCount > 0)
		m_bCol = true;
	else
		m_bCol = false;
}


HRESULT CCollider::Initialize_Prototype()
{
#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(PDEVICE_CONTEXT);
	m_pEffect = new BasicEffect(PDEVICE);
	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	if (FAILED(DEVICE->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, m_pInputLayout.GetAddressOf())))
		return E_FAIL;

#endif // _DEBUG


	return S_OK;
}

HRESULT CCollider::Initialize()
{
	return S_OK;
}



void CCollider::Start()
{
	__super::Start();
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
	__super::OnEnable();

	CCollision_Manager::Get_Instance()->Regist_Collider(this, m_iColIndex);

}

void CCollider::OnDisable()
{
	__super::OnDisable();

}
