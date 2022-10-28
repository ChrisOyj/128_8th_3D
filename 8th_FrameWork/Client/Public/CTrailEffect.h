#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CHierarchyNode;
class CGameObject;
class CTransform;
class CShader;
END

BEGIN(Client)
class CTrailEffect
	: public CGameObject
{
	DECLARE_PROTOTYPE(CTrailEffect);

private:
	CTrailEffect();
	virtual ~CTrailEffect();

public:
	static CTrailEffect* Create(_uint iGroupIdx, _uint iTriCnt, _float4 vLocalSwordLow, _float4 vLocalSwordHigh, CHierarchyNode* pSwordBone, CTransform* pUnitTransform
	, _float4 vGlowFlag, _float4 vColor, wstring wstrMaskMapPath, wstring wstrColorMapPath
	);

public:
	void		Set_ShaderResource(CShader* pShader, const char* pConstantName);

	void	Set_EffectFlag(_float4 vEffectFlag) { m_vShaderFlag = vEffectFlag; }

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

public:
	HRESULT	SetUp_TrailEffect(_uint iGroupIdx, _uint iTriCnt, _float4 vLocalSwordLow, _float4 vLocalSwordHigh, CHierarchyNode* pSwordBone, CTransform* pUnitTransform
		, _float4 vGlowFlag, wstring wstrMaskMapPath, wstring wstrColorMapPath);

protected:
	virtual void	My_LateTick() override;

private:
	CTransform* m_pUnitTransform = nullptr;
	_float4		m_vShaderFlag = SH_EFFECT_NOBLOOM;
	_float4		m_vGlowFlag = GLOW_CHAKRA(1.f);
	_float4		m_vColor = _float4(0.f, 0.f, 0.f, 1.f);
};

END