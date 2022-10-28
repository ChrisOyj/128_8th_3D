#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CHierarchyNode;
class CShader;
END

BEGIN(Client)
class CUnit;

class CWire
	: public CGameObject
{
	DECLARE_PROTOTYPE(CWire);

private:
	CWire();
	virtual ~CWire();

public:
	static CWire* Create(CUnit* pOwner, CHierarchyNode* pRightHand);

	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);
	void	Set_ShaderResource(CShader* pShader, const char* name);
public:
	void	Reset_Wire(CGameObject* pTargetObj);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	_float4		m_vEffectFlag = SH_EFFECT_DEFAULT;
	_float4		m_vGlowFlag = GLOW_CHAKRA(1.f);

	CUnit* m_pOwner = nullptr;
	CHierarchyNode* m_pRightHand = nullptr;
	CGameObject*	m_pTargetObj = nullptr;

	_float			m_fTimeAcc = 0.f;

private:
	virtual void My_Tick() override;

};

END