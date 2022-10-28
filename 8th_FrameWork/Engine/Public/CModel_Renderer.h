#pragma once
#include "Renderer.h"

BEGIN(Engine)
class CModel;

class ENGINE_DLL CModel_Renderer
	: public CRenderer
{
	DECLARE_PROTOTYPE(CModel_Renderer)

	enum SHADOW_TYPE {DEFAULTSHADOW, STATICSHADOW, NOSHADOW, SHADOW_END};

private:
	CModel_Renderer(_uint iGroupID);
	virtual ~CModel_Renderer();

public:
	static CModel_Renderer* Create(_uint iGroupID, const RENDER_GROUP& eRenderGroup,
		const _uint& iCurPass, const _float4& vOffsetPos = _float4(0.f, 0.f, 0.f, 1.f));


public:
	void	Set_ShadowType(SHADOW_TYPE eType) { m_eShadowType = eType; }

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize() override;
	virtual void		Start() override;
	virtual void		Tick() override;
	virtual void		Late_Tick() override;
	virtual HRESULT		Render();
	virtual HRESULT		Bake_Shadow(_float4x4 ViewMatrix) override;
	virtual void		Release() override;

private:
	SHADOW_TYPE	m_eShadowType = NOSHADOW;
	CModel* m_pModelCom = nullptr;

};

END