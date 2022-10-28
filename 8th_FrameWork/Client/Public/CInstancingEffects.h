#pragma once
#include "CEffect.h"

BEGIN(Client)
class CInstancingEffects
	: public CEffect
{
	DECLARE_PROTOTYPE(CInstancingEffects);

	friend class CWindow_Effect;

public:
	struct INSTANCING_CREATE_DATA
	{
		_uint		iNumInstance = 1;

		_int	iOffsetPositionCount = 0;
		_float4* pOffsetPositions = nullptr;


		_float fCreateRange = 0.f; // 积己 馆版
		_float fCreateRangeY = 0.f; // 积己 馆版
		_float fJumpPower = 0.f;
		_float fJumpPowerRange = 0.f;
		_float	fGravity = 9.8f;
		_float	fGravityRange = 0.f;
		_float	fCurGroundY = 0.f;
		_float	fCurGroundYRange = 0.f;
		_float	fSpeed = 0.f;
		_float	fSpeedRange = 0.f;
		_float	fTurnSpeed = 0.f;
		_float	fTurnSpeedRange = 0.f;

		_float4	vStartDir = _float4(0.f, 0.f, 0.f, 0.f);
		_float4	vStartDirRange = _float4(1.f, 1.f, 1.f, 0.f);
		_float	fStartDistance = 0.5f;
		_float	fStartDistanceRange = 0.5f;

		_float4	vScale = _float4(0.01f, 0.01f, 0.01f, 0.1f);
		_float4	vScaleRange = _float4(0.f, 0.f, 0.f, 0.f);

		_float4 vColor;
		_float4 vColorRange;

		_float	fFadeInStartTime = 5.f;
		_float	fFadeInStartTimeRange = 5.f;
		_float	fFadeInTime;
		_float	fFadeInTimeRange;
		_float	fFadeOutStartTime = 5.f;
		_float	fFadeOutStartTimeRange = 5.f;
		_float	fFadeOutTime;
		_float	fFadeOutTimeRange;

		_float	fTargetAlpha = 1.f;

		_float4	vFadeInTargetScale = _float4(0.1f, 0.1f, 0.3f, 0.f);
		_float4	vFadeInTargetScaleRange;

		_float4	vFadeOutTargetScale = _float4(0.01f, 0.01f, 0.3f, 0.f);
		_float4	vFadeOutTargetScaleRange;

		_float	fSpeedChangeSpeed;
		_float	fSpeedChangeSpeedRange;
	};

	struct INSTANCING_DATA
	{
		_float		fTimeAcc = 0.f;

		enum FADETYPE
		{
			FADEINREADY,
			FADEIN,
			FADEOUTREADY,
			FADEOUT,
			FADE_END,
		};
		FADETYPE	eCurFadeType = FADEINREADY;

		_float fJumpPower;
		_float	fGravity = 9.8f;
		_float	fCurGroundY = 0.f;
		_float	fOriginSpeed = 0.f;
		_float	fSpeed = 0.f;
		_float	fSpeedChangeSpeed = 0.f;
		_float	fTurnSpeed = 0.f;

		_float	fFadeInStartTime;
		_float	fFadeInTime;
		_float	fFadeOutStartTime;
		_float	fFadeOutTime;
		_float	fDuration;
		_float4	vColor;

		_float	fOriginAlpha;
		_float	fTargetAlpha;
		_float4	vOriginScale;
		_float4	vScale = _float4(1.f, 1.f, 1.f, 1.f);
		_float4	vDir = _float4(0.f, 0.f, 1.f, 0.f);
		_float4	vTurnDir = _float4(0.f, 0.f, 1.f, 0.f);


		_float4	vFadeInTargetScale;
		_float4	vFadeOutTargetScale;
		_float	fFadeInScaleLerpStartTime;
		_float	fScaleLerpTime;
		_float	fFadeOutScaleLerpStartTime;


		_float4x4 StartMatrix;
		_float4	vStartScale;
		_float	fStartSpeed;

		_float4	vStartPureLocalPos;
		_float4	vStartPureLocalDir;

		_float	fOriginY = 0.f;
		_float	fPrevY = 0.f;
		_float	fAcc = 0.f;
		_float	fPlusAcc = 0.f;

		_bool	bAlive = true;
	};
protected:
	CInstancingEffects();
	CInstancingEffects(const CInstancingEffects& _origin);
	virtual ~CInstancingEffects();

public:
	static CInstancingEffects* Create(wstring wstrFbxPath, _uint iNumInstance, const INSTANCING_CREATE_DATA& tCreateData, _hashcode _hcCode);

public:
	INSTANCING_CREATE_DATA& Get_CreateData() { return m_tCreateData; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

protected:
	_bool		m_bSoft = false;

	_uint		m_iNumDead = 0;

	_float4x4* m_pInstanceMatrices = nullptr;

	INSTANCING_DATA* m_pInstancingDatas = nullptr;
	INSTANCING_CREATE_DATA	m_tCreateData;


protected:
	virtual void	My_Tick() override;
	virtual void	My_LateTick() override;
	virtual void	OnEnable() override;

protected:
	HRESULT	SetUp_InstancingEffects(wstring wstrFbxPath, _uint iNumInstance, const INSTANCING_CREATE_DATA& tCreateData, _hashcode _hcCode);

protected:
	_bool	Fade_Lerp(_uint iIndex);
	virtual void	Dead_Instance(_uint iIndex);
	void	ReMake_OffsetPositions(_int iSize);
};

END