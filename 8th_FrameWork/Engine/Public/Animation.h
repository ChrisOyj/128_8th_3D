#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CAnimation final
{
private:
	CAnimation();
	virtual ~CAnimation();

	friend class CModel;

public:
	static CAnimation* Create(aiAnimation* pAIAnimation, class CModel* pModel);

public:
	HRESULT Initialize(aiAnimation* pAIAnimation, class CModel* pModel);
	void Update_TransformationMatrices(_float fTimeDelta);
	void	Release();

private:
	char			m_szName[MAX_PATH] = "";
	_uint			m_iNumChannels = 0; /* 현재 애니메이션이 사용ㅇ해야할 뼈의 갯수. */
	vector<class CChannel*>		m_Channels; /* 현재 애님에서 사용되는 뼈의 상태를 저장한 객체이다. */

	_float			m_fDuration = 0.f;
	_float			m_fTickPerSecond = 0.f;
	_float			m_fTimeAcc = 0.f;
	_bool			m_isFinished = false;




};

END