#include "stdafx.h"
#include "CFader.h"

#include "CShader.h"
#include "Texture.h"
#include "Transform.h"

#include "GameInstance.h"
#include "GameObject.h"

CFader::CFader(_uint iIdx)
	: CComponent(iIdx)
{
}

CFader::~CFader()
{
	Release();
}

CFader* CFader::Create(COMPONENT_PIPELINE ePipeLine, const FADEDESC& tFadeDesc)
{
	CFader* pInstance = new CFader(ePipeLine);

	pInstance->m_tFadeDesc = tFadeDesc;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CFader");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CFader::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
	_float fValue = 1.f;

	if (!Is_Disable())
		fValue = m_tFadeDesc.fAlpha;

	pShader->Set_RawValue(pConstantName, &fValue, sizeof(_float));
}

void CFader::Re_FadeIn()
{
	m_fTimeAcc = 0.f;
	m_tFadeDesc.fAlpha = 0.f;
	m_eState = FADEIN;

}

void CFader::Re_FadeOut()
{
	m_fTimeAcc = 0.f;
	m_tFadeDesc.fAlpha = 1.f;
	m_eState = FADEOUT;
}

HRESULT CFader::Initialize_Prototype()
{
	m_fOffsetX = 50.f;
	return S_OK;
}

HRESULT CFader::Initialize()
{
	return S_OK;
}

void CFader::Start()
{
	__super::Start();

	m_vTargetPos = m_pOwner->Get_Transform()->Get_MyWorld(WORLD_POS);
	m_vOriginScale = m_pOwner->Get_Transform()->Get_Scale();
	m_eState = FADEINREADY;
	m_tFadeDesc.fAlpha = 0.f;
}

void CFader::Tick()
{
	m_fTimeAcc += fDT * m_fFastTime;

	switch (m_eState)
	{
	case Client::CFader::FADEINREADY:
		FadeReady();
		break;

	case Client::CFader::FADEIN:
		FadeIn();
		break;

	case Client::CFader::FADEOUTREADY:
		FadeDelay();
		break;

	case Client::CFader::FADEOUT:
		FadeOut();
		break;

	default:
		break;
	}
}

void CFader::Late_Tick()
{
}

void CFader::Release()
{
}

void CFader::OnEnable()
{
	__super::OnEnable();
	//m_vTargetPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
	//m_vOriginScale = m_pOwner->Get_Transform()->Get_Scale();
	BIND_SHADERRESOURCES(CFader, "g_fAlpha");
}

void CFader::OnDisable()
{
	__super::OnDisable();

	//REMOVE_SHADERRESOURCES(CFader, "g_fAlpha");
}

void CFader::OnDead()
{
	REMOVE_SHADERRESOURCES(CFader, "g_fAlpha");
}

void CFader::FadeReady()
{
	if (m_tFadeDesc.bFadeInFlag == FADE_NONE)
	{
		return;
	}

	//Fade In 시간까지 대기하고 실행
	if (m_tFadeDesc.bFadeInFlag & FADE_TIME)
	{
		if (m_fTimeAcc < m_tFadeDesc.fFadeInStartTime)
			return;
		else
		{
			Change_State(FADEIN);
		}
	}

	if (m_tFadeDesc.bFadeInFlag & FADE_KEY)
	{
		if (m_bKeyInput)
		{
			if (m_fTimeAcc >= m_tFadeDesc.fFadeInStartTime)
			{
				m_bKeyInput = false;
				Change_State(FADEIN);
			}
		}
		else if (CGameInstance::Get_Instance()->Get_KeyState(m_tFadeDesc.eKeyType) == KEY_STATE::TAP)
		{
			m_bKeyInput = true;
			m_fTimeAcc = 0.f;
		}
	}


}

void CFader::FadeIn()
{
	if (m_tFadeDesc.fAlpha >= 1.f)
	{
		m_tFadeDesc.fAlpha = 1.f;
		Change_State(FADEOUTREADY);
		if (m_bFadeMessage)
		{
			m_pOwner->CallBack_FadeInEvent(m_tFadeDesc.eFadeOutType);
		}
	}
	else
	{
		OnFadeStyle(false);
	}
}

