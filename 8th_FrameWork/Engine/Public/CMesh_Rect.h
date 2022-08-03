#pragma once
#include "CMesh.h"

BEGIN(Engine)

class ENGINE_DLL CMesh_Rect final
	: public CMesh
{
	DECLARE_PROTOTYPE(CMesh_Rect);

private:
	CMesh_Rect(_uint iGroupIDX);
	virtual ~CMesh_Rect();

public:
	static CMesh_Rect* Create(_uint iGroupIdx);

public:
	// CMesh을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

};

END
