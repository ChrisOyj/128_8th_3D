#include "..\Public\CMesh_Cube.h"

CMesh_Cube::CMesh_Cube(_uint iGroupIDX)
	: CMesh(iGroupIDX)
{
}

CMesh_Cube::~CMesh_Cube()
{
}

CMesh_Cube* CMesh_Cube::Create(_uint iGroupIDX)
{
	CMesh_Cube* pInstance = new CMesh_Cube(iGroupIDX);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CMesh_Cube");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

CMesh_Cube* CMesh_Cube::Create_DecalBox(_uint iGroupIDX)
{
	CMesh_Cube* pInstance = new CMesh_Cube(iGroupIDX);

	if (FAILED(pInstance->SetUp_DecalBox()))
	{
		Call_MsgBox(L"Failed to SetUp_DecalBox : CMesh_Cube");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CMesh_Cube::Initialize_Prototype()
{
#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXCUBETEX);
	m_iNumVertices = 8;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXCUBETEX* pVertices = new VTXCUBETEX[m_iNumVertices];

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexUV = pVertices[0].vPosition;

	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexUV = pVertices[1].vPosition;

	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexUV = pVertices[2].vPosition;

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexUV = pVertices[3].vPosition;


	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexUV = pVertices[4].vPosition;

	pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexUV = pVertices[5].vPosition;

	pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexUV = pVertices[6].vPosition;

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexUV = pVertices[7].vPosition;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEXBUFFER


#pragma endregion
	m_iIndicesStride = sizeof(FACEINDICES16);
	m_iNumPrimitive = 12;
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

	/* +x */
	pIndices[0]._1 = 1; pIndices[0]._2 = 5; pIndices[0]._3 = 6;
	pIndices[1]._1 = 1; pIndices[1]._2 = 6; pIndices[1]._3 = 2;

	/* -x */
	pIndices[2]._1 = 4; pIndices[2]._2 = 0; pIndices[2]._3 = 3;
	pIndices[3]._1 = 4; pIndices[3]._2 = 3; pIndices[3]._3 = 7;

	/* +y */
	pIndices[4]._1 = 4; pIndices[4]._2 = 5; pIndices[4]._3 = 1;
	pIndices[5]._1 = 4; pIndices[5]._2 = 1; pIndices[5]._3 = 0;

	/* -y */
	pIndices[6]._1 = 3; pIndices[6]._2 = 2; pIndices[6]._3 = 6;
	pIndices[7]._1 = 3; pIndices[7]._2 = 6; pIndices[7]._3 = 7;

	/* +z */
	pIndices[8]._1 = 5; pIndices[8]._2 = 4; pIndices[8]._3 = 7;
	pIndices[9]._1 = 5; pIndices[9]._2 = 7; pIndices[9]._3 = 6;

	/* -z */
	pIndices[10]._1 = 0; pIndices[10]._2 = 1; pIndices[10]._3 = 2;
	pIndices[11]._1 = 0; pIndices[11]._2 = 2; pIndices[11]._3 = 3;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CMesh_Cube::Initialize()
{
	return S_OK;
}

HRESULT CMesh_Cube::SetUp_DecalBox()
{
#pragma region VERTEXBUFFER
	m_iStride = sizeof(VTXDECALCUBE);
	m_iNumVertices = 24;
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXDECALCUBE* pVertices = new VTXDECALCUBE[m_iNumVertices];

	pVertices[0] = VTXDECALCUBE(0.5f, -0.5f, -0.5f, 0.f, 0.f);
	pVertices[1] = VTXDECALCUBE(-0.5f, 0.5f, -0.5f, 1.f, 0.f);
	pVertices[2] = VTXDECALCUBE(0.5f, 0.5f, -0.5f, 1.f, 1.f);
	pVertices[3] = VTXDECALCUBE(0.5f, -0.5f, -0.5f, 0.f, 1.f);

	pVertices[4] = VTXDECALCUBE(-0.5f, -0.5f, 0.5f, 0.f, 0.f);
	pVertices[5] = VTXDECALCUBE(0.5f, -0.5f, 0.5f, 1.f, 0.f);
	pVertices[6] = VTXDECALCUBE(0.5f, 0.5f, 0.5f, 1.f, 1.f);
	pVertices[7] = VTXDECALCUBE(-0.5f, 0.5f, 0.5f, 0.f, 1.f);

	pVertices[8] = VTXDECALCUBE(-0.5f, 0.5f, -0.5f, 0.f, 0.f);
	pVertices[9] = VTXDECALCUBE(-0.5f, 0.5f, 0.5f, 1.f, 0.f);
	pVertices[10] = VTXDECALCUBE(0.5f, 0.5f,0.5f, 1.f, 1.f);
	pVertices[11] = VTXDECALCUBE(0.5f, 0.5f, -0.5f, 0.f, 1.f);

	pVertices[12] = VTXDECALCUBE(- 0.5f, - 0.5f, - 0.5f, 0.f, 0.f);
	pVertices[13] = VTXDECALCUBE( 0.5f, - 0.5f, - 0.5f, 1.f, 0.f);
	pVertices[14] = VTXDECALCUBE( 0.5f, - 0.5f,  0.5f, 1.f, 1.f);
	pVertices[15] = VTXDECALCUBE(- 0.5f, - 0.5f,  0.5f, 0.f, 1.f);

	pVertices[16] = VTXDECALCUBE(- 0.5f, - 0.5f,  0.5f, 0.f, 0.f);
	pVertices[17] = VTXDECALCUBE(- 0.5f,  0.5f,  0.5f, 1.f, 0.f);
	pVertices[18] = VTXDECALCUBE(- 0.5f,  0.5f, - 0.5f, 1.f, 1.f);
	pVertices[19] = VTXDECALCUBE(- 0.5f, - 0.5f, - 0.5f, 0.f, 1.f);

	pVertices[20] = VTXDECALCUBE( 0.5f, - 0.5f, - 0.5f, 0.f, 0.f);
	pVertices[21] = VTXDECALCUBE( 0.5f,  0.5f, - 0.5f, 1.f, 0.f);
	pVertices[22] = VTXDECALCUBE( 0.5f,  0.5f,  0.5f, 1.f, 1.f);
	pVertices[23] = VTXDECALCUBE( 0.5f, - 0.5f,  0.5f, 0.f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEXBUFFER


#pragma endregion
	m_iIndicesStride = sizeof(FACEINDICES16);
	m_iNumPrimitive = 12;
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

	// fill in the front face index data
	pIndices[0]._1 = 0; pIndices[0]._2 = 1; pIndices[0]._3 = 2;
	pIndices[1]._1 = 0; pIndices[1]._2 = 2; pIndices[1]._3 = 3;

	// fill in the back face index data
	pIndices[2]._1 = 4; pIndices[2]._2= 5; pIndices[2]._3 = 6;
	pIndices[3]._1 = 4; pIndices[3]._2 = 6; pIndices[3]._3 = 7;

	// fill in the top face index data
	pIndices[4]._1 = 8; pIndices[4]._2 = 9; pIndices[4]._3 = 10;
	pIndices[5]._1 = 8; pIndices[5]._2 = 10; pIndices[5]._3 = 11;

	// fill in the bottom face index data
	pIndices[6]._1 = 12; pIndices[6]._2 = 13; pIndices[6]._3 = 14;
	pIndices[7]._1 = 12; pIndices[7]._2 = 14; pIndices[7]._3 = 15;

	// fill in the left face index data
	pIndices[8]._1 = 16; pIndices[8]._2 = 17; pIndices[8]._3 = 18;
	pIndices[9]._1 = 16; pIndices[9]._2 = 18; pIndices[9]._3 = 19;

	// fill in the right face index data
	pIndices[10]._1 = 20; pIndices[10]._2 = 21; pIndices[10]._3 = 22;
	pIndices[11]._1 = 20; pIndices[11]._2 = 22; pIndices[11]._3 = 23;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}
