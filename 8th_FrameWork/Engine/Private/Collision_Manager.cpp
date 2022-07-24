#include "..\Public\Collision_Manager.h"

#include "Collider.h"
#include "GameObject.h"

CCollision_Manager::CCollision_Manager()
{
}


CCollision_Manager::~CCollision_Manager()
{
}

HRESULT CCollision_Manager::Initialize()
{
	CheckGroup(COL_TYPE::PLAYER, COL_TYPE::OBSTACLE);
	CheckGroup(COL_TYPE::ENEMY, COL_TYPE::PLAYER);
	CheckGroup(COL_TYPE::ENEMY, COL_TYPE::ENEMY);
	CheckGroup(COL_TYPE::OBSTACLE, COL_TYPE::ENEMY);
	CheckGroup(COL_TYPE::TRAP, COL_TYPE::ENEMY);
	CheckGroup(COL_TYPE::OBSTACLE, COL_TYPE::BUILDPREVIEW);
	CheckGroup(COL_TYPE::TRAP, COL_TYPE::BUILDPREVIEW);
	CheckGroup(COL_TYPE::PLAYER_ATTACK, COL_TYPE::OBSTACLE);
	CheckGroup(COL_TYPE::PLAYER_ATTACK, COL_TYPE::ENEMY);
	CheckGroup(COL_TYPE::ENEMY_ATTACK, COL_TYPE::PLAYER);
	CheckGroup(COL_TYPE::PLAYER_FOOT, COL_TYPE::BUILDING);
	CheckGroup(COL_TYPE::ENEMY_ATTACK, COL_TYPE::OBSTACLE);

	CheckGroup(COL_TYPE::PUBLIC_ATTACK, COL_TYPE::OBSTACLE);
	CheckGroup(COL_TYPE::PUBLIC_ATTACK, COL_TYPE::ENEMY);
	CheckGroup(COL_TYPE::PUBLIC_ATTACK, COL_TYPE::PLAYER);

	return S_OK;
}

void CCollision_Manager::Tick()
{
	for (_uint iRow = 0; iRow < (_uint)COL_TYPE::LAST; ++iRow)
	{
		for (_uint iCol = iRow; iCol < (_uint)COL_TYPE::LAST; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				Collider_GroupUpdate((COL_TYPE)iRow, (COL_TYPE)iCol);
			}
		}
	}

	for (_uint i = 0; i < (_uint)COL_TYPE::LAST; ++i)
	{
		for (auto iter = m_ColliderList[i].begin(); iter != m_ColliderList[i].end();)
		{
			CGameObject* pOwner = (*iter)->Get_Owner()->Get_RootParent();

			if (!pOwner)
			{
				++iter;
				continue;
			}

			if (pOwner->IsDead())
				iter = m_ColliderList[i].erase(iter);
			else
				iter++;
		}
	}
}

HRESULT CCollision_Manager::Render()
{
	return S_OK;
}

void CCollision_Manager::Add_Collider(CCollider * pCollider)
{
	m_ColliderList[(_uint)pCollider->Get_ColType()].push_back(pCollider);
}

