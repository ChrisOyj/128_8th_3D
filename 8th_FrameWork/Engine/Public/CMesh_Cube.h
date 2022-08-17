#pragma once
#include "CMesh.h"

BEGIN(Engine)

class ENGINE_DLL CMesh_Cube final : public CMesh
{
	DECLARE_PROTOTYPE(CMesh_Cube);

protected:
	CMesh_Cube(_uint iGroupIDX);
	virtual ~CMesh_Cube();

public:
	static CMesh_Cube* Create(_uint iGroupIDX);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;


};

END