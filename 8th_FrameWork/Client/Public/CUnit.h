#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CAnimator;
class CPhysics;
class CCell;
class CCollider;
class CCollider_Sphere;
class CShader;
END

BEGIN(Client)

class CState;

class CUnit abstract
	: public CGameObject
{
public:
	enum COLLIDER_PARTS{REALBODY, BODY, HITBOX, SKILLRANGE, RIGHTHAND, RIGHTFOOT, LEFTHAND, LEFTFOOT, NINZASWORD, PARTS_END};
	enum UNIT_STYLE{STYLE_HAND, STYLE_NINZASWORD, STYLE_WATER, STYLE_NIGHTGUY, UNIT_END};
	enum COOL_TYPE {DODGE, THROW, SKILL1, SKILL2, SKILL_SP, COOL_END};
	enum SKILL_TYPE {LAUGHINGMONK, DOMEDWALL, LEAFHURRICANE, SKILL_END};

protected:
	CUnit();
	virtual ~CUnit();

public:
	virtual void	Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);
	virtual void	Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eColType);
	virtual void	Unit_CollisionExit(CGameObject* pOtherObj, const _uint& eColType) {};

public:
	void	Set_ShaderResource(CShader* pShader, const char* pConstantName);

	void	Set_Passes(VTXANIM_PASS_TYPE ePassType);

public:
	virtual void	On_PlusHp(_float fHp);
	virtual void	On_Die();
	void	Switch_Bone(MODEL_PART_TYPE eType, string strBoneName);

public:
	void		Set_NoSkill(_bool bBool) { m_bNoSkill = bBool; }
	_bool		Can_Use(COOL_TYPE eType) { if (m_fCoolAcc[eType] <= 0.f && !m_bNoSkill) return true; return false; }
	void		On_Use(COOL_TYPE eType) { m_fCoolAcc[eType] = m_fCoolTime[eType]; }

public:
	void		Set_Style(UNIT_STYLE eType) { m_eStyleType = eType; }
	_bool		Is_Hand() { if (m_eStyleType == STYLE_HAND) return true; return false; }
	_bool		Is_NightGuy() { if (m_eStyleType == STYLE_NIGHTGUY) return true; return false; }
	_bool		Is_NinzaSword() { if (m_eStyleType == STYLE_NINZASWORD) return true; return false; }
	_bool		Is_Water() { if (m_eStyleType == STYLE_WATER) return true; return false; }

	void	Set_SkillType(_uint iSkillType) { m_eSkillType = (SKILL_TYPE)iSkillType; }
	void	Set_SkillType2(_uint iSkillType) { m_eSkillType2 = (SKILL_TYPE)iSkillType; }

public:
	UNIT_STATUS& Get_Status() { return m_tUnitStatus; }
	CPhysics* Get_PhysicsCom() { return m_pPhysics; }
	CUnit* Get_TargetUnit() { return m_pTargetUnit; }
	void	Set_TargetUnit(CUnit* pUnit) { m_pTargetUnit = pUnit; }
	STATE_TYPE	Get_CurState() { return m_eCurState; }
	CState*	Get_CurStateP() { return m_pCurState; }
	SKILL_TYPE	Get_SkillType() { return m_eSkillType; }
	SKILL_TYPE	Get_SkillType2() { return m_eSkillType2; }

public:
	void	Enter_State(STATE_TYPE eType);
	void	Enable_Collider(COLLIDER_PARTS eIndex, _bool bEnable);
	CCollider_Sphere*	Get_Collider(COLLIDER_PARTS eIndex);
	void	Disable_AllColliders();
	void	Enable_BodyColliders();

public:
	void	Create_FootTrails();
	void	Enable_FootTrails(_bool bBool);

public:
	// CGameObject을(를) 통해 상속됨
	
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

protected:
	_bool	m_bNoSkill = false;

	class CBoneChakra* m_pLeftFootTrail = nullptr;
	class CBoneChakra* m_pRightFootTrail = nullptr;

protected:
	_float	m_fCoolTime[COOL_END] = {};
	_float	m_fCoolAcc[COOL_END] = {};
	SKILL_TYPE	m_eSkillType = LEAFHURRICANE;
	SKILL_TYPE	m_eSkillType2 = SKILL_END;

protected:
	UNIT_STYLE		m_eStyleType = STYLE_HAND;
	UNIT_MODEL_DATA	m_tModelData;
protected:
	ANIM_TYPE		m_eStartPose = ANIM_WIN_TYPE_3;

	_float			m_fHitWhite = 1.f;
	_bool			m_bHit;
	_float			m_fColorTimeAcc = 0.f;

	CModel*			m_pModelCom = nullptr;
	CAnimator*		m_pAnimator = nullptr;
	CPhysics*		m_pPhysics = nullptr;
	UNIT_STATUS		m_tUnitStatus;
	STATE_TYPE		m_eCurState = STATE_END;
	CState* m_pCurState = nullptr;

	CUnit* m_pTargetUnit = nullptr;


	CCollider* m_pColliders[PARTS_END] = {};



protected:
	virtual	HRESULT	SetUp_Model(UNIT_MODEL_DATA tData);
	virtual	HRESULT	SetUp_Navigation(CCell* pStartCell);

private:
#ifdef _DEBUG
	_bool		m_bControlable = true;

public:
	void		Switch_Controlable() { m_bControlable = !m_bControlable; }
#endif


protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

};
END
