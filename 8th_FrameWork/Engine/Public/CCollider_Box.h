#pragma once
#include "Collider.h"

BEGIN(Engine)

class CGameObject;

typedef struct tag_OBBInfo
{
	_float4 vOffsetPos;
	_float4 vFinalPos;	// OBB박스의 중점(오브젝트의 정 가운데가 되어야 함)
	_float4 ObbSize;		// x, y, z 축 변의 길이
	_float4 vLocalVertex[8];		// 로컬 인덱스 8개
	_float4 vWorldVertex[8];		// 로컬 인덱스 8개
	_float4 LocalAxis[3];	// 로컬 투영 축
	_float4 WorldAxis[3];	// 월드 투영 축

#pragma region Functions
	tag_OBBInfo()
	{
		vFinalPos = _float4(0.f, 0.f, 0.f);
		ObbSize = _float4(0.f, 0.f, 0.f);
		LocalAxis[0] = _float4(1.f, 0.f, 0.f);
		LocalAxis[1] = _float4(0.f, 1.f, 0.f);
		LocalAxis[2] = _float4(0.f, 0.f, 1.f);
	}
	tag_OBBInfo(const _float4& size, const _float4& _vOffsetPos)
	{
		ObbSize = size;
		vOffsetPos = _vOffsetPos;
		LocalAxis[0] = _float4(1.f, 0.f, 0.f);
		LocalAxis[1] = _float4(0.f, 1.f, 0.f);
		LocalAxis[2] = _float4(0.f, 0.f, 1.f);

		//0-3 -> z > 0, 반시계
		vLocalVertex[0] = _float4( vOffsetPos.x - (ObbSize.x * 0.5f), vOffsetPos.y + (ObbSize.y * 0.5f), vOffsetPos.z - (ObbSize.z * 0.5f) );
		vLocalVertex[1] = _float4( vOffsetPos.x + (ObbSize.x * 0.5f), vOffsetPos.y + (ObbSize.y * 0.5f), vOffsetPos.z - (ObbSize.z * 0.5f) );
		vLocalVertex[2] = _float4( vOffsetPos.x + (ObbSize.x * 0.5f), vOffsetPos.y - (ObbSize.y * 0.5f), vOffsetPos.z - (ObbSize.z * 0.5f) );
		vLocalVertex[3] = _float4( vOffsetPos.x - (ObbSize.x * 0.5f), vOffsetPos.y - (ObbSize.y * 0.5f), vOffsetPos.z - (ObbSize.z * 0.5f) );

		//4-7 -> z < 0, 시계(축 반전시 반시계)
		vLocalVertex[4] = _float4( vOffsetPos.x - (ObbSize.x * 0.5f), vOffsetPos.y + (ObbSize.y * 0.5f), vOffsetPos.z + (ObbSize.z * 0.5f) );
		vLocalVertex[5] = _float4( vOffsetPos.x + (ObbSize.x * 0.5f), vOffsetPos.y + (ObbSize.y * 0.5f), vOffsetPos.z + (ObbSize.z * 0.5f) );
		vLocalVertex[6] = _float4( vOffsetPos.x + (ObbSize.x * 0.5f), vOffsetPos.y - (ObbSize.y * 0.5f), vOffsetPos.z + (ObbSize.z * 0.5f) );
		vLocalVertex[7] = _float4( vOffsetPos.x - (ObbSize.x * 0.5f), vOffsetPos.y - (ObbSize.y * 0.5f), vOffsetPos.z + (ObbSize.z * 0.5f) );
	}

	//void Set_float4x4(_float4x4* pmatTrans)
	//{
	//	_float4x4 WorldMat = *pmatTrans;

	//	(*((_float4*)&WorldMat.m[0])).Normalize();
	//	(*((_float4*)&WorldMat.m[1])).Normalize();
	//	(*((_float4*)&WorldMat.m[2])).Normalize();

	//	for (int i = 0; i < 8; ++i)
	//	{
	//		D3DXVec3TransformCoord(&vWorldVertex[i], &vLocalVertex[i], &WorldMat);
	//	}

	//	D3DXVec3TransformCoord(&ObbCenterPos, &vOffsetPos, &WorldMat);

	//	//WorldAxis[0] = (*((_float4*)&WorldMat.m[0])).Normalize();
	//	//WorldAxis[1] = (*((_float4*)&WorldMat.m[1])).Normalize();
	//	//WorldAxis[2] = (*((_float4*)&WorldMat.m[2])).Normalize();
	//}
#pragma endregion
}COL_INFO_BOX;

class ENGINE_DLL CCollider_Box final
	: public CCollider
{
	DECLARE_PROTOTYPE(CCollider_Box)

private:
	CCollider_Box(_uint iGroupID);
	virtual ~CCollider_Box();

public:
	static	CCollider_Box* Create(_uint iGroupID, _float4 vSize, const _uint& iColIndex, _float4 vOffsetPos);

public:
	COL_INFO_BOX& Get_ColInfo() { return m_tColInfo; }
	void	Set_BoxInfo(const _float4& vSize, const _float4& vOffsetPos);

public:
	virtual void Late_Tick() override;

private:
	COL_INFO_BOX	m_tColInfo;


};

END