void CFader::FadeDelay()
{
	if (m_tFadeDesc.bFadeOutFlag == FADE_NONE)
	{
		return;
	}

	if (m_tFadeDesc.bFadeOutFlag & FADE_TIME)
	{
		if (m_fTimeAcc >= m_tFadeDesc.fFadeOutStartTime)
		{
			Change_State(FADEOUT);
		}
	}

	if (m_tFadeDesc.bFadeOutFlag & FADE_KEY)
	{
		if (m_bKeyInput)
		{
			if (m_fTimeAcc >= m_tFadeDesc.fFadeOutStartTime)
			{
				m_bKeyInput = false;
				Change_State(FADEOUT);
				m_vTargetPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);
				m_vOriginScale = m_pOwner->Get_Transform()->Get_Scale();
			}
		}
		else if (CGameInstance::Get_Instance()->Get_KeyState(m_tFadeDesc.eKeyType) == KEY_STATE::TAP)
		{
			m_bKeyInput = true;
			m_fTimeAcc = 0.f;
		}
	}
}

void CFader::FadeOut()
{
	if (m_tFadeDesc.fAlpha <= 0.f)
	{
		OnFadeOut_Finish();
	}
	else
	{
		OnFadeStyle(true);
	}
}

void CFader::OnFadeStyle(_bool bFadeOut)
{
	_float fRatio = 0.f, fSign = 1.f;

	if (bFadeOut)
	{
		fRatio = (1.f - (m_fTimeAcc / m_tFadeDesc.fFadeOutTime));
	}
	else
	{
		fRatio = m_fTimeAcc / m_tFadeDesc.fFadeInTime;
	}

	if (fRatio < 0.f)
	{
		fSign = -1.f;
	}

	if (bFadeOut)
	{
		fRatio = SATURATE(fRatio * fRatio);
	}
	else
	{
		fRatio = SATURATE(sqrtf(fRatio));
	}

	m_tFadeDesc.fAlpha = fRatio * fSign;

	switch (m_tFadeDesc.eFadeStyle)
	{
	case Client::FADEDESC::FADE_STYLE_DEFAULT:
	{
	}
	break;

	case Client::FADEDESC::FADE_STYLE_MOVE:
	{
		_float4 vPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);

		//비율은 1에서 시작해서 0이 되어야함
		fRatio = 1.f - fRatio;
		vPos.x = m_vTargetPos.x - (m_fOffsetX * fRatio);
		m_pOwner->Get_Transform()->Set_World(WORLD_POS, vPos);
	}
	break;

	case Client::FADEDESC::FADE_STYLE_SCALEUP:
	{
		//비율은 1에서 시작해서 0이 되어야함
		fRatio = 1.f - fRatio;

		//곱해주는놈이 2에서 1로 줄어야함
		_float4 vScale = m_vOriginScale * (m_fOffsetScale * fRatio + 1.f);
		m_pOwner->Get_Transform()->Set_Scale(vScale);
	}
	break;

	case Client::FADEDESC::FADE_STYLE_SCALEDOWN:
	{
		_float4 vScale = m_vOriginScale * (fRatio);
		m_pOwner->Get_Transform()->Set_Scale(vScale);
	}
	break;

	default:
		break;
	}
}

void CFader::Change_State(FADE_STATE eState)
{
	m_eState = eState;
	m_fTimeAcc = 0.f;

}

void CFader::OnFadeOut_Finish()
{

	if (m_bFadeMessage)
	{
		m_pOwner->CallBack_FadeOutEvent(m_tFadeDesc.eFadeOutType);
	}


	switch (m_tFadeDesc.eFadeOutType)
	{
	case FADEDESC::FADEOUT_DELETE:
		DELETE_GAMEOBJECT(m_pOwner);

		break;

	case FADEDESC::FADEOUT_DISABLE:
		DISABLE_GAMEOBJECT(m_pOwner);

		break;

	case FADEDESC::FADEOUT_NEXTTEXTURE:
	{
		CTexture* pTextureCom = GET_COMPONENT_FROM(m_pOwner, CTexture);
		if (pTextureCom->Next_Texture())
		{
			Change_State(FADEINREADY);
		}
		else
			DISABLE_GAMEOBJECT(m_pOwner);
	}
	break;

	case FADEDESC::FADEOUT_RANDOMTEXTURE:
	{
		CTexture* pTextureCom = GET_COMPONENT_FROM(m_pOwner, CTexture);
		pTextureCom->Random_Texture();
		Change_State(FADEINREADY);
	}
	break;

	case FADEDESC::FADEOUT_NONE:
		Change_State(FADEINREADY);
		break;

	default:
		break;
	}
}
