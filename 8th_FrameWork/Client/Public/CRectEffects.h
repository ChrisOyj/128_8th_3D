#pragma once
#include "CInstancingEffects.h"

BEGIN(Client)
class CRectEffects
	: public CInstancingEffects
{
	DECLARE_PROTOTYPE(CRectEffects);

protected:
	CRectEffects();
	CRectEffects(const CRectEffects& _origin);
	virtual ~CRectEffects();

	friend class CWindow_Effect;

public:
	static CRectEffects* Create(_uint iNumInstance, const INSTANCING_CREATE_DATA& tCreateData, wstring wstrTexturePath,
		_hashcode _hcCode, _bool bBillBoard = true, _bool bSorting = true);

	static CRectEffects* Create(_float4 vStartPos);

	static CRectEffects* Create(ifstream* pReadFile);
	static CRectEffects* Create_Anim(ifstream* pReadFile);

public:
	virtual void		Self_Reset(CGameObject* pGameObject, _float4 vStartPos);

	virtual void	Set_ShaderResourceFlag(CShader* pShader, const char* pConstantName) override;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Re_Initialize() override;
	virtual HRESULT Start() override;

protected:
	virtual void	My_Tick() override;
	virtual void	My_LateTick() override;
	virtual void	OnEnable() override;

private:
	VTXRECTINSTANCE* m_pRectInstances = nullptr;

	_bool		m_bBillBoard = false;
	_bool		m_bSorting = false;

	_uint		m_iWidthSize = 1;
	_uint		m_iHeightSize = 1;

	_float		m_fDissolvePower = 2.f;
	_float		m_fDuration = 0.1f;


private:
	virtual void	Dead_Instance(_uint iIndex) override;
	void		Set_NewStartPos(_uint iIndex);

private:
	HRESULT	SetUp_RectEffects(ifstream* pReadFile);
	HRESULT	SetUp_RectEffects_Anim(ifstream* pReadFile);

	void		Update_Animation(_uint iIndex);

};

END