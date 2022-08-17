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
	_uint			m_iNumChannels = 0; /* ���� �ִϸ��̼��� ��뤷�ؾ��� ���� ����. */
	vector<class CChannel*>		m_Channels; /* ���� �ִԿ��� ���Ǵ� ���� ���¸� ������ ��ü�̴�. */

	_float			m_fDuration = 0.f;
	_float			m_fTickPerSecond = 0.f;
	_float			m_fTimeAcc = 0.f;
	_bool			m_isFinished = false;




};

END