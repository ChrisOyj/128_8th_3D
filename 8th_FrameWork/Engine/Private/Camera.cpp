#include "..\Public\Camera.h"



CCamera::CCamera()
{
	ZeroMemory(&m_tProj, sizeof(PROJ_TRANSFORM));
}

CCamera::~CCamera()
{
}

HRESULT CCamera::Initialize_Prototype()
{
	m_tProj.fFOV = XMConvertToRadians(45.f);
	m_tProj.fNear = 0.05f;
	m_tProj.fFar = 1500.f;

	return S_OK;
}

void CCamera::OnEnable()
{
	__super::OnEnable();
}

void CCamera::OnDisable()
{
	__super::OnDisable();

}
