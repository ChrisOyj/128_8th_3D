#pragma once
#include "Engine_Defines.h"

/* 각 애니메이션마다 따로 할당되어 보관된다. */

BEGIN(Engine)

typedef struct tagKeyFrame
{
	XMFLOAT3		vScale;
	XMFLOAT4		vRotation;
	XMFLOAT3		vPosition;
	float			fTime;
} KEYFRAME;

class CChannel final
{
private:
	CChannel();
	virtual ~CChannel();

	friend class CAnimation;

public:
	static CChannel* Create(aiNodeAnim* pAIChannel, class CModel* pModel);

public:
	HRESULT Initialize(aiNodeAnim* pAIChannel, class CModel* pModel);
	void Update_TransformationMatrices(_float fCurrentTime);
	void Release();

private:
	char						m_szName[MAX_PATH] = "";
	_uint						m_iNumKeyframes = 0;
	vector<KEYFRAME>			m_KeyFrames;
	class CHierarchyNode*		m_pHierarchyNode = nullptr;
	_uint						m_iCurrrentKeyFrame = 0;



};

END