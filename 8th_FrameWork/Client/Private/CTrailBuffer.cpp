#include "stdafx.h"
#include "CTrailBuffer.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "HIerarchyNode.h"
#include "Texture.h"
#include "Transform.h"

#include "Renderer.h"
#include "CShader.h"


CTrailBuffer::CTrailBuffer(_uint iGroupIdx)
	: CMesh(iGroupIdx)
{
}

CTrailBuffer::~CTrailBuffer()
{
}

CTrailBuffer* CTrailBuffer::Create(_uint iGroupIdx, _uint iTriCnt, _float4 vLocalSwordLow, _float4 vLocalSwordHigh, CHierarchyNode* pSwordBone, CTransform* pUnitTransform)
{
	CTrailBuffer* pInstance = new CTrailBuffer(iGroupIdx);

	pInstance->m_iTriCnt = iTriCnt;
	pInstance->m_vLocalSwordHigh = vLocalSwordHigh;
	pInstance->m_vLocalSwordLow = vLocalSwordLow;
	pInstance->m_pRefBone = pSwordBone;
	pInstance->m_pOwnerTransform = pUnitTransform;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CTrailBuffer");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize : CTrailBuffer");
		return nullptr;
	}

	return pInstance;
}

HRESULT CTrailBuffer::Initialize_Prototype()
{
#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = m_iTriCnt + 2;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXTEX* pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXTEX) * m_iNumVertices);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEXBUFFER


#pragma endregion
	m_iIndicesStride = sizeof(FACEINDICES16);
	m_iNumPrimitive = m_iTriCnt;
	m_iNumIndices = 3 * m_iNumPrimitive;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eToplogy = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesStride * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	FACEINDICES16* pIndices = new FACEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iTriCnt; i += 2)
	{
		pIndices[i]._1 = i + 3;
		pIndices[i]._2 = i + 1;
		pIndices[i]._3 = i + 0;

		pIndices[i + 1]._1 = i + 2;
		pIndices[i + 1]._2 = i + 3;
		pIndices[i + 1]._3 = i + 0;
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);


	return S_OK;
}

HRESULT CTrailBuffer::Initialize()
{
	m_TransformationMatrix = DEFAULT_TRANS_MATRIX;



	return S_OK;
}

void CTrailBuffer::Start()
{
	CComponent::Start();


}

void CTrailBuffer::OnEnable()
{
	CComponent::OnEnable();
}

void CTrailBuffer::OnDisable()
{
	CComponent::OnDisable();
}

void CTrailBuffer::Tick()
{

}

