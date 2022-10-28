#include "stdafx.h"
#include "CUser.h"
#include "MainApp.h"
#include "GameInstance.h"

#include "Texture.h"
#include "GameObject.h"
#include "CFader.h"
#include "CShader.h"
#include "CCrossHair.h"

#include "CScript_FollowCam.h"
#include "Model.h"
#include "HIerarchyNode.h"
#include "CUnit_Player.h"
#include "CCamera_Cinema.h"
#include "Transform.h"
#include "CInGameUI.h"
#include "CTargetCursor.h"
#include "CWire.h"
#include "CUI_Skill.h"
#include "CUI_Combo.h"
#include "CUI_Intro.h"
#include "CUnit_Enemy.h"
#include "CBossHpBar.h"
#include "CUI_HUD.h"
#include "Loading_Manager.h"
#include "CBossAura.h"

#include "CEffects_Factory.h"
#include "Functor.h"
#include "CGoal.h"

#include "CGoTarget.h"
#include "CState_Manager.h"
#include "CNavigation.h"
#include "CTerrain.h"
#include "CCell.h"
#include "CCinemaDark.h"

#include "CBossHpBar.h"

#include "CScreenEffect.h"
#include "CTargetObject.h"

#include "CKunai.h"

#include "CBoneChakra.h"

#include "CBurger.h"


IMPLEMENT_SINGLETON(CUser);

CUser::CUser()
{
}
CUser::~CUser()
{
}

void CUser::On_FinalCutScene(_uint iIndex)
{
	if (m_bFinalScene)
		return;

	GAMEINSTANCE->Stop_Sound((CHANNEL_GROUP)CHANNEL_BGM);

	m_bFinalScene = true;


	Disable_HUD();

	_float4 vBurgerPos = m_pTerrain->Get_Cell(123354)->Get_Point(CCell::POINT_A);
	vBurgerPos.w = 1.f;

	CGameObject* pBurger = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CBurger), vBurgerPos);

	pBurger->Get_Transform()->Set_Look(_float4(0.f, 0.f, -1.f, 0.f));


	Start_SkillCinema(CCamera_Cinema::CINEMA_ENDING);
	m_pSkillCam->Set_Parent(pBurger);
	m_pSkillCam->Set_FollowTarget(pBurger);


	if (!m_pCinemaDarkBottom)
	{
		m_pCinemaDarkBottom = CCinemaDark::Create(false);
		m_pCinemaDarkUp = CCinemaDark::Create(true);
		m_pCinemaDarkBottom->Initialize();
		m_pCinemaDarkUp->Initialize();
		CREATE_GAMEOBJECT(m_pCinemaDarkBottom, GROUP_UI);
		CREATE_GAMEOBJECT(m_pCinemaDarkUp, GROUP_UI);
	}
	else
	{
		ENABLE_GAMEOBJECT(m_pCinemaDarkBottom);
		ENABLE_GAMEOBJECT(m_pCinemaDarkUp);
	}
	

}

void CUser::On_BossDeath()
{
	//1. KO
	m_pInGameUI->m_pUI_Combo->On_KO();
	//2. Mission
	m_pInGameUI->m_pUI_Intro->On_Mission(1);

	//3. 
	m_pInGameUI->m_pUI_Intro->On_Dialog(10);
	GAMEINSTANCE->Play_Sound(L"Voice_Boss_Death", (CHANNEL_GROUP)CHANNEL_VOICE);

	if (m_pBossHpBar)
	DISABLE_GAMEOBJECT(m_pBossHpBar);
	DISABLE_GAMEOBJECT(m_pGoal->Get_TargetObject());

	//3. 두껍좌 만들기

	

}

