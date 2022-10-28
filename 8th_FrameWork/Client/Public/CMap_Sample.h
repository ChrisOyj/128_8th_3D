#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CMap_Sample
	: public CGameObject
{
	DECLARE_PROTOTYPE(CMap_Sample);

private:
	CMap_Sample();
	virtual ~CMap_Sample();

public:
	static CMap_Sample* Create(wstring wstrFilePath);

public:
	_float4			Get_OffsetPos() { return m_vOffsetPos; }
	void			Set_OffsetPos(_float4 vPos) { m_vOffsetPos = vPos; }
	_float4			Get_OriginPos() { return m_vPickedPos; }
	void			Set_OriginPos(_float4 vPos) { m_vPickedPos = vPos; }

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

	virtual void My_LateTick() override;

private:
	_float4	m_vOffsetPos;
	_float4	m_vPickedPos;
	
private:
	HRESULT		SetUp_Model(wstring wstrFilePath);

};
END
