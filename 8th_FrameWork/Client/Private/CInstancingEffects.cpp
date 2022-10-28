#include "stdafx.h"
#include "CInstancingEffects.h"


#include "GameInstance.h"

#include "Model.h"
#include "Physics.h"
#include "CUnit.h"
#include "CModel_Renderer.h"
#include "CShader.h"
#include "Texture.h"

#include "Transform.h"
#include "CComponent_Factory.h"

#include "CCamera_Follow.h"
#include "CEffects_Factory.h"
#include "CUtility_Transform.h"

#include "CInstanceMesh.h"



CInstancingEffects::CInstancingEffects()
{
}

CInstancingEffects::CInstancingEffects(const CInstancingEffects& _origin)
	: CEffect(_origin)
	, m_tCreateData(_origin.m_tCreateData)
	, m_iNumDead(0)
{
	if (_origin.m_tCreateData.pOffsetPositions)
	{
		
		m_tCreateData.pOffsetPositions = new _float4[m_tCreateData.iOffsetPositionCount];
		memcpy(m_tCreateData.pOffsetPositions, _origin.m_tCreateData.pOffsetPositions, sizeof(_float4) * m_tCreateData.iOffsetPositionCount);

	}

	if (_origin.m_pInstanceMatrices)
	{
		m_pInstanceMatrices = new _float4x4[m_tCreateData.iNumInstance];
		memcpy(m_pInstanceMatrices, _origin.m_pInstanceMatrices, sizeof(_float4x4) * m_tCreateData.iNumInstance);
	}
	
	if (_origin.m_pInstancingDatas)
	{
		m_pInstancingDatas = new INSTANCING_DATA[m_tCreateData.iNumInstance];
		memcpy(m_pInstancingDatas, _origin.m_pInstancingDatas, sizeof(INSTANCING_DATA) * m_tCreateData.iNumInstance);
	}
}

CInstancingEffects::~CInstancingEffects()
{
	SAFE_DELETE_ARRAY(m_pInstanceMatrices);
	SAFE_DELETE_ARRAY(m_pInstancingDatas);
	if (m_tCreateData.iOffsetPositionCount >0)
	SAFE_DELETE_ARRAY(m_tCreateData.pOffsetPositions);
}

CInstancingEffects* CInstancingEffects::Create(wstring wstrFbxPath, _uint iNumInstance, const INSTANCING_CREATE_DATA& tCreateData, _hashcode _hcCode)
{
	CInstancingEffects* pInstance = new CInstancingEffects();

	pInstance->m_tCreateData.iNumInstance = iNumInstance;

	if (FAILED(pInstance->SetUp_InstancingEffects(wstrFbxPath, iNumInstance, tCreateData, _hcCode)))
	{
		Call_MsgBox(L"Failed to SetUp_MeshContainer : CInstancingEffects");
		SAFE_DELETE(pInstance);
		return nullptr;
	}

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CInstancingEffects");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CInstancingEffects::Initialize_Prototype()
{
	m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODELINSTANCE,
		VTXMODEL_INSTANCE_DECLARATION::Element, VTXMODEL_INSTANCE_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXINSTANCE_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);
	Add_Component(CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, m_wstrPath.c_str(), m_tCreateData.iNumInstance, m_matTrans));

	//Add_Component(CComponent_Factory::Clone_Component<CPhysics>(this));


	if (m_wstrPath == L"../bin/resources/meshes/effects/Small/SM_EFF_Grass_01.fbx" ||
		m_wstrPath == L"../bin/resources/meshes/effects/Small/SM_EFF_Leaf_01.fbx")
	{
		GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[1]->Add_Texture(L"../bin/resources/textures/modeltextures/T_EFF_GMS_Leaf_01_M.png");
		GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[1]->Set_CurTextureIndex(1);
	}
	else if (m_wstrPath == L"../bin/resources/meshes/effects/Small/SM_EFF_SmallRock_01.fbx")
	{
		GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[1]->Add_Texture(L"../bin/resources/textures/modeltextures/T_EFF_GMS_Ground_01.png");
		GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[1]->Set_CurTextureIndex(1);
	}
	else if (m_wstrPath == L"../bin/resources/meshes/effects/Text/SM_EFF_Text_Reanimation_01.fbx")
	{
		GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[1]->Add_Texture(L"../bin/resources/textures/black.png");
		GET_COMPONENT(CModel)->Get_Materials().front().second.pTextures[1]->Set_CurTextureIndex(1);
	}



	m_pInstanceMatrices = new _float4x4[m_tCreateData.iNumInstance];
	ZeroMemory(m_pInstanceMatrices, sizeof(_float4x4) * m_tCreateData.iNumInstance);

	m_pInstancingDatas = new INSTANCING_DATA[m_tCreateData.iNumInstance];

	return S_OK;
}

