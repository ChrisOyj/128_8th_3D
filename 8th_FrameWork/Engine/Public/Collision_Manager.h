#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CCollider;

union COLLIDER_ID
{
	struct
	{
		_uint Left_id;
		_uint Right_id;
	};
	_ulonglong ID;

};

class CCollision_Manager
	: public CManager
{
	DECLARE_SINGLETON(CCollision_Manager)

private:
	CCollision_Manager();
	virtual ~CCollision_Manager();

public:
	HRESULT		Initialize();
	void		Tick();
	HRESULT		Render();

private:
	/* Only Collider class can access this*/
	friend class CCollider;
	void Add_Collider(CCollider* pCollider);

private:
	void CheckGroup(COL_TYPE _eLeft, COL_TYPE _eRight);
	void Collider_GroupUpdate(COL_TYPE _eLeft, COL_TYPE _eRight);
	bool IsCollision(CCollider* _pLeft, CCollider* _pRight);
	bool IsOBBCollision(CCollider* _pLeft, CCollider* _pRight);
	bool IsSphereCollision(CCollider* _pLeft, CCollider* _pRight);


private:
	list<CCollider*>	m_ColliderList[(_uint)COL_TYPE::LAST];

	_uint m_arrCheck[(_uint)COL_TYPE::LAST];

	map<_ulonglong, bool> m_mapColInfo;
};



END
