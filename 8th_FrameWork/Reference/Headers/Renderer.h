#pragma once
#include "Component.h"

BEGIN(Engine)

class CShader;
class CMesh;
class CTexture;

class ENGINE_DLL CRenderer
	: public CComponent
{
	DECLARE_PROTOTYPE(CRenderer);

protected:
	CRenderer(_uint iGroupID);
	CRenderer(const CRenderer& _origin);
	virtual ~CRenderer();

public:
	static CRenderer* Create(_uint iGroupID, const _uint& iCurPass, const _float4& vOffsetPos);

public:
	void		Set_RenderGroup(RENDER_GROUP	eRenderGroup) { m_eRenderGroup = eRenderGroup; }
	void		Set_Pass(const _uint& iCurPass) { m_iCurPass = iCurPass; }
	_float4		Get_WorldPosition();
	_float4		Get_FinalPos() { return m_vFinalPos; }

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize() override;
	virtual void		Tick() override;
	virtual void		Late_Tick() override;
	virtual HRESULT		Render();

protected:
	CShader*					m_pShaderCom = nullptr;
	CMesh*						m_pMeshCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

	_uint						m_iCurPass = 0;

	RENDER_GROUP				m_eRenderGroup;
	_float4						m_vOffsetPos;
	_float4						m_vFinalPos;
	
	//_uint						m_iShaderFlags;

protected:
	friend class CGameObject;
	virtual void Release() override;

	

};

END

