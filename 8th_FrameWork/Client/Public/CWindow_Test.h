#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CUnit;

class CWindow_Test
	: public CImGui_Window
{

private:
	CWindow_Test();
	virtual ~CWindow_Test();

public:
	static CWindow_Test* Create();

public:
	void SetUp_TestWindow(CUnit* pPlayer, class CScript_FollowCam* pFollowCamCom);

	void	Set_Dragon(CEffect* pEffect) { m_pDragon = pEffect; }

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	CUnit* m_pPlayer = nullptr;
	class CEffect* m_pDragon = nullptr;
	class CScript_FollowCam* m_pFollowCamCom = nullptr;
};
END

