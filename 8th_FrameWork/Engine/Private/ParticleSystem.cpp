#include "..\Public\ParticleSystem.h"

#include "GameInstance.h"
#include "GameObject.h"

#include "Shader_Manager.h"
#include "Prototype_Manager.h"

#include "Mesh_Rect.h"
#include "Mesh_Cube.h"
#include "Particle_Default.h"
#include "Particle_Pop.h"
#include "Particle_Cube.h"
#include "Texture.h"

#include "Transform.h"

CParticleSystem::CParticleSystem()
{
}

CParticleSystem::CParticleSystem(const CParticleSystem& _origin)
	: CRenderer(_origin)
	, m_ePassType(_origin.m_ePassType)
	, m_eParticle_Type (_origin.m_eParticle_Type)
	, m_vRandomRange_ParticlesPos(_origin.m_vRandomRange_ParticlesPos)
	, m_vRandomRange_ParticlesDir(_origin.m_vRandomRange_ParticlesDir)
	, m_vRandomRange_ParticlesScale(_origin.m_vRandomRange_ParticlesScale)
	, m_vRandomRange_ParticlesColor(_origin.m_vRandomRange_ParticlesColor)
	, m_fRandomRange_ParticlesFadeOutSpeed(_origin.m_fRandomRange_ParticlesFadeOutSpeed)
	, m_fRandomRange_ParticlesSpeed(_origin.m_fRandomRange_ParticlesSpeed)
	, m_fRandomRange_ParticleTurnSpeed(_origin.m_fRandomRange_ParticleTurnSpeed)
{
	if (_origin.m_pMesh)
		m_pMesh = _origin.m_pMesh->Clone();

	if (_origin.m_pTexture)
		m_pTexture = _origin.m_pTexture->Clone();
}


CParticleSystem::~CParticleSystem()
{
	Release();
}

CParticleSystem * CParticleSystem::Create(
	CGameObject * pOwner, const _tchar * pTextureFilePath, 
	PARTICLE_PASS_TYPE ePassType, PARTICLE_TYPE eParticleType,
	_float4 vRandomRange_ParticlesPos, _float4 vRandomRange_ParticlesDir, 
	_float4 vRandomRange_ParticlesScale, _float4 vRandomRange_ParticlesColor
	, _float fRandomRange_ParticlesSpeed, _float fRandomRange_ParticlesFadeOutSpeed)
{

	CParticleSystem*		pParticleSystem = GET_CLONE(CParticleSystem);

	if (!pParticleSystem)
		return nullptr;

	pParticleSystem->m_pOwner = pOwner;

	if ((ePassType == P_PASS_DEFAULT || ePassType == P_PASS_CUBE || ePassType == P_PASS_SPECULAR) &&
		!(pParticleSystem->m_pTexture = CTexture::Create(TEX_DEFAULT, pTextureFilePath, 1)))
	{
		delete pParticleSystem;
		return nullptr;
	}

	pParticleSystem->m_ePassType = ePassType;
	pParticleSystem->m_eParticle_Type = eParticleType;
	pParticleSystem->m_vRandomRange_ParticlesPos = vRandomRange_ParticlesPos;
	pParticleSystem->m_vRandomRange_ParticlesDir = vRandomRange_ParticlesDir;
	pParticleSystem->m_vRandomRange_ParticlesScale = vRandomRange_ParticlesScale;
	pParticleSystem->m_vRandomRange_ParticlesColor = vRandomRange_ParticlesColor;
	pParticleSystem->m_fRandomRange_ParticlesFadeOutSpeed = fRandomRange_ParticlesFadeOutSpeed;
	pParticleSystem->m_fRandomRange_ParticlesSpeed = fRandomRange_ParticlesSpeed;

	if (FAILED(pParticleSystem->Initialize()))
	{
		Call_MsgBox(L"FAILED to Initialize : CParticleSystem");
		delete pParticleSystem;
		return nullptr;
	}

	return pParticleSystem;

	return nullptr;
}

