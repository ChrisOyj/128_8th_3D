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
	m_tProj.fFOV = XMConvertToRadians(70.f);
	m_tProj.fNear = 0.2f;
	m_tProj.fFar = 500.f;

	return S_OK;
}

void CCamera::OnEnable()
{
}

void CCamera::OnDisable()
{
}
