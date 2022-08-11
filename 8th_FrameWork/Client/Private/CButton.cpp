#include "stdafx.h"
#include "CButton.h"

#include "CShader.h"
#include "Texture.h"
#include "Transform.h"

#include "GameInstance.h"
#include "GameObject.h"

#include "Functor.h"

CButton::CButton(_uint iIdx)
    : CComponent(iIdx)
{
}

CButton::~CButton()
{
    Release();
}

CButton* CButton::Create(COMPONENT_PIPELINE ePipeLine)
{
    CButton* pInstance = new CButton(ePipeLine);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CButton");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

void CButton::Set_ShaderResource(CShader* pShader, const char* pConstantName)
{
    _float fValue = 1.f;
    pShader->Set_RawValue(pConstantName, &fValue, sizeof(_float));
}

HRESULT CButton::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CButton::Initialize()
{

    return S_OK;
}

void CButton::Tick()
{
  /* 클릭시 등록되어 있는 델리게이트 실행 */
}

void CButton::Late_Tick()
{
    if (Is_MouseOn())
    {
        if (!m_bMouseOn)
        {
            m_bMouseOn = true;
            CallBack_MouseOn();
        }

        if (KEY(LBUTTON, TAP))
            CallBack_MouseDown();

    }
    else
        m_bMouseOn = false;
}

void CButton::Release()
{
}

void CButton::OnEnable()
{
    __super::OnEnable();
}

void CButton::OnDisable()
{
    __super::OnDisable();

    //REMOVE_SHADERRESOURCES(CButton, "g_fAlpha");
}

void CButton::OnDead()
{
}

_bool CButton::Is_MouseOn()
{
    _float4 vScale = m_pOwner->Get_Transform()->Get_Scale();
    _float4 vPos = m_pOwner->Get_Transform()->Get_World(WORLD_POS);

    vPos = CFunctor::RealToOrthoPosition(vPos);

	RECT    rc;
	::SetRect(&rc, (_int)(vPos.x - vScale.x * 0.5f),
		(_int)(vPos.y - vScale.y * 0.5f),
		(_int)(vPos.x + vScale.x * 0.5f),
		(_int)(vPos.y + vScale.y * 0.5f));

    POINT   ptMouse;
    ::GetCursorPos(&ptMouse);
    ::ScreenToClient(g_hWnd, &ptMouse);

    return ::PtInRect(&rc, ptMouse);
}
