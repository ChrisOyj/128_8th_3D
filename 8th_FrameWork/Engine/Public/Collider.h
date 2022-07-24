#pragma once
#include "Component.h"

BEGIN(Engine)
typedef struct tag_OBBInfo
{
	_float3 vOffsetPos;
	_float3 ObbCenterPos;	// OBB�ڽ��� ����(������Ʈ�� �� ����� �Ǿ�� ��)
	_float3 ObbSize;		// x, y, z �� ���� ����
	_float3 vLocalVertex[8];		// ���� �ε��� 8��
	_float3 vWorldVertex[8];		// ���� �ε��� 8��
	_float3 LocalAxis[3];	// ���� ���� ��
	_float3 WorldAxis[3];	// ���� ���� ��

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

		//0-3 -> z > 0, �ݽð�
		vLocalVertex[0] = { vOffsetPos.x - (ObbSize.x * 0.5f), vOffsetPos.y + (ObbSize.y * 0.5f), vOffsetPos.z - (ObbSize.z * 0.5f) };
		vLocalVertex[1] = { vOffsetPos.x + (ObbSize.x * 0.5f), vOffsetPos.y + (ObbSize.y * 0.5f), vOffsetPos.z - (ObbSize.z * 0.5f) };
		vLocalVertex[2] = { vOffsetPos.x + (ObbSize.x * 0.5f), vOffsetPos.y - (ObbSize.y * 0.5f), vOffsetPos.z - (ObbSize.z * 0.5f) };
		vLocalVertex[3] = { vOffsetPos.x - (ObbSize.x * 0.5f), vOffsetPos.y - (ObbSize.y * 0.5f), vOffsetPos.z - (ObbSize.z * 0.5f) };

		//4-7 -> z < 0, �ð�(�� ������ �ݽð�)
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
	// CComponent��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;


private:
	static _uint	g_iNextID;
	_uint			m_iID = 0; // �浹ü ������ ID ��
	bool			m_bSphere = false;
	COL_TYPE		m_eColType = COL_TYPE::LAST;
	OBBINFO			m_tObbInfo;

	
};

END