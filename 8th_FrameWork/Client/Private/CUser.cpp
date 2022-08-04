#include "stdafx.h"
#include "CUser.h"
#include "MainApp.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CUser);

CUser::CUser()
{
}
CUser::~CUser()
{
}

HRESULT CUser::Initialize()
{
	POINT ptMouse;

	ptMouse.x = g_iWinCX >> 1;
	ptMouse.y = g_iWinCY >> 1;
	m_ptCenter = ptMouse;

	return S_OK;
}

void CUser::Tick()
{
	Fix_CursorPosToCenter();
}

void CUser::Fix_CursorPosToCenter()
{
	if (GetFocus() != g_hWnd)
		return;

	/* Set Mouse Cursor on Center of Screen */
	POINT ptMouse;
	ptMouse = m_ptCenter;
	::ClientToScreen(g_hWnd, &ptMouse);
	::SetCursorPos(ptMouse.x, ptMouse.y);
}

void CUser::KeyInput_FPSSetter()
{
	_double dCurFPSLimit = CGameInstance::Get_Instance()->Get_FPSLimitTime();

	if (KEY(F3, TAP))
	{
		dCurFPSLimit = 1. / 30.;
	}
	else if (KEY(F2, TAP))
	{
		dCurFPSLimit = 1. / 120.;
	}
	else if (KEY(F1, TAP))
	{
		dCurFPSLimit = 0.;
	}

	CGameInstance::Get_Instance()->Set_FPSLimitTIme(dCurFPSLimit);

}