void CCollision_Manager::CheckGroup(COL_TYPE _eLeft, COL_TYPE _eRight)
{
	_uint iRow = (_uint)_eLeft;
	_uint iCol = (_uint)_eRight;

	if (iRow > iCol)
	{
		iRow = (_uint)_eRight;
		iCol = (_uint)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
		m_arrCheck[iRow] &= ~(1 << iCol);
	else
		m_arrCheck[iRow] |= (1 << iCol);
}

void CCollision_Manager::Collider_GroupUpdate(COL_TYPE _eLeft, COL_TYPE _eRight)
{
	list<CCollider*>& LeftList = m_ColliderList[(UINT)_eLeft];
	list<CCollider*>& RightList = m_ColliderList[(UINT)_eRight];

	map<_ulonglong, bool>::iterator iter;

	for (auto LeftIter = LeftList.begin(); LeftIter != LeftList.end();)
	{
		CGameObject* pLeftOwner = (*LeftIter)->Get_Owner();
		

		for (auto RightIter = RightList.begin(); RightIter != RightList.end();)
		{
			/* Owners */
			CGameObject* pRightOwner = (*RightIter)->Get_Owner();
		
			if ((*LeftIter) == (*RightIter)) // 나 자신과 충돌 방지
			{
				++RightIter;
				continue;
			}

			COLLIDER_ID ID;
			ID.Left_id = (*LeftIter)->Get_ID();		// 4바이트
			ID.Right_id = (*RightIter)->Get_ID();		// 4바이트
													//합쳐서 나온 8바이트는 절대 안겹침.

			iter = m_mapColInfo.find(ID.ID);

			//충돌 정보가 아예 미등록 상태인 경우
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false)); // 아예 등록조차 안되있던 상황이므로 맵에 추가
				iter = m_mapColInfo.find(ID.ID); // 넣은 값 다시 이터로 받고
			}

	
			if (IsCollision((*LeftIter), (*RightIter)))
			{
				//현재 충돌 중
				if (iter->second) // 지금도 충돌중인데 이전에도 충돌중인 상황
				{
					if (!pLeftOwner->IsValid() || !pRightOwner->IsValid()) // 둘중 하나가 유효하지 않을 경우
					{
						pLeftOwner->OnCollisionExit(pRightOwner, _eRight);
						pRightOwner->OnCollisionExit(pLeftOwner, _eLeft);
						iter->second = false;
						++RightIter;
						continue;
					}

					pLeftOwner->OnCollisionStay(pRightOwner, _eRight);
					pRightOwner->OnCollisionStay(pLeftOwner, _eLeft);

					
				}
				else // 이번에 최초 충돌
				{
					if (pLeftOwner->IsValid() && pRightOwner->IsValid()) // 둘다 활성화일 때만 실행
					{
						pLeftOwner->OnCollisionEnter(pRightOwner, _eRight);
						pRightOwner->OnCollisionEnter(pLeftOwner, _eLeft);
						iter->second = true;
					}
				}
			}
			else // 현재 충돌중이지 않음
			{
				if (iter->second) // 방금까지 충돌중이었음, 충돌이 막 벗어나진 시점
				{
					pLeftOwner->OnCollisionExit(pRightOwner, _eRight);
					pRightOwner->OnCollisionExit(pLeftOwner, _eLeft);
					iter->second = false;
				}
			}
			/*if (pRightOwner->IsDead())
			{
				RightIter = RightList.erase(RightIter);
				continue;
			}
			*/
			RightIter++;
		}
		/*if (pLeftOwner->IsDead())
		{
			LeftIter = LeftList.erase(LeftIter);
			continue;
		}*/
		LeftIter++;
	}
}

bool CCollision_Manager::IsCollision(CCollider * _pLeft, CCollider * _pRight)
{
	if (_pLeft->IsSphere() || _pRight->IsSphere())
	{
		return IsSphereCollision(_pLeft, _pRight);
	}
	else
	{
		if (IsSphereCollision(_pLeft, _pRight))
		{
			return IsOBBCollision(_pLeft, _pRight);
		}
		else
			return false;
	}
}

