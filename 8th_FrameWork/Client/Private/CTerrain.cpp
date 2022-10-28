#include "stdafx.h"
#include "CTerrain.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CShader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "CMesh_Rect.h"
#include "CMesh_Terrain.h"

#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

#include "CCell.h"

CTerrain::CTerrain()
{
}

CTerrain::~CTerrain()
{
    for (auto& elem : m_vecCells)
        SAFE_DELETE(elem);

    m_vecCells.clear();
}

CTerrain* CTerrain::Create(_uint iNumVerticesX, _uint iNumVerticesZ)
{
    CTerrain* pInstance = new CTerrain;
    if (FAILED(pInstance->SetUp_TerrainMesh(iNumVerticesX, iNumVerticesZ)))
    {
        Call_MsgBox(L"Failed to SetUp_TerrainMesh : CTerrain");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to SetUp_TerrainMesh : CTerrain");
        SAFE_DELETE(pInstance);
    }

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to SetUp_TerrainMesh : CTerrain");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

CTerrain* CTerrain::Create(const _tchar* pFilePath)
{
    CTerrain* pInstance = new CTerrain;
    if (FAILED(pInstance->SetUp_TerrainMesh(pFilePath)))
    {
        Call_MsgBox(L"Failed to SetUp_TerrainMesh : CTerrain");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to SetUp_TerrainMesh : CTerrain");
        SAFE_DELETE(pInstance);
        return nullptr;
    }

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to SetUp_TerrainMesh : CTerrain");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

CCell* CTerrain::Get_CenterCell()
{
    _uint iNumFaces = m_pTerrainMesh->Get_NumPrimitive();
    _float fNumVerticesX = sqrtf(iNumFaces * 0.5f);
    
    _float fCenterVertex = fNumVerticesX * 0.5f;

    _uint iIndex = (fNumVerticesX * fCenterVertex) + fCenterVertex;

    return m_vecCells[iIndex];
}

CCell* CTerrain::Find_Cell(_float4 vPosition)
{
    //Å¸ÀÏ 
    m_iNumCellsX;



    return nullptr;
}

HRESULT CTerrain::Initialize_Prototype()
{
    CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXNOR,
        VTXNORTEX_DECLARATION::Element, VTXNORTEX_DECLARATION::iNumElements);
    pShader->Initialize();
    Add_Component(pShader);

    Add_Component(CRenderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXNOR_PASS_NAVIGATION));
    Add_Component(CComponent_Factory::Clone_Component<CTexture>(this));


    return S_OK;
}

HRESULT CTerrain::Initialize()
{
    m_pTerrainMesh = static_cast<CMesh_Terrain*>(GET_COMPONENT(CMesh));

    if (!m_pTerrainMesh)
        return E_FAIL;

    _uint iNumCells = (_uint)m_vecCells.size();
    _int iNumLineX = m_iNumCellsX;

    for (_int i = 0; i < iNumCells; ++i)
    {
        //È¦¼ö : ¾Æ·¡ »ï°¢Çü
        if (i % 2 == 1)
        {
            //¿À¸¥ÂÊ
            if (i-1 >= 0)
                m_vecCells[i]->Set_Neighbor(CCell::LINE_AB, m_vecCells[i-1]);

            //¾Æ·¡
            if ((i - iNumLineX) - 1 >= 0)
                m_vecCells[i]->Set_Neighbor(CCell::LINE_BC, m_vecCells[i - iNumLineX - 1]);

            //¿ÞÂÊ
            if (i - 3 >= 0)
                m_vecCells[i]->Set_Neighbor(CCell::LINE_CA, m_vecCells[i - 3]);

        }
        else // Â¦¼ö°¡ À§»ï°¢Çü
        {
            //À§
            if (i + iNumLineX + 1 < iNumCells)
                m_vecCells[i]->Set_Neighbor(CCell::LINE_AB, m_vecCells[i + iNumLineX + 1]);

            //¿À¸¥ÂÊ
            if (i + 3 < iNumCells)
                m_vecCells[i]->Set_Neighbor(CCell::LINE_BC, m_vecCells[i + 3]);

            //¿ÞÂÊ
            if (i + 1 < iNumCells)
                m_vecCells[i]->Set_Neighbor(CCell::LINE_CA, m_vecCells[i + 1]);
        }
    }

    return S_OK;
}

HRESULT CTerrain::Start()
{
    
    if (!m_vecCells.empty())
        m_pTerrainMesh->ReMap_Vertices();



    __super::Start();

    DISABLE_COMPONENT(GET_COMPONENT(CRenderer));
    return S_OK;
}

void CTerrain::My_Tick()
{
    if (KEY(F5, TAP))
    {
        if (GET_COMPONENT(CRenderer)->Is_Disable())
            ENABLE_COMPONENT(GET_COMPONENT(CRenderer));
        else
            DISABLE_COMPONENT(GET_COMPONENT(CRenderer));
    }
}

void CTerrain::My_LateTick()
{
    //Á¤Á¡¸¶´Ù ¹ØÀ¸·Î ray½î±â


}

