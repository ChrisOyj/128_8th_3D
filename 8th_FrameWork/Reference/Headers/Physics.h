#pragma once
#include "Component.h"

BEGIN(Engine)

typedef struct tag_Physics
{
	_float4	vDir;
	_float4 vTurnDir;
	_float	fSpeed = 0.f;
	_float	fTurnSpeed = 0.f;

	_bool	bAir = false;
	_float	fJumpPower = 0.f;		// JumpPower for Cal
	_float	fAcc = 0.f;
	_float	fPlusAcc = 0.f;
	_float	fGravity = 0.f;
	_float	fOriginY = 0.f;

}PHYSICS;

class ENGINE_DLL CPhysics final
	: public CComponent
{
	DECLARE_PROTOTYPE(CPhysics)

private:
	CPhysics(CGameObject* pOwner);
	virtual ~CPhysics();

public:
	static	CPhysics*	Create(CGameObject* pOwner);

public:
	PHYSICS&	Get_Physics() { return m_tPhysics; }

	void		Set_Jump(const _float& fJumpPower = 0.f);
	void		Set_Speed(const _float& fSpeed)				{ m_tPhysics.fSpeed = fSpeed; }
	void		Set_TurnSpeed(const _float& fTurnSpeed)		{ m_tPhysics.fTurnSpeed = fTurnSpeed; }
	void		Set_Dir(const _float4& vDir)				{ m_tPhysics.vDir = vDir; m_tPhysics.vDir.Normalize(); }
	void		Set_TurnDir(const _float4& vTurnDir)		{ m_tPhysics.vTurnDir = vTurnDir; }

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual	void	OnEnable() override;
	virtual	void	OnDisable() override;

private:
	PHYSICS	m_tPhysics;

private:
	virtual void Release() override;
	void	Move();
	void	Free_Fall();
	void	Turn();

};

END