bool CCollision_Manager::IsOBBCollision(CCollider * _pLeft, CCollider * _pRight)
{
	OBBINFO LeftBox = _pLeft->Get_ObbInfo();
	OBBINFO RightBox = _pRight->Get_ObbInfo();

	D3DXVECTOR3 Axis_c1[3] = {
		LeftBox.vWorldVertex[1] - LeftBox.vWorldVertex[0],
		LeftBox.vWorldVertex[3] - LeftBox.vWorldVertex[0],
		LeftBox.vWorldVertex[4] - LeftBox.vWorldVertex[0] };

	D3DXVECTOR3 Axis_c2[3] = {
		RightBox.vWorldVertex[1] - RightBox.vWorldVertex[0],
		RightBox.vWorldVertex[3] - RightBox.vWorldVertex[0],
		RightBox.vWorldVertex[4] - RightBox.vWorldVertex[0] };

	// 2. 축을 기준으로 검산 
	for (int i = 0; i < 3; ++i)
	{
		// 기준이 되는 축 생성
		D3DXVECTOR3 Axis_norm;
		D3DXVec3Normalize(&Axis_norm, &Axis_c1[i]);

		// 2 - 1. 축에대한 길이 A와 B 도출
		map<float, UINT> mValue;
		UINT minIndex, maxIndex;

		//가장 짧은길이의 정점을 찾아낸다.
		for (int v = 0; v < sizeof(RightBox.vWorldVertex) / sizeof(D3DXVECTOR3); ++v)
		{
			mValue.insert(make_pair(fabsf(D3DXVec3Dot(&Axis_norm, &RightBox.vWorldVertex[v])), v));
		}


		// 비교될 상대 대상의 최단, 최장 정점의 Index 번호

		map<float, UINT>::iterator iter;
		iter = mValue.begin();

		for (iter = mValue.begin(); iter != mValue.end(); ++iter) {}
		--iter;

		minIndex = mValue.begin()->second;
		maxIndex = iter->second;


		// padding
		maxIndex = maxIndex;

		// 1, 3, 4 순서로 가장 큼
		FLOAT A, B, Dist;
		D3DXVECTOR3 vec;
		switch (i)
		{
		case 0:
			vec = LeftBox.vWorldVertex[1] - LeftBox.vWorldVertex[0];
			break;

		case 1:
			vec = LeftBox.vWorldVertex[3] - LeftBox.vWorldVertex[0];
			break;

		case 2:
			vec = LeftBox.vWorldVertex[4] - LeftBox.vWorldVertex[0];
			break;
		}

		A = fabsf(D3DXVec3Length(&vec));

		D3DXVECTOR3 max = RightBox.vWorldVertex[maxIndex];
		D3DXVECTOR3 min = RightBox.vWorldVertex[minIndex];

		FLOAT B1 = (D3DXVec3Dot(&Axis_norm, &RightBox.vWorldVertex[maxIndex]));
		FLOAT B2 = (D3DXVec3Dot(&Axis_norm, &RightBox.vWorldVertex[minIndex]));

		B = fabs(B1 - B2);

		A = A * 0.5F;
		B = B * 0.5F;

		// 2 - 2. 두 원점에대한 축의 내적연산으로 길이 Dist 도출

		B1 = (D3DXVec3Dot(&Axis_norm, &LeftBox.ObbCenterPos));
		B2 = (D3DXVec3Dot(&Axis_norm, &RightBox.ObbCenterPos));
		Dist = fabsf(B1 - B2);

		// 2 - 3. Dist와 A * 0.5 + B * 0.5 를 비교
		if (Dist > A + B)
			return false;
	}

	// 2. 축을 기준으로 검산 
	for (int i = 0; i < 3; ++i)
	{
		// 기준이 되는 축 생성
		D3DXVECTOR3 Axis_norm;
		D3DXVec3Normalize(&Axis_norm, &Axis_c2[i]);

		// 2 - 1. 축에대한 길이 A와 B 도출
		map<float, UINT> mValue;
		UINT minIndex, maxIndex;

		//가장 짧은길이의 정점을 찾아낸다.
		for (int v = 0; v < sizeof(LeftBox.vWorldVertex) / sizeof(_float3); ++v)
		{
			mValue.insert(make_pair((D3DXVec3Dot(&Axis_norm, &LeftBox.vWorldVertex[v])), v));
		}


		// 비교될 상대 대상의 최단, 최장 정점의 Index 번호
		map<float, UINT>::iterator iter;
		iter = mValue.begin();

		for (iter = mValue.begin(); iter != mValue.end(); ++iter) {}
		--iter;

		minIndex = mValue.begin()->second;
		maxIndex = iter->second;

		// 1, 3, 4 순서로 가장 큼
		FLOAT A, B, Dist;
		D3DXVECTOR3 vec;
		switch (i)
		{
		case 0:
			vec = RightBox.vWorldVertex[1] - RightBox.vWorldVertex[0];
			break;

		case 1:
			vec = RightBox.vWorldVertex[3] - RightBox.vWorldVertex[0];
			break;

		case 2:
			vec = RightBox.vWorldVertex[4] - RightBox.vWorldVertex[0];
			break;
		}

		A = fabsf(D3DXVec3Length(&vec));

		FLOAT B1 = (D3DXVec3Dot(&Axis_norm, &LeftBox.vWorldVertex[maxIndex]));
		FLOAT B2 = (D3DXVec3Dot(&Axis_norm, &LeftBox.vWorldVertex[minIndex]));

		B = fabs(B1 - B2);

		A = A * 0.5F;
		B = B * 0.5F;

		// 2 - 2. 두 원점에대한 축의 내적연산으로 길이 Dist 도출

		B1 = (D3DXVec3Dot(&Axis_norm, &LeftBox.ObbCenterPos));
		B2 = (D3DXVec3Dot(&Axis_norm, &RightBox.ObbCenterPos));
		Dist = fabsf(B1 - B2);

		// 2 - 3. Dist와 A * 0.5 + B * 0.5 를 비교
		if (Dist > A + B)
			return false;
	}

	return true;
}

bool CCollision_Manager::IsSphereCollision(CCollider * _pLeft, CCollider * _pRight)
{
	OBBINFO LeftInfo = _pLeft->Get_ObbInfo();
	OBBINFO RightInfo = _pRight->Get_ObbInfo();

	_float3 CenterDiff = RightInfo.ObbCenterPos - LeftInfo.ObbCenterPos;
	_float Dist = fabsf(CenterDiff.Length());

	_float LeftRadius = (LeftInfo.ObbCenterPos - LeftInfo.vWorldVertex[0]).Length();
	_float RightRadius = (RightInfo.ObbCenterPos - RightInfo.vWorldVertex[0]).Length();

	if (Dist > LeftRadius + RightRadius)
	{
		return false;
	}
	else
	{
		return true;
	}
}
