#pragma once
#include "CMesh.h"

class CMesh_Rect final
	: public CMesh
{
	DECLARE_PROTOTYPE(CMesh_Rect);

private:
	CMesh_Rect(CGameObject* pOwner);
	virtual ~CMesh_Rect();

public:
	static CMesh_Rect* Create();

public:
	// CMesh을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

};