void CTrailBuffer::Update_Curve()
{
	if (m_bTrailOn)
	{
		if (m_iVtxCount < m_iNumVertices)
		{
			D3D11_MAPPED_SUBRESOURCE		SubResource;

			DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);


			m_iEndIndex = min(m_iCatmullRomCount * 2 + m_iVtxCount, m_iNumVertices - 2);

			m_iCatmullRomIndex[2] = m_iEndIndex - 2;
			m_iCatmullRomIndex[3] = m_iEndIndex;
			((VTXTEX*)SubResource.pData)[m_iEndIndex - 2].vPosition = ((VTXTEX*)SubResource.pData)[m_iVtxCount - 2].vPosition;
			((VTXTEX*)SubResource.pData)[m_iEndIndex - 1].vPosition = ((VTXTEX*)SubResource.pData)[m_iVtxCount - 1].vPosition;

			//아래쪽
			_float4 vHandPos = m_vLocalSwordLow;

			if (!m_pRefBone)
			{
				_float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();
				vHandPos = vHandPos.MultiplyCoord(matWorld);
			}
			else
			{
				_matrix matRightHand = m_pRefBone->Get_CombinedMatrix();
				matRightHand = XMMatrixMultiply(matRightHand, m_TransformationMatrix.XMLoad());
				_float4x4 matBone;
				XMStoreFloat4x4(&matBone, matRightHand);
				_float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();

				vHandPos = vHandPos.MultiplyCoord(matBone);
				vHandPos = vHandPos.MultiplyCoord(matWorld);
			}


			((VTXTEX*)SubResource.pData)[m_iEndIndex].vPosition.x = vHandPos.x;
			((VTXTEX*)SubResource.pData)[m_iEndIndex].vPosition.y = vHandPos.y;
			((VTXTEX*)SubResource.pData)[m_iEndIndex].vPosition.z = vHandPos.z;


			//위쪽
			vHandPos = m_vLocalSwordHigh;
			if (!m_pRefBone)
			{
				_float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();
				vHandPos = vHandPos.MultiplyCoord(matWorld);
			}
			else
			{
				_matrix matRightHand = m_pRefBone->Get_CombinedMatrix();
				matRightHand = XMMatrixMultiply(matRightHand, m_TransformationMatrix.XMLoad());
				_float4x4 matBone;
				XMStoreFloat4x4(&matBone, matRightHand);
				_float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();

				vHandPos = vHandPos.MultiplyCoord(matBone);
				vHandPos = vHandPos.MultiplyCoord(matWorld);
			}

			((VTXTEX*)SubResource.pData)[m_iEndIndex + 1].vPosition.x = vHandPos.x;
			((VTXTEX*)SubResource.pData)[m_iEndIndex + 1].vPosition.y = vHandPos.y;
			((VTXTEX*)SubResource.pData)[m_iEndIndex + 1].vPosition.z = vHandPos.z;


			//++i 수상
			for (_uint i = 0; i < m_iCatmullRomCount; i++)
			{
				_uint iIndex = i * 2 + m_iVtxCount - 2;
				_float fWeight = _float(i + 1) / (m_iCatmullRomCount + 1);

				XMStoreFloat3(&((VTXTEX*)SubResource.pData)[iIndex].vPosition,
					XMVectorCatmullRom(
						XMLoadFloat3(&((VTXTEX*)SubResource.pData)[m_iCatmullRomIndex[0]].vPosition),
						XMLoadFloat3(&((VTXTEX*)SubResource.pData)[m_iCatmullRomIndex[1]].vPosition),
						XMLoadFloat3(&((VTXTEX*)SubResource.pData)[m_iCatmullRomIndex[2]].vPosition),
						XMLoadFloat3(&((VTXTEX*)SubResource.pData)[m_iCatmullRomIndex[3]].vPosition),
						fWeight
					));

				XMStoreFloat3(&((VTXTEX*)SubResource.pData)[iIndex + 1].vPosition,
					XMVectorCatmullRom(
						XMLoadFloat3(&((VTXTEX*)SubResource.pData)[m_iCatmullRomIndex[0] + 1].vPosition),
						XMLoadFloat3(&((VTXTEX*)SubResource.pData)[m_iCatmullRomIndex[1] + 1].vPosition),
						XMLoadFloat3(&((VTXTEX*)SubResource.pData)[m_iCatmullRomIndex[2] + 1].vPosition),
						XMLoadFloat3(&((VTXTEX*)SubResource.pData)[m_iCatmullRomIndex[3] + 1].vPosition),
						fWeight
					));
			}



			m_iVtxCount = min(m_iNumVertices, m_iEndIndex + 2);
			if (m_iVtxCount > m_iNumVertices - 1)
				m_iVtxCount = m_iNumVertices - 1;
			m_iCatmullRomIndex[0] = m_iCatmullRomIndex[1];
			m_iCatmullRomIndex[1] = m_iCatmullRomIndex[2];

			for (_uint i = 0; i < m_iVtxCount; i += 2)
			{
				((VTXTEX*)SubResource.pData)[i].vTexUV = { (_float)i / ((_float)m_iVtxCount - 2), 1.f };
				((VTXTEX*)SubResource.pData)[i + 1].vTexUV = { (_float)i / ((_float)m_iVtxCount - 2), 0.f };
			}

			m_iCatmullRomCount += 1;
			DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);

		}
		else
		{
			//버텍스사이즈 초과함
			_uint	iRemoveCount = m_iVtxCount / 8;

			if (iRemoveCount % 2 > 0)
			{
				iRemoveCount -= 1;
			}

			if (iRemoveCount < 2)
				iRemoveCount = 2;

			if (m_iVtxCount >= 2)
				m_iVtxCount -= iRemoveCount;

			if (m_iCatmullRomCount > 0)
				m_iCatmullRomCount -= 1;



			D3D11_MAPPED_SUBRESOURCE		SubResource;
			DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);


			if (m_iVtxCount <= m_iNumVertices / 4)
			{
				for (_uint i = 2; i < m_iNumVertices; i += 2)
				{
					((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[0].vPosition;
					((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[1].vPosition;
				}
				m_iVtxCount = 0;

			}


			for (_uint i = 0; i < m_iVtxCount; i += 2)
			{
				((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i].vPosition;
				((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i + 1].vPosition;
			}
			DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);
		}
	}
	else
	{
		//꺼져있으면 점점사라지게
		if (m_iVtxCount > 0)
		{
			_uint	iRemoveCount = m_iVtxCount / 8;

			if (iRemoveCount % 2 > 0)
			{
				iRemoveCount -= 1;
			}

			if (iRemoveCount < 2)
				iRemoveCount = 2;

			if (m_iVtxCount >= 2)
				m_iVtxCount -= iRemoveCount;

			D3D11_MAPPED_SUBRESOURCE		SubResource;
			DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

			if (m_iVtxCount <= m_iNumVertices / 3)
			{
				for (_uint i = 2; i < m_iNumVertices; i += 2)
				{
					((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[0].vPosition;
					((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[1].vPosition;
				}
				m_iVtxCount = 0;

			}

			for (_uint i = 0; i < m_iVtxCount; i += 2)
			{
				((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i].vPosition;
				((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i + 1].vPosition;
			}


			DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);
		}
	}
}

void CTrailBuffer::Update_NonCurve()
{
	if (m_bTrailOn)
	{
		if (m_iVtxCount < m_iNumVertices)
		{
			D3D11_MAPPED_SUBRESOURCE		SubResource;

			DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

			//아래쪽
			_float4 vHandPos = m_vLocalSwordLow;

			if (!m_pRefBone)
			{
				_float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();
				vHandPos = vHandPos.MultiplyCoord(matWorld);
			}
			else
			{
				_matrix matRightHand = m_pRefBone->Get_CombinedMatrix();
				matRightHand = XMMatrixMultiply(matRightHand, m_TransformationMatrix.XMLoad());
				_float4x4 matBone;
				XMStoreFloat4x4(&matBone, matRightHand);
				_float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();

				vHandPos = vHandPos.MultiplyCoord(matBone);
				vHandPos = vHandPos.MultiplyCoord(matWorld);
			}


			((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.x = vHandPos.x;
			((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.y = vHandPos.y;
			((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.z = vHandPos.z;


			//위쪽
			vHandPos = m_vLocalSwordHigh;
			if (!m_pRefBone)
			{
				_float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();
				vHandPos = vHandPos.MultiplyCoord(matWorld);
			}
			else
			{
				_matrix matRightHand = m_pRefBone->Get_CombinedMatrix();
				matRightHand = XMMatrixMultiply(matRightHand, m_TransformationMatrix.XMLoad());
				_float4x4 matBone;
				XMStoreFloat4x4(&matBone, matRightHand);
				_float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();

				vHandPos = vHandPos.MultiplyCoord(matBone);
				vHandPos = vHandPos.MultiplyCoord(matWorld);
			}

			((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.x = vHandPos.x;
			((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.y = vHandPos.y;
			((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.z = vHandPos.z;

			m_iVtxCount += 2;

			if (m_iVtxCount > m_iNumVertices)
				m_iVtxCount = m_iNumVertices;

			for (_uint i = 0; i < m_iVtxCount; i += 2)
			{
				((VTXTEX*)SubResource.pData)[i].vTexUV = { (_float)i / ((_float)m_iVtxCount - 2), 1.f };
				((VTXTEX*)SubResource.pData)[i + 1].vTexUV = { (_float)i / ((_float)m_iVtxCount - 2), 0.f };
			}

			DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);

		}
		else
		{
			//버텍스사이즈 초과함
			_uint	iRemoveCount = 2;

			if (iRemoveCount % 2 > 0)
			{
				iRemoveCount -= 1;
			}

			if (iRemoveCount < 2)
				iRemoveCount = 2;

			if (m_iVtxCount >= 2)
				m_iVtxCount -= iRemoveCount;


			D3D11_MAPPED_SUBRESOURCE		SubResource;
			DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

			if (m_iVtxCount <= m_iNumVertices / 4)
			{
				for (_uint i = 2; i < m_iNumVertices; i += 2)
				{
					((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[0].vPosition;
					((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[1].vPosition;
				}
				m_iVtxCount = 0;

			}


			for (_uint i = 0; i < m_iVtxCount; i += 2)
			{
				((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i].vPosition;
				((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i + 1].vPosition;
			}
			DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);
		}
	}
	else
	{
		//버텍스사이즈 초과함
		_uint	iRemoveCount = 2;

		if (iRemoveCount % 2 > 0)
		{
			iRemoveCount -= 1;
		}

		if (iRemoveCount < 2)
			iRemoveCount = 2;

		if (m_iVtxCount >= 2)
			m_iVtxCount -= iRemoveCount;


		D3D11_MAPPED_SUBRESOURCE		SubResource;
		DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

		if (m_iVtxCount <= m_iNumVertices / 4)
		{
			for (_uint i = 2; i < m_iNumVertices; i += 2)
			{
				((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[0].vPosition;
				((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[1].vPosition;
			}
			m_iVtxCount = 0;

		}


		for (_uint i = 0; i < m_iVtxCount; i += 2)
		{
			((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i].vPosition;
			((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[iRemoveCount + i + 1].vPosition;
		}
		DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);
	}


}

void CTrailBuffer::Late_Tick()
{
	m_fTimeAcc += fDT;

	if (m_fTimeAcc > m_fCreateTrailTimer)
	{
		m_fTimeAcc = 0.f;

		if (m_bCatMullRom)
			Update_Curve();
		else
			Update_NonCurve();
	}
}

_uint3 CTrailBuffer::Get_Indices(_uint iIndex) const
{
	return _uint3(((FACEINDICES16*)m_pIndices)[iIndex]._1,
		((FACEINDICES16*)m_pIndices)[iIndex]._2,
		((FACEINDICES16*)m_pIndices)[iIndex]._3);
}

void CTrailBuffer::Set_TrailOn()
{
	m_iCatmullRomCount = 0;
	m_iVtxCount = 0;
	m_bTrailOn = true;
	m_fTimeAcc = 0.f;
	D3D11_MAPPED_SUBRESOURCE		SubResource;
	DEVICE_CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	_float4 vHandPos = m_vLocalSwordLow;

	if (!m_pRefBone)
	{
		_float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();
		vHandPos = vHandPos.MultiplyCoord(matWorld);
	}
	else
	{
		_matrix matRightHand = m_pRefBone->Get_CombinedMatrix();
		matRightHand = XMMatrixMultiply(matRightHand, m_TransformationMatrix.XMLoad());
		_float4x4 matBone;
		XMStoreFloat4x4(&matBone, matRightHand);
		_float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();

		vHandPos = vHandPos.MultiplyCoord(matBone);
		vHandPos = vHandPos.MultiplyCoord(matWorld);
	}

	((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.x = vHandPos.x;
	((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.y = vHandPos.y;
	((VTXTEX*)SubResource.pData)[m_iVtxCount].vPosition.z = vHandPos.z;


	//위쪽
	vHandPos = m_vLocalSwordHigh;

	if (!m_pRefBone)
	{
		_float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();
		vHandPos = vHandPos.MultiplyCoord(matWorld);
	}
	else
	{
		_matrix matRightHand = m_pRefBone->Get_CombinedMatrix();
		matRightHand = XMMatrixMultiply(matRightHand, m_TransformationMatrix.XMLoad());
		_float4x4 matBone;
		XMStoreFloat4x4(&matBone, matRightHand);
		_float4x4 matWorld = m_pOwnerTransform->Get_WorldMatrix();

		vHandPos = vHandPos.MultiplyCoord(matBone);
		vHandPos = vHandPos.MultiplyCoord(matWorld);
	}

	((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.x = vHandPos.x;
	((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.y = vHandPos.y;
	((VTXTEX*)SubResource.pData)[m_iVtxCount + 1].vPosition.z = vHandPos.z;

	m_iVtxCount += 2;

	for (_uint i = m_iVtxCount; i < m_iNumVertices; i += 2)
	{
		((VTXTEX*)SubResource.pData)[i].vPosition = ((VTXTEX*)SubResource.pData)[0].vPosition;
		((VTXTEX*)SubResource.pData)[i + 1].vPosition = ((VTXTEX*)SubResource.pData)[1].vPosition;
	}


	DEVICE_CONTEXT->Unmap(m_pVB.Get(), 0);


}

void CTrailBuffer::Set_TrailOff()
{
	m_bTrailOn = false;

}
