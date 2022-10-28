#include "stdafx.h"
#include "CEnemyHPBar.h"

#include "GameInstance.h"

#include "CGameObject_Factory.h"
#include "Texture.h"

#include "CDefault_UI.h"

#include "Functor.h"
#include "CShader.h"
#include "CFader.h"
#include "CUnit_Player.h"
#include "CUser.h"

#include "Transform.h"



CEnemyHPBar::CEnemyHPBar()
{
}

CEnemyHPBar::~CEnemyHPBar()
{
}

CEnemyHPBar* CEnemyHPBar::Create(class CUnit* pFollowUnit)
{
	CEnemyHPBar* pInstance = new CEnemyHPBar;

	pInstance->m_pFollowUnit = pFollowUnit;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CEnemyHPBar");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CEnemyHPBar::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CEnemyHPBar::Initialize()
{
	vector<CDefault_UI*>	vecUIs = CGameObject_Factory::Create_vecUI_Reverse(L"InGame_Enemy");

	if (vecUIs.size() != UI_END)
		return E_FAIL;

	//이름순으로 숙숟 들어오게
	for (_uint i = 0; i < UI_END; ++i)
	{
		m_arrUI[i] = vecUIs[i];

	}

	m_vOriginBarScale = m_arrUI[HP_BAR]->Get_Transform()->Get_Scale();
	m_vOffsetPos = _float4( 0.f, 2.f, 0.f, 1.f );




	return S_OK;
}

HRESULT CEnemyHPBar::Start()
{
	for (_uint i = 0; i < UI_END; ++i)
	{
		CREATE_GAMEOBJECT(m_arrUI[i], GROUP_UI);
	}
	return S_OK;
}

void CEnemyHPBar::My_LateTick()
{
	if (!m_pFollowUnit->Is_Valid())
	{
		DISABLE_GAMEOBJECT(this);
		return;
	}

	//유닛의 투영위치 구하기
	_float4 vTargetWorldPos = m_pFollowUnit->Get_Transform()->Get_World(WORLD_POS);

	//절두체 해서 일단 거르기
	if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(vTargetWorldPos.XMLoad(), 0.f))
	{
		for (_uint i = 0; i < UI_END; ++i)
			DISABLE_GAMEOBJECT(m_arrUI[i]);
	}
	else
	{
		for (_uint i = 0; i < UI_END; ++i)
			ENABLE_GAMEOBJECT(m_arrUI[i]);
	}
	


	vTargetWorldPos += m_vOffsetPos;

	//ENABLE_COMPONENT(GET_COMPONENT(CRenderer));

	_float4x4 matVP = GAMEINSTANCE->Get_CurViewMatrix() * GAMEINSTANCE->Get_CurProjMatrix();

	vTargetWorldPos = vTargetWorldPos.MultiplyCoord(matVP);

	vTargetWorldPos.x *= g_iWinCX * 0.5f;
	vTargetWorldPos.y *= g_iWinCY * 0.5f;
	vTargetWorldPos.z = 0.1f;
	vTargetWorldPos.w = 1.f;
	m_arrUI[HP_LINE]->Get_Transform()->Set_World(WORLD_POS, vTargetWorldPos);
	m_arrUI[HP_LINE]->Get_Transform()->Make_WorldMatrix();

	//체력 구하기
	_float fMaxHp = m_pFollowUnit->Get_Status().fMaxHP;
	_float fCurHp = m_pFollowUnit->Get_Status().fHP;

	_float fRatio = fCurHp / fMaxHp;
	if (fRatio <= 0.f)
	{
		fRatio = 0.01f;
	}

	_float4 vNewScale = m_vOriginBarScale;
	vNewScale.x *= fRatio;

	_float fMovePos = (m_vOriginBarScale.x - vNewScale.x) * 0.41f;
	vTargetWorldPos.x -= fMovePos;

	m_arrUI[HP_BAR]->Get_Transform()->Set_World(WORLD_POS, vTargetWorldPos);
	m_arrUI[HP_BAR]->Get_Transform()->Set_Scale(vNewScale);
	m_arrUI[HP_BAR]->Get_Transform()->Make_WorldMatrix();


}

void CEnemyHPBar::OnEnable()
{
	__super::OnEnable();

	for (_uint i = 0; i < UI_END; ++i)
		ENABLE_GAMEOBJECT(m_arrUI[i]);

}

void CEnemyHPBar::OnDisable()
{
	__super::OnDisable();

	for (_uint i = 0; i < UI_END; ++i)
		DISABLE_GAMEOBJECT(m_arrUI[i]);
}