HRESULT CInstancingEffects::Initialize()
{
	m_fTimeAcc = 0.f;
	m_iNumDead = 0;
	//GET_COMPONENT(CPhysics)->Set_NaviOn();

	for (_int i = 0; i < m_tCreateData.iNumInstance; ++i)
	{
		_float4 vStartPos = _float4(0.f, 0.f, 0.f, 1.f);

		

		_float4 vStartDir = _float4(
			frandom(-m_tCreateData.fCreateRange, m_tCreateData.fCreateRange),
			frandom(-m_tCreateData.fCreateRangeY, m_tCreateData.fCreateRangeY),
			frandom(-m_tCreateData.fCreateRange, m_tCreateData.fCreateRange),
			0.f);

		vStartPos += vStartDir;

		m_pInstancingDatas[i].fSpeed = m_tCreateData.fSpeed + frandom(-m_tCreateData.fSpeedRange, m_tCreateData.fSpeedRange);
		m_pInstancingDatas[i].fTurnSpeed = m_tCreateData.fTurnSpeed + frandom(-m_tCreateData.fTurnSpeedRange, m_tCreateData.fTurnSpeedRange);
		m_pInstancingDatas[i].fJumpPower = m_tCreateData.fJumpPower + frandom(-m_tCreateData.fJumpPowerRange, m_tCreateData.fJumpPowerRange);
		m_pInstancingDatas[i].fGravity = m_tCreateData.fGravity + frandom(-m_tCreateData.fGravityRange, m_tCreateData.fGravityRange);
		m_pInstancingDatas[i].fCurGroundY = m_tCreateData.fCurGroundY + frandom(-m_tCreateData.fCurGroundYRange, m_tCreateData.fCurGroundYRange);
		m_pInstancingDatas[i].fSpeedChangeSpeed = m_tCreateData.fSpeedChangeSpeed + frandom(-m_tCreateData.fSpeedChangeSpeedRange, m_tCreateData.fSpeedChangeSpeedRange);
		m_pInstancingDatas[i].fTargetAlpha = m_tCreateData.fTargetAlpha;
		
		m_pInstancingDatas[i].fFadeInStartTime = m_tCreateData.fFadeInStartTime + frandom(-m_tCreateData.fFadeInStartTimeRange, m_tCreateData.fFadeInStartTimeRange);
		m_pInstancingDatas[i].fFadeInTime = m_tCreateData.fFadeInTime + frandom(-m_tCreateData.fFadeInTimeRange, m_tCreateData.fFadeInTimeRange);
		m_pInstancingDatas[i].fFadeOutStartTime = m_tCreateData.fFadeOutStartTime + frandom(-m_tCreateData.fFadeOutStartTimeRange, m_tCreateData.fFadeOutStartTimeRange);
		m_pInstancingDatas[i].fFadeOutTime = m_tCreateData.fFadeOutTime + frandom(-m_tCreateData.fFadeOutTimeRange, m_tCreateData.fFadeOutTimeRange);
		
		m_pInstancingDatas[i].vDir = vStartDir;

		if (m_pInstancingDatas[i].fJumpPower > 0.f)
			m_pInstancingDatas[i].vDir.y = 0.f;

		m_pInstancingDatas[i].vColor = _float4(
			frandom(-m_tCreateData.vColorRange.x, m_tCreateData.vColorRange.x),
			frandom(-m_tCreateData.vColorRange.y, m_tCreateData.vColorRange.y),
			frandom(-m_tCreateData.vColorRange.z, m_tCreateData.vColorRange.z),
			0.f);
		m_pInstancingDatas[i].vColor += m_tCreateData.vColor;

		m_pInstancingDatas[i].vDir.Normalize();

		m_pInstancingDatas[i].vScale = _float4(
			frandom(-m_tCreateData.vScaleRange.x, m_tCreateData.vScaleRange.x),
			frandom(-m_tCreateData.vScaleRange.y, m_tCreateData.vScaleRange.y),
			frandom(-m_tCreateData.vScaleRange.z, m_tCreateData.vScaleRange.z),
			0.f);

		m_pInstancingDatas[i].vScale += m_tCreateData.vScale;

		if (m_pInstancingDatas[i].vScale.x <= 0.f)
			m_pInstancingDatas[i].vScale.x = 0.1f;
		if (m_pInstancingDatas[i].vScale.y <= 0.f)
			m_pInstancingDatas[i].vScale.y = 0.1f;
		if (m_pInstancingDatas[i].vScale.z <= 0.f)
			m_pInstancingDatas[i].vScale.z = 0.1f;

		m_pInstancingDatas[i].vStartScale = m_pInstancingDatas[i].vScale;

		m_pInstancingDatas[i].vFadeInTargetScale = _float4(
			frandom(-m_tCreateData.vFadeInTargetScaleRange.x, m_tCreateData.vFadeInTargetScaleRange.x),
			frandom(-m_tCreateData.vFadeInTargetScaleRange.y, m_tCreateData.vFadeInTargetScaleRange.y),
			frandom(-m_tCreateData.vFadeInTargetScaleRange.z, m_tCreateData.vFadeInTargetScaleRange.z),
			0.f);

		m_pInstancingDatas[i].vFadeInTargetScale += m_tCreateData.vFadeInTargetScale;
		if (m_pInstancingDatas[i].vFadeInTargetScale.x <= 0.f)
			m_pInstancingDatas[i].vFadeInTargetScale.x = 0.1f;
		if (m_pInstancingDatas[i].vFadeInTargetScale.y <= 0.f)
			m_pInstancingDatas[i].vFadeInTargetScale.y = 0.1f;
		if (m_pInstancingDatas[i].vFadeInTargetScale.z <= 0.f)
			m_pInstancingDatas[i].vFadeInTargetScale.z = 0.1f;

		m_pInstancingDatas[i].vFadeOutTargetScale = _float4(
			frandom(-m_tCreateData.vFadeOutTargetScaleRange.x, m_tCreateData.vFadeOutTargetScaleRange.x),
			frandom(-m_tCreateData.vFadeOutTargetScaleRange.y, m_tCreateData.vFadeOutTargetScaleRange.y),
			frandom(-m_tCreateData.vFadeOutTargetScaleRange.z, m_tCreateData.vFadeOutTargetScaleRange.z),
			0.f);

		m_pInstancingDatas[i].vFadeOutTargetScale += m_tCreateData.vFadeOutTargetScale;
		if (m_pInstancingDatas[i].vFadeOutTargetScale.x <= 0.f)
			m_pInstancingDatas[i].vFadeOutTargetScale.x = 0.1f;
		if (m_pInstancingDatas[i].vFadeOutTargetScale.y <= 0.f)
			m_pInstancingDatas[i].vFadeOutTargetScale.y = 0.1f;
		if (m_pInstancingDatas[i].vFadeOutTargetScale.z <= 0.f)
			m_pInstancingDatas[i].vFadeOutTargetScale.z = 0.1f;


		m_pInstancingDatas[i].vTurnDir = _float4(
			frandom(-1.f, 1.f),
			frandom(-1.f, 1.f),
			frandom(-1.f, 1.f),
			0.f);
		m_pInstancingDatas[i].vTurnDir.Normalize();
		
		
		*((_float4*)(&(m_pInstanceMatrices[i].m[0]))) = _float4(1.f, 0.f, 0.f, 0.f);
		*((_float4*)(&(m_pInstanceMatrices[i].m[1]))) = _float4(0.f, 1.f, 0.f, 0.f);
		*((_float4*)(&(m_pInstanceMatrices[i].m[2]))) = _float4(0.f, 0.f, 1.f, 0.f);

		//시작위치
		memcpy(m_pInstanceMatrices[i].m[3], &vStartPos, sizeof(_float4));


		//회전시켜놓기
		_float4 vLook = m_pInstancingDatas[i].vDir;
		*((_float4*)(&(m_pInstanceMatrices[i].m[2]))) = vLook * m_pInstancingDatas[i].vStartScale.z;

		_float4 vUp = { 0.f, 1.f, 0.f };
		if ((vLook.y < 1.1f && vLook.y > 0.9f) ||
			(vLook.y > -1.1f && vLook.y < -0.9f)
			)
			vUp = _float4(0.f, 0.f, 1.f, 0.f);

		vUp.Normalize();
		_float4 vRight = vUp.Cross(vLook);
		*((_float4*)(&(m_pInstanceMatrices[i].m[0]))) = vRight.Normalize() * m_pInstancingDatas[i].vStartScale.x;

		vUp = vLook.Cross(vRight);
		*((_float4*)(&(m_pInstanceMatrices[i].m[1]))) = vUp.Normalize() * m_pInstancingDatas[i].vStartScale.y;

		//초기화용 매트릭스 저장
		m_pInstancingDatas[i].StartMatrix = m_pInstanceMatrices[i];

		//Jump
		m_pInstancingDatas[i].fPrevY = vStartPos.y;
		m_pInstancingDatas[i].fOriginY = vStartPos.y;
		m_pInstancingDatas[i].fAcc = 0.f;
		m_pInstancingDatas[i].fPlusAcc = 1.5f;


		m_pInstancingDatas[i].bAlive = true;
	}

	GET_COMPONENT(CModel)->Set_OutlinePower(0.3f);

    return S_OK;
}

