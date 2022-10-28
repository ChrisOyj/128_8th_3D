#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CDefault_UI;

class CBossHPBar
	: public CGameObject
{
	DECLARE_PROTOTYPE(CBossHPBar);

public:
	enum UI_TYPE {
		HP_BAR, HP_LINE, UI_END
	};


private:
	CBossHPBar();
	virtual ~CBossHPBar();


public:
	static CBossHPBar* Create(class CUnit* pFollowUnit);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	//base, 아이콘들
	class CUnit* m_pFollowUnit = nullptr;

	CDefault_UI* m_arrUI[UI_END];

	_float4	m_vOriginPos;
	_float4	m_vOriginBarScale;

private:
	virtual void My_LateTick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

};
END

