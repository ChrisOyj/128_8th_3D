#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

#define		EFFECT_FOLLOWTARGET 0x001
#define		EFFECT_TARGETPOS 0x002
#define		EFFECT_REFBONE 0x004
#define		EFFECT_COLLIDER 0x008
#define		EFFECT_BILLBOARD 0x010
#define		EFFECT_TEMP 0x020

BEGIN(Engine)
class CShader;
class CHierarchyNode;
END

BEGIN(Client)

class CEffect abstract
	: public CGameObject
{
	friend class CWindow_Effect;
	friend class CWindow_Test;

public:
	enum EFFECT_TYPE
	{
		MESH,
		PARTICLE,
		ANIMPARTICLE,
		EFFECT_END,
	};
	enum DISABLE_TYPE {FADE, UV, WALL, NONE, DISABLE_END};

protected:
	CEffect();
	virtual ~CEffect();

public:
	static	CEffect* Create_Effect_FromBinFile(string strFileKey);

public:
	void	Set_EffectFlag(_byte bFlag) { m_bEffectFlag = bFlag; }
	void	Set_GlowFlag(_float4 vGlowFlag) { m_vGlowFlag = vGlowFlag; }

public:
	void		Set_OffsetPos(_float4 vPos) { m_vOffsetPos = vPos; }
	virtual void		Self_Reset(CGameObject* pGameObject, _float4 vStartPos);
	virtual void		Self_Reset_Turn(CGameObject* pGameObject, _float4 vStartPos);
	virtual void		Reset(_float4 vStartPos);
	void		Reset(CGameObject* pGameObject);
	void		Reset(_float4 vStartPos, _float4 vTargetPosition);
	void		Reset(_float4 vStartPos, _float4x4 matTrans);
	virtual void		Reset(_float4x4 matWorld);
	
	void		Set_RefBone(CHierarchyNode* pNode) { m_pRefBone = pNode; m_fTimeAcc = 0.f; m_bEffectFlag |= EFFECT_REFBONE; }
	void		Set_FollowTarget(CGameObject* pTarget) { m_pFollowTarget = pTarget; }

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObj, const _uint& eColType, _float4 vColPos);
	virtual void	Set_ShaderResource(CShader* pShader, const char* pConstantName);
	virtual void	Set_ShaderResourceAlpha(CShader* pShader, const char* pConstantName);
	virtual void	Set_ShaderResourceFlag(CShader* pShader, const char* pConstantName);
	virtual void	Set_ShaderResourceGlowFlag(CShader* pShader, const char* pConstantName);

public:
	void		Set_ColliderOn(_float fRadius, COL_GROUP_CLIENT eColType);
	void		Set_FadeInStartTime(_float fTime) { m_fFadeInStartTime = fTime; }


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Re_Initialize() { return S_OK; };
	virtual HRESULT Start() override;

protected:
	_float	m_fColliderAcc = 0.f;
	_float	m_fColliderMaxTime = 0.1f;
	_bool	m_bColliderOn = false;

protected:
	_float	m_fTurnAngle = 0.f;

	EFFECT_TYPE	m_eEffectType = MESH;
	SHADER_FILE_ID	m_eShaderType = SHADER_VTXEFFECTS;

	_float4	m_vEffectFlag = SH_EFFECT_DEFAULT;
	_float4	m_vGlowFlag = GLOW_CHAKRA(1.f);

	_float	m_fAlpha = 1.f;
	_float	m_fCurUVPlusX = 0.f;
	_float	m_fCurUVPlusY = 0.f;
	_float	m_fTimeAcc = 0.f;


	_float	m_fUVSpeedX = 0.f;
	_float	m_fUVSpeedY = 0.f;


	wstring	m_wstrPath;
	wstring	m_wstrMaskMapPath;
	wstring	m_wstrColorMapPath;
	wstring	m_wstrNormalMapPath;

	_float4x4 m_matTrans;

	DISABLE_TYPE		m_eDisableType = NONE;
	_uint				m_iPassType = 0;

	_hashcode	m_hcMyCode;

	_byte		m_bEffectFlag = 0;


	_float4x4	m_matRotZ;

public:
	void	Set_DisableType(DISABLE_TYPE eType) { m_eDisableType = eType; }

protected:
	_float4		m_vTurnDir = _float4(0.f, 0.f, 0.f, 0.f);
	_float		m_fTurnSpeed = 0.f;

protected:
	CGameObject* m_pFollowTarget = nullptr;
	_float4		m_vOffsetPos;
	void	Update_FollowTarget();

protected:
	_float		m_fMoveSpeed = 0.f;
	_float4		m_vTargetPosition;
	void	Update_TargetPos();
	virtual void	On_Target();

protected:
	CHierarchyNode* m_pRefBone = nullptr;
	virtual void	Stick_RefBone();

protected:
	enum FADETYPE
	{
		FADEINREADY,
		FADEIN,
		FADEOUTREADY,
		FADEOUT,
		FADE_END,
	};
	FADETYPE	m_eCurFadeType = FADEINREADY;
	_float	m_fFadeTimeAcc = 0.f;
	_float	m_fFadeInStartTime = 0.f;
	_float	m_fFadeInTime = 0.f;
	_float	m_fFadeOutStartTime = 9999.f;
	_float	m_fFadeOutTime = 0.f;
	
	_float4	m_vStartScale = _float4(1.f, 1.f, 1.f, 1.f);
	_float4	m_vFadeInTargetScale = _float4(1.f, 1.f, 1.f, 1.f);
	_float4	m_vFadeOutTargetScale = _float4(1.f, 1.f, 1.f, 1.f);
	_float	m_fTargetAlpha = 1.f;
	_float4	m_vColor;

	public:
		void	Set_FadeOut() { m_eCurFadeType = FADEOUTREADY; m_fFadeTimeAcc = 999999.f; }

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual	void	OnDisable() override;

protected:
	void		Update_Disable();
	void		Update_Turn();
	void		Update_Fade();
};

END