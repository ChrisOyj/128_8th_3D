#include "stdafx.h"
#include "CState_Manager.h"

#include "CIdle_Player.h"
#include "CWalk_Player.h"
#include "CWalk_End_Player.h"
#include "CRun_Player.h"
#include "CRun_End_Player.h"
#include "CJump_Player.h"
#include "CFall_Player.h"
#include "CLand_Player.h"
#include "CDoubleJump_Player.h"
#include "CDash_Player.h"
#include "CWallJump_Player.h"
#include "CAirDashStart_Player.h"
#include "CAirDash_Player.h"
#include "CAirDashEnd.h"
#include "CCJ_Start_Player.h"
#include "CCJ_Loop_Player.h"
#include "CCJ_End_Player.h"
#include "CWire_Player.h"
#include "CWireDash_Player.h"
#include "CWallJumpEnd_Player.h"
#include "CGuard_Player.h"
#include "CDodge_Player.h"

#include "CHit_Air_Player.h"
#include "CHit_Ground_Player.h"
#include "CHit_Fall_Player.h"
#include "CBlasted_Ground_Player.h"
#include "CDown_Player.h"
#include "CGetUp_Player.h"
#include "CWallHit_Player.h"
#include "CGuardHit_Player.h"
#include "CPose.h"

#include "WaterFall.h"

#include "CStruggle_Enemy.h"

#include "CNightGuy.h"
#include "CNightGuy_Run.h"
#include "CKamui.h"
#include "CHandAttack_1.h"
#include "CHandAttack_2.h"
#include "CHandAttack_3.h"
#include "CHandAttack_4.h"
#include "CHandAttack_5.h"
#include "CHandAttack_Dash.h"
#include "CHandAttack_Power.h"
#include "CHandAerialAttack_1.h"
#include "CHandAerialAttack_2.h"
#include "CHandAerialAttack_3.h"
#include "CHandAerialAttack_4.h"

#include "CLeafHurricane.h"
#include "CLeafHurricane_Attack.h"
#include "CThrow_Player.h"

#include "CNSAttack_1.h"
#include "CNSAttack_2.h"
#include "CNSAttack_3.h"
#include "CNSAttack_4.h"
#include "CNSAerialAttack_1.h"
#include "CNSAerialAttack_2.h"
#include "CNSAerialAttack_3.h"

#include "CRasengan.h"
#include "CFireball.h"
#include "CRasengan_Run.h"
#include "CRasengan_Attack.h"
#include "CEightGates.h"

#include "CBossChidori.h"
#include "CIdle_Enemy.h"
#include "CAppearance.h"
#include "CWalk_Enemy.h"
#include "CRun_Enemy.h"
#include "CDash_Enemy.h"
#include "CJump_Enemy.h"
#include "CGuard_Enemy.h"
#include "CGuardHit_Enemy.h"
#include "CDodge_Enemy.h"
#include "CWallJumpEnd_Enemy.h"
#include	"CWallJump_Enemy.h"
#include "CHit_Ground.h"
#include "CBlasted_Ground.h"
#include "CHit_Air.h"
#include "CLand_Enemy.h"
#include "CFall_Enemy.h"
#include "CHit_Fall.h"
#include "CGetUp_Enemy.h"
#include "CDown_Enemy.h"
#include "CWallHit_Enemy.h"
#include "CWallHitEnd_Enemy.h"
#include "CDeath_Enemy.h"

#include "CHandAttack_1_Enemy.h"
#include "CHandAttack_2_Enemy.h"
#include "CHandAttack_3_Enemy.h"
#include "CHandAttack_4_Enemy.h"
#include "CHandAerialAttack_4_Enemy.h"
#include "CThrow_Enemy.h"

#include "CSharkBomb.h"
#include "CWaterPrison.h"


#include "CGoTarget.h"
#include "CWallJump_Go.h"
#include "CWallJumpEnd_Go.h"
#include "CFall_Go.h"
#include "CLand_Go.h"

#include "CChidori.h"
#include "CChidori_Attack.h"
#include "CChidori_Run.h"

#include "CLaughingMonk.h"
#include "CDomedWall.h"
#include "CDomedWall_End.h"
#include "CState_Boss.h"

#include "CLeafHurricane_Enemy.h"
#include "CLeafHurricane_Attack_Enemy.h"

#include "CRasenSuriken.h"

