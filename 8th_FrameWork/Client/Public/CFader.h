#pragma once
#include "Component.h"
#include "Client_Defines.h"

#define	FADE_NONE	0
#define	FADE_TIME	0x01
#define	FADE_KEY	0x02
#define	FADE_COL	0x04
#define	FADE_TEMP	0x08

BEGIN(Engine)
class CShader;
END

BEGIN(Client)
class CFader
	: public CComponent
{
public:
	enum FADE_STATE
	{
		FADEIN,
		FADEDELAY,
		FADEOUT,
	};

public:
	DECLARE_PROTOTYPE(CFader);

private:
	CFader(_uint iGroupIdx);
	~CFader();

public:
	static CFader* Create(COMPONENT_PIPELINE	ePipeLine, const FADEDESC& tFadeDesc);

public:
	FADEDESC& Get_FadeDesc() { return m_tFadeDesc; }

public:
	virtual void	Set_ShaderResource(CShader* pShader, const char* pConstantName) override;

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
	FADEDESC	m_tFadeDesc;
	FADE_STATE	m_eState = FADEIN;
	_float		m_fTimeAcc = 0.f;

private:
	void	FadeIn();
	void	FadeDelay();
	void	FadeOut();

private:
	void	Change_State(FADE_STATE eState);
	void	OnFadeOut_Finish();

};
END
