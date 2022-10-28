#pragma once
#include "Component.h"

BEGIN(Engine)
class CAnimation;
class CHierarchyNode;

class ENGINE_DLL CAnimator
	: public CComponent
{
	DECLARE_PROTOTYPE(CAnimator)

private:
	CAnimator(_uint iGroupIdx);
	CAnimator(const CAnimator& rhs);
	virtual ~CAnimator();

public:
	static CAnimator* Create(_uint iGroupIdx, wstring wstrModelFilePath);

public:
	void	Add_Animations(wstring wstrModelFilePath);
	_float	Calculate_Duration(_uint iTypeIndex, _uint iAnimIndex, _uint iKeyFrame);

public:
	
	_uint		Get_CurAnimFrame();

	_uint		Get_CurAnimTypeIndex() { return m_iCurrentAnimationTypeIndex; }
	_uint		Get_CurAnimIndex() { return m_iCurrentAnimationIndex; }

	void		Set_CurFrame(_uint iFrame);
	void		Set_CurAnimIndex(_uint iTypeIndex, _uint iAnimIndex);
	void		Set_AnimSpeed(_uint iTypeIndex, _uint iAnimIndex, _float fSpeed);
	

	void		Set_InterpolationTime(_uint iTypeIndex, _uint iIdx, _float fTime);
	_bool		Is_CurAnimFinished();


public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void	Start() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;
	virtual void Release() override;


private:
	wstring					m_wstrModelFilePath;
	_uint					m_iCurrentAnimationTypeIndex = 0;
	_uint					m_iCurrentAnimationIndex = 0;

	vector<vector<CAnimation*>>		m_vecAnimations;
	CHierarchyNode* m_pFootNode = nullptr;
	

private:
	HRESULT		SetUp_Animations(wstring wstrModelFilePath);


	

};

END