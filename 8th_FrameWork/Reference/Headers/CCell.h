#pragma once
#include "Engine_Defines.h"

#define CELL_BLOCKED	0x01
#define CELL_WALL		0x02
#define CELL_GROUND		0x04
#define CELL_GRASS		0x08

BEGIN(Engine)


class ENGINE_DLL CCell
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };

public:
	CCell();
	virtual ~CCell();

public:
	static CCell* Create(const _float3* pPoints, _int iIndex);


public:
	_vector Get_Point(POINT ePoint) {
		return XMLoadFloat3(&m_vPoints[ePoint]);
	}

	_vector Get_Line(LINE eLine) {
		return XMLoadFloat3(&m_vLine[eLine]);
	}

	_int Get_Index() const {
		return m_iMyIndex;
	}

	_float4	Get_CellNormal() { return m_vCellNormal; }
	_float4	Get_CellRight() { return m_vCellRight; }
	_float	Get_CellAngle() { return m_fAngle; }

	void Set_Neighbor(LINE eLine, CCell* pNeighBorCell) {
		m_pNeighborCells[eLine] = pNeighBorCell;
	}

	void	Set_Flags(_byte bFlag) { m_bCellFlag = bFlag; }
	void	Add_Flags(_byte bFlag) { m_bCellFlag |= bFlag; }
	_bool	IsBlocked() { 

		if (m_bCellFlag & CELL_BLOCKED) 
			return true; 

		return false;
	}

	_bool	IsWall() {
		if (m_bCellFlag & CELL_WALL)
			return true;

		return false;
	}

	_bool	IsGrass() {
		if (m_bCellFlag & CELL_GRASS)
			return true;

		return false;
	}

	_bool	IsGround() {
		if (m_bCellFlag & CELL_GROUND)
			return true;

		return false;
	}

	CCell* Get_NeighborCell(LINE eLine) { return m_pNeighborCells[eLine]; }

public:
	// CComponent을(를) 통해 상속됨
	HRESULT Initialize(const _float3* pPoints, _int iIndex);
	void	Make_Matrix();
	_bool Compare_Points(_vector vSourPoint, _vector vDestPoint);
	_bool isIn(_vector vPosition, CCell** pOutNeighborCell, LINE* pOutLine);

private:
	_uint						m_iMyIndex = 0;
	_float3						m_vPoints[POINT_END];
	_float3						m_vLine[LINE_END];
	_float3						m_vNormal[LINE_END];
	CCell*						m_pNeighborCells[LINE_END] = {nullptr, nullptr, nullptr};

	_byte						m_bCellFlag = 0;
	_float4						m_vCellNormal;
	_float4						m_vCellRight;
	_float						m_fAngle = 0.f;
};

END