void CParticleSystem::Start_ParticleSystem(
	_float4 vParticlesPos, _float4 vParticlesDir, _float4 vParticlesScale,
	_float4 vParticlesColor, _float fParticleGenerationCycle, _float fParticleGenerationTime,
	_uint iParticleGenerationCnt, _float fParticleInitSpeed, _float fParticleChangeSpeed,
	_float fParticleFadeOutSpeed, _uint iMaxParticleCnt)
{
	m_vParticlesPos = vParticlesPos;
	m_vParticlesDir = vParticlesDir;
	m_vParticlesScale = vParticlesScale;
	m_vParticlesColor = vParticlesColor;
	m_fParticleGenerationCycle = fParticleGenerationCycle;
	m_fGenerationCycleAcc = m_fParticleGenerationCycle;
	m_fParticleGenerationTime = fParticleGenerationTime;
	m_fGenerationAcc = 0.f;
	m_iParticleGenerationCnt = iParticleGenerationCnt;
	m_fParticleInitSpeed = fParticleInitSpeed;
	m_fParticleChangeSpeed = fParticleChangeSpeed;
	m_fParticleFadeOutSpeed = fParticleFadeOutSpeed;
	m_iMaxParticleCnt = iMaxParticleCnt;
}

HRESULT CParticleSystem::Initialize_Prototype()
{
	m_eRenderGroup = RENDER_PARTICLE;




	return S_OK;
}

HRESULT CParticleSystem::Initialize()
{
	switch (m_eParticle_Type)
	{
	case PARTICLE_DEFAULT:
	case PARTICLE_POP:
		if (!(m_pMesh = CMesh_Rect::Create(D3DCOLOR_XRGB(0, 0, 0), _float4(1.f, 1.f, 1.f))))
			return E_FAIL;
		break;

	case PARTICLE_CUBE:
		if (!(m_pMesh = CMesh_Cube::Create(D3DCOLOR_XRGB(0,0,0), _float4(1.f, 1.f, 1.f))))
			return E_FAIL;
		break;

	default:
		break;
	}

	

	return S_OK;
}

void CParticleSystem::Tick()
{
	m_fDT = fDT;

	Update_ParticleSystem();

}

HRESULT CParticleSystem::Render()
{

	if (m_pTexture &&
		FAILED(CShader_Manager::Get_Instance()->Set_Particle_Texture(m_pTexture->Get_Texture())))
		return E_FAIL;


	for (auto& tParticle : m_pParticles)
	{
		CShader_Manager::Get_Instance()->Begin_Particle_Pass(m_ePassType, tParticle.pParticle);

		if (FAILED(m_pMesh->Render()))
			return E_FAIL;

		CShader_Manager::Get_Instance()->End_Particle_Pass();
	}

	return S_OK;
}

void CParticleSystem::Update_ParticleSystem()
{
	/* 1. Create Particles */
	Create_Particles();

	/* 2. Update Particles */
	Update_Particles();

	/* 3. Collect Particles */
	Collect_Particles();

	m_iCurParticlesCnt = (_uint)m_pParticles.size();

	

}

