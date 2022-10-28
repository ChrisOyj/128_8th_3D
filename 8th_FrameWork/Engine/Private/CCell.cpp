#include "CCell.h"

CCell::CCell()
{
}

CCell::~CCell()
{
}

CCell* CCell::Create(const _float3* pPoints, _int iIndex)
{
	CCell* pInstance = new CCell();

	if (FAILED(pInstance->Initialize(pPoints, iIndex)))
	{
		Call_MsgBox(L"Failed to Initialize : CCell");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
	m_iMyIndex = iIndex;

	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	XMStoreFloat3(&m_vLine[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	XMStoreFloat3(&m_vLine[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	XMStoreFloat3(&m_vLine[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));

	m_vNormal[LINE_AB] = _float3(m_vLine[LINE_AB].z * -1.f, 0.f, m_vLine[LINE_AB].x);
	m_vNormal[LINE_BC] = _float3(m_vLine[LINE_BC].z * -1.f, 0.f, m_vLine[LINE_BC].x);
	m_vNormal[LINE_CA] = _float3(m_vLine[LINE_CA].z * -1.f, 0.f, m_vLine[LINE_CA].x);

	

	return S_OK;

}

void CCell::Make_Matrix()
{
	_float4 vQuaternion;

	//법선벡터
	_vector V1, V2;
	V1 = (XMLoadFloat3(&m_vPoints[CCell::POINT_A]) - XMLoadFloat3(&m_vPoints[CCell::POINT_B]));
	V2 = (XMLoadFloat3(&m_vPoints[CCell::POINT_C]) - XMLoadFloat3(&m_vPoints[CCell::POINT_B]));

	m_vCellNormal = XMVector3Cross(V1, V2);
	m_vCellNormal *= -1.f;
	m_vCellNormal.Normalize();


	if (m_iMyIndex % 2 == 1)
	{
		//홀수가 아래삼각형
		m_vCellRight = XMLoadFloat3(&m_vLine[CCell::LINE_BC]);
		m_vCellRight *= -1.f;
		m_vCellRight.Normalize();

	}
	else
	{
		m_vCellRight = XMLoadFloat3(&m_vLine[CCell::LINE_AB]);
		m_vCellRight.Normalize();
	}

	_float4 vDefaultUp = _float4(0.f, 1.f, 0.f, 0.f);
	m_fAngle = m_vCellNormal.Dot(vDefaultUp);
	m_fAngle = XMConvertToDegrees(acosf(m_fAngle));
}

_bool CCell::Compare_Points(_vector vSourPoint, _vector vDestPoint)
{
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}


	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;

		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
	}

	return false;
}

_bool CCell::isIn(_vector vPosition, CCell** pOutNeighborCell, LINE* pOutLine)
{
	/* 이 쎌의 세변에 대해서 나갔는지 안낙ㅆ다ㅣ;ㅓ 판단한다. */
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector	vDir = vPosition - XMLoadFloat3(&m_vPoints[i]);

		/* 바깥으로 나갔으면 갱신 */
		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(XMLoadFloat3(&m_vNormal[i])))))
		{
			*pOutNeighborCell = m_pNeighborCells[i];
			*pOutLine = (LINE)i;

			return false;
		}
	}


	return true;
}
