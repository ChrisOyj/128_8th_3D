#pragma once
#include "Collider.h"

BEGIN(Engine)

struct COL_INFO_SPHERE
{
	_float fRadius = 0.f;
	_float4 vOffsetPos = { 0.f, 0.f, 0.f, 1.f };
	_float4 vFinalPos = { 0.f, 0.f, 0.f, 1.f };
};

class ENGINE_DLL CCollider_Sphere final
	: public CCollider
{
	DECLARE_PROTOTYPE(CCollider_Sphere);

private:
	CCollider_Sphere(CGameObject* pOwner);
	virtual ~CCollider_Sphere();

public:
	static	CCollider_Sphere* Create(CGameObject* pOwner, _float fRadius, const _uint& iColIndex, _float4 vOffsetPos);

public:
	COL_INFO_SPHERE& Get_ColInfo() { return m_tColInfo; }

public:
	virtual void Late_Tick() override;

private:
	COL_INFO_SPHERE	m_tColInfo;


};

END