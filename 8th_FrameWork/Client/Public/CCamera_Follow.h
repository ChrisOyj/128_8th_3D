#pragma once
#include "Camera.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCell;
class CNavigation;
END

BEGIN(Client)

class CCamera_Follow final
	: public CCamera
{
	DECLARE_PROTOTYPE(CCamera_Follow);

private:
	CCamera_Follow();
	virtual ~CCamera_Follow();

public:
	static CCamera_Follow* Create(CGameObject* pTarget, CCell* pStartCell);

public:
	// CCamera을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT	Start() override;

private:
	CNavigation* m_pNaviCom = nullptr;
	class CScript_FollowCam* m_pFollowScript = nullptr;
	_bool			m_bCurOnWall = false;

private:
	HRESULT	SetUp_Components(CGameObject* pTarget, CCell* pStartCell);

private:
	virtual void Late_Tick() override;

};

END