HRESULT CInstancingEffects::Start()
{
	CGameObject::Start();
	return S_OK;
}

void CInstancingEffects::My_Tick()
{
	_float fTimeDelta = fDT;
	for (_int i = 0; i < m_tCreateData.iNumInstance; ++i)
	{
		if (!m_pInstancingDatas[i].bAlive)
			continue;

		m_pInstancingDatas[i].fTimeAcc += fTimeDelta;

		//3. FADE
		if (!Fade_Lerp(i))
		{
			*((_float4*)(&(m_pInstanceMatrices[i].m[3]))) = _float4(0.f, -100.f, 0.f, 1.f);
			continue;
		}
		else
		{
			if ((*((_float4*)(&(m_pInstanceMatrices[i].m[3])))).y < -99.f)
			{
				*((_float4*)(&(m_pInstanceMatrices[i].m[3]))) = *((_float4*)(&(m_pInstancingDatas[i].StartMatrix.m[3])));
			}
		}


		_float4 vOriginPos = *((_float4*)(&(m_pInstanceMatrices[i].m[3])));

		//1. 속도 변화.
		m_pInstancingDatas[i].fSpeed += m_pInstancingDatas[i].fSpeedChangeSpeed * fTimeDelta;
		if (m_pInstancingDatas[i].fSpeed <= 0.f)
		{
			Dead_Instance(i);
			continue;
		}

		vOriginPos += m_pInstancingDatas[i].vDir * m_pInstancingDatas[i].fSpeed * fTimeDelta;

		//2. 자유낙하
		_float fFreeFallY = 0.f;
		_float fFreeFallPower = 0.f;

		m_pInstancingDatas[i].fAcc += m_pInstancingDatas[i].fPlusAcc * fTimeDelta;

		fFreeFallY = m_pInstancingDatas[i].fOriginY + ((m_pInstancingDatas[i].fJumpPower * m_pInstancingDatas[i].fAcc) - (m_pInstancingDatas[i].fGravity * 0.5f * m_pInstancingDatas[i].fAcc * m_pInstancingDatas[i].fAcc));

		//이동량 = 현재 Y - 이전 Y
		fFreeFallPower = fFreeFallY - m_pInstancingDatas[i].fPrevY;

		m_pInstancingDatas[i].fPrevY = fFreeFallY;

		vOriginPos.y += fFreeFallPower;

		if (vOriginPos.y < m_pInstancingDatas[i].fCurGroundY)
		{
			Dead_Instance(i);
			continue;
		}

		//3. FADE
		//Fade_Lerp(i);

		//회전
		_float4 vRight = *((_float4*)(&(m_pInstanceMatrices[i].m[0])));
		_float4 vUp = *((_float4*)(&(m_pInstanceMatrices[i].m[1])));
		_float4 vLook = *((_float4*)(&(m_pInstanceMatrices[i].m[2])));

		_float4x4 matRot;

		matRot = XMMatrixRotationAxis(m_pInstancingDatas[i].vTurnDir.XMLoad(), m_pInstancingDatas[i].fTurnSpeed * fTimeDelta);

		vLook = vLook.MultiplyNormal(matRot);
		vRight = vRight.MultiplyNormal(matRot);
		vUp = vUp.MultiplyNormal(matRot);


		*((_float4*)(&(m_pInstanceMatrices[i].m[0]))) = vRight.Normalize() * m_pInstancingDatas[i].vScale.x;
		*((_float4*)(&(m_pInstanceMatrices[i].m[1]))) = vUp.Normalize() * m_pInstancingDatas[i].vScale.y;
		*((_float4*)(&(m_pInstanceMatrices[i].m[2]))) = vLook.Normalize() * m_pInstancingDatas[i].vScale.z;
		*((_float4*)(&(m_pInstanceMatrices[i].m[3]))) = vOriginPos;
	}


	if (m_iNumDead >= m_tCreateData.iNumInstance)
	{
		DISABLE_GAMEOBJECT(this);
	}

}

