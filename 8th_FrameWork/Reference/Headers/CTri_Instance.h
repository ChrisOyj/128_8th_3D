#pragma once
#include "CMesh.h"

BEGIN(Engine)
class ENGINE_DLL CTri_Instance
	: public CMesh
{
	DECLARE_PROTOTYPE(CTri_Instance);

private:
	CTri_Instance(_uint iGroupIndex);
	CTri_Instance(const CTri_Instance& _origin);
	virtual ~CTri_Instance();

public:
	static CTri_Instance* Create(_uint iNumInstance);

public:
	virtual _uint3 Get_Indices(_uint iIndex) const {
		return _uint3(((FACEINDICES16*)m_pIndices)[iIndex]._1,
			((FACEINDICES16*)m_pIndices)[iIndex]._2,
			((FACEINDICES16*)m_pIndices)[iIndex]._3);
	}

public:
	void	ReMap_Instances(VTXTRIINSTANCE* pTriInstances);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Render() override;

private:
	ComPtr<ID3D11Buffer>				m_pVBInstance = nullptr;
	_uint						m_iInstanceStride = 0;
	_uint						m_iNumInstance = 0;

private:
	HRESULT	SetUp_InstanceMesh(_uint iNumInstance);
};
END