HRESULT CTerrain::SetUp_TerrainMesh(_uint iNumVerticesX, _uint iNumVerticesZ)
{
    CMesh_Terrain* pTerrain = CMesh_Terrain::Create(CP_BEFORE_RENDERER, iNumVerticesX, iNumVerticesZ);

    if (!pTerrain)
        return E_FAIL;

    pTerrain->Initialize();
    Add_Component<CMesh>(pTerrain);

    return S_OK;
}

HRESULT CTerrain::SetUp_TerrainMesh(const _tchar* pFilePath)
{
    wstring wstrFilePath = L"../bin/terrain/";
    wstrFilePath += pFilePath;
    wstrFilePath += L".dat";

    ifstream	readFile(wstrFilePath, ios::binary);

    if (!readFile.is_open())
    {
        return E_FAIL;
    }


    _uint       iNumVerticesX, iNumVerticesZ;

    readFile.read((char*)&iNumVerticesX, sizeof(_uint));
    readFile.read((char*)&iNumVerticesZ, sizeof(_uint));

    CMesh_Terrain* pTerrain = CMesh_Terrain::Create(CP_BEFORE_RENDERER, iNumVerticesX, iNumVerticesZ);

    if (!pTerrain)
        return E_FAIL;

    pTerrain->Initialize();
    Add_Component<CMesh>(pTerrain);

    _uint iNumVertices = pTerrain->Get_NumVertices();
    _float3* pVerticesPos = pTerrain->Get_VerticesPos();
    _float4* pVerticesColor = pTerrain->Get_VerticesColor();

    readFile.read((char*)pVerticesPos, sizeof(_float3) * iNumVertices);
    readFile.read((char*)pVerticesColor, sizeof(_float4) * iNumVertices);

    _uint iIndex = 0;
    m_vecCells.reserve((iNumVertices / 3) + 1);

    _uint iNumFaces = pTerrain->Get_NumPrimitive();
    _uint			iIndexSize = pTerrain->Get_IndexSize();

    _float3* pPoints = new _float3[CCell::POINT_END];

    for (_uint i = 0; i < iNumFaces; ++i)
    {
        _uint3		iIndices = pTerrain->Get_Indices(i);

        pPoints[CCell::POINT_A] = pVerticesPos[iIndices._1];
        pPoints[CCell::POINT_B] = pVerticesPos[iIndices._2];
        pPoints[CCell::POINT_C] = pVerticesPos[iIndices._3];
        m_vecCells.push_back(CCell::Create(pPoints, iIndex++));

        _byte   bCellFlag = 0;

        //¹ý¼±º¤ÅÍ
        _vector V1, V2;
        _float4 vCellNormal;

        V1 = (XMLoadFloat3(&pPoints[CCell::POINT_A]) - XMLoadFloat3(&pPoints[CCell::POINT_B]));
        V2 = (XMLoadFloat3(&pPoints[CCell::POINT_C]) - XMLoadFloat3(&pPoints[CCell::POINT_B]));

        vCellNormal = XMVector3Cross(V1, V2);
        vCellNormal.Normalize();

        if (fabs(vCellNormal.y) < 0.5f)
        {
            bCellFlag |= CELL_WALL;
            pVerticesColor[iIndices._1] = _float4(0.f, 0.f, 1.f, 1.f);
            pVerticesColor[iIndices._2] = _float4(0.f, 0.f, 1.f, 1.f);
            pVerticesColor[iIndices._3] = _float4(0.f, 0.f, 1.f, 1.f);
            m_vecCells.back()->Make_Matrix();

        }



        for (_uint k = 0; k < CCell::POINT_END; ++k)
        {
            if (pPoints[k].y <= -1.f)
            {
                bCellFlag |= CELL_BLOCKED;
                pVerticesColor[iIndices._1] = _float4(1.f, 0.f, 0.f, 1.f);
                pVerticesColor[iIndices._2] = _float4(1.f, 0.f, 0.f, 1.f);
                pVerticesColor[iIndices._3] = _float4(1.f, 0.f, 0.f, 1.f);
                break;
            }
        }

        if (pVerticesColor[iIndices._1].x <= 0.f &&
            pVerticesColor[iIndices._1].y <= 0.7f &&
            pVerticesColor[iIndices._1].z <= 0.f)
            bCellFlag |= CELL_GRASS;

        if (pVerticesColor[iIndices._1].x >= 0.8f &&
            pVerticesColor[iIndices._1].y >= 0.6f &&
            pVerticesColor[iIndices._1].z >= 0.1f)
            bCellFlag |= CELL_GROUND;


        m_vecCells.back()->Set_Flags(bCellFlag);

    }

    delete[] pPoints;

    readFile.read((char*)&m_iStartIndex, sizeof(_uint));

    readFile.close();

    m_iNumCellsX = (iNumVerticesX - 1) * 2;
    m_iNumCellsZ = (iNumVerticesZ - 1) * 2;


    return S_OK;
}
