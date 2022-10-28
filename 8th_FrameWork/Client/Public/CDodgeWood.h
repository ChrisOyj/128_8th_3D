#pragma once
#include "CEffect.h"

BEGIN(Client)

class CDodgeWood final
	: public CEffect
{
	DECLARE_PROTOTYPE(CDodgeWood);

private:
	CDodgeWood();
	virtual ~CDodgeWood();

public:
	static CDodgeWood* Create();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Start() override;

public:
	virtual void OnEnable();
};

END