void CParticleSystem::Create_Particles()
{
	/* 1. Check Generaetion Time */

	if (m_fGenerationAcc > m_fParticleGenerationTime)
		return;

	m_fGenerationAcc += m_fDT;

	/* 2. Check Generaete Cycle Time */
	m_fGenerationCycleAcc += m_fDT;

	if (m_fGenerationCycleAcc < m_fParticleGenerationCycle)
		return;

	m_fGenerationCycleAcc = 0.f;

	/* 3. Check Cur ParticleCnt with MaxCnt */
	

	/* =========== Now Begin Creating Particles ============== */

	for (_uint i = 0; i < m_iParticleGenerationCnt; ++i)
	{
		if (m_iCurParticlesCnt >= m_iMaxParticleCnt)
			return;


		CParticle*	pNewParticle = nullptr;

		/* 1. Check Particle Collection */
		if (!m_pParticles_Collection.empty())
		{
			pNewParticle = m_pParticles_Collection.front();
			m_pParticles_Collection.pop_front();
			_float4 vPos, vDir;
			vPos = RandomRange(m_vParticlesPos, m_vRandomRange_ParticlesPos);
			vDir = RandomRange(m_vParticlesDir, m_vRandomRange_ParticlesDir);
			pNewParticle->Recycle(vPos, vDir);
		}
		else /* 2. Create Particle */
		{

			/* SetUp Particle */
			_float4 vPos, vDir, vScale;
			_float4 vColor;
			_float fFadeOutSpeed, fSpeed;

			vPos = RandomRange(m_vParticlesPos, m_vRandomRange_ParticlesPos);
			vDir = RandomRange(m_vParticlesDir, m_vRandomRange_ParticlesDir);
			vScale = RandomRange(m_vParticlesScale, m_vRandomRange_ParticlesScale);
			vColor = RandomRange(m_vParticlesColor, m_vRandomRange_ParticlesColor);
			fFadeOutSpeed = m_fParticleFadeOutSpeed;
			fSpeed = m_fParticleInitSpeed;
			fFadeOutSpeed += frandom(m_fRandomRange_ParticlesFadeOutSpeed * -1.f, m_fRandomRange_ParticlesFadeOutSpeed);
			fSpeed += frandom(m_fRandomRange_ParticlesSpeed * -1.f, m_fRandomRange_ParticlesSpeed);


			switch (m_eParticle_Type)
			{
			case PARTICLE_DEFAULT:
				pNewParticle = CParticle_Default::Create(this);
				pNewParticle->SetUp(vPos, vDir, vScale, vColor, fSpeed, m_fParticleChangeSpeed, fFadeOutSpeed);

				break;
			case PARTICLE_POP:
			{
				_float fJumpPower = m_fParticleJumpPower;
				fJumpPower += frandom(m_fRandomRange_ParticleJumpPower * -1.f, m_fRandomRange_ParticleJumpPower);
				pNewParticle = CParticle_Pop::Create(this);
				static_cast<CParticle_Pop*>(pNewParticle)->SetUp(vPos, vDir, vScale, vColor, fSpeed, m_fParticleChangeSpeed, fFadeOutSpeed, fJumpPower);
			}

				break;
			case PARTICLE_CUBE:
			{
				_float fJumpPower = m_fParticleJumpPower;
				fJumpPower += frandom(m_fRandomRange_ParticleJumpPower * -1.f, m_fRandomRange_ParticleJumpPower);

				_float fTurnSpeed = m_fParticleTurnSpeed;
				fTurnSpeed += frandom(m_fRandomRange_ParticleTurnSpeed * -1.f, m_fRandomRange_ParticleTurnSpeed);
				pNewParticle = CParticle_Cube::Create(this);
				static_cast<CParticle_Cube*>(pNewParticle)->SetUp(vPos, vDir, vScale, vColor, fSpeed, m_fParticleChangeSpeed, fFadeOutSpeed, fJumpPower, fTurnSpeed);
			}
				
				break;

			default:
				break;
			}

		


		}

		
		m_pParticles.push_back(PARTICLE(pNewParticle));

	}

	

}

void CParticleSystem::Update_Particles()
{
	if (m_pParticles.empty())
		return;

	_float4 vCamPos = CCamera_Manager::Get_Instance()->Get_ViewPos();

	for (auto iter = m_pParticles.begin(); iter != m_pParticles.end();)
	{
		CParticle* pParticle = iter->pParticle;

		iter->fDistance = (vCamPos - pParticle->Get_Pos()).Length();

		if (false == pParticle->Tick(m_fDT))
		{
			iter = m_pParticles.erase(iter);
			m_pParticles_Collection.push_back(pParticle);
		}
		else
			++iter;
	}

	m_pParticles.sort(
		[](const PARTICLE& a, const PARTICLE& b)
	{
		return a.fDistance > b.fDistance;
	}
	);
}

void CParticleSystem::Collect_Particles()
{
	if (m_fGenerationAcc > m_fParticleGenerationTime)
	{
		//m_pParticles_Collection.clear();
	}

}

_float4 CParticleSystem::RandomRange(const _float4 & vStandard, const _float4 & vRandomRange)
{
	_float4 vResult = vStandard;
	vResult.x += frandom(vRandomRange.x * -1.f, vRandomRange.x);
	vResult.y += frandom(vRandomRange.y * -1.f, vRandomRange.y);
	vResult.z += frandom(vRandomRange.z * -1.f, vRandomRange.z);
	return vResult;
}

_float4 CParticleSystem::RandomRange(const _float4 & vStandard, const _float4 & vRandomRange)
{
	_float4 vResult = vStandard;
	_float fRand = frandom(vRandomRange.x * -1.f, vRandomRange.x);

	vResult.x += fRand;
	vResult.y += fRand;
	vResult.z += fRand;
	vResult.w += frandom(vRandomRange.w * -1.f, vRandomRange.w);

	return vResult;
}

void CParticleSystem::Release()
{
	for (auto& tParticle : m_pParticles)
		delete tParticle.pParticle;

	m_pParticles.clear();

	for (auto& pParticle : m_pParticles_Collection)
		delete pParticle;

	m_pParticles_Collection.clear();

	if (m_pMesh)
		delete m_pMesh;

	if (m_pTexture)
		delete m_pTexture;

	m_pTexture = nullptr;
	m_pMesh = nullptr;

}
