#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CDefault_UI;

class CTargetObject
	: public CGameObject
{
	DECLARE_PROTOTYPE(CTargetObject);

public:
	enum UI_TYPE {
		MINIMAP_EFFECT, MINIMAP_ICON, UI_END
	};


private:
	CTargetObject();
	virtual ~CTargetObject();


public:
	static CTargetObject* Create(class CGameObject* pFollowUnit);

	void	On_TargetBoss(CGameObject* pBoss);
	void	On_TargetRunAwayBoss(CGameObject* pBoss);
	void	On_TargetGoal(CGameObject* pGoal);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	//base, 아이콘들
	CGameObject* m_pFollowUnit = nullptr;

	CDefault_UI* m_arrUI[UI_END];

	_float4	m_vOffsetPos;

private:
	virtual void My_LateTick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;

};
END

