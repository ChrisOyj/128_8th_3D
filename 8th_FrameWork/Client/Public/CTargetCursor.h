#pragma once
#include "CCustom_UI.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CTargetCursor final
	: public CCustom_UI
{
	DECLARE_PROTOTYPE(CTargetCursor);

protected:
	CTargetCursor();
	virtual ~CTargetCursor();

public:
	static CTargetCursor* Create();

public:
	void	Set_Target(class CUnit* pUnit);
	void	On_FixedTarget(_bool bActivate);
public:
	void	Set_ShaderResource(CShader* pShader, const char* pConstantName);

public:
	HRESULT	Initialize() override;

private:
	class CUnit* m_pTargetUnit = nullptr;
	class CLockedCursor* m_pLockedCursor = nullptr;
	_float4			m_vDefaultColor;
	_float4			m_vLockedColor;

	_bool			m_bLocked = false;

private:
	HRESULT	SetUp_Components();

private:
	virtual void My_LateTick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;
};

END