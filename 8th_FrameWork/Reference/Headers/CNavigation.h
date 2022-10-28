#pragma once
#include "Component.h"

BEGIN(Engine)

class CCell;
class CPhysics;



class ENGINE_DLL CNavigation
	: public CComponent
{
public:
	enum CELL_TYPE {DEFAULT, WALL, BLOCKED, CELL_END};
	DECLARE_PROTOTYPE(CNavigation)

protected:
	CNavigation(_uint iGroupIdx);
	virtual ~CNavigation();

public:
	static CNavigation* Create(_uint iGroupIdx, CCell* pStartCell, CPhysics* pPhysicsCom);

public:
	void		Set_StartCell(CCell* pStartCell) { m_pCurCell = pStartCell; }
	CCell* Get_CurCell() { return m_pCurCell; }

	void		Set_Epsilon(_float fEpsilon) { m_fEpsilon = fEpsilon; }
	void		Set_ExamArena(_bool bEnable, _float4 vCenter) {
		m_bExamArena = bEnable;
		m_vExamArenaCenter	= vCenter;
	}

	void		Reset_OnWall() { m_bOnWall = false; 	m_bBlocked = false;
	}
	_bool		Is_OnWall() { return m_bOnWall; }
	_bool		Is_InWall();
	_float4x4	Get_CurWallMat();
	_float		Get_TargetY() { return m_fTargetY; }
	_float4		Get_CurWallNormal();
	_bool		Is_Blocked() { return m_bBlocked; }
public:
	//CELL_TYPE	isMove(_vector vPosition, _float4* pOutPos);
	_float4		Enter_Wall();
	void		Exit_Wall();

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;

protected:
	CCell*		m_pCurCell = nullptr;
	CCell*		m_pCurWallCell = nullptr;
	CPhysics*	m_pPhysicsCom = nullptr;
	_float		m_fEpsilon = 0.f;

	_bool		m_bOnWall = false;
	_bool		m_bExamArena = false;

	_bool		m_bOrigin = false;
	_float4		m_vExamArenaCenter;
	_float		m_fExamDistance = 59.f;

	_float		m_fTargetY = 0.f;

	_bool		m_bInWall = false;
	_bool		m_bBlocked = false;


protected:
	//_float4		Correct_Move(_float4 vPos);

	_float4		Correct_Move_Default(_float4 vPos);

	//현재 위치랑, 셀 던져주면
	_float4		Correct_Move_BlockedAgain(CCell* pCell, _uint eOutLine, _float4 vPos, _float4 _vDir, _float _fSpeed, _int iDepth);

	_float4		Get_NewPosFromCell(CCell* pCell, _vector vPos);
	_float4		Get_NewDirFromCellLine(CCell* pCell, _uint eOutLine, _vector vPos, _float4 _vDir, _float _fSpeed);
	_float4		Get_NewPosFromWall(CCell* pCell, _float4 vRayPos, _float4 vRayDir);

};

END