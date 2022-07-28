#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer abstract
	: public CComponent
{
protected:
	CRenderer(CGameObject* pOwner);
	CRenderer(const CRenderer& _origin);
	virtual ~CRenderer();

public:
	void		Set_RenderGroup(RENDER_GROUP	eRenderGroup) { m_eRenderGroup = eRenderGroup; }
	_float4		Get_WorldPosition();
	_float4		Get_FinalPos() { return m_vFinalPos; }

public:
	virtual void Late_Tick() override;
	virtual HRESULT		Render() PURE;

protected:
	RENDER_GROUP				m_eRenderGroup;
	_float4					m_vOffsetPos;
	_float4					m_vFinalPos;

protected:
	friend class CGameObject;
	virtual void Release() override;

};

END