void CInstancingEffects::My_LateTick()
{
	static_cast<CInstanceMesh*>(GET_COMPONENT(CModel)->Get_MeshContainers().front().second)
		->ReMap_Instances(m_pInstanceMatrices);
}

void CInstancingEffects::OnEnable()
{
	__super::OnEnable();
	m_iNumDead = 0;

	//시작위치
	for (_int i = 0; i < m_tCreateData.iNumInstance; ++i)
	{
		memcpy(&m_pInstanceMatrices[i], &m_pInstancingDatas[i].StartMatrix, sizeof(_float4x4));

		_float4 vStartPos = *((_float4*)(&(m_pInstancingDatas[i].StartMatrix.m[3])));

		m_pInstancingDatas[i].vScale = m_pInstancingDatas[i].vStartScale;
		m_pInstancingDatas[i].vColor.w = 0.f;
		m_pInstancingDatas[i].eCurFadeType = INSTANCING_DATA::FADEINREADY;
		m_pInstancingDatas[i].fTimeAcc = 0.f;

		//Jump
		m_pInstancingDatas[i].fPrevY = vStartPos.y;
		m_pInstancingDatas[i].fOriginY = vStartPos.y;
		m_pInstancingDatas[i].fAcc = 0.f;
		m_pInstancingDatas[i].fTimeAcc = 0.f;
		m_pInstancingDatas[i].bAlive = true;
	}
	
}

