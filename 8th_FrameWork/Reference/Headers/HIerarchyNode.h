#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CHierarchyNode final
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode();

	friend class CModel;


public:
	static CHierarchyNode* Create(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth);

public:
	_uint Get_Depth() const {
		return m_iDepth;
	}

	const char* Get_Name() const {
		return m_szName;
	}

	void Set_OffsetMatrix(_float4x4 OffsetMatrix) {
		m_OffsetMatrix = OffsetMatrix;
	}

	void Set_TransformationMatrix(_float4x4 TransformationMatrix) {
		m_TransformationMatrix = TransformationMatrix;
	}

public:
	HRESULT Initialize(aiNode* pAINode, CHierarchyNode* pParent, _uint iDepth);
	void	Release();

private:
	_uint			m_iDepth = 0;
	char			m_szName[MAX_PATH] = "";

	_float4x4		m_OffsetMatrix;
	_float4x4		m_TransformationMatrix;
	_float4x4		m_CombinedTransformationMatrix;

	CHierarchyNode*	m_pParent = nullptr;

};

END