#include "CAnimator.h"

#include "GameInstance.h"

#include "Model.h"
#include "Channel.h"
#include "Animation.h"
#include "CShader.h"
#include "GameObject.h"
#include "HIerarchyNode.h"

#include "CResource_Animation.h"

CAnimator::CAnimator(_uint iGroupIdx)
	: CComponent(iGroupIdx)
{
}

CAnimator::CAnimator(const CAnimator& rhs)
	: CComponent(rhs)
	, m_iCurrentAnimationIndex(rhs.m_iCurrentAnimationIndex)
	, m_wstrModelFilePath(rhs.m_wstrModelFilePath)
{

	SetUp_Animations(m_wstrModelFilePath);
}

CAnimator::~CAnimator()
{
	Release();
}

CAnimator* CAnimator::Create(_uint iGroupIdx, wstring wstrModelFilePath)
{
	CAnimator* pInstance = new CAnimator(iGroupIdx);

	pInstance->m_wstrModelFilePath = wstrModelFilePath;

	if (FAILED(pInstance->SetUp_Animations(wstrModelFilePath)))
	{
		Call_MsgBox(L"Failed to SetUp_Animations : CAnimator");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CAnimator");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CAnimator::Add_Animations(wstring wstrModelFilePath)
{
	MODEL_DATA* pModelData = CGameInstance::Get_Instance()->Get_ModelData(wstrModelFilePath, TYPE_ONLYANIMATION);
	_uint iSize = (_uint)pModelData->m_vecResourceKeys.size();

	m_vecAnimations.push_back(vector<CAnimation*>());

	for (_uint i = 0; i < iSize; ++i)
	{
		CResource* pResource = CGameInstance::Get_Instance()->Get_Resource(pModelData->m_vecResourceKeys[i]);
		CResource_Animation* pAnimResource = dynamic_cast<CResource_Animation*>(pResource);

		if (!pResource)
			return;

		CAnimation* pAnimation = CAnimation::Create(pAnimResource);

		if (nullptr == pAnimation)
			return;

		m_vecAnimations.back().push_back(pAnimation);
	}
}

_float CAnimator::Calculate_Duration(_uint iTypeIndex, _uint iAnimIndex, _uint iKeyFrame)
{
	if (iTypeIndex >= m_vecAnimations.size())
		return 0.f;

	if (iAnimIndex >= m_vecAnimations[iTypeIndex].size())
		return 0.f;

	_float fResult = m_vecAnimations[iTypeIndex][iAnimIndex]->Calculate_Duration(iKeyFrame);

	return fResult;
}

_uint CAnimator::Get_CurAnimFrame()
{
	return m_vecAnimations[m_iCurrentAnimationTypeIndex][m_iCurrentAnimationIndex]->m_Channels.front()->Get_CurKeyFrame();
}

void CAnimator::Set_CurFrame(_uint iFrame)
{
	for (auto& elem : m_vecAnimations[m_iCurrentAnimationTypeIndex][m_iCurrentAnimationIndex]->m_Channels)
	{
		elem->m_iCurrentKeyFrame = iFrame;
	}
}

void CAnimator::Set_CurAnimIndex(_uint iTypeIndex, _uint iNewIdx)
{
	if (iTypeIndex >= m_vecAnimations.size())
		return;

	if (iNewIdx >= m_vecAnimations[iTypeIndex].size())
		return;

	//다음녀석ㄱ한테 보간하라고 시켜
	m_vecAnimations[iTypeIndex][iNewIdx]->OnInterpolate(m_vecAnimations[iTypeIndex][iNewIdx]);

	//이전껀 리셋해놓고
	m_vecAnimations[m_iCurrentAnimationTypeIndex][m_iCurrentAnimationIndex]->Reset();

	m_iCurrentAnimationTypeIndex = iTypeIndex;
	m_iCurrentAnimationIndex = iNewIdx;


}

void CAnimator::Set_AnimSpeed(_uint iTypeIndex, _uint iAnimIndex, _float fSpeed)
{
	if (iTypeIndex >= m_vecAnimations.size())
		return;

	if (iAnimIndex >= m_vecAnimations[iTypeIndex].size())
		return;

	m_vecAnimations[iTypeIndex][iAnimIndex]->m_fAnimSpeed = fSpeed;

}

void CAnimator::Set_InterpolationTime(_uint iTypeIndex, _uint iIdx, _float fTime)
{
	if (iTypeIndex >= m_vecAnimations.size())
		return;

	if (iIdx >= m_vecAnimations[iTypeIndex].size())
		return;

	m_vecAnimations[iTypeIndex][iIdx]->m_fInterpolationTime = fTime;
}

_bool CAnimator::Is_CurAnimFinished()
{
	return m_vecAnimations[m_iCurrentAnimationTypeIndex][m_iCurrentAnimationIndex]->m_isFinished;
}

HRESULT CAnimator::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CAnimator::Initialize()
{
	return S_OK;
}

void CAnimator::Start()
{
	__super::Start();

	CModel* pModelCom = GET_COMPONENT_FROM(m_pOwner, CModel);

	if (!pModelCom)
	{
		Call_MsgBox(L"Failed to Find CModel : CAnimator");
		return;
	}

	for (auto& vecAnim : m_vecAnimations)
	{
		for (auto& pAnim : vecAnim)
		{
			if (FAILED(pAnim->Set_HierarchyNodes(pModelCom)))
			{
				Call_MsgBox(L"Failed to Set_HierarchyNodes : CAnimator");
				return;
			}
		}
		
	}

	vector<pair<_uint, CHierarchyNode*>>& vecHierarchyNodes = pModelCom->Get_Hierarchynodes();
	for (auto& elem : vecHierarchyNodes)
	{
		if (!strcmp(elem.second->Get_Name(), "CharacterRoot"))
		{
			m_pFootNode = elem.second;
			break;
		}
	}
}

void CAnimator::Tick()
{
	m_vecAnimations[m_iCurrentAnimationTypeIndex][m_iCurrentAnimationIndex]->Update_Matrices();
}

void CAnimator::Late_Tick()
{
}

void CAnimator::Release()
{
	for (auto& elem : m_vecAnimations)
		for (auto& pAnim : elem)
			SAFE_DELETE(pAnim);
}

HRESULT CAnimator::SetUp_Animations(wstring wstrModelFilePath)
{
	MODEL_DATA* pModelData = CGameInstance::Get_Instance()->Get_ModelData(wstrModelFilePath, TYPE_ONLYANIMATION);
	_uint iSize = (_uint)pModelData->m_vecResourceKeys.size();

	m_vecAnimations.push_back(vector<CAnimation*>());


	for (_uint i = 0; i < iSize; ++i)
	{
		CResource* pResource = CGameInstance::Get_Instance()->Get_Resource(pModelData->m_vecResourceKeys[i]);
		CResource_Animation* pAnimResource = dynamic_cast<CResource_Animation*>(pResource);

		if (!pResource)
			return E_FAIL;

		CAnimation* pAnimation = CAnimation::Create(pAnimResource);

		if (nullptr == pAnimation)
			return E_FAIL;

		m_vecAnimations.back().push_back(pAnimation);
	}

	return S_OK;
}