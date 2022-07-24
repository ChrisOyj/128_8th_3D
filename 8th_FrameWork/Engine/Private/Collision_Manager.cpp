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
		
			if ((*LeftIter) == (*RightIter)) // �� �ڽŰ� �浹 ����
			{
				++RightIter;
				continue;
			}

			COLLIDER_ID ID;
			ID.Left_id = (*LeftIter)->Get_ID();		// 4����Ʈ
			ID.Right_id = (*RightIter)->Get_ID();		// 4����Ʈ
													//���ļ� ���� 8����Ʈ�� ���� �Ȱ�ħ.

			iter = m_mapColInfo.find(ID.ID);

			//�浹 ������ �ƿ� �̵�� ������ ���
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false)); // �ƿ� ������� �ȵ��ִ� ��Ȳ�̹Ƿ� �ʿ� �߰�
				iter = m_mapColInfo.find(ID.ID); // ���� �� �ٽ� ���ͷ� �ް�
			}

	
			if (IsCollision((*LeftIter), (*RightIter)))
			{
				//���� �浹 ��
				if (iter->second) // ���ݵ� �浹���ε� �������� �浹���� ��Ȳ
				{
					if (!pLeftOwner->IsValid() || !pRightOwner->IsValid()) // ���� �ϳ��� ��ȿ���� ���� ���
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
				else // �̹��� ���� �浹
				{
					if (pLeftOwner->IsValid() && pRightOwner->IsValid()) // �Ѵ� Ȱ��ȭ�� ���� ����
					{
						pLeftOwner->OnCollisionEnter(pRightOwner, _eRight);
						pRightOwner->OnCollisionEnter(pLeftOwner, _eLeft);
						iter->second = true;
					}
				}
			}
			else // ���� �浹������ ����
			{
				if (iter->second) // ��ݱ��� �浹���̾���, �浹�� �� ����� ����
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

	// 2. ���� �������� �˻� 
	for (int i = 0; i < 3; ++i)
	{
		// ������ �Ǵ� �� ����
		D3DXVECTOR3 Axis_norm;
		D3DXVec3Normalize(&Axis_norm, &Axis_c1[i]);

		// 2 - 1. �࿡���� ���� A�� B ����
		map<float, UINT> mValue;
		UINT minIndex, maxIndex;

		//���� ª�������� ������ ã�Ƴ���.
		for (int v = 0; v < sizeof(RightBox.vWorldVertex) / sizeof(D3DXVECTOR3); ++v)
		{
			mValue.insert(make_pair(fabsf(D3DXVec3Dot(&Axis_norm, &RightBox.vWorldVertex[v])), v));
		}


		// �񱳵� ��� ����� �ִ�, ���� ������ Index ��ȣ

		map<float, UINT>::iterator iter;
		iter = mValue.begin();

		for (iter = mValue.begin(); iter != mValue.end(); ++iter) {}
		--iter;

		minIndex = mValue.begin()->second;
		maxIndex = iter->second;


		// padding
		maxIndex = maxIndex;

		// 1, 3, 4 ������ ���� ŭ
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

		// 2 - 2. �� ���������� ���� ������������ ���� Dist ����

		B1 = (D3DXVec3Dot(&Axis_norm, &LeftBox.ObbCenterPos));
		B2 = (D3DXVec3Dot(&Axis_norm, &RightBox.ObbCenterPos));
		Dist = fabsf(B1 - B2);

		// 2 - 3. Dist�� A * 0.5 + B * 0.5 �� ��
		if (Dist > A + B)
			return false;
	}

	// 2. ���� �������� �˻� 
	for (int i = 0; i < 3; ++i)
	{
		// ������ �Ǵ� �� ����
		D3DXVECTOR3 Axis_norm;
		D3DXVec3Normalize(&Axis_norm, &Axis_c2[i]);

		// 2 - 1. �࿡���� ���� A�� B ����
		map<float, UINT> mValue;
		UINT minIndex, maxIndex;

		//���� ª�������� ������ ã�Ƴ���.
		for (int v = 0; v < sizeof(LeftBox.vWorldVertex) / sizeof(_float3); ++v)
		{
			mValue.insert(make_pair((D3DXVec3Dot(&Axis_norm, &LeftBox.vWorldVertex[v])), v));
		}


		// �񱳵� ��� ����� �ִ�, ���� ������ Index ��ȣ
		map<float, UINT>::iterator iter;
		iter = mValue.begin();

		for (iter = mValue.begin(); iter != mValue.end(); ++iter) {}
		--iter;

		minIndex = mValue.begin()->second;
		maxIndex = iter->second;

		// 1, 3, 4 ������ ���� ŭ
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

		// 2 - 2. �� ���������� ���� ������������ ���� Dist ����

		B1 = (D3DXVec3Dot(&Axis_norm, &LeftBox.ObbCenterPos));
		B2 = (D3DXVec3Dot(&Axis_norm, &RightBox.ObbCenterPos));
		Dist = fabsf(B1 - B2);

		// 2 - 3. Dist�� A * 0.5 + B * 0.5 �� ��
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
