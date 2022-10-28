#pragma once
#include "CCustom_UI.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CLockedCursor final
	: public CCustom_UI
{
	DECLARE_PROTOTYPE(CLockedCursor);

protected:
	CLockedCursor();
	virtual ~CLockedCursor();

public:
	static CLockedCursor* Create(class CTargetCursor* pTargetCursor);

public:
	void	Set_ShaderResource(CShader* pShader, const char* pConstantName);

public:
	HRESULT	Initialize() override;

private:
	class CTargetCursor* m_pTargetCursor = nullptr;
	_float4			m_vDefaultColor;

private:
	HRESULT	SetUp_Components();

private:
	virtual void My_LateTick() override;
};

END