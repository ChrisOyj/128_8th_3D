#include "stdafx.h"
#include "CScreenEffect.h"


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
#include "Functor.h"

#include "CInstanceMesh.h"
#include "CTri_Instance.h"


CScreenEffect::CScreenEffect()
{
}

CScreenEffect::CScreenEffect(const CScreenEffect& _origin)
	: CEffect(_origin)
	, m_iNumInstance(_origin.m_iNumInstance)
	, m_iNumDeads(0)
{
	if (_origin.m_pInstancingDatas)
	{
		m_pInstancingDatas = new CScreenEffect::INSTANCING_DATA[m_iNumInstance];
		memcpy(m_pInstancingDatas, _origin.m_pInstancingDatas, sizeof(CScreenEffect::INSTANCING_DATA) * m_iNumInstance);
	}

	if (_origin.m_pTriInstances)
	{
		m_pTriInstances = new VTXTRIINSTANCE[m_iNumInstance];
		memcpy(m_pTriInstances, _origin.m_pTriInstances, sizeof(VTXTRIINSTANCE) * m_iNumInstance);
	}

}

CScreenEffect::~CScreenEffect()
{
	SAFE_DELETE_ARRAY(m_pTriInstances);
	SAFE_DELETE_ARRAY(m_pInstancingDatas);
}

CScreenEffect* CScreenEffect::Create(_uint iNumInstance)
{
	CScreenEffect* pInstance = new CScreenEffect();

	pInstance->m_iNumInstance = iNumInstance;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CScreenEffect");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CScreenEffect::Initialize_Prototype()
{
	m_hcMyCode = HASHCODE(CScreenEffect);

	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXTRIINSTANCE,
		VTXTRI_INSTANCE_DECLARATION::Element, VTXTRI_INSTANCE_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CRenderer* pRenderer = CRenderer::Create(CP_RENDERER, RENDER_UI, VTXTRIINSTANCE_PASS_DEFAULT
		, _float4(0.f, 0.f, 0.f, 1.f));

	Add_Component<CRenderer>(pRenderer);
	Add_Component<CMesh>(CTri_Instance::Create(m_iNumInstance));
	//Add_Component<CTexture>(CTexture::Create(0, m_wstrPath.c_str(), 1));

	m_pTriInstances = new VTXTRIINSTANCE[m_iNumInstance];
	ZeroMemory(m_pTriInstances, sizeof(VTXTRIINSTANCE) * m_iNumInstance);

	m_pInstancingDatas = new INSTANCING_DATA[m_iNumInstance];
	m_vOffsetPos = _float4(0.f, 0.5f, 0.f, 1.f);

	return S_OK;
}

HRESULT CScreenEffect::Initialize()
{
	m_fTimeAcc = 0.f;
	m_iNumDeads = 0;

	_float	fStandardSize = 3.f;

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		//크기 랜덤
		m_pInstancingDatas[i].fTargetDistance = frandom(fStandardSize * 0.5f, fStandardSize * 2.f);
		m_pInstancingDatas[i].fFadeInStartTime = frandom(0.f, 0.3f);
		m_pInstancingDatas[i].fFadeOutStartTime = frandom(0.f, 0.4f);
		m_pInstancingDatas[i].fFadeTime = frandom(0.f, 0.15f);
		m_pInstancingDatas[i].fRadius = frandom(200.f, 600.f);

		//생성위치는 벽쪽기준 점
		_uint iDividedBy4 = m_iNumInstance / 4;

		//위 벽
		if (i < iDividedBy4)
		{
			m_pInstancingDatas[i].vCreatePos.x = frandom((_float)g_iWinCX * -0.5f, (_float)g_iWinCX * 0.5f);
			m_pInstancingDatas[i].vCreatePos.y = g_iWinCY * -0.5f;
		}
		//아래 벽
		else if (i < iDividedBy4*2)
		{
			m_pInstancingDatas[i].vCreatePos.x = frandom((_float)g_iWinCX * -0.5f, (_float)g_iWinCX * 0.5f);
			m_pInstancingDatas[i].vCreatePos.y = g_iWinCY * 0.5f;
		}
		//왼 벽
		else if (i < iDividedBy4 * 3)
		{
			m_pInstancingDatas[i].vCreatePos.x = g_iWinCX * -0.5f;
			m_pInstancingDatas[i].vCreatePos.y = frandom((_float)g_iWinCY * -0.5f, (_float)g_iWinCY * 0.5f);
		}
		//오른 벽
		else
		{
			m_pInstancingDatas[i].vCreatePos.x = g_iWinCX * 0.5f;
			m_pInstancingDatas[i].vCreatePos.y = frandom((_float)g_iWinCY * -0.5f, (_float)g_iWinCY * 0.5f);
		}

		//색도 랜덤므
		_float fRandColor = (_float)random(0, 1);
		m_pTriInstances[i].vColor = _float4(fRandColor, fRandColor, fRandColor, 0);
		
	}


	return S_OK;
}

