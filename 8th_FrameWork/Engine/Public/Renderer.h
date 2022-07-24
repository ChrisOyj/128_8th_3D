#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer abstract
	: public CComponent
{
protected:
	CRenderer();
	CRenderer(const CRenderer& _origin);
	virtual ~CRenderer();

public:
	void		Set_RenderGroup(RENDERGROUP	eRenderGroup) { m_eRenderGroup = eRenderGroup; }

public:
	virtual void Late_Tick() override;
	virtual HRESULT		Render() PURE;

protected:
	RENDERGROUP				m_eRenderGroup;

protected:
	friend class CGameObject;
	virtual void Release() override;

};

END

