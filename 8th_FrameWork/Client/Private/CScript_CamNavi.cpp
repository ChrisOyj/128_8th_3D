#include "stdafx.h"
#include "CScript_CamNavi.h"
#include "GameInstance.h"

#include "Physics.h"
#include "Transform.h"

#include "CCell.h"
#include "GameObject.h"
#include "CTerrain.h"

CScript_CamNavi::CScript_CamNavi(_uint iGroupIdx)
	: CNavigation(iGroupIdx)
{
}

CScript_CamNavi::~CScript_CamNavi()
{
}

CScript_CamNavi* CScript_CamNavi::Create(_uint iGroupIdx, CCell* pStartCell, CPhysics* pPhysicsCom, CGameObject* pFollowTarget)
{
	CScript_CamNavi* pInstance = new CScript_CamNavi(iGroupIdx);

	pInstance->m_pCurCell = pStartCell;
	pInstance->m_pPhysicsCom = pPhysicsCom;
	pInstance->m_pFollowTarget = pFollowTarget;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CScript_CamNavi");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CScript_CamNavi::Tick()
{
	//1. 네비 안타고 그냥 움직이면서
	_float fSpeed = m_pPhysicsCom->Calculate_Speed();
	_float4 vCurPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
	_float4 vNewPos, vOutPlanePos, vFinalPos, vDir;
	vDir = m_pPhysicsCom->Get_Physics().vDir;

	vNewPos = vCurPos + vDir * fSpeed * fDT;


	m_bInWall = false;

	if (m_pCurCell->IsWall())
	{
		for (_uint i = 0; i < (_uint)CCell::POINT_END; ++i)
		{
			//세 점중 하나라도 내 높이보다 높다면 벽에 걸린거임.
			if (vCurPos.y + 1.f < XMVectorGetY(m_pCurCell->Get_Point(CCell::POINT(i))))
			{
				m_bInWall = true;
				break;
			}
		}
	}

	if (m_bInWall)
	{
		//벽에 들어있을 경우
		//벽의 법선벡터로 좀 밀려나야함
		vNewPos += m_pCurCell->Get_CellNormal() * 2.f * fDT;
	}

	vFinalPos = Correct_Move_Default(vNewPos);

	if (m_bInWall)
	{
		//벽에 들어있을 경우
		//벽의 법선벡터로 좀 밀려나야함
		vFinalPos += m_pCurCell->Get_CellNormal() * 2.f * fDT;
	}

	if (!m_pCurCell->IsWall())
	{
		_float fNewGroundY = Get_NewPosFromCell(m_pCurCell, vFinalPos.XMLoad()).y;
		m_pPhysicsCom->Get_PhysicsDetail().fCurGroundY = fNewGroundY;
	}


	//2. 플레이어와 나 사이의 셀을 조사
	_float4	vFollowTargetPos = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);
	vFollowTargetPos.y += 1.f;
	//내 위치와 플레이어의 셀을 던져서
	CCell* pOwnerCell = GET_COMPONENT_FROM(m_pFollowTarget, CNavigation)->Get_CurCell();
	CCell* pWallCell = Find_Cells(vNewPos, pOwnerCell);

	//3. 벽이 업승면 그냥 진행
	

	//4. 벽이 있으면
	if (pWallCell)
	{
		// 1. 플레이어가 벽보다 높은지 확인
		_bool bWallCol = false;
		for (_uint i = 0; i < (_uint)CCell::POINT_END; ++i)
		{
			//세 점중 하나라도 내 높이보다 높다면 벽에 걸린거임.
			if (vFollowTargetPos.y < XMVectorGetY(pWallCell->Get_Point(CCell::POINT(i))))
			{
				bWallCol = true;
				break;
			}
		}
		//벽에 걸렸으면 높이 갱신 안혀
		if (bWallCol)
		{
			m_bOnWall = true;
			CCell* FollowTargetCell = GET_COMPONENT_FROM(m_pFollowTarget, CNavigation)->Get_CurCell();
			_float fNewGroundY = Get_NewPosFromCell(FollowTargetCell, m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS).XMLoad()).y;
			m_pPhysicsCom->Get_PhysicsDetail().fCurGroundY = fNewGroundY;

			vFinalPos += (m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS) - vFinalPos).Normalize() * m_pPhysicsCom->Get_Physics().fSpeed * fDT;
		}

		//m_bOnWall = false;
	}


	//Check Air를 통해서 갱신해준 땅 높이보다 낮으면 땅으로 높이 보정
	_float fNewY = m_pPhysicsCom->Check_Air(vFinalPos.y);

	if (fNewY != -10.f)
		vFinalPos.y = fNewY;

	m_pOwner->Get_Transform()->Set_World(WORLD_POS, vFinalPos);

}

CCell* CScript_CamNavi::Find_Cells(_float4 vCurPos, CCell* pCurCell)
{
	CCell* pNeighBorCell = nullptr;
	CCell::LINE	eOutLine = CCell::LINE_END;

	if (pCurCell->isIn(vCurPos.XMLoad(), &pNeighBorCell, &eOutLine))
	{
		//안에 있으면
		return nullptr;
	}
	else
	{
		//이웃이 없으면
		while (1)
		{
			if (pNeighBorCell == nullptr)
				return nullptr;

			if (pNeighBorCell->IsWall())
			{
				//벽이면 찾은기
				return pNeighBorCell;
			}

			if (pNeighBorCell->isIn(vCurPos.XMLoad(), &pNeighBorCell, &eOutLine))
			{
				return nullptr;
			}
		}
	}
}

CCell* CScript_CamNavi::Find_CurrentCell(_float4 vCurPos, CCell* pCurCell)
{
	CCell* pNeighBorCell = nullptr;
	CCell::LINE	eOutLine = CCell::LINE_END;

	if (pCurCell->isIn(vCurPos.XMLoad(), &pNeighBorCell, &eOutLine))
	{
		//안에 있으면
		return pCurCell;
	}
	else
	{
		//이웃이 없으면
		while (1)
		{
			if (pNeighBorCell == nullptr)
				return pCurCell;

			if (pNeighBorCell->isIn(vCurPos.XMLoad(), &pNeighBorCell, &eOutLine))
			{
				return pNeighBorCell;
			}
		}
	}
}