void CUser::On_StageBegin()
{
	//객체들 상태 변경
	list<CGameObject*>& enemyList = GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY);
	if (m_bStage2)
	{
		//임무창, Dialog, 적이랑 캐릭터 상태 되돌리기
		static_cast<CUnit_Enemy*>(enemyList.front())->Enter_State(STATE_CHIDORI_ENEMY);
		static_cast<CUnit_Enemy*>(enemyList.front())->Enable_BodyColliders();
		
		m_pInGameUI->m_pUI_Intro->On_Mission(0);
		//m_pInGameUI->m_pUI_Intro->On_Dialog(4);

		//보스 특별 UI,
		//보스 체력바 띄우기
		m_pBossHpBar = CBossHPBar::Create(static_cast<CUnit_Enemy*>(enemyList.front()));
		m_pBossHpBar->Initialize();
		CREATE_GAMEOBJECT(m_pBossHpBar, GROUP_UI);

		m_pGoal->Get_TargetObject()->On_TargetBoss(enemyList.front());

	}
	else
	{
		CGameObject* pGameObject = CEffects_Factory::Get_Instance()->Create_Effects(HASHCODE(CScreenEffect), PLAYER);
		DISABLE_GAMEOBJECT(pGameObject);

		if (CLoading_Manager::Get_Instance()->Get_LoadLevel() == LEVEL_STAGE1)
		{
			GAMEINSTANCE->Play_BGM(L"BGM_Combat");
			GAMEINSTANCE->Set_ChannelVolume((CHANNEL_GROUP)CHANNEL_BGM, 0.3f);

			m_pInGameUI->m_pUI_Intro->On_StartIntro(0);

			for (auto& elem : enemyList)
			{
				static_cast<CUnit_Enemy*>(elem)->Enter_State(STATE_IDLE_ENEMY);
				static_cast<CUnit_Enemy*>(elem)->On_StageBegin();
			}
		}
		else
		{
			GAMEINSTANCE->Play_BGM(L"BGM_Konoha");

			m_iCurLeftEnemyCnt = 7;
			m_bStage2 = true;
			//Goal
			m_pInGameUI->m_pUI_Intro->On_StartIntro(2);
			static_cast<CUnit_Enemy*>(enemyList.front())->Enter_State(STATE_GOTARGET);
			m_pGoal->Enable_UI();
			m_pGoal->Get_TargetObject()->On_TargetRunAwayBoss(enemyList.front());
		}
	}
	
	m_pPlayer->Enter_State(STATE_IDLE_PLAYER);


	

}


//광원의 뷰행렬 투영행렬을 이용해서 지형 픽셀의 광원으로부터의 깊이를 구함
//지형 그릴 때 이미 그려진 지형픽셀 비교ㅕ해서
//depth스텐싧 ㅠ를 하나 만들어줌


void CUser::On_FinalEvent()
{
	//마지막이벤트
	//0. FadeDark 생성
	//이건 시네마캠에서 만듬
	// 플레이어 상태 포즈로
	CState_Manager::Get_Instance()->Get_State(STATE_POSE)->Set_AnimType(ANIM_WIN_TYPE_3);
	CState_Manager::Get_Instance()->Get_State(STATE_POSE)->Set_AnimIndex(0);
	m_pPlayer->Enter_State(STATE_POSE);
	//1. 플레이어 적 바라보게
	//플레이어 위치는 중앙으루
	
	CUnit_Enemy* pBoss = static_cast<CUnit_Enemy*>(GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY).front());
	_float4 vPlayerPos = m_pTerrain->Get_Cell(123354)->Get_Point(CCell::POINT_A);
	vPlayerPos.w = 1.f;
	_float4 vEnemyPos = pBoss->Get_Transform()->Get_World(WORLD_POS);
	_float4 vDir = (vEnemyPos - vPlayerPos);
	vDir.y = 0.f;
	vDir.Normalize();
	m_pPlayer->Get_Transform()->Set_World(WORLD_POS, vPlayerPos);
	m_pPlayer->Get_Transform()->Set_LerpLook(vDir, 0.4f);
	m_pPlayer->Get_Transform()->Make_WorldMatrix();
	//2. 적 상태 재생
	pBoss->Enter_State(STATE_BOSS);
	//3. 시네마 재생
	m_pCinemaCam->Start_Cinema(CCamera_Cinema::CINEMA_BOSS);

	//4. UI끄기
	Disable_StaticUIs();
	//5. 시네마 UI 생성
	m_pCinemaDarkBottom = CCinemaDark::Create(false);
	m_pCinemaDarkUp = CCinemaDark::Create(true);
	m_pCinemaDarkBottom->Initialize();
	m_pCinemaDarkUp->Initialize();
	CREATE_GAMEOBJECT(m_pCinemaDarkBottom, GROUP_UI);
	CREATE_GAMEOBJECT(m_pCinemaDarkUp, GROUP_UI);

	//6. 
}

