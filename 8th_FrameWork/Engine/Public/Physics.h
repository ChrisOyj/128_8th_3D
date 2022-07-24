#pragma once
#include "Component.h"

BEGIN(Engine)

typedef struct tag_Physics
{
	_float3	vDir;
	_float3 vTurnDir;
	_float	fSpeed;
	_float	fTurnSpeed;

	bool	bAir;
	bool	bOnBuilding;
	_float	fJumpPower;		// JumpPower for Cal
	_float	fAcc;
	_float	fPlusAcc;
	_float	fGravity;
	_float	fOriginY;

}PHYSICS;

class ENGINE_DLL CPhysics final
	: public CComponent
{
	DECLARE_PROTOTYPE(CPhysics)

private:
	CPhysics();
	virtual ~CPhysics();

public:
	static	CPhysics*	Create();

public:
	PHYSICS&	Get_Physics() { return m_tPhysics; }

	void		Set_Jump(const _float& fJumpPower = 0.f);
	void		Set_Speed(const _float& fSpeed)				{ m_tPhysics.fSpeed = fSpeed; }
	void		Set_TurnSpeed(const _float& fTurnSpeed)		{ m_tPhysics.fTurnSpeed = fTurnSpeed; }
	void		Set_Dir(const _float3& vDir)				{ m_tPhysics.vDir = vDir; m_tPhysics.vDir.Normalize(); }
	void		Set_TurnDir(const _float3& vTurnDir)		{ m_tPhysics.vTurnDir = vTurnDir; }

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;

public:
	void	Turn(const _float3& vTurnDir, const _float& fTurnSpeed);

private:
	PHYSICS	m_tPhysics;

private:
	virtual void Release() override;
	void	Move();
	void	Free_Fall();
	void	Turn();

};

END