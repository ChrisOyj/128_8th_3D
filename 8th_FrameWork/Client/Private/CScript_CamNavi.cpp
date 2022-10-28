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
	//1. �׺� ��Ÿ�� �׳� �����̸鼭
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
			//�� ���� �ϳ��� �� ���̺��� ���ٸ� ���� �ɸ�����.
			if (vCurPos.y + 1.f < XMVectorGetY(m_pCurCell->Get_Point(CCell::POINT(i))))
			{
				m_bInWall = true;
				break;
			}
		}
	}

	if (m_bInWall)
	{
		//���� ������� ���
		//���� �������ͷ� �� �з�������
		vNewPos += m_pCurCell->Get_CellNormal() * 2.f * fDT;
	}

	vFinalPos = Correct_Move_Default(vNewPos);

	if (m_bInWall)
	{
		//���� ������� ���
		//���� �������ͷ� �� �з�������
		vFinalPos += m_pCurCell->Get_CellNormal() * 2.f * fDT;
	}

	if (!m_pCurCell->IsWall())
	{
		_float fNewGroundY = Get_NewPosFromCell(m_pCurCell, vFinalPos.XMLoad()).y;
		m_pPhysicsCom->Get_PhysicsDetail().fCurGroundY = fNewGroundY;
	}


	//2. �÷��̾�� �� ������ ���� ����
	_float4	vFollowTargetPos = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);
	vFollowTargetPos.y += 1.f;
	//�� ��ġ�� �÷��̾��� ���� ������
	CCell* pOwnerCell = GET_COMPONENT_FROM(m_pFollowTarget, CNavigation)->Get_CurCell();
	CCell* pWallCell = Find_Cells(vNewPos, pOwnerCell);

	//3. ���� ���¸� �׳� ����
	

	//4. ���� ������
	if (pWallCell)
	{
		// 1. �÷��̾ ������ ������ Ȯ��
		_bool bWallCol = false;
		for (_uint i = 0; i < (_uint)CCell::POINT_END; ++i)
		{
			//�� ���� �ϳ��� �� ���̺��� ���ٸ� ���� �ɸ�����.
			if (vFollowTargetPos.y < XMVectorGetY(pWallCell->Get_Point(CCell::POINT(i))))
			{
				bWallCol = true;
				break;
			}
		}
		//���� �ɷ����� ���� ���� ����
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


	//Check Air�� ���ؼ� �������� �� ���̺��� ������ ������ ���� ����
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
		//�ȿ� ������
		return nullptr;
	}
	else
	{
		//�̿��� ������
		while (1)
		{
			if (pNeighBorCell == nullptr)
				return nullptr;

			if (pNeighBorCell->IsWall())
			{
				//���̸� ã����
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
		//�ȿ� ������
		return pCurCell;
	}
	else
	{
		//�̿��� ������
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
