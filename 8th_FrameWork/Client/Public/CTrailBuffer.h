#pragma once
#include "CMesh.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CHierarchyNode;
class CGameObject;
class CTransform;
END

BEGIN(Client)
class CTrailBuffer
	: public CMesh
{
	DECLARE_PROTOTYPE(CTrailBuffer);

private:
	CTrailBuffer(_uint iGroupIdx);
	virtual ~CTrailBuffer();

public:
	static CTrailBuffer* Create(_uint iGroupIdx, _uint iTriCnt, _float4 vLocalSwordLow, _float4 vLocalSwordHigh, CHierarchyNode* pSwordBone, CTransform* pUnitTransform);

public:
	virtual _uint3 Get_Indices(_uint iIndex) const override;

	void	Set_NoCurve() { m_bCatMullRom = false; }
	void	Set_TrailOn();
	void	Set_TrailOff();

public:
	// CMesh을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual	void	Tick() override;

	virtual	void	Late_Tick() override;

private:
	void	Update_Curve();
	void	Update_NonCurve();

private:
	_bool	m_bCatMullRom = true;
	_bool	m_bTrailOn = false;

	_uint	m_iTriCnt = 0;
	_uint	m_iVtxCount = 0;

	_float4	m_vLocalSwordLow;
	_float4	m_vLocalSwordHigh;

	CHierarchyNode* m_pRefBone = nullptr;
	CTransform* m_pOwnerTransform = nullptr;

	_float4x4	m_TransformationMatrix;

	_float		m_fTimeAcc = 0.f;
	_float		m_fCreateTrailTimer = 0.01f;

private:
	_uint		m_iEndIndex = 0; // Catmullrom을 위한 인덱스
	_uint		m_iCatmullRomCount = 0; // 보간될 값의 Cnt
	_uint		m_iCatmullRomIndex[4] = {};

};

END