#include "stdafx.h"
#include "CCamera_Cinema.h"

#include "GameInstance.h"

#include "CScript_Freecam.h"
#include "CComponent_Factory.h"
#include "Transform.h"

#include "CFadeDark.h"
#include "CFader.h"

#include "CUser.h"
#include "Loading_Manager.h"

#include "CDefault_UI.h"
#include "CGameObject_Factory.h"
#include "Functor.h"
#include "CUtility_Transform.h"

#include "CUnit_Player.h"

#include "CAnimator.h"
#include "Texture.h"

CCamera_Cinema::CCamera_Cinema()
{
}

CCamera_Cinema::~CCamera_Cinema()
{
}

void CCamera_Cinema::On_Finish()
{
	m_bNodeFinish = true;
	m_fTimeAcc = 0.f;

	if (m_eCurrentType < CINEMA_KAMUI)
	{
#ifdef _DEBUG
		if (CLoading_Manager::Get_Instance()->Get_LoadLevel() == LEVEL_UNITY)
			return;

		if (m_bTesting)
			return;
#endif
		CFadeDark* pFadeDark = CFadeDark::Create(0.5f, 1.f, 1.f);
		GET_COMPONENT_FROM(pFadeDark, CFader)->Force_KeyInput();
		CREATE_GAMEOBJECT(pFadeDark, GROUP_UI);

		pFadeDark->CallBack_FadeInEvent += bind(&CCamera_Cinema::On_GameStart, this, 0);
		pFadeDark->CallBack_FadeOutEvent += bind(&CCamera_Cinema::On_GameStartFadeOut, this, 0);
	}
	else if (m_eCurrentType == CINEMA_ENDING)
	{
		CFadeDark* pFadeDark = CFadeDark::Create(0.5f, 1.f, 1.f);
		GET_COMPONENT_FROM(pFadeDark, CFader)->Force_KeyInput();
		CREATE_GAMEOBJECT(pFadeDark, GROUP_UI);

		pFadeDark->CallBack_FadeInEvent += bind(&CCamera_Cinema::On_GameStart, this, 1);
		pFadeDark->CallBack_FadeOutEvent += bind(&CCamera_Cinema::On_GameStartFadeOut, this, 1);
	}
	else
	{
		//스킬연출캠일 때

		m_pTransform->Set_Look(m_vecCamNode[m_eCurrentType].back().vLook);
		m_pTransform->Make_WorldMatrix();

		CCamera*	pPlayerCam = GAMEINSTANCE->Change_Camera(L"PlayerCam");
		pPlayerCam->Get_Transform()->Get_Transform().matMyWorld = m_pTransform->Get_WorldMatrix();
		pPlayerCam->Get_Transform()->Make_WorldMatrix();
	}


}

void CCamera_Cinema::On_GameStart(const _uint& iNum)
{
	//카메라 바꾸고, 
	CCamera* pPlayerCam = GAMEINSTANCE->Change_Camera(L"PlayerCam");

	//UI들 띄우고
	CUser::Get_Instance()->Enable_StaticUIs();
	CUser::Get_Instance()->On_CinemaIntroTurnOff();

	

	if (m_eCurrentType == CINEMA_BOSS)
	{
		m_pTransform->Set_Look(m_vecCamNode[m_eCurrentType].back().vLook);
		m_pTransform->Make_WorldMatrix();

		pPlayerCam->Get_Transform()->Get_Transform().matMyWorld = m_pTransform->Get_WorldMatrix();
		pPlayerCam->Get_Transform()->Make_WorldMatrix();

		CUser::Get_Instance()->Delete_Cinema();
		if (m_pName)
			DELETE_GAMEOBJECT(m_pName);
		if( m_pInfo)
			DELETE_GAMEOBJECT(m_pInfo);
		m_pName = nullptr;
		m_pInfo = nullptr;
	}
	else if (m_eCurrentType == CINEMA_ENDING)
	{
		CUser::Get_Instance()->Delete_Cinema();
		if (m_pName)
			DELETE_GAMEOBJECT(m_pName);
		if (m_pInfo)
			DELETE_GAMEOBJECT(m_pInfo);
		m_pName = nullptr;
		m_pInfo = nullptr;
	}

}

