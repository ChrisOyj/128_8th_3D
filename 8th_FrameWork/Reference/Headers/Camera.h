#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract
	: public CGameObject
{
protected:
	CCamera();
	virtual ~CCamera();

public:
	const PROJ_TRANSFORM& Get_Proj() { return m_tProj; }

	// CGameObject을(를) 통해 상속됨
private:
	virtual HRESULT Initialize_Prototype() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

protected:
	PROJ_TRANSFORM	m_tProj;
};

END