void CUser::Delete_Cinema()
{
	GET_COMPONENT_FROM(m_pCinemaDarkBottom, CFader)->Force_KeyInput();
	GET_COMPONENT_FROM(m_pCinemaDarkUp, CFader)->Force_KeyInput();
	m_pCinemaDarkBottom = nullptr;
	m_pCinemaDarkUp = nullptr;
}

void CUser::Set_Player(CUnit_Player* pPlayer)
{
	m_pPlayer = pPlayer;
	m_pNoiseTexture = GET_COMPONENT_FROM(m_pPlayer, CTexture);
}

void CUser::Start_Cinema(_uint iCinemaType)
{
	m_pCinemaCam->Start_Cinema(CCamera_Cinema::CINEMA_TYPE(iCinemaType));
}

void CUser::Start_SkillCinema(_uint iCinemaType)
{
	m_pSkillCam->Start_Cinema(CCamera_Cinema::CINEMA_TYPE(iCinemaType));
}

void CUser::Reset_Wire(CGameObject* pKunai)
{
	m_pWire->Reset_Wire(pKunai);
}

void CUser::Disable_Wire()
{
	DISABLE_GAMEOBJECT(m_pWire);
}

HRESULT CUser::On_EnableIngameUIs()
{
	m_pInGameUI = CInGameUI::Create();

	if (FAILED(m_pInGameUI->Initialize()))
		return E_FAIL;

	CREATE_STATIC(m_pInGameUI, HASHCODE(CInGameUI));

	Set_SkillCoolTime(0, 0, 5.f);
	Set_SkillCoolTime(0, 1, 6.f);
	On_StyleChange(0);
	Set_SkillCoolTime(0, 2, 5.f);
	Set_SkillCoolTime(0, 3, 5.f);
	Set_SkillCoolTime(0, 4, 20.f);

	Set_SkillCoolTime(1, 0, 5.f);
	Set_SkillCoolTime(1, 1, 6.f);
	Set_SkillCoolTime(1, 2, 5.f);
	Set_SkillCoolTime(1, 3, 5.f);
	Set_SkillCoolTime(1, 4, 10.f);

	Set_SkillCoolTime(2, 0, 5.f);
	Set_SkillCoolTime(2, 1, 6.f);
	Set_SkillCoolTime(2, 2, 5.f);
	Set_SkillCoolTime(2, 3, 5.f);
	Set_SkillCoolTime(2, 4, 10.f);


	Start_HpLerp(100.f, 100.f);
	CHierarchyNode* pRightHand = GET_COMPONENT_FROM(m_pPlayer, CModel)->Find_HierarchyNode("RightHand");
	m_pWire = CWire::Create(m_pPlayer, pRightHand);
	CREATE_STATIC(m_pWire, HASHCODE(CWire));
	DISABLE_GAMEOBJECT(m_pWire);

	return S_OK;
}

void CUser::Disable_StaticUIs()
{
	DISABLE_GAMEOBJECT(m_pCrossHair);
	DISABLE_GAMEOBJECT(m_pTargetCursor);
	DISABLE_GAMEOBJECT(m_pInGameUI);

	m_pPlayer->On_FixedTarget(false);
}

void CUser::Disable_HUD()
{
	m_pInGameUI->Disable_HUD();
}
void CUser::Enable_HUD()
{
	m_pInGameUI->Enable_HUD();
}

void CUser::Enable_StaticUIs()
{
	ENABLE_GAMEOBJECT(m_pCrossHair);
	ENABLE_GAMEOBJECT(m_pTargetCursor);
	ENABLE_GAMEOBJECT(m_pInGameUI);
	
}

void CUser::Set_Target(CUnit* pUnit)
{
	m_pTargetCursor->Set_Target(pUnit);
}

void CUser::On_FixedTarget(_bool bActivate)
{
	m_pTargetCursor->On_FixedTarget(bActivate);
	m_pFollowCam->On_FixedTarget(bActivate, m_pPlayer->Get_TargetUnit());
}

void CUser::Set_CrossHair(_uint iIndex)
{
	if (!m_pCrossHair)
		return;


	GET_COMPONENT_FROM(m_pCrossHair, CFader)->Force_KeyInput();
}

