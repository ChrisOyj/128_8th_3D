#pragma once
#include "Component.h"

BEGIN(Engine)
typedef struct tag_OBBInfo
{
	_float3 vOffsetPos;
	_float3 ObbCenterPos;	// OBB박스의 중점(오브젝트의 정 가운데가 되어야 함)
	_float3 ObbSize;		// x, y, z 축 변의 길이
	_float3 vLocalVertex[8];		// 로컬 인덱스 8개
	_float3 vWorldVertex[8];		// 로컬 인덱스 8개
	_float3 LocalAxis[3];	// 로컬 투영 축
	_float3 WorldAxis[3];	// 월드 투영 축

#pragma region Functions
	tag_OBBInfo()
	{
		ObbCenterPos = { 0, 0, 0 };
		ObbSize = { 0, 0, 0 };
		LocalAxis[0] = { 1, 0, 0 };
		LocalAxis[1] = { 0, 1, 0 };
		LocalAxis[2] = { 0, 0, 1 };
	}
	tag_OBBInfo(const _float3& size, const _float3& _vOffsetPos)
	{
		ObbSize = size;
		vOffsetPos = _vOffsetPos;
		LocalAxis[0] = { 1, 0, 0 };
		LocalAxis[1] = { 0, 1, 0 };
		LocalAxis[2] = { 0, 0, 1 };

		//0-3 -> z > 0, 반시계
		vLocalVertex[0] = { vOffsetPos.x - (ObbSize.x * 0.5f), vOffsetPos.y + (ObbSize.y * 0.5f), vOffsetPos.z - (ObbSize.z * 0.5f) };
		vLocalVertex[1] = { vOffsetPos.x + (ObbSize.x * 0.5f), vOffsetPos.y + (ObbSize.y * 0.5f), vOffsetPos.z - (ObbSize.z * 0.5f) };
		vLocalVertex[2] = { vOffsetPos.x + (ObbSize.x * 0.5f), vOffsetPos.y - (ObbSize.y * 0.5f), vOffsetPos.z - (ObbSize.z * 0.5f) };
		vLocalVertex[3] = { vOffsetPos.x - (ObbSize.x * 0.5f), vOffsetPos.y - (ObbSize.y * 0.5f), vOffsetPos.z - (ObbSize.z * 0.5f) };

		//4-7 -> z < 0, 시계(축 반전시 반시계)
		vLocalVertex[4] = { vOffsetPos.x - (ObbSize.x * 0.5f), vOffsetPos.y + (ObbSize.y * 0.5f), vOffsetPos.z + (ObbSize.z * 0.5f) };
		vLocalVertex[5] = { vOffsetPos.x + (ObbSize.x * 0.5f), vOffsetPos.y + (ObbSize.y * 0.5f), vOffsetPos.z + (ObbSize.z * 0.5f) };
		vLocalVertex[6] = { vOffsetPos.x + (ObbSize.x * 0.5f), vOffsetPos.y - (ObbSize.y * 0.5f), vOffsetPos.z + (ObbSize.z * 0.5f) };
		vLocalVertex[7] = { vOffsetPos.x - (ObbSize.x * 0.5f), vOffsetPos.y - (ObbSize.y * 0.5f), vOffsetPos.z + (ObbSize.z * 0.5f) };
	}

	void Set_float4x4(_float4x4* pmatTrans)
	{
		_float4x4 WorldMat = *pmatTrans;

		(*((_float3*)&WorldMat.m[0])).Normalize();
		(*((_float3*)&WorldMat.m[1])).Normalize();
		(*((_float3*)&WorldMat.m[2])).Normalize();

		for (int i = 0; i < 8; ++i)
		{
			D3DXVec3TransformCoord(&vWorldVertex[i], &vLocalVertex[i], &WorldMat);
		}

		D3DXVec3TransformCoord(&ObbCenterPos, &vOffsetPos, &WorldMat);

		//WorldAxis[0] = (*((_float3*)&WorldMat.m[0])).Normalize();
		//WorldAxis[1] = (*((_float3*)&WorldMat.m[1])).Normalize();
		//WorldAxis[2] = (*((_float3*)&WorldMat.m[2])).Normalize();
	}
#pragma endregion

}OBBINFO;

class ENGINE_DLL CCollider final
	: public CComponent
{
	DECLARE_PROTOTYPE(CCollider)

private:
	CCollider();
	CCollider(const CCollider& origin);
	virtual ~CCollider();

public:
	static	CCollider*	Create(_float3 vSize, COL_TYPE eColType, _float3 vOffsetPos, bool bSphere = false);

public:
	const OBBINFO&			Get_ObbInfo()	const	{ return m_tObbInfo; }
	COL_TYPE				Get_ColType()	const	{ return m_eColType; }
	const _uint&			Get_ID()		const	{ return m_iID; }

	void					Set_ColType(const COL_TYPE& eColType)		{ m_eColType = eColType; }
	void					Set_Sphere(bool bSphere)					{ m_bSphere = bSphere; }
	void					Set_ObbInfo(const _float3& _vSize, const _float3& _vOffsetPos = _float3(0.f, 0.f, 0.f));

	bool					IsSphere()		const	{ return m_bSphere; }

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;


private:
	static _uint	g_iNextID;
	_uint			m_iID = 0; // 충돌체 고유한 ID 값
	bool			m_bSphere = false;
	COL_TYPE		m_eColType = COL_TYPE::LAST;
	OBBINFO			m_tObbInfo;

	
};

END