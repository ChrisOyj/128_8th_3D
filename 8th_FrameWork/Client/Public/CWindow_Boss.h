#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CUnit;

class CWindow_Boss
	: public CImGui_Window
{

private:
	CWindow_Boss();
	virtual ~CWindow_Boss();

public:
	static CWindow_Boss* Create();

public:
	void	Set_State(class CState_Boss* pState) { m_pStateBoss = pState; }

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	class CState_Boss* m_pStateBoss = nullptr;
};
END

