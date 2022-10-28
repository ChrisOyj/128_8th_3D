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

struct PHYSICSDETAIL
{
	_float	fAccel = 0.f;
	_float	fMaxAccel = 0.f;
	_float	fMaxSpeed = 0.f;
	_float	fFriction = 0.f;
	_float	fFrictionRatio = 1.f;
	_float	fAirFriction = 0.f;
	_float	fCurGroundY = 0.f;
	_float	fPrevY = 0.f;
	_float4	vCurWallNormal;

};

class ENGINE_DLL CPhysics final
	: public CComponent
{
	DECLARE_PROTOTYPE(CPhysics)

private:
	CPhysics(_uint iGroupID);
	virtual ~CPhysics();

public:
	static	CPhysics*	Create(_uint iGroupID);

public:
	PHYSICS&	Get_Physics() { return m_tPhysics; }
	PHYSICSDETAIL&	Get_PhysicsDetail() { return m_tPhysicsDetail; }
	void		Set_NaviOn() { m_bNaviOn = true; };

	void		Set_MaxSpeed(const _float& fMaxSpeed) { m_tPhysicsDetail.fMaxSpeed = fMaxSpeed; }
	void		Set_Accel(const _float& fAccel) { m_tPhysicsDetail.fAccel = fAccel; }
	void		Set_Jump(const _float& fJumpPower = 0.f);
	void		Set_Speed(const _float& fSpeed)				{ m_tPhysics.fSpeed = fSpeed; }
	void		Mul_Speed(const _float& fSpeed)				{ m_tPhysics.fSpeed *= fSpeed; }
	void		Set_SpeedasMax()				{ m_tPhysics.fSpeed = m_tPhysicsDetail.fMaxSpeed; }
	void		Set_TurnSpeed(const _float& fTurnSpeed)		{ m_tPhysics.fTurnSpeed = fTurnSpeed; }
	void		Set_Dir(const _float4& vDir)				{ m_tPhysics.vDir = vDir; m_tPhysics.vDir.Normalize(); }
	void		Set_TurnDir(const _float4& vTurnDir)		{ m_tPhysics.vTurnDir = vTurnDir; }
public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual	void	OnEnable() override;
	virtual	void	OnDisable() override;

public:
	_float	Calculate_Speed();
	_float	Calculate_FreeFall();
	_float	Check_Air(_float fCurY);

private:
	_bool			m_bNaviOn = false;
	PHYSICS			m_tPhysics;
	PHYSICSDETAIL	m_tPhysicsDetail;

private:
	virtual void Release() override;
	void	Turn();

};

END