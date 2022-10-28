#pragma once
#include "CMesh_Rect.h"

BEGIN(Engine)
class ENGINE_DLL CRect_Instance
	: public CMesh_Rect
{
	DECLARE_PROTOTYPE(CRect_Instance);

private:
	CRect_Instance(_uint iGroupIndex);
	CRect_Instance(const CRect_Instance& _origin);
	virtual ~CRect_Instance();

public:
	static CRect_Instance* Create(_uint iNumInstance);

public:
	void	ReMake_Instance(_uint iNumInstance);

public:
	void	ReMap_Instances(VTXRECTINSTANCE* pRectInstances);

public:
	virtual void	Release() override;
	virtual HRESULT Render() override;

private:
	ComPtr<ID3D11Buffer>				m_pVBInstance = nullptr;
	_uint						m_iInstanceStride = 0;
	_uint						m_iNumInstance = 0;

private:
	HRESULT	SetUp_InstanceMesh(_uint iNumInstance);


};
END