HRESULT CInstancingEffects::SetUp_InstancingEffects(wstring wstrFbxPath, _uint iNumInstance, const INSTANCING_CREATE_DATA& tCreateData, _hashcode _hcCode)
{
	m_wstrPath = wstrFbxPath;
	m_tCreateData.iNumInstance = iNumInstance;
	m_tCreateData = tCreateData;
	m_hcMyCode = _hcCode;

	

	return S_OK;
}

_bool CInstancingEffects::Fade_Lerp(_uint iIndex)
{
	switch (m_pInstancingDatas[iIndex].eCurFadeType)
	{
	case INSTANCING_DATA::FADEINREADY:
		if (m_pInstancingDatas[iIndex].fTimeAcc >= m_pInstancingDatas[iIndex].fFadeInStartTime)
		{
			m_pInstancingDatas[iIndex].fTimeAcc = 0.f;
			m_pInstancingDatas[iIndex].eCurFadeType = INSTANCING_DATA::FADEIN;
			m_pInstancingDatas[iIndex].vOriginScale = m_pInstancingDatas[iIndex].vScale = m_pInstancingDatas[iIndex].vScale;
			m_pInstancingDatas[iIndex].fOriginAlpha = 0.f;


		}
		else
		{
			return false;

		}


		break;

	case INSTANCING_DATA::FADEIN:
		if (m_pInstancingDatas[iIndex].fTimeAcc >= m_pInstancingDatas[iIndex].fFadeInTime)
		{
			m_pInstancingDatas[iIndex].fTimeAcc = 0.f;
			m_pInstancingDatas[iIndex].eCurFadeType = INSTANCING_DATA::FADEOUTREADY;
			m_pInstancingDatas[iIndex].vScale = m_pInstancingDatas[iIndex].vFadeInTargetScale;
			m_pInstancingDatas[iIndex].vColor.w = m_pInstancingDatas[iIndex].fTargetAlpha;
		}
		else
		{
			_float fRatio = m_pInstancingDatas[iIndex].fTimeAcc / m_pInstancingDatas[iIndex].fFadeInTime;

			if (m_bSoft)
				fRatio *= fRatio;

			m_pInstancingDatas[iIndex].vScale = 
				XMVectorLerp(m_pInstancingDatas[iIndex].vOriginScale.XMLoad(), m_pInstancingDatas[iIndex].vFadeInTargetScale.XMLoad(), fRatio);

			m_pInstancingDatas[iIndex].vColor.w = m_pInstancingDatas[iIndex].fTargetAlpha * fRatio;
		}

		break;

	case INSTANCING_DATA::FADEOUTREADY:
		if (m_pInstancingDatas[iIndex].fTimeAcc >= m_pInstancingDatas[iIndex].fFadeOutStartTime)
		{
			m_pInstancingDatas[iIndex].fTimeAcc = 0.f;
			m_pInstancingDatas[iIndex].eCurFadeType = INSTANCING_DATA::FADEOUT;
			m_pInstancingDatas[iIndex].vOriginScale = m_pInstancingDatas[iIndex].vScale = m_pInstancingDatas[iIndex].vFadeInTargetScale;
			m_pInstancingDatas[iIndex].fOriginAlpha = m_pInstancingDatas[iIndex].vColor.w = m_pInstancingDatas[iIndex].fTargetAlpha;
		}

		break;

	case INSTANCING_DATA::FADEOUT:
		if (m_pInstancingDatas[iIndex].fTimeAcc >= m_pInstancingDatas[iIndex].fFadeOutTime)
		{
			m_pInstancingDatas[iIndex].fTimeAcc = 0.f;
			Dead_Instance(iIndex);
			return false;
		}
		else
		{
			_float fRatio = m_pInstancingDatas[iIndex].fTimeAcc / m_pInstancingDatas[iIndex].fFadeOutTime;
			if (m_bSoft)
				fRatio = sqrtf(fRatio);

			m_pInstancingDatas[iIndex].vScale =
				XMVectorLerp(m_pInstancingDatas[iIndex].vOriginScale.XMLoad(), m_pInstancingDatas[iIndex].vFadeOutTargetScale.XMLoad(), fRatio);

			m_pInstancingDatas[iIndex].vColor.w = (1.f - fRatio);
		}

		break;

	default:
		break;
	}

	return true;
}

