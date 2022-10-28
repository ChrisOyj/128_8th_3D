#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CGoal
	: public CGameObject
{
	DECLARE_PROTOTYPE(CGoal);

private:
	CGoal();
	virtual ~CGoal();

public:
	static CGoal* Create();

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);

public:
	void		Enable_UI();
	class CTargetObject* Get_TargetObject() { return m_pTargetObject; };

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

private:
	class CTargetObject* m_pTargetObject = nullptr;
	CGameObject* m_pGoalEffect1 = nullptr;
	CGameObject* m_pGoalEffect2 = nullptr;
	_bool	m_bEnter = false;
	_uint	m_iEventIndex = 0;

public:
	virtual void	My_Tick();
	virtual void	My_LateTick() {};
	virtual void OnEnable();
	virtual void OnDisable();

};

END