HRESULT CScreenEffect::Start()
{
	CGameObject::Start();
	//GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&CEffect::Set_ShaderResourceFlag, this, placeholders::_1, "g_vFlag");

	return S_OK;
}

void CScreenEffect::My_Tick()
{
	_float fTimeDelta = fDT;


	//유닛의 투영위치 구하기
	_float4 vTargetWorldPos = m_pFollowTarget->Get_Transform()->Get_World(WORLD_POS);
	vTargetWorldPos += m_vOffsetPos;
	_float4x4 matVP = GAMEINSTANCE->Get_CurViewMatrix() * GAMEINSTANCE->Get_CurProjMatrix();
	_float4 vTargetProjPos = vTargetWorldPos.MultiplyCoord(matVP);
	vTargetProjPos.x *= g_iWinCX * 0.5f;
	vTargetProjPos.y *= g_iWinCY * 0.5f;
	vTargetProjPos.z = 0.8f;
	vTargetProjPos.w = 1.f;

	//range
	_float fRadius = 100.f;


	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		if (!m_pInstancingDatas[i].bAlive)
			continue;

		m_pInstancingDatas[i].fTimeAcc += fTimeDelta;

		switch (m_pInstancingDatas[i].eCurFadeType)
		{
		case INSTANCING_DATA::FADEINREADY:
			if (m_pInstancingDatas[i].fTimeAcc > m_pInstancingDatas[i].fFadeInStartTime)
			{
				m_pInstancingDatas[i].fTimeAcc = 0.f;
				m_pInstancingDatas[i].fOriginDistance = m_pInstancingDatas[i].fDistance;
				m_pInstancingDatas[i].eCurFadeType = INSTANCING_DATA::FADEIN;
			}
			break;
		case INSTANCING_DATA::FADEIN:
			if (m_pInstancingDatas[i].fTimeAcc > m_pInstancingDatas[i].fFadeTime)
			{
				m_pInstancingDatas[i].fTimeAcc = 0.f;
				m_pInstancingDatas[i].fOriginDistance = m_pInstancingDatas[i].fTargetDistance;
				m_pInstancingDatas[i].eCurFadeType = INSTANCING_DATA::FADEOUTREADY;
			}
			else
			{
				_float fRatio = m_pInstancingDatas[i].fTimeAcc / m_pInstancingDatas[i].fFadeTime;
				m_pInstancingDatas[i].fDistance =
				CFunctor::Lerp_Float(m_pInstancingDatas[i].fOriginDistance, m_pInstancingDatas[i].fTargetDistance, fRatio);

				m_pTriInstances[i].vColor.w =
					CFunctor::Lerp_Float(0.f, 1.f, fRatio);
			}

			break;

		case INSTANCING_DATA::FADEOUTREADY:
			if (m_pInstancingDatas[i].fTimeAcc > m_pInstancingDatas[i].fFadeOutStartTime)
			{
				m_pInstancingDatas[i].fTimeAcc = 0.f;
				m_pInstancingDatas[i].fOriginDistance = m_pInstancingDatas[i].fTargetDistance;
				m_pInstancingDatas[i].eCurFadeType = INSTANCING_DATA::FADEOUT;
			}
			else
			{
				m_pInstancingDatas[i].fDistance = m_pInstancingDatas[i].fTargetDistance;
				m_pTriInstances[i].vColor.w = 1.f;

			}
			break;

		case INSTANCING_DATA::FADEOUT:
			if (m_pInstancingDatas[i].fTimeAcc > m_pInstancingDatas[i].fFadeTime)
			{
				Dead_Instance(i);
				m_pInstancingDatas[i].fTimeAcc = 0.f;
				m_pInstancingDatas[i].eCurFadeType = INSTANCING_DATA::FADEINREADY;
			}
			else
			{
				_float fRatio = m_pInstancingDatas[i].fTimeAcc / m_pInstancingDatas[i].fFadeTime;
				m_pInstancingDatas[i].fDistance =
					CFunctor::Lerp_Float(m_pInstancingDatas[i].fOriginDistance, 0.f, fRatio);

				m_pTriInstances[i].vColor.w =
					CFunctor::Lerp_Float(1.f, 0.f, fRatio);
			}
			break;

		default:
			break;
		}

		//투영위치로부터 내 점의 방향
		_float4 vDir;
		vDir = XMVector2Normalize((XMLoadFloat2(&m_pInstancingDatas[i].vCreatePos) - vTargetProjPos.XMLoad()));

		//2번인덱스 점
		_float4 vMyPoint2 = vTargetProjPos + vDir * m_pInstancingDatas[i].fRadius;

		//0,1번 인덱스 점
		m_pInstancingDatas[i].vCreatePos;

		_float4 vMyPoint0, vMyPoint1;

		//가로인지 새로인지를 알아야함
		_uint iDividedBy4 = m_iNumInstance / 4;

		//위,아래 벽
		if (i < iDividedBy4 || i < iDividedBy4 * 2)
		{
			vMyPoint0.x = m_pInstancingDatas[i].vCreatePos.x - m_pInstancingDatas[i].fDistance * 0.5f;
			vMyPoint1.x = m_pInstancingDatas[i].vCreatePos.x + m_pInstancingDatas[i].fDistance * 0.5f;

			vMyPoint0.y = m_pInstancingDatas[i].vCreatePos.y;
			vMyPoint1.y = m_pInstancingDatas[i].vCreatePos.y;
		}
		//왼, 오른 벽
		else
		{
			vMyPoint0.y = m_pInstancingDatas[i].vCreatePos.y - m_pInstancingDatas[i].fDistance * 0.5f;
			vMyPoint1.y = m_pInstancingDatas[i].vCreatePos.y + m_pInstancingDatas[i].fDistance * 0.5f;
			vMyPoint0.x = m_pInstancingDatas[i].vCreatePos.x;
			vMyPoint1.x = m_pInstancingDatas[i].vCreatePos.x;
		}

		vMyPoint0.z = 0.8f;
		vMyPoint0.w = 1.f;

		vMyPoint1.z = 0.8f;
		vMyPoint1.w = 1.f;
		vMyPoint2.z = 0.8f;
		vMyPoint2.w = 1.f;

		XMStoreFloat3(&m_pTriInstances[i].vPosition[0], vMyPoint0.XMLoad());
		XMStoreFloat3(&m_pTriInstances[i].vPosition[1], vMyPoint1.XMLoad());
		XMStoreFloat3(&m_pTriInstances[i].vPosition[2], vMyPoint2.XMLoad());

		
	}




	if (m_iNumDeads >= m_iNumInstance)
	{
		DISABLE_GAMEOBJECT(this);
	}

}

void CScreenEffect::My_LateTick()
{
	static_cast<CTri_Instance*>(GET_COMPONENT(CMesh))->ReMap_Instances(m_pTriInstances);

}

void CScreenEffect::OnEnable()
{
	CEffect::OnEnable();

	m_iNumDeads = 0;

	//시작위치
	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		//모두 초기화
		m_pTriInstances[i].vColor.w = 0.f;
		m_pInstancingDatas[i].fDistance = 0.f;
		m_pInstancingDatas[i].fOriginDistance = 0.f;
		m_pInstancingDatas[i].fTimeAcc = 0.f;
		m_pInstancingDatas[i].bAlive = true;
		m_pInstancingDatas[i].eCurFadeType = CScreenEffect::INSTANCING_DATA::FADEINREADY;
	}

}

void CScreenEffect::Dead_Instance(_uint iIndex)
{
	m_pInstancingDatas[iIndex].bAlive = false;
	m_iNumDeads++;
}
