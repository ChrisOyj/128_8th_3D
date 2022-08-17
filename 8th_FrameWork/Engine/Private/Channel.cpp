#include "..\Public\Channel.h"
#include "HIerarchyNode.h"
#include "Model.h"
#include "Channel.h"

CChannel::CChannel()
{
}

CChannel::~CChannel()
{
	Release();
}

CChannel* CChannel::Create(aiNodeAnim* pAIChannel, CModel* pModel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, pModel)))
	{
		Call_MsgBox(L"Failed to Initialize : CChannel");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CChannel::Initialize(aiNodeAnim * pAIChannel, CModel* pModel)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	CHierarchyNode*		pHierarchyNode = pModel->Find_HierarcyNode(m_szName);
	if (nullptr == pHierarchyNode)
		return E_FAIL;


	m_iNumKeyframes = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyframes = max(m_iNumKeyframes, pAIChannel->mNumPositionKeys);

	_float3		vScale;
	_float4		vRotation;
	_float3		vPosition;

	for (_uint i = 0; i < m_iNumKeyframes; ++i)
	{
		KEYFRAME		KeyFrame;
		ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

		if (pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = (_float)pAIChannel->mScalingKeys[i].mTime;
		}

		if (pAIChannel->mNumRotationKeys > i)
		{
			/*memcpy(&vRotation, &pAIChannel->mRotationKeys[i].mValue, sizeof(_float4));*/
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.fTime = (_float)pAIChannel->mScalingKeys[i].mTime;
		}

		if (pAIChannel->mNumPositionKeys > i)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = (_float)pAIChannel->mScalingKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_KeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}

void CChannel::Update_TransformationMatrices(_float fCurrentTime)
{
	_vector			vScale, vRotation, vPosition;

	if (fCurrentTime > m_KeyFrames.back().fTime)
	{
		vScale = XMLoadFloat3(&m_KeyFrames[m_iCurrrentKeyFrame].vScale);
		vRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrrentKeyFrame].vRotation);
		vPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrrentKeyFrame].vPosition);	
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	else
	{
		while (fCurrentTime > m_KeyFrames[m_iCurrrentKeyFrame + 1].fTime)
		{
			++m_iCurrrentKeyFrame;
		}

		_float fRatio = (fCurrentTime - m_KeyFrames[m_iCurrrentKeyFrame].fTime)
			/ (m_KeyFrames[m_iCurrrentKeyFrame + 1].fTime - m_KeyFrames[m_iCurrrentKeyFrame].fTime);

		_vector			vSourScale, vSourRotation, vSourPosition;
		_vector			vDestScale, vDestRotation, vDestPosition;

		vSourScale = XMLoadFloat3(&m_KeyFrames[m_iCurrrentKeyFrame].vScale);
		vSourRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrrentKeyFrame].vRotation);
		vSourPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrrentKeyFrame].vPosition);

		vDestScale = XMLoadFloat3(&m_KeyFrames[m_iCurrrentKeyFrame + 1].vScale);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[m_iCurrrentKeyFrame + 1].vRotation);
		vDestPosition = XMLoadFloat3(&m_KeyFrames[m_iCurrrentKeyFrame + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}



	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);
	_float4x4 TransMat;
	XMStoreFloat4x4(&TransMat, TransformationMatrix);

	if (nullptr != m_pHierarchyNode)
		m_pHierarchyNode->Set_TransformationMatrix(TransMat);
}

void CChannel::Release()
{
}

