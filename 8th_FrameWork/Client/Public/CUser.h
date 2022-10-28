#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CUser
{
	DECLARE_SINGLETON(CUser);

private:
	CUser();
	~CUser();

public:
	void	On_FinalCutScene(_uint iIndex);
	void	On_BossDeath();
	void On_StageBegin();
	void	On_FinalEvent();
	void Delete_Cinema();

public:
	class CUnit_Player* Get_Player() { return m_pPlayer; }
	void				Set_Player(class CUnit_Player* pPlayer);

	class CCamera_Cinema* Get_SkillCam() { return m_pSkillCam; }
	class CScript_FollowCam* Get_FollowCam() { return m_pFollowCam; }
	void				Set_FollowCam(class CScript_FollowCam* pFollowCam) { m_pFollowCam = pFollowCam; }
	void				Set_CinemaCam(class CCamera_Cinema* pFollowCam) { m_pCinemaCam = pFollowCam; }
	void				Set_SkillCinemaCam(class CCamera_Cinema* pFollowCam) { m_pSkillCam = pFollowCam; }
	void	Start_Cinema(_uint iCinemaType);
	void	Start_SkillCinema(_uint iCinemaType);

	void				Set_PlayerModelData(UNIT_MODEL_DATA& tData) { m_tPlayerModelData = tData; }
	UNIT_MODEL_DATA& Get_PlayerModelData() { return m_tPlayerModelData; }

public:
	void				Reset_Wire(CGameObject* pKunai);
	void				Disable_Wire();
public:
	HRESULT				On_EnableIngameUIs();

public:
	void				Disable_StaticUIs();
	void				Disable_HUD();
	void				Enable_HUD();
	void				Enable_StaticUIs();

public:
	_bool				Is_LockedTarget() { return m_bFixCursor; }
	void				Set_Target(class CUnit* pUnit);
	void				On_FixedTarget(_bool bActivate);
	void				Set_CrossHair(_uint iIndex);

public:
	void				Set_SkillCoolTime(_uint iStyle, _uint iSkillType, _float fCoolTime);
	_bool				Can_UseSkill(_uint iSkillType);
	void				On_UseSkill(_uint iSkillType);
	void				On_StyleChange(_uint iStyle);

public:
	void				Start_HpLerp(_float fCurHp, _float fMaxHp);

public:
	void	On_RefreshCombo();
	void	On_KO();

public:
	void	On_CinemaIntroTurnOff();
	void	On_CinemaIntroTurnOn(_uint iCinemaIntroIndex);
	void	On_CinemaMessage(_uint iIndex);
	void	On_CinemaMessageOff();
	void	On_ClearIntro();
	void	On_Dialog(_uint iIndex);
	void	Show_ClearIntro();


public:
	void	On_Stage2Event(const _uint& iEventNum);
	_uint	Get_EventNum() { return m_iStage2EventNum; }


public:
	void	Set_FixCursor(_bool bEnable) { m_bFixCursor = bEnable;  ::ShowCursor(!bEnable); }
	void	Clear_KeyCommands() {
		m_KeyCommands.clear();  m_KeyCommands.push_back(KEY::LAST);
	}
	KEY		Get_LastKey() { return m_KeyCommands.back(); }

public:
	class CUnit* Find_ClosestEnemy();
	class CUnit* Find_RealClosestEnemy();

public:
	HRESULT	Initialize();
	void	Tick();

public:
	void	Fix_CursorPosToCenter();
	void	KeyInput_FPSSetter();
	void	Update_KeyCommands();
	void	Update_WireMode();
	void	SetUp_NoiseResources();

public:
	void	Turn_WireMode(_bool bOn = false);
	void	Switch_CrossHair(const _uint& iIndex);

public:
	void	Set_Terrain(class CTerrain* pTerrain) { m_pTerrain = pTerrain; }

public:
	_bool	On_BossPhase2();
	_bool	On_BossPhaseEnd();

public:
	void	Set_Goal(class CGoal* pGoal) { m_pGoal = pGoal; }
	CGoal* Get_Goal() { return m_pGoal; }

private:
	_bool	m_bBossPhase2 = false;
	_bool	m_bBossPhaseEnd = false;
	_bool	m_bFinalScene = false;

private:
	class CGoal* m_pGoal = nullptr;
private:
	class CTexture* m_pNoiseTexture = nullptr;
	class CBossHPBar* m_pBossHpBar = nullptr;


private:
	class CTerrain* m_pTerrain = nullptr;
	_bool	m_bStage2 = false;
	_uint	m_iStage2EventNum = 0;
	_uint	m_iCurLeftEnemyCnt = 7;
	CGameObject* m_pCinemaDarkUp = nullptr;
	CGameObject* m_pCinemaDarkBottom = nullptr;

private:
	_bool	m_bFreeCam = false;
	_bool	m_bFixCursor = false;
	_bool	m_bWireMode = false;
	POINT	m_ptCenter;
	UNIT_MODEL_DATA	m_tPlayerModelData;
	list<KEY>		m_KeyCommands;

	class CScript_FollowCam* m_pFollowCam = nullptr;
	class CCamera_Cinema* m_pCinemaCam = nullptr;
	class CCamera_Cinema* m_pSkillCam = nullptr;

	CGameObject* m_pCrossHair = nullptr;
	class CTargetCursor* m_pTargetCursor = nullptr;

	class CUnit_Player* m_pPlayer = nullptr;

	class CInGameUI* m_pInGameUI = nullptr;

	class CWire* m_pWire = nullptr;

private:
	class CBossAura* m_pBossAura = nullptr;

public:
	void Set_BossAura(class CBossAura* pBossAura) { m_pBossAura = pBossAura; }
	class CBossAura* Get_BossAura() { return m_pBossAura; }
};
END