void CInstancingEffects::Dead_Instance(_uint iIndex)
{
	m_pInstancingDatas[iIndex].bAlive = false;
	m_iNumDead++;

	//작아지게해서 안보이게
	*((_float4*)(&(m_pInstanceMatrices[iIndex].m[0]))) *= 0.00001f;
	*((_float4*)(&(m_pInstanceMatrices[iIndex].m[1]))) *= 0.00001f;
	*((_float4*)(&(m_pInstanceMatrices[iIndex].m[2]))) *= 0.00001f;
}

void CInstancingEffects::ReMake_OffsetPositions(_int iSize)
{
	_int iPrevCnt = m_tCreateData.iOffsetPositionCount;
	m_tCreateData.iOffsetPositionCount = iSize;

	_float4* pNewOffsets = new _float4[iSize];
	ZeroMemory(pNewOffsets, sizeof(_float4) * iSize);

	//갱신 갯수가 이전 갯수보다 작을 때.
	if (iPrevCnt > iSize)
	{
		for (_int i = 0; i < iSize; ++i)
		{
			pNewOffsets[i] = m_tCreateData.pOffsetPositions[i];
		}

	}
	else
	{
		for (_int i = 0; i < iPrevCnt; ++i)
		{
			pNewOffsets[i] = m_tCreateData.pOffsetPositions[i];
		}
	}
	
	if (iPrevCnt > 0)
		SAFE_DELETE_ARRAY(m_tCreateData.pOffsetPositions);

	m_tCreateData.pOffsetPositions = pNewOffsets;
}
