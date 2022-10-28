#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CCollider;

union COLLIDER_ID
{
	struct
	{
		_uint Left_ID;
		_uint Right_ID;
	};
	_ulonglong ID;

};

class CCollision_Manager
{
	DECLARE_SINGLETON(CCollision_Manager)

private:
	CCollision_Manager();
	virtual ~CCollision_Manager();

public:
	HRESULT		Initialize();
	void		Tick();
	void		Clear_All();

#ifdef _DEBUG
	HRESULT		Render();
#endif

public:
	HRESULT	Regist_Collider(CCollider* pCollider, const _uint& _iGroupIndex);
	HRESULT Check_Group(const _uint& _eLeft, const _uint& _eRight);

private:
	list<CCollider*>	m_pColliderList[COL_END];

	_uint m_arrCheck[COL_END];
	map<_ulonglong, _bool> m_mapColInfo;

	_bool	m_bRender = false;

private:
	void Collider_GroupUpdate(const _uint& _eLeft, const _uint& _eRight);

	_bool Is_Collision(CCollider* _pLeft, CCollider* _pRight);
	_bool Is_OBBCollision(CCollider* _pLeft, CCollider* _pRight);
	_bool Is_SphereCollision(CCollider* _pLeft, CCollider* _pRight);
	_bool Is_OBBtoSphereCollision(CCollider* _pOBB, CCollider* _pSphere);
	_bool Check_BeforeOBB(CCollider* _pLeft, CCollider* _pRight);

private:
	HRESULT	Is_InIndex(const _uint& iIdx);
	_float4	Compute_ColPosition(CCollider* _pLeft, CCollider* _pRight);

	map<_ulonglong, _bool>::iterator	Find_PrevColInfo(const _uint& _iLeftID, const _uint& _iRightID);



};



END
