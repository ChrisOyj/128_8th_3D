#pragma once
#include "CScriptable.h"
#include "Client_Defines.h"

BEGIN(Client)

class CScript_FollowCam final
	: public CScriptable
{
	DECLARE_PROTOTYPE(CScript_FollowCam);

private:
	CScript_FollowCam(_uint iGroupIdx);
	virtual ~CScript_FollowCam();

public:
	static CScript_FollowCam* Create(_uint iGroupIdx);

public:
	// CScriptable을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;
	virtual void OnDisable() override;
};

END