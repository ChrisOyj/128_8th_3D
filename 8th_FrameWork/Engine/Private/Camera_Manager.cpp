#include "..\Public\Camera_Manager.h"

#include "GameInstance.h"
#include "Transform.h"

#include "Camera.h"

#include "CShader_Manager.h"

IMPLEMENT_SINGLETON(CCamera_Manager)


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
	
	*((_float4*)&matBillBoard.m[WORLD_POS]) = _float4(0.f, 0.f, 0.f);

	_float4 InverseLook = *((_float4*)&matBillBoard.m[WORLD_LOOK]) * 1.f;

	*((_float4*)&matBillBoard.m[WORLD_LOOK]) = InverseLook;

	_float4 vUp = { 0.f, 1.f, 0.f };
	_float4 vRight = vUp.Cross(InverseLook);
	*((_float4*)&matBillBoard.m[WORLD_RIGHT]) = vRight;

	vUp = InverseLook.Cross(vRight);
	*((_float4*)&matBillBoard.m[WORLD_UP]) = vUp;


	return matBillBoard;
}

_float4 CCamera_Manager::Get_ViewPos()
{
	return m_pCurCam->Get_Transform()->Get_World(WORLD_POS);
}

HRESULT CCamera_Manager::SetUp_ShaderResources(_bool Ortho)
{
	_float4x4	matView, matProj;

	if (Ortho)
	{
		matView = m_matOrthoView;
		matProj = m_matOrthoProj;
	}
	else
	{
		matView = m_tView.matView;
		matProj = m_tProj.matProj;
	}

	matView.Transpose();
	matProj.Transpose();

	if (FAILED(CShader_Manager::Get_Instance()->Set_RawValue_All("g_ViewMatrix", (&matView), sizeof(_float4x4))))
		return E_FAIL;

	return CShader_Manager::Get_Instance()->Set_RawValue_All("g_ProjMatrix", (&matProj), sizeof(_float4x4));
}

HRESULT CCamera_Manager::Initialize(const GRAPHICDESC& GraphicDesc)
{
	_float4 Pos = _float4(0.f, 0.f, -5.f);
	_float4 ZeroVector = _float4(0.f, 0.f, 0.f);
	m_tView.vUp =  _float4(0.0f, 1.0f, 0.0f, 0.f);

	m_fAspect = (_float)GraphicDesc.iWinCX / (_float)GraphicDesc.iWinCY;

	m_tProj.matProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f),
		m_fAspect,
		1.f, 1000.0f);

	m_tView.matView = XMMatrixLookAtLH(Pos.XMLoad(), ZeroVector.XMLoad(), m_tView.vUp.XMLoad());

	m_matOrthoProj = XMMatrixOrthographicLH((_float)GraphicDesc.iWinCX, (_float)GraphicDesc.iWinCY, 0.0f, 1.f);

	m_matOrthoView.Identity();

	return S_OK;
}

void CCamera_Manager::Make_ViewProj()
{
	Make_ViewMatrix();
	Make_ProjMatrix();
}

CCamera * CCamera_Manager::Change_Camera(wstring strKey)
{
	auto iter = m_mapCam.find(Convert_ToHash(strKey));

	if (iter == m_mapCam.end())
	{
		Call_MsgBox(L"Failed to Find CCamera : CCamera_Manager");
		return nullptr;
	}


	if (m_pCurCam)
		DISABLE_GAMEOBJECT(m_pCurCam);
	
	ENABLE_GAMEOBJECT(iter->second);
	m_pCurCam = iter->second;
	Make_ViewProj();

	return m_pCurCam;
}

void CCamera_Manager::Add_Camera(wstring strKey, CCamera * pCamera)
{
	auto iter = m_mapCam.find(Convert_ToHash(strKey));

	if (iter != m_mapCam.end())
	{
		Call_MsgBox(L"Alreadt Exist Camera : CCamera_Manager");
		return;
	}

	m_mapCam.emplace(Convert_ToHash(strKey), pCamera);
	//DISABLE_GAMEOBJECT(pCamera);
}

void CCamera_Manager::Make_ViewMatrix()
{
	m_tView.matView = m_pCurCam->Get_Transform()->Get_WorldMatrix();
	m_tView.matView.Inverse();
}

void CCamera_Manager::Make_ProjMatrix()
{
	m_tProj = m_pCurCam->Get_Proj();

	m_tProj.matProj = XMMatrixPerspectiveFovLH(m_tProj.fFOV,
		m_fAspect,
		m_tProj.fNear, m_tProj.fFar);
}
