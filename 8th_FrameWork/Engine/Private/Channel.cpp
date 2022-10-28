#include "..\Public\Channel.h"
#include "HIerarchyNode.h"
#include "CAnimator.h"
#include "Channel.h"
#include "Model.h"

CChannel::CChannel()
{
}

CChannel::~CChannel()
{
	Release();
}

CChannel* CChannel::Create(CHANNEL_DESC pAIChannel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel)))
	{
		Call_MsgBox(L"Failed to Initialize : CChannel");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CChannel::Set_HierarchyNode(CModel* pModel)
{
	m_pHierarchyNode = pModel->Find_HierarchyNode(m_szName);

	if (nullptr == m_pHierarchyNode)
		return E_FAIL;

	return S_OK;
}

HRESULT CChannel::Initialize(CHANNEL_DESC tChannelDesc)
{
	strcpy_s(m_szName, tChannelDesc.strName.c_str());
	
	m_iNumKeyframes = tChannelDesc.iNumKeyFrames;
	m_pKeyFrames = tChannelDesc.pKeyFrames;

	return S_OK;
}

void CChannel::Update_TransformationMatrices(_float fCurrentTime)
{
	_vector			vScale, vRotation, vPosition;
	KEYFRAME& tPrevFrame = m_pHierarchyNode->Get_PrevKeyFrame();
	KEYFRAME& tCurFrame = m_pHierarchyNode->Get_CurKeyFrame();

	if (fCurrentTime > m_pKeyFrames[m_iNumKeyframes-1].fTime)
	{
		vScale = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame].vScale);
		vRotation = XMLoadFloat4(&m_pKeyFrames[m_iCurrentKeyFrame].vRotation);
		vPosition = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame].vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	else
	{
		while (fCurrentTime > m_pKeyFrames[m_iCurrentKeyFrame + 1].fTime)
		{
			++m_iCurrentKeyFrame;
		}

		_float fRatio = (fCurrentTime - m_pKeyFrames[m_iCurrentKeyFrame].fTime)
			/ (m_pKeyFrames[m_iCurrentKeyFrame + 1].fTime - m_pKeyFrames[m_iCurrentKeyFrame].fTime);

		_vector			vSourScale, vSourRotation, vSourPosition;
		_vector			vDestScale, vDestRotation, vDestPosition;

		vSourScale = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame].vScale);
		vSourRotation = XMLoadFloat4(&m_pKeyFrames[m_iCurrentKeyFrame].vRotation);
		vSourPosition = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame].vPosition);

		vDestScale = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame + 1].vScale);
		vDestRotation = XMLoadFloat4(&m_pKeyFrames[m_iCurrentKeyFrame + 1].vRotation);
		vDestPosition = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	XMStoreFloat3(&tCurFrame.vScale, vScale);
	XMStoreFloat4(&tCurFrame.vRotation, vRotation);
	XMStoreFloat3(&tCurFrame.vPosition, vPosition);


	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);
	_float4x4 TransMat;
	XMStoreFloat4x4(&TransMat, TransformationMatrix);

	if (nullptr != m_pHierarchyNode)
	{
		m_pHierarchyNode->Set_TransformationMatrix(TransMat);
	}
	else
	{
		int i = 0;
	}
}

void CChannel::Interpolate_Matrix(_float fCurrentTime, _float fMaxTime)
{
	//전 키프레임
	KEYFRAME& tPrevFrame = m_pHierarchyNode->Get_PrevKeyFrame();

	//이게 매프레임 마다 저장용 
	KEYFRAME& tCurFrame = m_pHierarchyNode->Get_CurKeyFrame();

	//현재 매트릭스
	_vector			vScale, vRotation, vPosition;

	_float fRatio = fCurrentTime / fMaxTime;

	_vector			vSourScale, vSourRotation, vSourPosition;
	_vector			vDestScale, vDestRotation, vDestPosition;

	vSourScale = XMLoadFloat3(&tPrevFrame.vScale);
	vSourRotation = XMLoadFloat4(&tPrevFrame.vRotation);
	vSourPosition = XMLoadFloat3(&tPrevFrame.vPosition);

	vDestScale = XMLoadFloat3(&m_pKeyFrames[0].vScale);
	vDestRotation = XMLoadFloat4(&m_pKeyFrames[0].vRotation);
	vDestPosition = XMLoadFloat3(&m_pKeyFrames[0].vPosition);

	vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
	vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
	vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
	vPosition = XMVectorSetW(vPosition, 1.f);

	//이전 애니메이션의 그 프레임 상태를 받아와서
	// 그걸로 해야해.

	XMStoreFloat3(&tCurFrame.vScale, vScale);
	XMStoreFloat4(&tCurFrame.vRotation, vRotation);
	XMStoreFloat3(&tCurFrame.vPosition, vPosition);

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);
	_float4x4 TransMat;
	XMStoreFloat4x4(&TransMat, TransformationMatrix);

	if (nullptr != m_pHierarchyNode)
		m_pHierarchyNode->Set_TransformationMatrix(TransMat);
	else
	{
		int i = 0;
	}

}

void CChannel::Release()
{
}

