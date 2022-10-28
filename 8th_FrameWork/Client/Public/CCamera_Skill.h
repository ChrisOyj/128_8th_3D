#pragma once
#include "CCamera_Cinema.h"

BEGIN(Client)
class CCamera_Skill
{
	DECLARE_PROTOTYPE(CCamera_Skill);
	DECLARE_GAMEOBJECT(CCamera_Skill);
	friend class CWindow_Cinema;

private:
	CCamera_Skill();
	virtual ~CCamera_Skill();


private:
	CGameObject* m_pFollowTarget = nullptr;

};

END