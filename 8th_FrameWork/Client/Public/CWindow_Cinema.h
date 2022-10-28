#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CUnit;

class CWindow_Cinema
	: public CImGui_Window
{

private:
	CWindow_Cinema();
	virtual ~CWindow_Cinema();

public:
	static CWindow_Cinema* Create();

public:
	void SetUp_CinemaWindow(class CCamera_Cinema* pCinemaCam);
	void Set_SkillCam(class CCamera_Cinema* pSkillCam);

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	class CCamera_Cinema* m_pCinemaCam = nullptr;
	class CCamera_Cinema* m_pSkillCam = nullptr;

	class CCamera_Cinema* m_pCurCam = nullptr;
	string	m_strCinemaName;
};
END