void CCamera_Cinema::On_GameStartFadeOut(const _uint& iNum)
{
	if (m_eCurrentType == CINEMA_ENDING)
	{
		CUser::Get_Instance()->Show_ClearIntro();
		return;
	}


	//객체들 상태 돌려놓고, 진짜 게임 시작
	CUser::Get_Instance()->On_StageBegin();



}

void CCamera_Cinema::Start_Cinema(CINEMA_TYPE	eType)
{
	if (m_vecCamNode[eType].size() <= 1)
		return;

	if (eType < CINEMA_KAMUI)
	{
		ENABLE_GAMEOBJECT(m_pFadeDark);
		GET_COMPONENT_FROM(m_pFadeDark, CFader)->Force_KeyInput();
		GET_COMPONENT_FROM(m_pFadeDark, CFader)->Re_FadeIn();
		m_pFadeDark->CallBack_FadeInEvent.Clear();
		m_pFadeDark->CallBack_FadeInEvent += bind(&CCamera_Cinema::RealStart_Cinema, this, (_uint)eType);
	}
	else if (eType == CINEMA_KOTO)
	{
		m_pFadeDark = GAMEINSTANCE->Get_StaticObj<CFadeDark>();

		m_pFollowTarget =  m_pParent = GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY).front();
		ENABLE_GAMEOBJECT(m_pFadeDark);
		GET_COMPONENT_FROM(m_pFadeDark, CFader)->Force_KeyInput();
		GET_COMPONENT_FROM(m_pFadeDark, CFader)->Re_FadeIn();
		m_pFadeDark->CallBack_FadeInEvent.Clear();
		m_pFadeDark->CallBack_FadeInEvent += bind(&CCamera_Cinema::RealStart_Cinema, this, (_uint)eType);

	}
	else if (eType == CINEMA_ENDING)
	{
		m_pFadeDark = GAMEINSTANCE->Get_StaticObj<CFadeDark>();
		ENABLE_GAMEOBJECT(m_pFadeDark);
		GET_COMPONENT_FROM(m_pFadeDark, CFader)->Force_KeyInput();
		GET_COMPONENT_FROM(m_pFadeDark, CFader)->Re_FadeIn();
		m_pFadeDark->CallBack_FadeInEvent.Clear();
		m_pFadeDark->CallBack_FadeInEvent += bind(&CCamera_Cinema::RealStart_Cinema, this, (_uint)eType);

	}

	else
	{
		m_pFollowTarget = m_pParent = PLAYER;
		RealStart_Cinema((_uint)eType);

	}


	
}

void CCamera_Cinema::RealStart_Cinema(const _uint& iCinemaType)
{
	

	m_eCurrentType = (CINEMA_TYPE)iCinemaType;
	m_iCurrentIndex = 0;

	m_fCurSpeed = 0.f;
	m_vCurDir = m_vecCamNode[m_eCurrentType][m_iCurrentIndex + 1].vPosition - m_vecCamNode[m_eCurrentType][m_iCurrentIndex].vPosition;
	m_vCurDir.Normalize();
	m_vOriginPos = m_vecCamNode[m_eCurrentType][m_iCurrentIndex].vPosition;
	m_vOriginLook = m_vecCamNode[m_eCurrentType][m_iCurrentIndex].vLook;
	m_fTimeAcc = 0.f;
	m_pTransform->Set_World(WORLD_POS, m_vOriginPos);
	m_pTransform->Set_Look(m_vOriginLook);
	m_bNodeFinish = false;


	m_pTransform->Make_WorldMatrix();

	if (m_eCurrentType == CINEMA_BOSS)
	{

		CUser::Get_Instance()->On_CinemaMessage(4);
	}

	if (iCinemaType < CINEMA_KAMUI)
	{

		GAMEINSTANCE->Change_Camera(L"Cinema");

		if (m_eCurrentType < CINEMA_BOSS)
		{
			GAMEINSTANCE->Play_Sound(L"Env_StageIntro", (CHANNEL_GROUP)CHANNEL_ENVIRONMENT);
			GAMEINSTANCE->Play_Sound(L"BGM_Intro", (CHANNEL_GROUP)CHANNEL_UI);
		}
	}
	else
	{
		//SKILL 연출
		GAMEINSTANCE->Change_Camera(L"Skill");
		m_tProj.fFOV = XMConvertToRadians(45.f);


	}
}