void CUser::Set_SkillCoolTime(_uint iStyle, _uint iSkillType, _float fCoolTime)
{
	m_pInGameUI->m_pUI_Skill->Set_SkillCoolTime(iStyle, (CUI_Skill::SKILL_TYPE)iSkillType, fCoolTime);

}

_bool CUser::Can_UseSkill(_uint iSkillType)
{
	if (!PLAYER->Can_Use((CUnit::COOL_TYPE)iSkillType))
		return false;

	return m_pInGameUI->m_pUI_Skill->Can_UseSkill((CUI_Skill::SKILL_TYPE)iSkillType);
}

void CUser::On_UseSkill(_uint iSkillType)
{
	m_pInGameUI->m_pUI_Skill->On_UseSkill((CUI_Skill::SKILL_TYPE)iSkillType);
}

void CUser::On_StyleChange(_uint iStyle)
{
	m_pInGameUI->m_pUI_Skill->On_StyleChange(iStyle);
}

void CUser::Start_HpLerp(_float fCurHp, _float fMaxHp)
{
	m_pInGameUI->m_pUI_HUD->Start_HpLerp(fCurHp, fMaxHp);
}

void CUser::On_RefreshCombo()
{
	m_pInGameUI->m_pUI_Combo->On_RefreshCombo();
}

void CUser::On_KO()
{
	m_pInGameUI->m_pUI_Combo->On_KO();
	m_iCurLeftEnemyCnt--;

	if (m_bStage2 && m_iStage2EventNum < 3)
	{

		//1이면 맨처음 잡몹 다 잡은상황
		if (m_iStage2EventNum <= 1)
		{
			if (m_iCurLeftEnemyCnt <= 3)
			{
				m_pInGameUI->m_pUI_Intro->On_Mission(2);
				//미션지 생성 + 골인지점 다시 생성
				_float4 vGoalPos = m_pTerrain->Get_Cell(107538)->Get_Point(CCell::POINT_A);
				vGoalPos.y += 0.5f;
				vGoalPos.w = 1.f;
				ENABLE_GAMEOBJECT(m_pGoal);
				m_pGoal->Get_Transform()->Set_World(WORLD_POS, vGoalPos);
				CUnit_Enemy* pBoss = static_cast<CUnit_Enemy*>(GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY).front());
				m_pGoal->Get_TargetObject()->On_TargetRunAwayBoss(pBoss);

				m_iCurLeftEnemyCnt = 7;
			}
			
		}

		//5면 두번쨰잡몹 다 잡음
		if (m_iStage2EventNum == 2 || m_iStage2EventNum == 5)
		{
			if (m_iCurLeftEnemyCnt <= 1)
			{
				m_pInGameUI->m_pUI_Intro->On_Mission(2);
				//미션지 생성 + 골인지점 다시 생성
				_float4 vGoalPos = m_pTerrain->Get_Cell(123354)->Get_Point(CCell::POINT_A);
				vGoalPos.y += 0.5f;
				vGoalPos.w = 1.f;

				ENABLE_GAMEOBJECT(m_pGoal);
				m_pGoal->Get_Transform()->Set_World(WORLD_POS, vGoalPos);
			}
			
		}
	}
}

void CUser::On_CinemaIntroTurnOn(_uint iCinemaIntroIndex)
{
	m_pInGameUI->m_pUI_Intro->On_CinemaIntro(iCinemaIntroIndex);
}

void CUser::On_CinemaMessage(_uint iIndex)
{
	m_pInGameUI->m_pUI_Intro->On_CinemaMessage(iIndex);

}

void CUser::On_CinemaMessageOff()
{
	m_pInGameUI->m_pUI_Intro->On_CinemaMessageOff();
}

void CUser::On_ClearIntro()
{

	m_pInGameUI->m_pUI_Intro->On_ClearIntro();
}

void CUser::On_Dialog(_uint iIndex)
{
	m_pInGameUI->m_pUI_Intro->On_Dialog(iIndex);
}

void CUser::Show_ClearIntro()
{
	m_pInGameUI->m_pUI_Intro->On_Clear();
}

