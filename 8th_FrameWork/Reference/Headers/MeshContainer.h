#pragma once
#include "CMesh.h"


BEGIN(Engine)
class CResource_Mesh;
class ENGINE_DLL CMeshContainer
	: public CMesh
{
	DECLARE_PROTOTYPE(CMeshContainer);
	
	friend class CModel;

protected:
	CMeshContainer(_uint iGroupIndex);
	virtual ~CMeshContainer();

public:
	static CMeshContainer* Create(CResource_Mesh* pMeshResource, _float4x4 TransformMatrix);

public:
	void		Set_ShaderFlag(_float4 vFlag) { m_vFlag = vFlag; }
	void		Set_OutlinePower(_float fOutline) { m_fOutline = fOutline; }

	_uint		Get_CurPass() { return m_iCurPass; }
	void		Set_CurPass(const _uint& iPass) { m_iCurPass = iPass; }
	string		Get_Name() { return string(m_szName); }

	void		Set_MaterialName(string strName) { m_MaterialName = strName; }
	string		Get_MaterialName() { return m_MaterialName; }

	_float4&		Get_CenterPos() { return m_vCenterPos; }
	_float		Get_MaxRange() { return m_fMaxRange; }

	void		Set_Color(_float4 vColor) { m_vColor = vColor; }
	
public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

	virtual _uint3 Get_Indices(_uint iIndex) const override {
		return _uint3(((FACEINDICES32*)m_pIndices)[iIndex]._1,
			((FACEINDICES32*)m_pIndices)[iIndex]._2,
			((FACEINDICES32*)m_pIndices)[iIndex]._3);
	}

	_float4 Get_LocalPos();

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void	Release();

public:
	HRESULT SetUp_Ptr(class CModel* pModel);
	void SetUp_BoneMatrices(class CShader* pShader, const char* pConstantName);
	virtual void		Set_ShaderResourceFlag(class CShader* pShader, const char* pConstantName);
	virtual void		Set_ShaderResource(class CShader* pShader, const char* pConstantName) override;


protected: /* 현재ㅑ 메시컨테이너에게 적용되어야할 머테리얼 인덱스*/
	BONE_DATA* m_pBoneDatas = nullptr;
	char		m_szName[MAX_PATH] = "";
	string		m_strRefBoneName;

	_uint									m_iNumBones = 0;
	vector<class CHierarchyNode*>			m_Bones;

	string								m_MaterialName;
	_uint								m_iMaterialIndex = 0;

	_float4								m_vFlag = SH_LIGHT_NOSPEC;
	_float4								m_vColor;

	_float								m_fOutline = 1.f;
	_uint								m_iCurPass = 0;
	_float4x4							m_TransformMatrix;
	_float4x4							m_OffsetMatrix;

	_float4					m_vCenterPos;
	_float					m_fMaxRange = 0.f;

protected:
	HRESULT	SetUp_MeshContainer(CResource_Mesh* pResource, _float4x4 TransformMatrix);
};

END