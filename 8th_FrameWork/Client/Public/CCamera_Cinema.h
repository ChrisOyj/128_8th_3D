#pragma once
#include "Camera.h"
#include "Client_Defines.h"

BEGIN(Client)

struct CAMERA_NODE
{
	enum NODETYPE
	{
		DEFAULT,
		TELEPORT,
		NODE_END
	};

	NODETYPE	eNodeType = DEFAULT;


	enum LERPTYPE
	{
		NORMAL,
		FASTER,
		SLOWER,
		FASTERTOSLOWER,
		LERP_END,
	};

	LERPTYPE	eLerpType = NORMAL;
	_float		fLerpTime = 1.f;
	_float4	vPosition;
	_float4	vLook;


	_float		fChangeSpeed = 0.f;
	_float		fMaxSpeed = 100.f;
	_float		fMinSpeed = 10.f;

	//다음으로 가기전에 딜레이
	_float	fDelayTime = 1.f;


};

class CCamera_Cinema final
	: public CCamera
{
	DECLARE_PROTOTYPE(CCamera_Cinema);
	DECLARE_GAMEOBJECT(CCamera_Cinema);
	friend class CWindow_Cinema;

public:
	enum CINEMA_TYPE
	{
		CINEMA_EXAMARENA,
		CINEMA_KONOHAVILLAGE,
		CINEMA_BOSS,
		CINEMA_KAMUI,
		CINEMA_EIGHTGATES,
		CINEMA_NIGHTGUY,
		CINEMA_KOTO,
		CINEMA_ENDING,
		CINEMA_WATER,

		CINEMA_END
	};
protected:
	CCamera_Cinema();
	virtual ~CCamera_Cinema();

public:
	void	On_Finish();
	void	On_GameStart(const _uint& iNum);
	void	On_GameStartFadeOut(const _uint& iNum);

	void	Start_Cinema(CINEMA_TYPE	eType);
	void	RealStart_Cinema(const _uint& iCinemaType);

public:
	void	Set_FollowTarget(CGameObject* pTarget) { m_pFollowTarget = pTarget; }
	void	Start_ShakingCamera(_float fPower, _float fTime);


public:
	void			On_SaveCinema(string strName);
	void			On_LoadCinema(string strPath, CINEMA_TYPE eIndex);

public:
	// CCamera을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

private:
	//들어오는 힘, 시간 계속 받아서 흔듬

	_float	m_fShakingTime = 0.f;
	_float	m_fShakingPower = 0.f;

	CGameObject* m_pFollowTarget = nullptr;

private:
	class CDefault_UI* m_pName = nullptr;
	class CDefault_UI* m_pInfo = nullptr;

private:
	_bool				m_bTesting = false;
	class CFadeDark* m_pFadeDark = nullptr;

protected:
	_bool				m_bNodeFinish = false;
	vector<CAMERA_NODE>		m_vecCamNode[CINEMA_END];

	CINEMA_TYPE			m_eCurrentType = CINEMA_EXAMARENA;
	_int				m_iCurrentIndex = 0;



	_float				m_fTimeAcc = 0.f;

	_float				m_fCurSpeed = 0.f;
	_float				m_fPrevMoveAmount = 0.f;
	_float4				m_vCurDir;

	_float4				m_vOriginPos;
	_float4				m_vOriginLook;

	vector<_float>				m_fOriginAngle[CINEMA_END];
	vector<_float>				m_fTargetAngle[CINEMA_END];



private:
	virtual void My_Tick() override;

private:
	void			Update_Lerp(_int iCurrentIndex);
	void			Update_Shaking();
	void			On_NextNode(_int iNextIndex);
	

};

END