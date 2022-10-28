#pragma once
#include "CResource.h"

BEGIN(Engine)



class CResource_Bone
	: public CResource
{
private:
	CResource_Bone();
	virtual ~CResource_Bone();

public:
	static CResource_Bone* Create(ifstream* pReadFile, string strName, CResource_Bone* pParent);

public:
	_uint						Get_NumChildren() { return m_iNumChildren; }
	_float4x4					Get_TransformationMatrix() { return m_matTransformation; }
	vector<CResource_Bone*>&	Get_ChildrenBones() { return m_ChildrenBones; }
public:
	// CResource을(를) 통해 상속됨
	virtual HRESULT Initialize(ifstream* pReadFile, string strName) override;

private:
	_uint		m_iNumChildren = 0;
	_float4x4	m_matTransformation;
	CResource_Bone* m_pParentBone = nullptr;
	vector<CResource_Bone*>	m_ChildrenBones;
};

END