void CCamera_Cinema::Start_ShakingCamera(_float fPower, _float fTime)
{
	//돌고있을 때
	if (m_fShakingTime > 0.f)
	{
		//새로온 힘이 더 약한애면

		if (fPower < m_fShakingPower)
			return;
	}

	m_fShakingPower = fPower;
	m_fShakingTime = fTime;
}

HRESULT CCamera_Cinema::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	m_tProj.fFOV = XMConvertToRadians(55.f);
	m_pTransform->Set_ParentFlag(MATRIX_NOSCALE);
	return S_OK;
}

HRESULT CCamera_Cinema::Initialize()
{
	m_pFadeDark = CFadeDark::Create(0.f, 0.5f, 0.3f);
	CREATE_STATIC(m_pFadeDark, HASHCODE(CFadeDark));
	DISABLE_GAMEOBJECT(m_pFadeDark);


	m_bNodeFinish = false;


	return S_OK;
}

void CCamera_Cinema::My_Tick()
{
	if (KEY(ENTER, TAP))
	{
		if (m_eCurrentType < CINEMA_KAMUI)
			On_Finish();

		return;
	}

	if (KEY(NUM9, TAP))
		m_bTesting = true;

	if (m_bNodeFinish)
		return;

	if (m_vecCamNode[m_eCurrentType].empty())
		return;


	_float4 vCurPos, vCurLook, vTargetPos, vTargetLook;
	CAMERA_NODE& tCurNode = m_vecCamNode[m_eCurrentType][m_iCurrentIndex];

	//거리가 가까워졌는지 체크
	vCurPos = m_pTransform->Get_MyWorld(WORLD_POS);
	vCurLook = m_pTransform->Get_MyWorld(WORLD_LOOK);
	vTargetPos = tCurNode.vPosition;
	vTargetLook = tCurNode.vLook;

	_float4 vTargetDir = (vTargetPos - vCurPos);
	_float fCurDistance = vTargetDir.Length();
	vTargetDir.Normalize();


	_float fMinDistance = m_fPrevMoveAmount + fDT;


	if (fCurDistance < fMinDistance)
	{
		//대기시간 세기
		m_fTimeAcc += fDT;
		if (m_fTimeAcc > tCurNode.fDelayTime)
		{
			On_NextNode(m_iCurrentIndex + 1);
		}
		else
		{
			m_pTransform->Set_World(WORLD_POS, vTargetPos);
			m_pTransform->Set_Look(vTargetLook);
		}
	}
	else
	{
		//속도올리고, 방향 맞추고, 이동
		switch (tCurNode.eLerpType)
		{
		case CAMERA_NODE::NORMAL:
		{
			//속도 변화량 없음
			m_fCurSpeed = tCurNode.fMaxSpeed;
		}
		break;

		case CAMERA_NODE::SLOWER:
		{
			//속도 변화량 줄어
			if (m_fCurSpeed > tCurNode.fMinSpeed)
				m_fCurSpeed -= tCurNode.fChangeSpeed * fDT;
			else
				m_fCurSpeed = tCurNode.fMinSpeed;

		}
		break;

		case CAMERA_NODE::FASTER:
		{
			//속도 변화량 줄어
			if (m_fCurSpeed < tCurNode.fMaxSpeed)
				m_fCurSpeed += tCurNode.fChangeSpeed * fDT;
			else
				m_fCurSpeed = tCurNode.fMaxSpeed;

		}
		break;

		case CAMERA_NODE::FASTERTOSLOWER:
		{
			//속도 변화량 줄어
			_float fOriginDistance = (vTargetPos - m_vOriginPos).Length();
			if (fCurDistance > fOriginDistance * 0.5f)
			{
				if (m_fCurSpeed < tCurNode.fMaxSpeed)
					m_fCurSpeed += tCurNode.fChangeSpeed * fDT;
				else
					m_fCurSpeed = tCurNode.fMaxSpeed;
			}
			else
			{
				if (m_fCurSpeed > tCurNode.fMinSpeed)
					m_fCurSpeed -= tCurNode.fChangeSpeed * fDT;
				else
					m_fCurSpeed = tCurNode.fMinSpeed;
			}

			

		}
		break;

		default:
			break;
		}


		//방향맞추기
		_float4 vDirDir = vTargetDir - m_vCurDir;
		m_vCurDir += vDirDir * fDT;
		m_vCurDir.Normalize();

		//이동
		m_fPrevMoveAmount = m_fCurSpeed * fDT;
		m_pTransform->Set_World(WORLD_POS, vCurPos + m_vCurDir * m_fCurSpeed * fDT);


		Update_Lerp(m_iCurrentIndex);
	}

	Update_Shaking();

	
	//m_pTransform->Make_WorldMatrix();
}

