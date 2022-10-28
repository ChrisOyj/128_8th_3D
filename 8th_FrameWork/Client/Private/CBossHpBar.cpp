#include "stdafx.h"
#include "CBossHPBar.h"

#include "GameInstance.h"

#include "CGameObject_Factory.h"
#include "Texture.h"

#include "CDefault_UI.h"

#include "Functor.h"
#include "CShader.h"
#include "CFader.h"
#include "CUnit_Player.h"
#include "CUser.h"
#include "Renderer.h"

#include "Transform.h"



CBossHPBar::CBossHPBar()
{
}

CBossHPBar::~CBossHPBar()
{
}

CBossHPBar* CBossHPBar::Create(class CUnit* pFollowUnit)
{
	CBossHPBar* pInstance = new CBossHPBar;

	pInstance->m_pFollowUnit = pFollowUnit;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CBossHPBar");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CBossHPBar::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CBossHPBar::Initialize()
{
	vector<CDefault_UI*>	vecUIs = CGameObject_Factory::Create_vecUI_Reverse(L"InGame_BossHp");

	if (vecUIs.size() != UI_END)
		return E_FAIL;

	//이름순으로 숙숟 들어오게
	for (_uint i = 0; i < UI_END; ++i)
	{
		m_arrUI[i] = vecUIs[i];
	}

	m_vOriginBarScale = m_arrUI[HP_BAR]->Get_Transform()->Get_Scale();
	m_vOriginPos = m_arrUI[HP_BAR]->Get_Transform()->Get_MyWorld(WORLD_POS);



	return S_OK;
}

HRESULT CBossHPBar::Start()
{
	for (_uint i = 0; i < UI_END; ++i)
	{
		CREATE_GAMEOBJECT(m_arrUI[i], GROUP_UI);
	}
	return S_OK;
}

void CBossHPBar::My_LateTick()
{
	if (!m_pFollowUnit->Is_Valid())
	{
		DISABLE_GAMEOBJECT(this);
		return;
	}

	//체력 구하기
	_float fMaxHp = m_pFollowUnit->Get_Status().fMaxHP;
	_float fCurHp = m_pFollowUnit->Get_Status().fHP;

	_float fRatio = fCurHp / fMaxHp;
	if (fRatio <= 0.f)
	{
		fRatio = 0.f;
	}

	_float4 vTargetWorldPos = m_vOriginPos;
	_float4 vNewScale = m_vOriginBarScale;
	vNewScale.x *= fRatio;

	_float fMovePos = (m_vOriginBarScale.x - vNewScale.x) * 0.3f;
	vTargetWorldPos.x -= fMovePos;

	m_arrUI[HP_BAR]->Get_Transform()->Set_World(WORLD_POS, vTargetWorldPos);
	m_arrUI[HP_BAR]->Get_Transform()->Set_Scale(vNewScale);
	m_arrUI[HP_BAR]->Get_Transform()->Make_WorldMatrix();


}

void CBossHPBar::OnEnable()
{
	__super::OnEnable();

	for (_uint i = 0; i < UI_END; ++i)
		ENABLE_GAMEOBJECT(m_arrUI[i]);

}

void CBossHPBar::OnDisable()
{
	__super::OnDisable();

	for (_uint i = 0; i < UI_END; ++i)
		DISABLE_GAMEOBJECT(m_arrUI[i]);
}