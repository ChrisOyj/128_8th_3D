#pragma once
#include "Obj.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract 
	: public CObj
{
protected:
	CComponent();
	virtual ~CComponent() = default;

public:
	virtual CComponent* Clone() PURE;

public:
	class CGameObject*	Get_Owner() { return m_pOwner; }
	void	Set_Owner(class CGameObject* pOwner) { m_pOwner = pOwner; }

public:
	virtual void Start() {}


protected:
	class CGameObject*	m_pOwner;
};

END