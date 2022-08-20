#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
	DECLARE_PROTOTYPE(CModel);

private:
	CModel(_uint iGroupIdx);
	CModel(const CModel& rhs);
	virtual ~CModel();

public:
	static CModel* Create(_uint iGroupIdx, MODEL_TYPE eType, const char* pModelFilePath, _float4x4 TransformMatrix);

public:
	_uint Get_NumMeshContainers() const {
		return m_iNumMeshContainers;
	}

	void Set_CurrentAnimation(_uint iAnimIndex) {
		m_iCurrentAnimationIndex = iAnimIndex;
	}


public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize() override;
	void Tick() override;
	void Late_Tick() override;
	HRESULT Play_Animation(_float fTimeDelta);
	HRESULT Render(_uint iMeshContainerIndex);
	void	Release();

public:
	class CHierarchyNode* Find_HierarcyNode(const char* pBoneName);


public:
	HRESULT Bind_SRV(class CShader* pShader, const char* pContantName, _uint iMeshContainerIndex, aiTextureType eType);

private:
	const aiScene*				m_pAIScene = nullptr;
	Assimp::Importer			m_Importer;
	MODEL_TYPE					m_eMODEL_TYPE = TYPE_END;
	_float4x4					m_TransformMatrix;
	_bool						m_bCloned = false;

private:
	vector<class CMeshContainer*>			m_MeshContainers;
	typedef vector<class CMeshContainer*>	MESHCONTAINERS;
	_uint			m_iNumMeshContainers = 0;

private:
	vector<MODEL_MATERIAL>					m_Materials;
	typedef vector<MODEL_MATERIAL>			MATERIALS;
	_uint			m_iNumMaterials = 0;

private:
	vector<class CHierarchyNode*>			m_HierarchyNodes;
	typedef vector<class CHierarchyNode*>	HIERARCHYNODES;

private:
	_uint									m_iCurrentAnimationIndex = 0;
	_uint									m_iNumAnimations;
	vector<class CAnimation*>				m_Animations;
	typedef vector<class CAnimation*>		ANIMATIONS;


private:
	HRESULT	SetUp_Model(MODEL_TYPE eType, const char* pModelFilePath, _float4x4 TransformMatrix);
	HRESULT Create_MeshContainers();
	HRESULT Create_Materials(const char* pModelFilePath);
	HRESULT Create_HierarchyNodes(aiNode* pNode, class CHierarchyNode* pParent, _uint iDepth);
	HRESULT Create_Animations();



};

END