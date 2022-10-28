#pragma once
#include "CNavigation.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
class CCell;
class CPhysics;
END

BEGIN(Client)
class CScript_CamNavi
	: public CNavigation
{
private:
	CScript_CamNavi(_uint iGroupIdx);
	virtual ~CScript_CamNavi();

public:
	static CScript_CamNavi* Create(_uint iGroupIdx, CCell* pStartCell, CPhysics* pPhysicsCom, CGameObject* pFollowTarget);


public:
	virtual void Tick() override;

private:
	CGameObject* m_pFollowTarget = nullptr;
	class CTerrain* m_pTerrain = nullptr;

private:
	CCell*	Find_Cells(_float4 vCurPos, CCell* pCurCell);
	CCell* Find_CurrentCell(_float4 vCurPos, CCell* pCurCell);


};
END
