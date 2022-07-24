#pragma once
#include "Component.h"

BEGIN(Engine)

enum WORLD {WORLD_RIGHT, WORLD_UP, WORLD_LOOK, WORLD_POS, WORLD_END};

class ENGINE_DLL CTransform final
	: public CComponent
{
private:
	CTransform();
	virtual ~CTransform();

	DECLARE_PROTOTYPE(CTransform);


public:
	static CTransform*	Create();

public:
	const TRANSFORM&	Get_Transform()		{ return m_tTransform; }
	_float4x4				Get_WorldMatrix(const _byte& matrixFlag = 0);
	_float3				Get_World(WORLD eType);
	_float3				Get_MyWorld(WORLD eType);
	_float3				Get_Scale() { return m_tTransform.vScale; }

	void				Set_World(WORLD eType, const _float3& vCol);
	void				Set_RealWorld(WORLD eType, const _float3& vCol);
	void				Set_MyMatrix(_float4x4 matWorld);
	void				Set_Look(const _float3& vLook);
	void				Set_Rect();


	void				Set_Scale(const _float3& vScale);
	void				Set_Y(const _float& fY);
	void				Set_ParentFlag(_byte parentFlag)			{ m_cParentFlag = parentFlag; }

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;

public:
	void			Make_WorldMatrix();

private:
	TRANSFORM	m_tTransform;
	_byte		m_cParentFlag = 0;
};
END