void CCamera_Cinema::Update_Lerp(_int iCurrentIndex)
{
	_float4 vCurPos, vCurLook, vTargetPos, vTargetLook;
	CAMERA_NODE& tCurNode = m_vecCamNode[m_eCurrentType][m_iCurrentIndex];
	vCurPos = m_pTransform->Get_MyWorld(WORLD_POS);
	vTargetPos = tCurNode.vPosition;

	_float fCurDistance = (vTargetPos - vCurPos).Length();
	_float fOriginDistance = (vTargetPos - m_vOriginPos).Length();


	_float fRatio = 1.f - (fCurDistance / fOriginDistance);

	/*switch (m_vecCamNode[m_eCurrentType][iCurrentIndex].eLerpType)
	{
	case CAMERA_NODE::DEFAULT:
		break;
	case CAMERA_NODE::SLOWER:
		fRatio = sqrtf(fRatio);
		break;
	case CAMERA_NODE::FASTER:
		fRatio = powf(fRatio, 2.f);
		break;
	case CAMERA_NODE::FASTERTOSLOWER:
		if (fRatio < 0.5f)
			fRatio = powf(fRatio, 2.f);
		else
			fRatio = sqrtf(fRatio);
		break;
	default:
		break;
	}*/



	//러프 시간 안넘었으면 보간
	_float4 vNewLook = XMVectorLerp(m_vOriginLook.XMLoad(), tCurNode.vLook.XMLoad(), fRatio);
	m_pTransform->Set_Look(vNewLook);

	_float fTurnAngle = CFunctor::Lerp_Float(m_fOriginAngle[m_eCurrentType][iCurrentIndex], m_fTargetAngle[m_eCurrentType][iCurrentIndex], fRatio);
	if (!vNewLook.Is_Zero())
		CUtility_Transform::Turn_ByAngle(m_pTransform, vNewLook, fTurnAngle);
}

