#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CDefault_UI;

class CEnemyHPBar
	: public CGameObject
{
	DECLARE_PROTOTYPE(CEnemyHPBar);

public:
	enum UI_TYPE {
		HP_BAR, HP_LINE, UI_END
	};


private:
	CEnemyHPBar();
	virtual ~CEnemyHPBar();


public:
	static CEnemyHPBar* Create(class CUnit* pFollowUnit);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	//base, 아이콘들
	class CUnit* m_pFollowUnit = nullptr;

	CDefault_UI* m_arrUI[UI_END];

	_float4	m_vOffsetPos;
	_float4	m_vOriginBarScale;

private:
	virtual void My_LateTick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

};
END