void CUser::On_Stage2Event(const _uint& iEventNum)
{
	m_iStage2EventNum = iEventNum;
	//fadeout 이벤트에서 이게 호출됏는데
	//1. 캐릭이 골에 추옫ㄹ
	//2. 이떄 Dialog 0
	//3. 저기서 dialog에 델리 넣어놓음
	//4. 그럼 여기로 들어옴.
	//5. 여기서 다이얼로그 1 실행
	//6. 1 나오다가 끝나면
	//7. 다시 fadeoutevent실행..?

	//첫메세지일때 0 부르고
	//그다음 여기와서 1불렀는데


	switch (iEventNum)
	{
	case 0:
		//첫 메세지다음 두번째꺼 띄워야함 + 적들 생성
		GAMEINSTANCE->Play_Sound(L"Voice_Boss_GoFight", (CHANNEL_GROUP)CHANNEL_VOICE);

		On_Dialog(1);
		PLAYER->Enter_State(STATE_IDLE_PLAYER);
		m_pInGameUI->m_pUI_Intro->On_Mission(0);

		{
			//적 다시 도망, 적 다 잡으면 Goal 위치 옮겨서 다시 생성
			list<CGameObject*>& enemyList = GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY);

			_uint iCnt = 0;
			for (auto& elem : enemyList)
			{
				if (iCnt == 0 || iCnt >= 5)
				{
					iCnt++;
					continue;
				}

				ENABLE_GAMEOBJECT(elem);
				static_cast<CUnit*>(elem)->Enter_State(STATE_APPEARANCE);
				iCnt++;

			}

			
			_float4 vGoalPos = m_pTerrain->Get_Cell(107538)->Get_Point(CCell::POINT_A);
			vGoalPos.w = 1.f;
			GET_STATE(STATE_GOTARGET, CGoTarget)->Set_TargetPos(vGoalPos);
			static_cast<CUnit*>(enemyList.front())->Enter_State(STATE_GOTARGET);

		}
		
		break;

	case 1:
		break;

	case 2:
		//두번째 도착 후 끝나면
		//마찬가지로 미션지 새로 생성
		m_pInGameUI->m_pUI_Intro->On_Mission(0);

		break;

	case 3:
		//최종보스뜬거임 얘 나오면
		break;

	case 5:
		//그냥 두번째 골인지점 도착했을 때
	{
		//적 다시 도망, 적 다 잡으면 Goal 위치 옮겨서 다시 생성
		list<CGameObject*>& enemyList = GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY);

		_uint iCnt = 0;
		for (auto& elem : enemyList)
		{
			if (iCnt == 0)
			{
				iCnt++;
				continue;
			}
			ENABLE_GAMEOBJECT(elem);
			static_cast<CUnit*>(elem)->Enter_State(STATE_APPEARANCE);
			static_cast<CUnit*>(elem)->Get_Status().fHP = static_cast<CUnit*>(elem)->Get_Status().fMaxHP;

			//뒤에 두놈은 할 필요 없음
			if (iCnt >= 5)
				continue;
			//적 위치 조정
			_uint iCellIndex = 0;
			switch (iCnt)
			{
			case 1:
				iCellIndex = 120684;
				break;
			case 2:
				iCellIndex = 118328;
				break;
			case 3:
				iCellIndex = 109304;
				break;
			case 4:
				iCellIndex = 109364;
				break;
			default:
				break;
			}
			_float4 vGoalPos = m_pTerrain->Get_Cell(iCellIndex)->Get_Point(CCell::POINT_A);
			vGoalPos.x += 0.1f;
			vGoalPos.z += 0.1f;
			vGoalPos.w = 1.f;
			GET_COMPONENT_FROM(elem, CNavigation)->Set_StartCell(m_pTerrain->Get_Cell(iCellIndex));
			elem->Get_Transform()->Set_World(WORLD_POS, vGoalPos);
			elem->Get_Transform()->Make_WorldMatrix();

			
			iCnt++;
		}


		//마지막지점 보스위치로
		_float4 vGoalPos = m_pTerrain->Get_Cell(141302)->Get_Point(CCell::POINT_A);
		vGoalPos.w = 1.f;
		GET_STATE(STATE_GOTARGET, CGoTarget)->Set_TargetPos(vGoalPos);
		static_cast<CUnit*>(enemyList.front())->Enter_State(STATE_GOTARGET);

	}

	default:
		break;
	}
}

