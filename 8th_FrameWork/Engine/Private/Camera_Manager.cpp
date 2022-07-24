#include "..\Public\Camera_Manager.h"

#include "GameInstance.h"
#include "Transform.h"
#include "Shader_Manager.h"
#include "Camera_Default.h"

CCamera_Manager::CCamera_Manager()
{
	ZeroMemory(&m_tView, sizeof(VIEW_TRANSFORM));
	ZeroMemory(&m_tProj, sizeof(PROJ_TRANSFORM));
}


CCamera_Manager::~CCamera_Manager()
{
}

_float4x4 CCamera_Manager::Get_BillBoard()
{
	_float4x4	matBillBoard = m_tView.matView;
	matBillBoard.Inverse();
	
	*((_float3*)&matBillBoard.m[WORLD_POS]) = _float3(0.f, 0.f, 0.f);

	_float3 InverseLook = *((_float3*)&matBillBoard.m[WORLD_LOOK]) * 1.f;

	*((_float3*)&matBillBoard.m[WORLD_LOOK]) = InverseLook;

	_float3 vUp = { 0.f, 1.f, 0.f };
	_float3 vRight = vUp.Cross(InverseLook);
	*((_float3*)&matBillBoard.m[WORLD_RIGHT]) = vRight;

	vUp = InverseLook.Cross(vRight);
	*((_float3*)&matBillBoard.m[WORLD_UP]) = vUp;


	return matBillBoard;
}

_float3 CCamera_Manager::Get_ViewPos()
{
	return m_pCurCam->Get_Transform()->Get_World(WORLD_POS);
}

HRESULT CCamera_Manager::Initialize(const GRAPHICDESC& GraphicDesc)
{
	_float3 Pos(0.f, 0.f, -5.f);
	_float3 ZeroVector(0.f, 0.f, 0.f);
	m_tView.vUp =  _float3(0.0f, 1.0f, 0.0f);

	m_fAspect = (_float)GraphicDesc.iWinCX / (_float)GraphicDesc.iWinCY;
	D3DXMatrixPerspectiveFovLH(&m_tProj.matProj, D3DXToRadian(90.f),
		m_fAspect,
		1.f, 1000.0f);

	D3DXMatrixLookAtLH(&m_tView.matView, &Pos, &ZeroVector, &m_tView.vUp);

	D3DXMatrixOrthoLH(&m_matOrthoProj, (_float)GraphicDesc.iWinCX, (_float)GraphicDesc.iWinCY, 0.0f, 1.f);
	m_matOrthoView.Identity();

	SetUp_Camera();
	Change_Camera(L"Default");

	return S_OK;
}

void CCamera_Manager::Tick()
{
}

void CCamera_Manager::Late_Tick()
{
	Make_ViewMatrix();
	Make_ProjMatrix();
}

CCamera * CCamera_Manager::Change_Camera(wstring strKey)
{
	auto iter = m_mapCam.find(strKey);

	if (iter == m_mapCam.end())
		return nullptr;

	if (m_pCurCam)
		DISABLE(m_pCurCam);
	
	ENABLE(iter->second);
	m_pCurCam = iter->second;
	m_pCurCam->Late_Tick();
	Make_ViewMatrix();
	Make_ProjMatrix();

	return m_pCurCam;
}

void CCamera_Manager::Add_Camera(wstring strKey, CCamera * pCamera)
{
	m_mapCam.emplace(strKey, pCamera);
	DISABLE(pCamera);
}

void CCamera_Manager::SetUp_Camera()
{
	CCamera*	pDefaultCam = CCamera_Default::Create();
	CREATE_STATIC(pDefaultCam, HASHCODE(CCamera_Default));
	DISABLE(pDefaultCam);
	Add_Camera(L"Default", pDefaultCam);
}

void CCamera_Manager::Make_ViewMatrix()
{

	D3DXMatrixInverse(&m_tView.matView, nullptr, &m_pCurCam->Get_Transform()->Get_WorldMatrix());

	//_float3 vPos = m_pCurCam->Get_Transform()->Get_World(WORLD_POS);
	//_float4 vViewPos = { vPos.x, vPos.y, vPos.z, 1.f };
	//CShader_Manager::Get_Instance()->Set_Parameter("vViewPos", vViewPos, 4);
}

void CCamera_Manager::Make_ProjMatrix()
{
	m_tProj = m_pCurCam->Get_Proj();

	D3DXMatrixPerspectiveFovLH(&m_tProj.matProj, m_tProj.fFOV,
		m_fAspect,
		m_tProj.fNear, m_tProj.fFar);
}