void CCamera_Cinema::Update_Shaking()
{
	m_fShakingTime -= fDT;
	if (m_fShakingTime < 0.f)
		m_fShakingTime = 0.f;

	if (m_fShakingTime > 0.f)
	{


		_float fShakingPower = m_fShakingTime * m_fShakingPower;

		_float4 vRandDir;
		vRandDir.x = frandom(-1.f, 1.f);
		vRandDir.y = frandom(-1.f, 1.f);
		vRandDir.z = frandom(-1.f, 1.f);
		vRandDir.w = 0.f;
		vRandDir.Normalize();

		m_vOriginLook = m_pTransform->Get_MyWorld(WORLD_LOOK);
		m_vOriginLook += vRandDir * fShakingPower;
		m_pTransform->Set_Look(m_vOriginLook);
	}
}

void CCamera_Cinema::On_NextNode(_int iNextIndex)
{
	if (m_vecCamNode[m_eCurrentType].size() <= iNextIndex)
	{
		On_Finish();

		return;
	}

	m_fPrevMoveAmount = 0.f;

	m_fTimeAcc = 0.f;
	m_vOriginPos = m_vecCamNode[m_eCurrentType][m_iCurrentIndex].vPosition;
	m_vOriginLook = m_vecCamNode[m_eCurrentType][m_iCurrentIndex].vLook;
	m_iCurrentIndex++;

	if (m_vecCamNode[m_eCurrentType][m_iCurrentIndex].eNodeType == CAMERA_NODE::TELEPORT)
	{
		m_fCurSpeed = 0.f;
		m_vCurDir = m_vecCamNode[m_eCurrentType][m_iCurrentIndex + 1].vPosition - m_vecCamNode[m_eCurrentType][m_iCurrentIndex].vPosition;
		m_vCurDir.Normalize();
		m_vOriginPos = m_vecCamNode[m_eCurrentType][m_iCurrentIndex].vPosition;
		m_vOriginLook = m_vecCamNode[m_eCurrentType][m_iCurrentIndex].vLook;
		m_pTransform->Set_World(WORLD_POS, m_vOriginPos);
		m_pTransform->Set_Look(m_vOriginLook);
	}


#ifdef _DEBUG
	if (CLoading_Manager::Get_Instance()->Get_LoadLevel() == LEVEL_UNITY)
		return;
#endif

	if (m_eCurrentType == CINEMA_EXAMARENA)
	{
		if (iNextIndex == 2)
		{
			GAMEINSTANCE->Play_Sound(L"Intro_StageStart", (CHANNEL_GROUP)CHANNEL_UI);
		}

		else if (iNextIndex == 4)
		{
			CUser::Get_Instance()->On_CinemaMessage(0);
			GAMEINSTANCE->Play_Sound(L"Voice_Man_Intro", (CHANNEL_GROUP)CHANNEL_VOICE);

		}
		else if (iNextIndex == 6)
		{
			CUser::Get_Instance()->On_CinemaMessage(1);
			//적 목소리
			GAMEINSTANCE->Play_Sound(L"Voice_Gaara_Intro", (CHANNEL_GROUP)CHANNEL_VOICE);
		}
	}
	else if (m_eCurrentType == CINEMA_KONOHAVILLAGE)
	{
		if (iNextIndex == 2)
		{
			GAMEINSTANCE->Play_Sound(L"Voice_Man_Intro", (CHANNEL_GROUP)CHANNEL_VOICE);
			CUser::Get_Instance()->On_CinemaMessage(2);
		}
		else if (iNextIndex == 4)
		{
			//적 목소리

			GAMEINSTANCE->Play_Sound(L"Voice_Boss_Intro", (CHANNEL_GROUP)CHANNEL_VOICE);
			//GAMEINSTANCE->Play_Sound(L"Intro_StageStart", (CHANNEL_GROUP)CHANNEL_UI);

			CUser::Get_Instance()->On_CinemaMessage(3);

		}
	}
	else if (m_eCurrentType == CINEMA_BOSS)
	{
		if (iNextIndex == 1)
		{
			GAMEINSTANCE->Play_Sound(L"Intro_StageStart", (CHANNEL_GROUP)CHANNEL_UI);
			GAMEINSTANCE->Play_Sound(L"Voice_Boss_Shobu", (CHANNEL_GROUP)CHANNEL_VOICE);

		}

		if (iNextIndex == 2)
		{
			CUser::Get_Instance()->On_CinemaMessageOff();
		}
		else if (iNextIndex == 8)
		{
			//이름 띄우기
			m_pName = CGameObject_Factory::Create_UI(L"InGame_Boss_Name");
			m_pInfo = CGameObject_Factory::Create_UI(L"InGame_Boss_Info");
			CREATE_GAMEOBJECT(m_pName, GROUP_UI);
			CREATE_GAMEOBJECT(m_pInfo, GROUP_UI);
			GET_COMPONENT_FROM(m_pName, CFader)->Force_KeyInput();
			GET_COMPONENT_FROM(m_pInfo, CFader)->Force_KeyInput();

			//보스 목소리
			CUser::Get_Instance()->On_CinemaMessage(5);
			GAMEINSTANCE->Play_Sound(L"Voice_Boss_Shobu", (CHANNEL_GROUP)CHANNEL_VOICE);

		}
	}
	else if (m_eCurrentType == CINEMA_ENDING)
	{

		if (iNextIndex > 0 && iNextIndex < 4)
		{
			GAMEINSTANCE->Play_Sound(L"Eff_Ping", (CHANNEL_GROUP)CHANNEL_EFFECTS);
			ENABLE_GAMEOBJECT(m_pFadeDark);
			GET_COMPONENT_FROM(m_pFadeDark, CFader)->Get_FadeDesc().fFadeOutStartTime = 0.f;
			GET_COMPONENT_FROM(m_pFadeDark, CFader)->Get_FadeDesc().fFadeOutTime = 0.2f;
			GET_COMPONENT_FROM(m_pFadeDark, CFader)->Force_KeyInput();
			GET_COMPONENT_FROM(m_pFadeDark, CFader)->Re_FadeIn();
			m_pFadeDark->CallBack_FadeInEvent.Clear();
			m_pFadeDark->CallBack_FadeOutEvent.Clear();

		}

		if (iNextIndex == 4)
			GAMEINSTANCE->Play_Sound(L"Eff_BigPing", (CHANNEL_GROUP)CHANNEL_EFFECTS);



		if (iNextIndex == 5)
		{
			//두꺼비목소리
			GAMEINSTANCE->Play_Sound(L"Eff_BigSummon", (CHANNEL_GROUP)CHANNEL_EFFECTS);
			GAMEINSTANCE->Play_Sound(L"Voice_Burger", (CHANNEL_GROUP)CHANNEL_VOICE);
			GAMEINSTANCE->Play_Sound(L"BGM_Intro", (CHANNEL_GROUP)CHANNEL_UI);


			if (!m_pName)
			{
				m_pName = CGameObject_Factory::Create_UI(L"InGame_Boss_Name");
				m_pInfo = CGameObject_Factory::Create_UI(L"InGame_Boss_Info");
				CREATE_GAMEOBJECT(m_pName, GROUP_UI);
				CREATE_GAMEOBJECT(m_pInfo, GROUP_UI);
			}


			GET_COMPONENT_FROM(m_pFadeDark, CFader)->Get_FadeDesc().fFadeInTime = 0.f;
			GET_COMPONENT_FROM(m_pFadeDark, CFader)->Get_FadeDesc().fFadeOutStartTime = 0.5f;
			GET_COMPONENT_FROM(m_pFadeDark, CFader)->Get_FadeDesc().fFadeOutTime = 0.3f;


			GET_COMPONENT_FROM(m_pName, CTexture)->Set_CurTextureIndex(1);
			GET_COMPONENT_FROM(m_pInfo, CTexture)->Set_CurTextureIndex(1);

			GET_COMPONENT_FROM(m_pName, CFader)->Force_KeyInput();
			GET_COMPONENT_FROM(m_pInfo, CFader)->Force_KeyInput();

			CUser::Get_Instance()->On_CinemaMessage(6);

		}
	}
	
	/*else if (m_eCurrentType == CINEMA_KAMUI)
	{
		if (iNextIndex == 6)
		{
			_uint iTest = GET_COMPONENT_FROM(PLAYER, CAnimator)->Get_CurAnimFrame();

			
		}
	}*/
	
	/*switch (m_vecCamNode[m_eCurrentType][iNextIndex].eNodeType)
	{
	case CAMERA_NODE::TELEPORT:
		m_pTransform->Set_World(WORLD_POS, m_vecCamNode[m_eCurrentType][iNextIndex].vPosition);
		m_pTransform->Set_Look(m_vecCamNode[m_eCurrentType][iNextIndex].vLook);
		break;

	case CAMERA_NODE::LERP:


	default:
		break;
	}*/
}

