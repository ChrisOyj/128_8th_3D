#pragma once
#include "Component.h"
#include "Client_Defines.h"

BEGIN(Engine)
END

BEGIN(Client)
class CButton
	: public CComponent
{
public:
	DECLARE_PROTOTYPE(CButton);

private:
	CButton(_uint iGroupIdx);
	~CButton();

public:
	static CButton* Create(COMPONENT_PIPELINE	ePipeLine);

public:
	virtual void	Set_ShaderResource(CShader* pShader, const char* pConstantName) override;

public:
	CDelegate<>	CallBack_MouseOn;
	CDelegate<>	CallBack_MouseDown;

public:
	// CComponent을(를) 통해 상속
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnDead()	override;

private:
	_bool	m_bMouseOn = false;

private:
	_bool		Is_MouseOn();

};
END