void CUser::On_CinemaIntroTurnOff()
{
	m_pInGameUI->m_pUI_Intro->On_CinemaIntroTurnOff();
}

CUnit* CUser::Find_ClosestEnemy()
{
	_float4 vPlayerPos = m_pPlayer->Get_Transform()->Get_World(WORLD_POS);

	list<CGameObject*>& pEnemies = GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY);

	_float fMinLength = 9999.f;
	CUnit* pClosestUnit = nullptr;


	for (auto& pEnemy : pEnemies)
	{
		if (!pEnemy->Is_Valid())
			continue;

		if (static_cast<CUnit*>(pEnemy)->Get_Status().fHP <= 0.f)
			continue;

		_float4 vEnemyPos = pEnemy->Get_Transform()->Get_World(WORLD_POS);
		vEnemyPos.y += 0.5f;

		if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(vEnemyPos.XMLoad(), 0.5f))
			continue;

		_float fLength;


		_float4x4 matVP = GAMEINSTANCE->Get_CurViewMatrix() * GAMEINSTANCE->Get_CurProjMatrix();

		_float4 vEnemyProjPos = vEnemyPos.MultiplyCoord(matVP);

		vEnemyProjPos.x *= g_iWinCX * 0.5f;
		vEnemyProjPos.y *= g_iWinCY * 0.5f;
		vEnemyProjPos.z = 0.f;
		vEnemyProjPos.w = 1.f;

		fLength = vEnemyProjPos.Length();

		//화면 중앙에서 너무 멀리 떨어져 있으면
		if (fLength > 300.f)
			continue;


		fLength = (vEnemyPos - vPlayerPos).Length();

		//거리가 너무 멀면
		if (fLength > 40.f)
			continue;

		if (fMinLength > fLength)
		{
#ifdef _DEBUG
			pClosestUnit = dynamic_cast<CUnit*>(pEnemy);
			if (pClosestUnit)
				fMinLength = fLength;
#else
			pClosestUnit = static_cast<CUnit*>(pEnemy);
			fMinLength = fLength;

#endif
		}
	}

	return pClosestUnit;
}

CUnit* CUser::Find_RealClosestEnemy()
{
	_float4 vPlayerPos = m_pPlayer->Get_Transform()->Get_World(WORLD_POS);

	list<CGameObject*>& pEnemies = GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY);

	_float fMinLength = 9999.f;
	CUnit* pClosestUnit = nullptr;

	for (auto& pEnemy : pEnemies)
	{
		_float4 vEnemyPos = pEnemy->Get_Transform()->Get_World(WORLD_POS);
		_float fLength;

		fLength = (vEnemyPos - vPlayerPos).Length();

		//거리가 너무 멀면
		if (fLength > 6.f)
			continue;

		if (fMinLength > fLength)
		{
#ifdef _DEBUG
			pClosestUnit = dynamic_cast<CUnit*>(pEnemy);
			if (pClosestUnit)
				fMinLength = fLength;
#else
			pClosestUnit = static_cast<CUnit*>(pEnemy);
			fMinLength = fLength;

#endif
		}
	}

	return pClosestUnit;
}

HRESULT CUser::Initialize()
{
	POINT ptMouse;

	ptMouse.x = g_iWinCX >> 1;
	ptMouse.y = g_iWinCY >> 1;
	m_ptCenter = ptMouse;

	m_KeyCommands.push_back(KEY::LAST);

	m_pCrossHair = CCrossHair::Create();
	if (FAILED(m_pCrossHair->Initialize()))
		return E_FAIL;
	//m_pCrossHair->CallBack_FadeOutEvent += bind(&CUser::Switch_CrossHair, this, 0);

	m_pTargetCursor = CTargetCursor::Create();

	if (FAILED(m_pTargetCursor->Initialize()))
		return E_FAIL;

	CREATE_STATIC(m_pCrossHair, HASHCODE(CCrossHair));
	CREATE_STATIC(m_pTargetCursor, HASHCODE(CTargetCursor));

	DISABLE_GAMEOBJECT(m_pCrossHair);
	DISABLE_GAMEOBJECT(m_pTargetCursor);

	

	return S_OK;
}