void CCamera_Cinema::On_SaveCinema(string strName)
{
	string strPath = "../bin/Cinema/";
	strPath += strName;
	strPath += ".dat";

	ofstream	writeFile(strPath, ios::binary);

	if (!writeFile.is_open())
	{
		Call_MsgBox(L"SSave 실패 ??!?!");
		return;
	}

	_uint iSize = (_uint)m_vecCamNode[m_eCurrentType].size();
	CAMERA_NODE* pCamNodes = new CAMERA_NODE[iSize];

	writeFile.write((char*)&iSize, sizeof(_uint));
	
	//저장이 안대는중

	for (_uint i = 0; i < iSize; ++i)
	{
		pCamNodes[i] = m_vecCamNode[m_eCurrentType][i];

		if (m_eCurrentType >= CINEMA_KAMUI)
		{
			writeFile.write((char*)&m_fOriginAngle[m_eCurrentType][i], sizeof(_float));
			writeFile.write((char*)&m_fTargetAngle[m_eCurrentType][i], sizeof(_float));
		}
		

	}


	writeFile.write((char*)pCamNodes, sizeof(CAMERA_NODE) * iSize);


	writeFile.close();

	SAFE_DELETE_ARRAY(pCamNodes);
}

void CCamera_Cinema::On_LoadCinema(string strName, CINEMA_TYPE eIndex)
{
	m_vecCamNode[eIndex].clear();
	string strPath = "../bin/Cinema/";
	strPath += strName;
	strPath += ".dat";

	ifstream	readFile(strPath, ios::binary);

	if (!readFile.is_open())
	{
		Call_MsgBox(L"LOAD 실패 ??!?!");
		return;
	}

	//여기서 앵글 읽기
	

	_uint iSize;
	readFile.read((char*)&iSize, sizeof(_uint));

	if (eIndex >= CINEMA_KAMUI)
	{
		for (_uint i = 0; i < iSize; ++i)
		{
			_float fOrigin = 0.f;
			_float fTarget = 0.f;

			readFile.read((char*)&fOrigin, sizeof(_float));
			readFile.read((char*)&fTarget, sizeof(_float));

			m_fOriginAngle[eIndex].push_back(fOrigin);
			m_fTargetAngle[eIndex].push_back(fTarget);
		}

	}


	CAMERA_NODE* pCamNodes = new CAMERA_NODE[iSize];
	readFile.read((char*)pCamNodes, sizeof(CAMERA_NODE) * iSize);

	for (_uint i = 0; i < iSize; ++i)
	{
		m_vecCamNode[eIndex].push_back(pCamNodes[i]);

		if (eIndex < CINEMA_KAMUI)
		{

			m_fOriginAngle[eIndex].push_back(0.f);
			m_fTargetAngle[eIndex].push_back(0.f);
		}
	}

	readFile.close();
	SAFE_DELETE_ARRAY(pCamNodes);

}