#include "CIdle_NightGuy.h"
#include "CWalk_NightGuy.h"
#include "CWalk_End_NightGuy.h"
#include "CRun_NightGuy.h"
#include "CRun_End_NightGuy.h"
#include "CJump_NightGuy.h"
#include "CFall_NightGuy.h"
#include "CLand_NightGuy.h"
#include "CDoubleJump_NightGuy.h"
#include "CDash_NightGuy.h"
#include "CWallJump_NightGuy.h"
#include "CWallJumpEnd_NightGuy.h"
#include "CAttack_1_NightGuy.h"
#include "CAttack_2_NightGuy.h"

#include "CBossFire.h"
#include "CBossKoto.h"

IMPLEMENT_SINGLETON(CState_Manager);

CState_Manager::CState_Manager()
{
}

CState_Manager::~CState_Manager()
{
	for (_uint i = 0; i < STATE_END; ++i)
	{
		SAFE_DELETE(m_arrStates[i]);
	}
}

HRESULT CState_Manager::Initialize()
{
	m_arrStates[STATE_IDLE_PLAYER] = CIdle_Player::Create();
	m_arrStates[STATE_WALK_PLAYER] = CWalk_Player::Create();
	m_arrStates[STATE_WALK_END_PLAYER] = CWalk_End_Player::Create();
	m_arrStates[STATE_RUN_PLAYER] = CRun_Player::Create();
	m_arrStates[STATE_RUN_END_PLAYER] = CRun_End_Player::Create();
	m_arrStates[STATE_JUMP_PLAYER] = CJump_Player::Create();
	m_arrStates[STATE_WALLJUMP_PLAYER] = CWallJump_Player::Create();
	m_arrStates[STATE_WALLJUMPEND_PLAYER] = CWallJumpEnd_Player::Create();
	m_arrStates[STATE_FALL_PLAYER] = CFall_Player::Create();
	m_arrStates[STATE_LAND_PLAYER] = CLand_Player::Create();
	m_arrStates[STATE_DOUBLEJUMP_PLAYER] = CDoubleJump_Player::Create();
	m_arrStates[STATE_DASH_PLAYER] = CDash_Player::Create();
	m_arrStates[STATE_AIRDASHSTART_PLAYER] = CAirDashStart_Player::Create();
	m_arrStates[STATE_AIRDASHLOOP_PLAYER] = CAirDash_Player::Create();
	m_arrStates[STATE_AIRDASHEND_PLAYER] = CAirDashEnd::Create();

	m_arrStates[STATE_CJ_START_PLAYER] = CCJ_Start_Player::Create();
	m_arrStates[STATE_CJ_LOOP_PLAYER] = CCJ_Loop_Player::Create();
	m_arrStates[STATE_CJ_END_PLAYER] = CCJ_End_Player::Create();

	m_arrStates[STATE_WIRE_PLAYER] = CWire_Player::Create();
	m_arrStates[STATE_WIREDASH_PLAYER] = CWireDash_Player::Create();

	m_arrStates[STATE_HIT_AIR_PLAYER] = CHit_Air_Player::Create();
	m_arrStates[STATE_HIT_FALL_PLAYER] = CHit_Fall_Player::Create();
	m_arrStates[STATE_HIT_GROUND_PLAYER] = CHit_Ground_Player::Create();
	m_arrStates[STATE_BLASTED_GROUND_PLAYER] = CBlasted_Ground_Player::Create();
	m_arrStates[STATE_DOWN_PLAYER] = CDown_Player::Create();
	m_arrStates[STATE_GETUP_PLAYER] = CGetUp_Player::Create();
	m_arrStates[STATE_WALLHIT_PLAYER] = CWallHit_Player::Create();
	m_arrStates[STATE_DODGE_PLAYER] = CDodge_Player::Create();



	m_arrStates[STATE_RASENGAN] = CRasengan::Create();
	m_arrStates[STATE_CHIDORI] = CChidori::Create();
	m_arrStates[STATE_RASENSURIKEN_PLAYER] = CRasenSuriken::Create();
	m_arrStates[STATE_GUARD_PLAYER] = CGuard_Player::Create();
	m_arrStates[STATE_GUARDHIT_PLAYER] = CGuardHit_Player::Create();
	m_arrStates[STATE_RASENGAN_RUN] = CRasengan_Run::Create();
	m_arrStates[STATE_EIGHTGATES] = CEightGates::Create();

	//이 밑으로 타겟 안찾는 상태


	m_arrStates[STATE_HANDATTACK_1_PLAYER] = CHandAttack_1::Create();
	m_arrStates[STATE_HANDATTACK_2_PLAYER] = CHandAttack_2::Create();
	m_arrStates[STATE_HANDATTACK_3_PLAYER] = CHandAttack_3::Create();
	m_arrStates[STATE_HANDATTACK_4_PLAYER] = CHandAttack_4::Create();
	m_arrStates[STATE_HANDATTACK_5_PLAYER] = CHandAttack_5::Create();
	m_arrStates[STATE_HANDATTACK_DASH_PLAYER] = CHandAttack_Dash::Create();
	m_arrStates[STATE_HANDATTACK_POWER_PLAYER] = CHandAttack_Power::Create();
	m_arrStates[STATE_HANDAERIALATTACK_1_PLAYER] = CHandAerialAttack_1::Create();
	m_arrStates[STATE_HANDAERIALATTACK_2_PLAYER] = CHandAerialAttack_2::Create();
	m_arrStates[STATE_HANDAERIALATTACK_3_PLAYER] = CHandAerialAttack_3::Create();
	m_arrStates[STATE_HANDAERIALATTACK_4_PLAYER] = CHandAerialAttack_4::Create();


	m_arrStates[STATE_NSATTACK_1_PLAYER] = CNSAttack_1::Create();
	m_arrStates[STATE_NSATTACK_2_PLAYER] = CNSAttack_2::Create();
	m_arrStates[STATE_NSATTACK_3_PLAYER] = CNSAttack_3::Create();
	m_arrStates[STATE_NSATTACK_4_PLAYER] = CNSAttack_4::Create();

	m_arrStates[STATE_NSAERIALATTACK_1_PLAYER] = CNSAerialAttack_1::Create();
	m_arrStates[STATE_NSAERIALATTACK_2_PLAYER] = CNSAerialAttack_2::Create();
	m_arrStates[STATE_NSAERIALATTACK_3_PLAYER] = CNSAerialAttack_3::Create();

	m_arrStates[STATE_THROW_PLAYER] = CThrow_Player::Create();

	m_arrStates[STATE_POSE] = CPose::Create();


	m_arrStates[STATE_KAMUI_PLAYER] = CKamui::Create();

	/*Night Guy*/
	m_arrStates[STATE_IDLE_NIGHTGUY] = CIdle_NightGuy::Create();
	m_arrStates[STATE_WALK_NIGHTGUY] = CWalk_NightGuy::Create();
	m_arrStates[STATE_WALK_END_NIGHTGUY] = CWalk_End_NightGuy::Create();
	m_arrStates[STATE_RUN_NIGHTGUY] = CRun_NightGuy::Create();
	m_arrStates[STATE_RUN_END_NIGHTGUY] = CRun_End_NightGuy::Create();
	m_arrStates[STATE_JUMP_NIGHTGUY] = CJump_NightGuy::Create();
	m_arrStates[STATE_WALLJUMP_NIGHTGUY] = CWallJump_NightGuy::Create();
	m_arrStates[STATE_WALLJUMPEND_NIGHTGUY] = CWallJumpEnd_NightGuy::Create();
	m_arrStates[STATE_FALL_NIGHTGUY] = CFall_NightGuy::Create();
	m_arrStates[STATE_LAND_NIGHTGUY] = CLand_NightGuy::Create();
	m_arrStates[STATE_DOUBLEJUMP_NIGHTGUY] = CDoubleJump_NightGuy::Create();
	m_arrStates[STATE_DASH_NIGHTGUY] = CDash_NightGuy::Create();
	m_arrStates[STATE_ATTACK_1_NIGHTGUY] = CAttack_1_NightGuy::Create();
	m_arrStates[STATE_ATTACK_2_NIGHTGUY] = CAttack_2_NightGuy::Create();





	m_arrStates[STATE_IDLE_ENEMY] = CIdle_Enemy::Create();
	m_arrStates[STATE_WALK_ENEMY] = CWalk_Enemy::Create();
	m_arrStates[STATE_APPEARANCE] = CAppearance::Create();
	m_arrStates[STATE_RUN_ENEMY] = CRun_Enemy::Create();
	m_arrStates[STATE_JUMP_ENEMY] = CJump_Enemy::Create();
	m_arrStates[STATE_DASH_ENEMY] = CDash_Enemy::Create();
	m_arrStates[STATE_GUARD_ENEMY] = CGuard_Enemy::Create();
	m_arrStates[STATE_GUARDHIT_ENEMY] = CGuardHit_Enemy::Create();
	m_arrStates[STATE_DODGE_ENEMY] = CDodge_Enemy::Create();
	m_arrStates[STATE_WALLJUMPEND_ENEMY] = CWallJumpEnd_Enemy::Create();
	m_arrStates[STATE_WALLJUMP_ENEMY] = CWallJump_Enemy::Create();




	m_arrStates[STATE_HIT_GROUND_ENEMY] = CHit_Ground::Create();
	m_arrStates[STATE_BLASTED_GROUND_ENEMY] = CBlasted_Ground::Create();
	m_arrStates[STATE_HIT_FALL_ENEMY] = CHit_Fall::Create();

	m_arrStates[STATE_HIT_AIR_ENEMY] = CHit_Air::Create();
	m_arrStates[STATE_FALL_ENEMY] = CFall_Enemy::Create();
	m_arrStates[STATE_LAND_ENEMY] = CLand_Enemy::Create();
	m_arrStates[STATE_DOWN_ENEMY] = CDown_Enemy::Create();
	m_arrStates[STATE_WALLHIT_ENEMY] = CWallHit_Enemy::Create();
	m_arrStates[STATE_WALLHITEND_ENEMY] = CWallHitEnd_Enemy::Create();
	m_arrStates[STATE_GETUP_ENEMY] = CGetUp_Enemy::Create();
	m_arrStates[STATE_DEATH_ENEMY] = CDeath_Enemy::Create();



	m_arrStates[STATE_HANDATTACK_1_ENEMY] = CHandAttack_1_Enemy::Create();
	m_arrStates[STATE_HANDATTACK_2_ENEMY] = CHandAttack_2_Enemy::Create();
	m_arrStates[STATE_HANDATTACK_3_ENEMY] = CHandAttack_3_Enemy::Create();
	m_arrStates[STATE_HANDATTACK_4_ENEMY] = CHandAttack_4_Enemy::Create();
	m_arrStates[STATE_HANDAERIALATTACK_4_ENEMY] = CHandAerialAttack_4_Enemy::Create();
	m_arrStates[STATE_THROW_ENEMY] = CThrow_Enemy::Create();



	m_arrStates[STATE_RASENGAN_ATTACK] = CRasengan_Attack::Create();
	m_arrStates[STATE_WATERFALL_PLAYER] = CWaterFall::Create();
	m_arrStates[STATE_NIGHTGUY_PLAYER] = CNightGuy::Create();
	m_arrStates[STATE_NIGHTGUYATTACK_PLAYER] = CNightGuy_Run::Create();
	
	m_arrStates[STATE_CHIDORI_RUN] = CChidori_Run::Create();
	m_arrStates[STATE_CHIDORI_ATTACK] = CChidori_Attack::Create();

	m_arrStates[STATE_FIREBALL_PLAYER] = CFireBall::Create();
	m_arrStates[STATE_WATERPRISON_PLAYER] = CWaterPrison::Create();
	m_arrStates[STATE_SHARKBOMB_PLAYER] = CSharkBomb::Create();
	m_arrStates[STATE_LEAFHURRICANE_PLAYER] = CLeafHurricane::Create();
	m_arrStates[STATE_LEAFHURRICANE_ATTACK_PLAYER] = CLeafHurricane_Attack::Create();




	m_arrStates[STATE_GOTARGET] = CGoTarget::Create();
	m_arrStates[STATE_WALLJUMP_GO] = CWallJump_Go::Create();
	m_arrStates[STATE_WALLJUMPEND_GO] = CWallJumpEnd_Go::Create();
	m_arrStates[STATE_FALL_GO] = CFall_Go::Create();
	m_arrStates[STATE_LAND_GO] = CLand_Go::Create();

	m_arrStates[STATE_BOSS] = CState_Boss::Create();
	m_arrStates[STATE_LAUGHINGMONK_ENEMY] = CLaughingMonk::Create();
	m_arrStates[STATE_DOMEDWALL_ENEMY] = CDomedWall::Create();
	m_arrStates[STATE_DOMEDWALLEND_ENEMY] = CDomedWall_End::Create();
	m_arrStates[STATE_LEAFHURRICANE_ENEMY] = CLeafHurricane_Enemy::Create();
	m_arrStates[STATE_LEAFHURRICANE_ATTACK_ENEMY] = CLeafHurricane_Attack_Enemy::Create();

	m_arrStates[STATE_CHIDORI_ENEMY] = CBossChidori::Create();
	m_arrStates[STATE_FIREBALL_ENEMY] = CBossFire::Create();
	m_arrStates[STATE_KOTOAMATSUKAMI] = CBossKoto::Create();

	m_arrStates[STATE_STRUGGLE_ENEMY] = CStruggle_Enemy::Create();

	for (_uint i = 0; i < STATE_END; ++i)
	{
		if (!m_arrStates[i])
			return E_FAIL;
	}

	return S_OK;
}
