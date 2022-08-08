#include "stdafx.h"
#include "CFader.h"

#include "CShader.h"
#include "Texture.h"

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
    pShader->Set_RawValue(pConstantName, &m_tFadeDesc.fAlpha, sizeof(_float));
}

HRESULT CFader::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFader::Initialize()
{

    return S_OK;
}

void CFader::Tick()
{
    m_fTimeAcc += fDT;

    switch (m_eState)
    {
    case Client::CFader::FADEIN:
        FadeIn();
        break;

    case Client::CFader::FADEDELAY:
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

    BIND_SHADERRESOURCES(CFader, "g_fAlpha");
}

void CFader::OnDisable()
{
    __super::OnDisable();

    REMOVE_SHADERRESOURCES(CFader, "g_fAlpha");
}

void CFader::FadeIn()
{
    if (m_tFadeDesc.fAlpha >= 1.f)
    {
        m_tFadeDesc.fAlpha = 1.f;
        Change_State(FADEDELAY);
    }
    else
    {
        m_tFadeDesc.fAlpha = m_fTimeAcc / m_tFadeDesc.fFadeInTime;
    }
}

void CFader::FadeDelay()
{
    if (m_tFadeDesc.bFadeFlag & FADE_TIME)
    {
        if (m_fTimeAcc >= m_tFadeDesc.fFadeOutStartTime)
        {
            Change_State(FADEOUT);
        }
    }
    
    if (m_tFadeDesc.bFadeFlag & FADE_KEY)
    {
        if (CGameInstance::Get_Instance()->Get_KeyState(m_tFadeDesc.eKeyType) == KEY_STATE::TAP)
        {
            Change_State(FADEOUT);
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
        m_tFadeDesc.fAlpha = (1.f - (m_fTimeAcc / m_tFadeDesc.fFadeOutTime));
    }
}

void CFader::Change_State(FADE_STATE eState)
{
    m_eState = eState;
    m_fTimeAcc = 0.f;

}

void CFader::OnFadeOut_Finish()
{
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
        CTexture* pTextureCom = m_pOwner->Get_Component<CTexture>()[0];
        if (pTextureCom->Next_Texture())
        {
            Change_State(FADEIN);
        }
        else
            DELETE_GAMEOBJECT(m_pOwner);
    }
        break;

    case FADEDESC::FADEOUT_RANDOMTEXTURE:
    {
        CTexture* pTextureCom = m_pOwner->Get_Component<CTexture>()[0];
        pTextureCom->Random_Texture();
        Change_State(FADEIN);
    }
    break;




    default:
        break;
    }
}
