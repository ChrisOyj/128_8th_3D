#include "stdafx.h"
#include "CTargetObject.h"

#include "GameInstance.h"

#include "CGameObject_Factory.h"
#include "Texture.h"

#include "CDefault_UI.h"

#include "Functor.h"
#include "Camera.h"
#include "CShader.h"
#include "CFader.h"
#include "CUnit_Player.h"
#include "CUser.h"

#include "Transform.h"



CTargetObject::CTargetObject()
{
}

CTargetObject::~CTargetObject()
{
}

CTargetObject* CTargetObject::Create(class CGameObject* pFollowUnit)
{
	CTargetObject* pInstance = new CTargetObject;

	pInstance->m_pFollowUnit = pFollowUnit;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Call_MsgBox(L"Failed to Initialize_Prototype : CTargetObject");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

void CTargetObject::On_TargetBoss(CGameObject* pBoss)
{
	ENABLE_GAMEOBJECT(this);
	m_pFollowUnit = pBoss;
	GET_COMPONENT_FROM(m_arrUI[MINIMAP_ICON], CTexture)->Set_CurTextureIndex(1);
	m_vOffsetPos = _float4(0.f, 2.5f, 0.f, 1.f);
	m_pTransform->Set_Scale(_float4(0.5f, 0.5f, 0.5f, 1.f));

}

void CTargetObject::On_TargetRunAwayBoss(CGameObject* pBoss)
{
	ENABLE_GAMEOBJECT(this);
	m_pFollowUnit = pBoss;
	m_vOffsetPos = _float4(0.f, 2.5f, 0.f, 1.f);
	GET_COMPONENT_FROM(m_arrUI[MINIMAP_ICON], CTexture)->Set_CurTextureIndex(0);


}

void CTargetObject::On_TargetGoal(CGameObject* pGoal)
{
	ENABLE_GAMEOBJECT(this);
	m_pFollowUnit = pGoal;
	GET_COMPONENT_FROM(m_arrUI[MINIMAP_ICON], CTexture)->Set_CurTextureIndex(0);

}

HRESULT CTargetObject::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CTargetObject::Initialize()
{
	vector<CDefault_UI*>	vecUIs = CGameObject_Factory::Create_vecUI_Reverse(L"InGame_MiniMap");

	if (vecUIs.size() != UI_END)
		return E_FAIL;

	//이름순으로 숙숟 들어오게
	for (_uint i = 0; i < UI_END; ++i)
	{
		m_arrUI[i] = vecUIs[i];

	}

	m_vOffsetPos = _float4(0.f, 2.f, 0.f, 1.f);




	return S_OK;
}

HRESULT CTargetObject::Start()
{
	for (_uint i = 0; i < UI_END; ++i)
	{
		CREATE_GAMEOBJECT(m_arrUI[i], GROUP_UI);
	}
	return S_OK;
}

void CTargetObject::My_LateTick()
{
	if (!m_pFollowUnit->Is_Valid())
	{
		DISABLE_GAMEOBJECT(this);
		return;
	}

	//유닛의 투영위치 구하기
	_float4 vTargetWorldPos = m_pFollowUnit->Get_Transform()->Get_World(WORLD_POS);

	//카메라 뒤에 있으면 안그리기
	_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
	_float4 vTargetWorldDir = vTargetWorldPos - GAMEINSTANCE->Get_ViewPos();
	

	if (vCamLook.Dot(vTargetWorldDir) < 0.f)
	{
		for (_uint i = 0; i < UI_END; ++i)
			if (!m_arrUI[i]->Is_Disable())
			DISABLE_GAMEOBJECT(m_arrUI[i]);

		return;
	}
	else
	{
		for (_uint i = 0; i < UI_END; ++i)
			if (m_arrUI[i]->Is_Disable())
				ENABLE_GAMEOBJECT(m_arrUI[i]);
	}



	vTargetWorldPos += m_vOffsetPos;

	_float4x4 matVP = GAMEINSTANCE->Get_CurViewMatrix() * GAMEINSTANCE->Get_CurProjMatrix();

	vTargetWorldPos = vTargetWorldPos.MultiplyCoord(matVP);

	vTargetWorldPos.x *= g_iWinCX * 0.5f;
	vTargetWorldPos.y *= g_iWinCY * 0.5f;
	vTargetWorldPos.z = 0.1f;
	vTargetWorldPos.w = 1.f;

	_float fRange = 35.f;
	if (vTargetWorldPos.x < g_iWinCX * -0.5f + fRange)
	{
		vTargetWorldPos.x = g_iWinCX * -0.5f + fRange;
	}
	else if (vTargetWorldPos.x > g_iWinCX * 0.5f - fRange)
	{
		vTargetWorldPos.x = g_iWinCX * 0.5f - fRange;
	}

	if (vTargetWorldPos.y < g_iWinCY * -0.5f + fRange)
	{
		vTargetWorldPos.y = g_iWinCY * -0.5f + fRange;
	}
	else if (vTargetWorldPos.y > g_iWinCY * 0.5f - fRange)
	{
		vTargetWorldPos.y = g_iWinCY * 0.5f - fRange;
	}


	m_arrUI[MINIMAP_ICON]->Get_Transform()->Set_World(WORLD_POS, vTargetWorldPos);
	m_arrUI[MINIMAP_ICON]->Get_Transform()->Make_WorldMatrix();
	m_arrUI[MINIMAP_EFFECT]->Get_Transform()->Set_World(WORLD_POS, vTargetWorldPos);
	m_arrUI[MINIMAP_EFFECT]->Get_Transform()->Make_WorldMatrix();



}

void CTargetObject::OnEnable()
{
	__super::OnEnable();

	for (_uint i = 0; i < UI_END; ++i)
		ENABLE_GAMEOBJECT(m_arrUI[i]);

}

void CTargetObject::OnDisable()
{
	__super::OnDisable();

	for (_uint i = 0; i < UI_END; ++i)
		DISABLE_GAMEOBJECT(m_arrUI[i]);
}