#pragma once
#include "CEffect.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CChakraJump_Guide
	: public CEffect
{
	DECLARE_PROTOTYPE(CChakraJump_Guide);

private:
	CChakraJump_Guide();
	virtual ~CChakraJump_Guide();

public:
	static CChakraJump_Guide* Create();

public:
	virtual void		Reset(_float4 vStartPos) override;
	
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	CGameObject* m_pLoopEffect = nullptr;

private:
	virtual void My_Tick() override;
	virtual	void	OnEnable() override;
	virtual	void	OnDisable() override;

};

END