void CUser::Tick()
{
	Fix_CursorPosToCenter();
}

void CUser::Fix_CursorPosToCenter()
{
	if (GetFocus() != g_hWnd || !m_bFixCursor)
		return;

	/* Set Mouse Cursor on Center of Screen */
	POINT ptMouse;
	ptMouse = m_ptCenter;
	::ClientToScreen(g_hWnd, &ptMouse);
	::SetCursorPos(ptMouse.x, ptMouse.y);
}

void CUser::KeyInput_FPSSetter()
{
	_double dCurFPSLimit = CGameInstance::Get_Instance()->Get_FPSLimitTime();

	if (KEY(F3, TAP))
	{
		dCurFPSLimit = 1. / 30.;
	}
	else if (KEY(F2, TAP))
	{
		dCurFPSLimit = 1. / 120.;
	}
	else if (KEY(F1, TAP))
	{
		dCurFPSLimit = 0.;
	}
	else if (KEY(F4, TAP))
	{
		m_bFreeCam = !m_bFreeCam;

		if (m_bFreeCam)
		{
			CGameInstance::Get_Instance()->Change_Camera(L"Free");
		}
		else
			CGameInstance::Get_Instance()->Change_Camera(L"PlayerCam");

	}

	CGameInstance::Get_Instance()->Set_FPSLimitTIme(dCurFPSLimit);

}

void CUser::Update_KeyCommands()
{
	vector<CKey_Manager::tKeyInfo>& vecKeys = CGameInstance::Get_Instance()->Get_KeyList();

	for (_uint i = 0; i < (_uint)KEY::LAST; ++i)
	{
		if (vecKeys[i].eState == KEY_STATE::TAP)
		{
			m_KeyCommands.push_back((KEY)i);
		}
	}
}

void CUser::Update_WireMode()
{
	if (KEY(Q, TAP))
	{
		CFunctor::Play_Sound(L"LockOff", CHANNEL_UI, m_pPlayer->Get_Transform()->Get_World(WORLD_POS));

		Turn_WireMode(!m_bWireMode);

	}

	if (m_bWireMode && KEY(LBUTTON, TAP))
	{
		m_pPlayer->Enter_State(STATE_WIRE_PLAYER);
	}


	//Test

	
	if (KEY(NUM4, TAP))
	{
		Start_HpLerp(100.f, 100.f);
	}


	if (KEY(NUM6, TAP))
	{
		GAMEINSTANCE->Set_TimeSpeed(0.1f);
	}
	if (KEY(NUM7, TAP))
	{
		GAMEINSTANCE->Set_TimeSpeed(1.f);
	}
}

void CUser::SetUp_NoiseResources()
{
	if (m_pNoiseTexture)
		m_pNoiseTexture->Set_ShaderResourceView(GET_COMPONENT_FROM(m_pPlayer, CShader), "g_BlendTexture");
}

void CUser::Turn_WireMode(_bool bOn)
{
	m_bWireMode = bOn;

	if (bOn)
	{
		m_pFollowCam->Start_FOVLerp(0.45f);

	}
	else
	{

		m_pFollowCam->Start_FOVLerp();
	}

	Set_CrossHair(0);

	
}

void CUser::Switch_CrossHair(const _uint& iIndex)
{

	GET_COMPONENT_FROM(m_pCrossHair, CTexture)->Set_CurTextureIndex(iIndex);
}

_bool CUser::On_BossPhase2()
{
	if (m_bBossPhase2)
		return false;

	m_bBossPhase2 = true;
	static_cast<CUnit*>(GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY).front())->Enter_State(STATE_KOTOAMATSUKAMI);
	DISABLE_GAMEOBJECT(m_pGoal->Get_TargetObject());

	return true;
	
}

_bool CUser::On_BossPhaseEnd()
{
	if (m_bBossPhaseEnd)
		return false;


	m_pFollowCam->Start_ShakingCamera(0.1f, 0.3f);
	On_Dialog(9);
	m_bBossPhaseEnd = true;
	m_pBossAura->On_StopFog();
	ENABLE_GAMEOBJECT(m_pGoal->Get_TargetObject());
	m_pPlayer->Set_NoSkill(false);

	return true;
}
