#include "stdafx.h"
#include "CWindow_Effect.h"

#include "GameInstance.h"

#include "Transform.h"
#include "Physics.h"
#include "Texture.h"
#include "CShader.h"
#include "CFader.h"
#include "Renderer.h"
#include "Model.h"
#include "MeshContainer.h"

#include "Functor.h"

#include "CGameObject_Factory.h"
#include "CComponent_Factory.h"
#include "CPrototype_Factory.h"

#include "CUtility_Json.h"

#include "CMap.h"
#include "CGround.h"
#include "CMap_Sample.h"
#include "CObject_Map.h"

#include "CUtility_File.h"
#include "CUtility_Transform.h"
#include "Camera.h"

#include "CUnit.h"
#include "CEffect.h"
#include "CDefault_Effect.h"
#include "CScript_FollowCam.h"
#include "CCamera_Cinema.h"

#include "CState_Manager.h"
#include "CState_Boss.h"

#include "CUnit_Player.h"
#include "CUser.h"

#include "CRectEffects.h"

CWindow_Effect::CWindow_Effect()
{
}

CWindow_Effect::~CWindow_Effect()
{
}

CWindow_Effect* CWindow_Effect::Create()
{
	CWindow_Effect* pInstance = new CWindow_Effect;

	if (FAILED(pInstance->Initialize()))
	{
		Call_MsgBox(L"Failed to Initialize : CWindow_Effect");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CWindow_Effect::Initialize()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	m_bEnable = false;
	SetUp_ImGuiDESC(typeid(CWindow_Effect).name(), ImVec2(400.f, 600.f), window_flags);

	m_MeshRootNode.strFolderPath = "../bin/resources/meshes";
	m_MeshRootNode.strFileName = "Effects";
	m_MeshRootNode.strFullPath = "../bin/resources/meshes/effects";
	Read_Folder("../bin/resources/meshes/effects", m_MeshRootNode);

	m_TextureRootNode.strFolderPath = "../bin/resources/textures";
	m_TextureRootNode.strFileName = "Effects";
	m_TextureRootNode.strFullPath = "../bin/resources/textures/effects";
	Read_Folder("../bin/resources/textures/effects", m_TextureRootNode);

	m_EffectRootNode.strFolderPath = "../bin";
	m_EffectRootNode.strFileName = "Effects";
	m_EffectRootNode.strFullPath = "../bin/Effects/";
	Read_Folder("../bin/effects", m_EffectRootNode);

	return S_OK;
}

void CWindow_Effect::Tick()
{
}

HRESULT CWindow_Effect::Render()
{
	if (FAILED(__super::Begin()))
		return E_FAIL;

	if (ImGui::CollapsingHeader("CONTROL SCENE"))
	{
		if (ImGui::Button("DISABLE_ENEMIES(8)") || (KEY(NUM8, TAP)))
		{
			for (auto& elem : GAMEINSTANCE->Get_ObjGroup(GROUP_ENEMY))
			{
				if (elem->Is_Disable())
					ENABLE_GAMEOBJECT(elem);
				else
					DISABLE_GAMEOBJECT(elem);
			}
		}

#ifdef _DEBUG
		ImGui::SameLine();

		if (ImGui::Button("STOP_PLAYER(9)") || (KEY(NUM9, TAP)))
		{
			PLAYER->Switch_Controlable();
		}

#endif // _DEBUG
	}

	Show_MainList();

	if (m_iCurrentIdx != 9999)
	{
		if (m_vecEffects[m_iCurrentIdx].iEffectType == 0)
			Show_EffectTab();
		else
			Show_ParticleTab();
	}





	__super::End();

	return S_OK;
}

void CWindow_Effect::Show_MainList()
{
	//이펙트들 리스트박스
	if (ImGui::Button("Save_Effects"))
	{
		Save_CurEffect();
	}
	static _uint g_iEffectIndex = 0;
	if (ImGui::Button("Create Mesh Effect"))
	{
		CDefault_Effect* pEffect = CDefault_Effect::Create();

		EFFECT_ITEM tItem;
		tItem.bSelected = false;
		tItem.pEffect = pEffect;
		tItem.strName = "Default_Effect_";
		tItem.strName += to_string(g_iEffectIndex++);
		tItem.iEffectType = 0;

		m_vecEffects.push_back(tItem);

		CREATE_GAMEOBJECT(pEffect, GROUP_EFFECT);
	}

	ImGui::SameLine();

	if (ImGui::Button("Create Particle Effect"))
	{
		CRectEffects* pEffect = CRectEffects::Create(PLAYER->Get_Transform()->Get_World(WORLD_POS));

		EFFECT_ITEM tItem;
		tItem.bSelected = false;
		tItem.pEffect = pEffect;
		tItem.strName = "Default_Particle_";
		tItem.strName += to_string(g_iEffectIndex++);
		tItem.iEffectType = 1;
		m_vecEffects.push_back(tItem);

		CREATE_GAMEOBJECT(pEffect, GROUP_EFFECT);
	}

	static _bool	g_bCamMatrix = false;
	if (ImGui::RadioButton("CameraMatrix", g_bCamMatrix))
		g_bCamMatrix = !g_bCamMatrix;

	ImGui::SameLine();

	if (ImGui::RadioButton("PlayerMatrix", !g_bCamMatrix))
		g_bCamMatrix = !g_bCamMatrix;

	ImGui::SameLine();
	static _bool g_bEnableAll = false;
	if (ImGui::RadioButton("ENABLE_ALL", g_bEnableAll))
		g_bEnableAll = !g_bEnableAll;

	if (ImGui::Button("EN / DIS ABLE_EFFECT(7)") || KEY(NUM7, TAP))
	{
		if (g_bEnableAll)
		{
			for (auto& elem : m_vecEffects)
			{

				if (elem.iEffectType == 0)
				{
					elem.pEffect->Reset(PLAYER->Get_Transform()->Get_World(WORLD_POS));
				}
				else
				{
					_float4x4 matTrans;
					if (g_bCamMatrix)
						matTrans = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS | MATRIX_NOSCALE);
					else
						matTrans = PLAYER->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS | MATRIX_NOSCALE);

					elem.pEffect->Reset(PLAYER->Get_Transform()->Get_World(WORLD_POS), matTrans);
				}

			}
		}
		else
		{
			if (m_iCurrentIdx != 9999)
			{
				if (m_vecEffects[m_iCurrentIdx].pEffect->Is_Disable())
				{
					if (m_vecEffects[m_iCurrentIdx].iEffectType == 0)
					{
						m_vecEffects[m_iCurrentIdx].pEffect->Reset(PLAYER->Get_Transform()->Get_World(WORLD_POS));
					}
					else
					{
						_float4x4 matTrans;
						if (g_bCamMatrix)
							matTrans = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS | MATRIX_NOSCALE);
						else
							matTrans = PLAYER->Get_Transform()->Get_WorldMatrix(MARTIX_NOTRANS | MATRIX_NOSCALE);

						m_vecEffects[m_iCurrentIdx].pEffect->Reset(PLAYER->Get_Transform()->Get_World(WORLD_POS), matTrans);
					}



				}
				else
					DISABLE_GAMEOBJECT(m_vecEffects[m_iCurrentIdx].pEffect);

			}
		}

		
	}

	if (ImGui::Button("DELETE_ALL"))
	{
		m_vecEffects.clear();
		m_iCurrentIdx = 9999;
	}

	ImGui::SameLine();

	if (ImGui::Button("RE-Initialize"))
	{
		if (m_iCurrentIdx != 9999)
		{
			m_vecEffects[m_iCurrentIdx].pEffect->Re_Initialize();
		}
	}


	if (ImGui::BeginListBox("CurrentEffects_List", ImVec2(360.f, 300.f)))
	{
		for (_uint i = 0; i < m_vecEffects.size(); ++i)
		{
			if (ImGui::Selectable(m_vecEffects[i].strName.c_str(), m_vecEffects[i].bSelected))
			{
				for (_uint j = 0; j < m_vecEffects.size(); ++j)
					m_vecEffects[j].bSelected = false;

				m_iCurrentIdx = i;
				m_vecEffects[i].bSelected = true;
			}

			if (m_vecEffects[i].bSelected)
			{
				ImGui::SetItemDefaultFocus();
			}

			if (m_vecEffects[i].pEffect->Is_Disable())
			{
				ImGui::SameLine(0.f, 70.f);
				ImGui::Text("disable");
			}
		}

		ImGui::EndListBox();
	}
	if (ImGui::CollapsingHeader(" - BINARY FILE - "))
	{
		if (ImGui::Button("Load_Effects"))
		{
			if (!m_CurSelectedEffectFileKey.empty())
			{
				Load_SelectedEffect();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Refresh"))
		{
			Read_Folder("../bin/effects", m_EffectRootNode);
		}

		if (ImGui::BeginListBox("Bin_Files_List", ImVec2(360.f, 300.f)))
		{
			Show_TreeData_Effect(m_EffectRootNode);

			ImGui::EndListBox();
		}
	}

}

void CWindow_Effect::Show_EffectTab()
{
	/* Effect Info */
	if (m_iCurrentIdx != 9999)
	{
		CEffect* pCurEffect = m_vecEffects[m_iCurrentIdx].pEffect;
		ImGui::Text("Selected Effect Name : ");
		ImGui::SameLine();
		if (ImGui::InputText("EFFECT_NAME", m_szCurEffectName, sizeof(m_szCurEffectName), ImGuiInputTextFlags_EnterReturnsTrue))
		{ 
			m_vecEffects[m_iCurrentIdx].strName = m_szCurEffectName;
		}

		if (ImGui::CollapsingHeader(" - MESH - "))
		{
			if (ImGui::Button("Change_Mesh(C)") || KEY(C, TAP))
			{
				if (!m_CurSelectedMeshFilePath.empty())
				{
					CModel* pModelCom = GET_COMPONENT_FROM(pCurEffect, CModel);
					Change_Model(pModelCom);

				}
			}

			if (ImGui::BeginListBox("FBX_Files_List", ImVec2(360.f, 300.f)))
			{
				Show_TreeData(m_MeshRootNode);

				ImGui::EndListBox();
			}


		}
		if (ImGui::CollapsingHeader(" - TEXTURE - "))
		{
			if (ImGui::Button("Bind_MaskMap(V)") || KEY(V,TAP))
			{
				if (!m_CurSelectedTextureFilePath.empty())
				{
					pCurEffect->m_wstrMaskMapPath = CFunctor::To_Wstring(m_CurSelectedTextureFilePath);
					CModel* pModelCom = GET_COMPONENT_FROM(pCurEffect, CModel);
					pModelCom->Get_Materials().front().second.pTextures[1]->Add_Texture(
						CFunctor::To_Wstring(m_CurSelectedTextureFilePath).c_str());
					pModelCom->Get_Materials().front().second.pTextures[1]->Set_CurTextureIndex(

						pModelCom->Get_Materials().front().second.pTextures[1]->Get_TextureSize() - 1
					);
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Bind_ColorMap(B)") || KEY(B, TAP))
			{
				if (!m_CurSelectedTextureFilePath.empty())
				{
					pCurEffect->m_wstrColorMapPath = CFunctor::To_Wstring(m_CurSelectedTextureFilePath);
					CModel* pModelCom = GET_COMPONENT_FROM(pCurEffect, CModel);
					CTexture* pTexture = pModelCom->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS];

					pTexture->Add_Texture(	CFunctor::To_Wstring(m_CurSelectedTextureFilePath).c_str());

					pTexture->Set_CurTextureIndex(pTexture->Get_TextureSize() - 1);
				}
			}

			if (ImGui::BeginListBox("Texture_Files_List", ImVec2(360.f, 300.f)))
			{
				Show_TreeData_Texture(m_TextureRootNode);

				ImGui::EndListBox();
			}
		}

		if (ImGui::CollapsingHeader(" - SHADER PASS -"))
		{
			static _bool	bSelect[VTXEFFECT_PASS_END] = {};
			memset(bSelect, 0, sizeof(_bool)* VTXEFFECT_PASS_END);
			bSelect[pCurEffect->m_iPassType] = true;
			CModel* pModelCom = GET_COMPONENT_FROM(pCurEffect, CModel);

			if (ImGui::Selectable("DEFAULT", &bSelect[VTXEFFECT_PASS_DEFAULT]))
				pCurEffect->m_iPassType = VTXEFFECT_PASS_DEFAULT;
			if (ImGui::Selectable("LINEX", &bSelect[VTXEFFECT_PASS_LINEX]))
				pCurEffect->m_iPassType = VTXEFFECT_PASS_LINEX;
			if (ImGui::Selectable("LINEY", &bSelect[VTXEFFECT_PASS_LINEY]))
				pCurEffect->m_iPassType = VTXEFFECT_PASS_LINEY;
			if (ImGui::Selectable("TEXT", &bSelect[VTXEFFECT_PASS_TEXT]))
				pCurEffect->m_iPassType = VTXEFFECT_PASS_TEXT;
			if (ImGui::Selectable("CARTOON", &bSelect[VTXEFFECT_PASS_CARTOON]))
				pCurEffect->m_iPassType = VTXEFFECT_PASS_CARTOON;
			if (ImGui::Selectable("DISTORTION", &bSelect[VTXEFFECT_PASS_DISTORTION]))
			{
				pCurEffect->m_iPassType = VTXEFFECT_PASS_DISTORTION;
				GET_COMPONENT_FROM(pCurEffect, CRenderer)->Set_RenderGroup(RENDER_DISTORTION);

			}
			if (ImGui::Selectable("NOISE", &bSelect[VTXEFFECT_PASS_NOISE]))
				pCurEffect->m_iPassType = VTXEFFECT_PASS_NOISE;

			pModelCom->Set_ShaderPassToAll(pCurEffect->m_iPassType);

		}
		if (ImGui::CollapsingHeader(" - TRANSLATION MATRIX - "))
		{
			static _float	fTurnDegrees[3] = {};
			if (ImGui::InputFloat3("Turn Degrees", fTurnDegrees, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				pCurEffect->m_matTrans = XMMatrixScaling(0.01f, 0.01f, 0.01f) 
					* XMMatrixRotationX(XMConvertToRadians(fTurnDegrees[0]))
					* XMMatrixRotationY(XMConvertToRadians(fTurnDegrees[1]))
					* XMMatrixRotationZ(XMConvertToRadians(fTurnDegrees[2]));

				//모델 다시 로드
				CModel* pModelCom = GET_COMPONENT_FROM(pCurEffect, CModel);
				Change_Model(pModelCom, pCurEffect->m_matTrans);
			}
		}
		if (ImGui::CollapsingHeader(" - EFFECT TYPE FLAG -"))
		{
			const static _uint iFlagCnt = 4;
			_bool	bFlagSelect[iFlagCnt] = {};

			if (pCurEffect->m_bEffectFlag & EFFECT_FOLLOWTARGET)
				bFlagSelect[0] = true;
			if (pCurEffect->m_bEffectFlag & EFFECT_TARGETPOS)
				bFlagSelect[1] = true;
			if (pCurEffect->m_bEffectFlag & EFFECT_COLLIDER)
				bFlagSelect[2] = true;
			if (pCurEffect->m_bEffectFlag & EFFECT_BILLBOARD)
				bFlagSelect[3] = true;

			if (ImGui::Selectable("FOLLOW_TARGET", &bFlagSelect[0]))
			{
				if (pCurEffect->m_bEffectFlag & EFFECT_FOLLOWTARGET)
				{
					pCurEffect->m_bEffectFlag &= ~EFFECT_FOLLOWTARGET;
				}
				else
				{
					pCurEffect->m_bEffectFlag |= EFFECT_FOLLOWTARGET;
				}
			}

			if (ImGui::Selectable("GO_TARGETPOS", &bFlagSelect[1]))
			{
				if (pCurEffect->m_bEffectFlag & EFFECT_TARGETPOS)
				{
					pCurEffect->m_bEffectFlag &= ~EFFECT_TARGETPOS;
				}
				else
				{
					pCurEffect->m_bEffectFlag |= EFFECT_TARGETPOS;
				}
			}

			if (ImGui::Selectable("COLLIDER", &bFlagSelect[2]))
			{
				if (pCurEffect->m_bEffectFlag & EFFECT_COLLIDER)
				{
					pCurEffect->m_bEffectFlag &= ~EFFECT_COLLIDER;
				}
				else
				{
					pCurEffect->m_bEffectFlag |= EFFECT_COLLIDER;
				}
			}

			if (ImGui::Selectable("BILLBOARD", &bFlagSelect[3]))
			{
				if (pCurEffect->m_bEffectFlag & EFFECT_BILLBOARD)
				{
					pCurEffect->m_bEffectFlag &= ~EFFECT_BILLBOARD;
				}
				else
				{
					pCurEffect->m_bEffectFlag |= EFFECT_BILLBOARD;
				}
			}

		}
		if (ImGui::CollapsingHeader(" - EFFECT DISALBE TYPE -"))
		{
			static _bool	bSelect[CEffect::DISABLE_END] = {};

			if (ImGui::Selectable("FADE", &bSelect[CEffect::FADE]))
			{
				pCurEffect->m_eDisableType = CEffect::FADE;
			}
			if (ImGui::Selectable("UV", &bSelect[CEffect::UV]))
			{
				pCurEffect->m_eDisableType = CEffect::UV;
			}
			if (ImGui::Selectable("WALL", &bSelect[CEffect::WALL]))
			{
				pCurEffect->m_eDisableType = CEffect::WALL;
			}
			if (ImGui::Selectable("NONE", &bSelect[CEffect::NONE]))
			{
				pCurEffect->m_eDisableType = CEffect::NONE;
			}

			memset(bSelect, 0, sizeof(_bool) * CEffect::DISABLE_END);
			bSelect[pCurEffect->m_eDisableType] = true;
		}
		if (ImGui::CollapsingHeader(" - EFFECT SHADER FLAG -"))
		{
			const static _uint iShFlagCnt = 3;
			_bool	bShFlagSelect[iShFlagCnt] = {};

			if (pCurEffect->m_vEffectFlag.x > 0.99f)
			{
				bShFlagSelect[0] = true;
			}
			else
			{
				bShFlagSelect[0] = false;
			}

			if (pCurEffect->m_vEffectFlag.y > 0.99f)
			{
				bShFlagSelect[1] = true;
			}
			else
			{
				bShFlagSelect[1] = false;
			}

			if (pCurEffect->m_vEffectFlag.w < 1.f)
			{
				bShFlagSelect[2] = false;
			}
			else
			{
				bShFlagSelect[2] = true;
			}

			/*if (pCurEffect->m_vEffectFlag.z > 0.99f)
			{
				bShFlagSelect[2] = true;
			}
			else
			{
				bShFlagSelect[2] = false;
			}*/

			if (ImGui::Selectable("GLOW", &bShFlagSelect[0]))
			{
				if (pCurEffect->m_vEffectFlag.x > 0.99f)
				{
					pCurEffect->m_vEffectFlag.x = 0.95f;
				}
				else
				{
					pCurEffect->m_vEffectFlag.x = 1.f;
				}

				GET_COMPONENT_FROM(pCurEffect, CModel)->Set_ShaderFlag(pCurEffect->m_vEffectFlag);
			}

			if (ImGui::Selectable("BLOOM", &bShFlagSelect[1]))
			{
				if (pCurEffect->m_vEffectFlag.y > 0.99f)
				{
					pCurEffect->m_vEffectFlag.y = 0.95f;
				}
				else
				{
					pCurEffect->m_vEffectFlag.y = 1.f;
				}

				GET_COMPONENT_FROM(pCurEffect, CModel)->Set_ShaderFlag(pCurEffect->m_vEffectFlag);

			}

			if (ImGui::Selectable("BLUR", &bShFlagSelect[2]))
			{
				if (pCurEffect->m_vEffectFlag.w > 0.99f)
				{
					pCurEffect->m_vEffectFlag.w = 0.95f;
				}
				else
				{
					pCurEffect->m_vEffectFlag.w = 1.f;
				}

				GET_COMPONENT_FROM(pCurEffect, CModel)->Set_ShaderFlag(pCurEffect->m_vEffectFlag);

			}

		/*	if (ImGui::Selectable("DISTORTION", &bShFlagSelect[2]))
			{
				if (pCurEffect->m_vEffectFlag.z > 0.99f)
				{
					pCurEffect->m_vEffectFlag.z = 0.95f;
				}
				else
				{
					pCurEffect->m_vEffectFlag.z = 1.f;
				}

				GET_COMPONENT_FROM(pCurEffect, CModel)->Set_ShaderFlag(pCurEffect->m_vEffectFlag);

			}*/

		}
		if (ImGui::CollapsingHeader(" - GLOW VECTOR "))
		{
			_float vGlowFlags[4] = { pCurEffect->m_vGlowFlag.x, pCurEffect->m_vGlowFlag.y, pCurEffect->m_vGlowFlag.z, pCurEffect->m_vGlowFlag .w};
			memcpy(vGlowFlags, &pCurEffect->m_vGlowFlag, sizeof(_float4));

			if (ImGui::InputFloat4("vGlowFlag", vGlowFlags, "%.2f"))
			{
				memcpy(&pCurEffect->m_vGlowFlag, vGlowFlags, sizeof(_float4));
			}
		}
		if (ImGui::CollapsingHeader(" - VARIABLES "))
		{
			_float	vUVSpeed[2] = { pCurEffect->m_fUVSpeedX , pCurEffect->m_fUVSpeedY};

			if (ImGui::InputFloat2("UV_SPEED XY", vUVSpeed, "%.2f"))
			{
				pCurEffect->m_fUVSpeedX = vUVSpeed[0];
				pCurEffect->m_fUVSpeedY = vUVSpeed[1];
			}

			if (ImGui::InputFloat("TURN_SPEED", &pCurEffect->m_fTurnSpeed, 0.1f))
			{
			}

			_float	vOffsetPos[3] = { pCurEffect->m_vOffsetPos.x, pCurEffect->m_vOffsetPos.y, pCurEffect->m_vOffsetPos.z};
			if (ImGui::InputFloat3("OFFSET POS", vOffsetPos, "%.2f"))
			{
				pCurEffect->m_vOffsetPos = _float4(vOffsetPos[0], vOffsetPos[1], vOffsetPos[2], 1.f);
			}

			_float	fTurnDir[3] = { pCurEffect->m_vTurnDir .x, pCurEffect->m_vTurnDir .y, pCurEffect->m_vTurnDir .z};
			if (ImGui::InputFloat3("Turn Direction", fTurnDir, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				pCurEffect->m_vTurnDir = _float4(fTurnDir[0], fTurnDir[1], fTurnDir[2], 0.f);
				pCurEffect->m_vTurnDir.Normalize();
			}

			/* FADE */
			if (ImGui::InputFloat("FadeIn StartTime", &pCurEffect->m_fFadeInStartTime, 0.1f))
			{
			}
			if (ImGui::InputFloat("FadeIn Time", &pCurEffect->m_fFadeInTime, 0.1f))
			{
			}
			if (ImGui::InputFloat("FadeOut StartTime", &pCurEffect->m_fFadeOutStartTime, 0.1f))
			{
			}
			if (ImGui::InputFloat("FadeOut Time", &pCurEffect->m_fFadeOutTime, 0.1f))
			{
			}
			if (ImGui::InputFloat("Target Alpha", &pCurEffect->m_fTargetAlpha, 0.1f))
			{
			}

			/* Scale*/
			_float	vStartScale[3] = { pCurEffect->m_vStartScale.x , pCurEffect->m_vStartScale.y, pCurEffect->m_vStartScale.z};
			_float	vFadeInTargetScale[3] = { pCurEffect->m_vFadeInTargetScale. x, pCurEffect->m_vFadeInTargetScale. y, pCurEffect->m_vFadeInTargetScale. z};
			_float	vFadeOutTargetScale[3] = { pCurEffect->m_vFadeOutTargetScale.x, pCurEffect->m_vFadeOutTargetScale.y, pCurEffect->m_vFadeOutTargetScale.z};

			if (ImGui::InputFloat3("vStartScale", vStartScale, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				pCurEffect->m_vStartScale.x = vStartScale[0];
				pCurEffect->m_vStartScale.y = vStartScale[1];
				pCurEffect->m_vStartScale.z = vStartScale[2];
			}

			if (ImGui::InputFloat3("vFadeInTargetScale", vFadeInTargetScale, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				pCurEffect->m_vFadeInTargetScale.x = vFadeInTargetScale[0];
				pCurEffect->m_vFadeInTargetScale.y = vFadeInTargetScale[1];
				pCurEffect->m_vFadeInTargetScale.z = vFadeInTargetScale[2];
			}

			if (ImGui::InputFloat3("vFadeOutTargetScale", vFadeOutTargetScale, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				pCurEffect->m_vFadeOutTargetScale.x = vFadeOutTargetScale[0];
				pCurEffect->m_vFadeOutTargetScale.y = vFadeOutTargetScale[1];
				pCurEffect->m_vFadeOutTargetScale.z = vFadeOutTargetScale[2];
			}


		}


	}


}

void CWindow_Effect::Show_ParticleTab()
{
	/* Effect Info */
	if (m_iCurrentIdx != 9999)
	{
		CEffect* pCurEffect = m_vecEffects[m_iCurrentIdx].pEffect;
		ImGui::Text("Selected Effect Name : ");
		ImGui::SameLine();
		if (ImGui::InputText("EFFECT_NAME", m_szCurEffectName, sizeof(m_szCurEffectName), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			m_vecEffects[m_iCurrentIdx].strName = m_szCurEffectName;
		}

		if (ImGui::CollapsingHeader(" - TEXTURES - "))
		{
			if (ImGui::Button("-Bind_MaskMap"))
			{
				if (!m_CurSelectedTextureFilePath.empty())
				{
					pCurEffect->m_wstrMaskMapPath = CFunctor::To_Wstring(m_CurSelectedTextureFilePath);


					if (pCurEffect->Get_Component<CTexture>().empty())
					{
						CTexture* pTexture = CTexture::Create(0, CFunctor::To_Wstring(m_CurSelectedTextureFilePath).c_str(), 1);
					}
					else
					{
						CTexture* pTexture = GET_COMPONENT_FROM(pCurEffect, CTexture);
						pTexture->Add_Texture(CFunctor::To_Wstring(m_CurSelectedTextureFilePath).c_str());
						pTexture->Set_CurTextureIndex(pTexture->Get_TextureSize() - 1);
					}
					
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("-Bind_ColorMap"))
			{
				if (!m_CurSelectedTextureFilePath.empty())
				{
					pCurEffect->m_wstrColorMapPath = CFunctor::To_Wstring(m_CurSelectedTextureFilePath);

					if (pCurEffect->Get_Component<CTexture>().empty())
					{
						CTexture* pTexture = CTexture::Create(0, CFunctor::To_Wstring(m_CurSelectedTextureFilePath).c_str(), 1);
					}
					else
					{
						list<CComponent*> pList = pCurEffect->Get_Component<CTexture>();
						auto iter = pList.begin();
						++iter;

						CTexture* pTexture = static_cast<CTexture*>(*iter);
						pTexture->Add_Texture(CFunctor::To_Wstring(m_CurSelectedTextureFilePath).c_str());
						pTexture->Set_CurTextureIndex(pTexture->Get_TextureSize() - 1);
					}
				}
				
			}

			ImGui::SameLine();

			if (ImGui::Button("-Bind_DissolveMap"))
			{
				if (!m_CurSelectedTextureFilePath.empty())
				{
					pCurEffect->m_wstrNormalMapPath = CFunctor::To_Wstring(m_CurSelectedTextureFilePath);

					if (pCurEffect->Get_Component<CTexture>().empty())
					{
						CTexture* pTexture = CTexture::Create(0, CFunctor::To_Wstring(m_CurSelectedTextureFilePath).c_str(), 1);
					}
					else
					{
						CTexture* pTexture = static_cast<CTexture*>(pCurEffect->Get_Component<CTexture>().back());
						pTexture->Add_Texture(CFunctor::To_Wstring(m_CurSelectedTextureFilePath).c_str());
						pTexture->Set_CurTextureIndex(pTexture->Get_TextureSize() - 1);
					}
				}

			}

			if (ImGui::BeginListBox("-Texture_Files_List", ImVec2(360.f, 300.f)))
			{
				Show_TreeData_Texture(m_TextureRootNode);

				ImGui::EndListBox();
			}
		}

		if (ImGui::CollapsingHeader("- SHADER PASS -"))
		{
			static _bool	bSelect[VTXRECTINSTANCE_PASS_END] = {};
			memset(bSelect, 0, sizeof(_bool) * VTXRECTINSTANCE_PASS_END);
			bSelect[pCurEffect->m_iPassType] = true;

			CRenderer* pRenderer = GET_COMPONENT_FROM(pCurEffect, CRenderer);

			if (ImGui::Selectable("DEFAULT", &bSelect[VTXRECTINSTANCE_PASS_DEFAULT]))
				pCurEffect->m_iPassType = VTXRECTINSTANCE_PASS_DEFAULT;
			if (ImGui::Selectable("COLORMAP", &bSelect[VTXRECTINSTANCE_PASS_COLORMAP]))
				pCurEffect->m_iPassType = VTXRECTINSTANCE_PASS_COLORMAP;
			if (ImGui::Selectable("DEBUG_TEST", &bSelect[VTXRECTINSTANCE_PASS_DEBUG]))
				pCurEffect->m_iPassType = VTXRECTINSTANCE_PASS_DEBUG;
			if (ImGui::Selectable("ANIMATION", &bSelect[VTXRECTINSTANCE_PASS_ANIMATION]))
				pCurEffect->m_iPassType = VTXRECTINSTANCE_PASS_ANIMATION;
			if (ImGui::Selectable("ANIMATION_ALPHA", &bSelect[VTXRECTINSTANCE_PASS_ANIMATIONALPHA]))
				pCurEffect->m_iPassType = VTXRECTINSTANCE_PASS_ANIMATIONALPHA;

			pRenderer->Set_Pass(pCurEffect->m_iPassType);

		}
		if (ImGui::CollapsingHeader("- EFFECT SHADER FLAG -"))
		{
			const static _uint iShFlagCnt = 3;
			_bool	bShFlagSelect[iShFlagCnt] = {};

			if (pCurEffect->m_vEffectFlag.x > 0.99f)
			{
				bShFlagSelect[0] = true;
			}
			else
			{
				bShFlagSelect[0] = false;
			}

			if (pCurEffect->m_vEffectFlag.y > 0.99f)
			{
				bShFlagSelect[1] = true;
			}
			else
			{
				bShFlagSelect[1] = false;
			}
			if (pCurEffect->m_vEffectFlag.w < 1.f)
			{
				bShFlagSelect[2] = true;
			}
			else
			{
				bShFlagSelect[2] = false;
			}

			if (ImGui::Selectable("GLOW", &bShFlagSelect[0]))
			{
				if (pCurEffect->m_vEffectFlag.x > 0.99f)
				{
					pCurEffect->m_vEffectFlag.x = 0.95f;
				}
				else
				{
					pCurEffect->m_vEffectFlag.x = 1.f;
				}
				
			}

			if (ImGui::Selectable("BLOOM", &bShFlagSelect[1]))
			{
				if (pCurEffect->m_vEffectFlag.y > 0.99f)
				{
					pCurEffect->m_vEffectFlag.y = 0.95f;
				}
				else
				{
					pCurEffect->m_vEffectFlag.y = 1.f;
				}


			}

			if (ImGui::Selectable("BLUR", &bShFlagSelect[2]))
			{
				if (pCurEffect->m_vEffectFlag.w > 0.99f)
				{
					pCurEffect->m_vEffectFlag.w = 0.95f;
				}
				else
				{
					pCurEffect->m_vEffectFlag.w = 1.f;
				}


			}

		}
		if (ImGui::CollapsingHeader("- GLOW VECTOR -"))
		{
			_float vGlowFlags[4] = { pCurEffect->m_vGlowFlag.x, pCurEffect->m_vGlowFlag.y, pCurEffect->m_vGlowFlag.z, pCurEffect->m_vGlowFlag.w };
			memcpy(vGlowFlags, &pCurEffect->m_vGlowFlag, sizeof(_float4));

			if (ImGui::InputFloat4("vGlowFlag", vGlowFlags, "%.2f"))
			{
				memcpy(&pCurEffect->m_vGlowFlag, vGlowFlags, sizeof(_float4));
			}
		}
		if (ImGui::CollapsingHeader("- OFFSET POSITIONS "))
		{
			CInstancingEffects::INSTANCING_CREATE_DATA& tCurData = static_cast<CInstancingEffects*>(pCurEffect)->Get_CreateData();

			_int iTemp = tCurData.iOffsetPositionCount;
			if (ImGui::InputInt("iNumOffsets", &iTemp, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				static_cast<CInstancingEffects*>(pCurEffect)->ReMake_OffsetPositions(iTemp);
			}

			for (_uint i = 0; i < tCurData.iOffsetPositionCount; ++i)
			{
				string strName = "OffsetPosition_";
				strName += to_string(i);

				_float	vOffset[3] = { tCurData.pOffsetPositions[i] .x, tCurData.pOffsetPositions[i] .y, tCurData.pOffsetPositions[i] .z};
				
				if (ImGui::InputFloat3(strName.c_str(), vOffset, "%.2f"))
				{
					tCurData.pOffsetPositions[i].x = vOffset[0];
					tCurData.pOffsetPositions[i].y = vOffset[1];
					tCurData.pOffsetPositions[i].z = vOffset[2];
				}


			}


		}
		if (ImGui::CollapsingHeader("- VARIALBLES - "))
		{
			CInstancingEffects::INSTANCING_CREATE_DATA& tCurData = static_cast<CInstancingEffects*>(pCurEffect)->Get_CreateData();

			_int	iNumInstance = tCurData.iNumInstance;
			if (ImGui::InputInt("NumInstance", &iNumInstance, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				tCurData.iNumInstance = (_uint)iNumInstance;
				pCurEffect->Re_Initialize();
			}

			if (ImGui::RadioButton("bBillBoard", static_cast<CRectEffects*>(pCurEffect)->m_bBillBoard))
			{
				static_cast<CRectEffects*>(pCurEffect)->m_bBillBoard = !static_cast<CRectEffects*>(pCurEffect)->m_bBillBoard;
			}

			ImGui::SameLine();

			if (ImGui::RadioButton("bSoftLerp", static_cast<CRectEffects*>(pCurEffect)->m_bSoft))
			{
				static_cast<CRectEffects*>(pCurEffect)->m_bSoft = !static_cast<CRectEffects*>(pCurEffect)->m_bSoft;
			}

			_float	vStartDir[3] = { tCurData.vStartDir.x, tCurData.vStartDir.y, tCurData.vStartDir.z };
			if (ImGui::InputFloat3("vStartDir", vStartDir, "%.3f"))
			{
				tCurData.vStartDir.x = vStartDir[0];
				tCurData.vStartDir.y = vStartDir[1];
				tCurData.vStartDir.z = vStartDir[2];
			}

			_float	vStartDirRange[3] = { tCurData.vStartDirRange.x, tCurData.vStartDirRange.y, tCurData.vStartDirRange.z };
			if (ImGui::InputFloat3("vStartDirRange", vStartDirRange, "%.3f"))
			{
				tCurData.vStartDirRange.x = vStartDirRange[0];
				tCurData.vStartDirRange.y = vStartDirRange[1];
				tCurData.vStartDirRange.z = vStartDirRange[2];
			}

			_float	fStartDistance[2] = { tCurData.fStartDistance , tCurData.fStartDistanceRange };
			if (ImGui::InputFloat2("fStartDistance, Range", fStartDistance, "%.2f"))
			{
				tCurData.fStartDistance = fStartDistance[0];
				tCurData.fStartDistanceRange = fStartDistance[1];
			}

			_float	fJumpPowers[2] = { tCurData.fJumpPower , tCurData.fJumpPowerRange};
			if (ImGui::InputFloat2("fJumpPower, Range", fJumpPowers, "%.2f"))
			{
				tCurData.fJumpPower = fJumpPowers[0];
				tCurData.fJumpPowerRange = fJumpPowers[1];
			}

			_float	fGravityRanges[2] = { tCurData.fGravity , tCurData.fGravityRange};
			if (ImGui::InputFloat2("fGravity, Range", fGravityRanges, "%.2f"))
			{
				tCurData.fGravity = fGravityRanges[0];
				tCurData.fGravityRange = fGravityRanges[1];
			}

			_float	fSpeeds[2] = { tCurData.fSpeed , tCurData.fSpeedRange};
			if (ImGui::InputFloat2("fSpeed, Range", fSpeeds, "%.2f"))
			{
				tCurData.fSpeed = fSpeeds[0];
				tCurData.fSpeedRange = fSpeeds[1];
			}

			_float	fSpeedChanges[2] = { tCurData.fSpeedChangeSpeed , tCurData.fSpeedChangeSpeedRange};
			if (ImGui::InputFloat2("fSpeedChange, Range", fSpeedChanges, "%.2f"))
			{
				tCurData.fSpeedChangeSpeed = fSpeedChanges[0];
				tCurData.fSpeedChangeSpeedRange = fSpeedChanges[1];
			}

			_float	fTurnSpeeds[2] = { tCurData.fTurnSpeed, tCurData.fTurnSpeedRange};
			if (ImGui::InputFloat2("fTurnSpeed, Range", fTurnSpeeds, "%.2f"))
			{
				tCurData.fTurnSpeed = fTurnSpeeds[0];
				tCurData.fTurnSpeedRange = fTurnSpeeds[1];
			}

			if (ImGui::InputFloat("fTargetAlpha", &tCurData.fTargetAlpha))
			{
			}

			if (ImGui::InputFloat("fDissolvePower(Anim)", &((CRectEffects*)pCurEffect)->m_fDissolvePower))
			{
			}

			if (ImGui::InputFloat("fDuration(Animation)", &((CRectEffects*)pCurEffect)->m_fDuration))
			{
			}


			_int	iSizes[2] = { ((CRectEffects*)pCurEffect)->m_iWidthSize, ((CRectEffects*)pCurEffect)->m_iHeightSize};
			if (ImGui::InputInt2("iAnimSizes(Animation)", iSizes))
			{
				((CRectEffects*)pCurEffect)->m_iWidthSize = iSizes[0];
				((CRectEffects*)pCurEffect)->m_iHeightSize = iSizes[1];
			}


			_float	vColors[3] = { tCurData.vColor.x , tCurData.vColor.y, tCurData.vColor.z};
			if (ImGui::InputFloat3("vColor", vColors, "%.2f"))
			{
				tCurData.vColor.x = vColors[0];
				tCurData.vColor.y = vColors[1];
				tCurData.vColor.z = vColors[2];
			}

			_float	vScales[3] = { tCurData.vScale.x, tCurData.vScale.y, tCurData.vScale.z};
			if (ImGui::InputFloat3("vStartScale", vScales, "%.3f"))
			{
				tCurData.vScale.x = vScales[0];
				tCurData.vScale.y = vScales[1];
				tCurData.vScale.z = vScales[2];
			}

			_float	vTargetScales[3] = { tCurData.vFadeInTargetScale.x, tCurData.vFadeInTargetScale.y, tCurData.vFadeInTargetScale.z};
			if (ImGui::InputFloat3("vTargetScale", vTargetScales, "%.3f"))
			{
				tCurData.vFadeInTargetScale.x = vTargetScales[0];
				tCurData.vFadeInTargetScale.y = vTargetScales[1];
				tCurData.vFadeInTargetScale.z = vTargetScales[2];
			}

			_float	vTargetScalesRange[3] = { tCurData.vFadeInTargetScaleRange.x, tCurData.vFadeInTargetScaleRange.y, tCurData.vFadeInTargetScaleRange.z};
			if (ImGui::InputFloat3("vTargetScaleRange", vTargetScalesRange, "%.3f"))
			{
				tCurData.vFadeInTargetScaleRange.x = vTargetScalesRange[0];
				tCurData.vFadeInTargetScaleRange.y = vTargetScalesRange[1];
				tCurData.vFadeInTargetScaleRange.z = vTargetScalesRange[2];
			}

			_float	vFadeOutScales[3] = { tCurData.vFadeOutTargetScale.x, tCurData.vFadeOutTargetScale.y, tCurData.vFadeOutTargetScale.z};
			if (ImGui::InputFloat3("vFadeOutScale", vFadeOutScales, "%.3f"))
			{
				tCurData.vFadeOutTargetScale.x = vFadeOutScales[0];
				tCurData.vFadeOutTargetScale.y = vFadeOutScales[1];
				tCurData.vFadeOutTargetScale.z = vFadeOutScales[2];
			}

			_float	vFadeOutScalesRange[3] = { tCurData.vFadeOutTargetScaleRange.x , 	tCurData.vFadeOutTargetScaleRange.y, 	tCurData.vFadeOutTargetScaleRange.z};
			if (ImGui::InputFloat3("vFadeOutScaleRange", vFadeOutScalesRange, "%.3f"))
			{
				tCurData.vFadeOutTargetScaleRange.x = vFadeOutScalesRange[0];
				tCurData.vFadeOutTargetScaleRange.y = vFadeOutScalesRange[1];
				tCurData.vFadeOutTargetScaleRange.z = vFadeOutScalesRange[2];
			}


			/* FADE */
			_float	fFIST[2] = { tCurData.fFadeInStartTime, tCurData.fFadeInStartTimeRange};
			if (ImGui::InputFloat2("FadeIn StartTime, Range", fFIST, "%.2f"))
			{
				tCurData.fFadeInStartTime = fFIST[0];
				tCurData.fFadeInStartTimeRange = fFIST[1];
			}

			_float	fFIST2[2] = { tCurData.fFadeInTime, tCurData.fFadeInTimeRange};
			if (ImGui::InputFloat2("FadeIn Time, Range", fFIST2, "%.2f"))
			{
				tCurData.fFadeInTime = fFIST2[0];
				tCurData.fFadeInTimeRange = fFIST2[1];
			}

			_float	fFIST3[2] = { tCurData.fFadeOutStartTime, tCurData.fFadeOutStartTimeRange};
			if (ImGui::InputFloat2("FadeOut StartTime, Range", fFIST3, "%.2f"))
			{
				tCurData.fFadeOutStartTime = fFIST3[0];
				tCurData.fFadeOutStartTimeRange = fFIST3[1];
			}

			_float	fFIST4[2] = { tCurData.fFadeOutTime, tCurData.fFadeOutTimeRange};
			if (ImGui::InputFloat2("FadeOut Time, Range", fFIST4, "%.2f"))
			{
				tCurData.fFadeOutTime = fFIST4[0];
				tCurData.fFadeOutTimeRange = fFIST4[1];
			}

		}


	}
}

void CWindow_Effect::Save_CurEffect()
{
	if (m_iCurrentIdx == 9999)
		return;

	string savePath = "../bin/effects/";
	savePath += m_vecEffects[m_iCurrentIdx].strName;
	savePath += ".bin";
	ofstream	writeFile(savePath, ios::binary);

	if (!writeFile.is_open())
	{
		Call_MsgBox(L"SSave 실패 ??!?!");
		return;
	}

	CEffect* pCurEffect = m_vecEffects[m_iCurrentIdx].pEffect;

	if (m_vecEffects[m_iCurrentIdx].iEffectType == 1)
	{
		if (pCurEffect->m_iPassType == VTXRECTINSTANCE_PASS_ANIMATION || pCurEffect->m_iPassType == VTXRECTINSTANCE_PASS_ANIMATIONALPHA)
			m_vecEffects[m_iCurrentIdx].iEffectType = 2;
		else
			m_vecEffects[m_iCurrentIdx].iEffectType = 1;

	}

	//1. 타입
	writeFile.write((char*)&m_vecEffects[m_iCurrentIdx].iEffectType, sizeof(_uint));

	switch (m_vecEffects[m_iCurrentIdx].iEffectType)
	{
	case 0:
	{

		//메쉬랑 텍스쳐 먼저
		CModel * pModelCom = GET_COMPONENT_FROM(pCurEffect, CModel);

		string	strModelFilePath = CFunctor::To_String(pModelCom->Get_ModelFilePath());
		CUtility_File::Write_Text(&writeFile, strModelFilePath.c_str());

		string	strMaskMapPath = CFunctor::To_String(pModelCom->Get_TextureFilePath(1));
		CUtility_File::Write_Text(&writeFile, strMaskMapPath.c_str());

		string	strColorMapPath = CFunctor::To_String(pModelCom->Get_TextureFilePath(aiTextureType_DIFFUSE_ROUGHNESS));
		CUtility_File::Write_Text(&writeFile, strColorMapPath.c_str());

		writeFile.write((char*)&pCurEffect->m_eShaderType, sizeof(_uint));
		writeFile.write((char*)&pCurEffect->m_vEffectFlag, sizeof(_float4));
		writeFile.write((char*)&pCurEffect->m_vGlowFlag, sizeof(_float4));
		writeFile.write((char*)&pCurEffect->m_fUVSpeedX, sizeof(_float));
		writeFile.write((char*)&pCurEffect->m_fUVSpeedY, sizeof(_float));
		writeFile.write((char*)&pCurEffect->m_matTrans, sizeof(_float4x4));
		writeFile.write((char*)&pCurEffect->m_eDisableType, sizeof(_uint));
		writeFile.write((char*)&pCurEffect->m_iPassType, sizeof(_uint));
		writeFile.write((char*)&pCurEffect->m_bEffectFlag, sizeof(_byte));
		writeFile.write((char*)&pCurEffect->m_vTurnDir, sizeof(_float4));
		writeFile.write((char*)&pCurEffect->m_fTurnSpeed, sizeof(_float));
		writeFile.write((char*)&pCurEffect->m_vOffsetPos, sizeof(_float4));
		writeFile.write((char*)&pCurEffect->m_fMoveSpeed, sizeof(_float));
		writeFile.write((char*)&pCurEffect->m_fFadeInStartTime, sizeof(_float));
		writeFile.write((char*)&pCurEffect->m_fFadeInTime, sizeof(_float));
		writeFile.write((char*)&pCurEffect->m_fFadeOutStartTime, sizeof(_float));
		writeFile.write((char*)&pCurEffect->m_fFadeOutTime, sizeof(_float));
		writeFile.write((char*)&pCurEffect->m_vStartScale, sizeof(_float4));
		writeFile.write((char*)&pCurEffect->m_vFadeInTargetScale, sizeof(_float4));
		writeFile.write((char*)&pCurEffect->m_vFadeOutTargetScale, sizeof(_float4));
		writeFile.write((char*)&pCurEffect->m_fTargetAlpha, sizeof(_float));

		writeFile.close();
		break;
	}

	case 1:
		//텍스쳐부텅
	{
		CUtility_File::Write_Text(&writeFile, CFunctor::To_String(pCurEffect->m_wstrMaskMapPath).c_str());
		CUtility_File::Write_Text(&writeFile, CFunctor::To_String(pCurEffect->m_wstrColorMapPath).c_str());


		writeFile.write((char*)&pCurEffect->m_vEffectFlag, sizeof(_float4));
		writeFile.write((char*)&pCurEffect->m_vGlowFlag, sizeof(_float4));
		writeFile.write((char*)&pCurEffect->m_iPassType, sizeof(_uint));



		writeFile.write((char*)&static_cast<CRectEffects*>(pCurEffect)->m_bBillBoard, sizeof(_bool));
		writeFile.write((char*)&static_cast<CRectEffects*>(pCurEffect)->m_bSoft, sizeof(_bool));

		CInstancingEffects::INSTANCING_CREATE_DATA* tData = &static_cast<CInstancingEffects*>(pCurEffect)->m_tCreateData;
		writeFile.write((char*)tData, sizeof(CInstancingEffects::INSTANCING_CREATE_DATA));

		writeFile.write((char*)tData->pOffsetPositions, sizeof(_float4) * tData->iOffsetPositionCount);
		break;
	}
	case 2: //Anim Particle
	{
		CUtility_File::Write_Text(&writeFile, CFunctor::To_String(pCurEffect->m_wstrMaskMapPath).c_str());
		CUtility_File::Write_Text(&writeFile, CFunctor::To_String(pCurEffect->m_wstrColorMapPath).c_str());
		CUtility_File::Write_Text(&writeFile, CFunctor::To_String(pCurEffect->m_wstrNormalMapPath).c_str());

		writeFile.write((char*)&pCurEffect->m_vEffectFlag, sizeof(_float4));
		writeFile.write((char*)&pCurEffect->m_vGlowFlag, sizeof(_float4));
		writeFile.write((char*)&pCurEffect->m_iPassType, sizeof(_uint));

		writeFile.write((char*)&static_cast<CRectEffects*>(pCurEffect)->m_bBillBoard, sizeof(_bool));
		writeFile.write((char*)&static_cast<CRectEffects*>(pCurEffect)->m_bSoft, sizeof(_bool));
		writeFile.write((char*)&static_cast<CRectEffects*>(pCurEffect)->m_iWidthSize, sizeof(_uint));
		writeFile.write((char*)&static_cast<CRectEffects*>(pCurEffect)->m_iHeightSize, sizeof(_uint));
		writeFile.write((char*)&static_cast<CRectEffects*>(pCurEffect)->m_fDuration, sizeof(_float));
		writeFile.write((char*)&static_cast<CRectEffects*>(pCurEffect)->m_fDissolvePower, sizeof(_float));

		CInstancingEffects::INSTANCING_CREATE_DATA* tData = &static_cast<CInstancingEffects*>(pCurEffect)->m_tCreateData;
		writeFile.write((char*)tData, sizeof(CInstancingEffects::INSTANCING_CREATE_DATA));

		writeFile.write((char*)tData->pOffsetPositions, sizeof(_float4) * tData->iOffsetPositionCount);

		break;
	}
	default:
		break;
	}

	Call_MsgBox(L"Save 성공");

}

void CWindow_Effect::Load_SelectedEffect()
{
	if (m_CurSelectedEffectFileKey.empty())
		return;

	_int iFind2 = (_int)m_CurSelectedEffectFileKey.find(".");
	string strFileKey = m_CurSelectedEffectFileKey.substr(0, iFind2);

	CEffect* pNewEffect = CEffect::Create_Effect_FromBinFile(strFileKey);

	if (!pNewEffect)
		return;

	if (FAILED(pNewEffect->Initialize()))
		Call_MsgBox(L"Failed to Initialize Effect");

	EFFECT_ITEM tItem;
	tItem.bSelected = false;
	tItem.iEffectType = pNewEffect->m_eEffectType;
	tItem.pEffect = pNewEffect;
	tItem.strName = strFileKey;

	CREATE_GAMEOBJECT(pNewEffect, GROUP_EFFECT);
	pNewEffect->m_pFollowTarget = PLAYER;
	m_vecEffects.push_back(tItem);
}

void CWindow_Effect::Read_Folder(const char* pFolderPath, TREE_DATA& tRootTree)
{
	for (filesystem::directory_iterator FileIter(pFolderPath);
		FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		_int iFind = (_int)strFullPath.rfind("\\") + 1;
		string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

		TREE_DATA	tTreeData;
		tTreeData.strFullPath = strFullPath;
		tTreeData.strFileName = strFileName;
		tTreeData.strFolderPath = pFolderPath;
		if (entry.is_directory())
		{
			Read_Folder(strFullPath.c_str(), tTreeData);
		}
		else
		{
			_int iFindExt = (int)strFileName.rfind(".") + 1;
			string strExtName = strFileName.substr(iFindExt, strFileName.length() - iFindExt);

			if (strExtName == "dat")
				continue;
		}

		tRootTree.vecChildren.push_back(tTreeData);

	}
}

void CWindow_Effect::Show_TreeData(TREE_DATA& tTree)
{
	if (!tTree.vecChildren.empty())
	{
		if (ImGui::TreeNode(tTree.strFileName.c_str()))
		{
			for (auto& tTreeData : tTree.vecChildren)
			{
				Show_TreeData(tTreeData);
			}

			ImGui::TreePop();
		}

	}
	else
	{
		_bool bSelected = false;

		if (m_CurSelectedMeshFilePath == tTree.strFullPath)
		{
			bSelected = true;
		}


		if (ImGui::Selectable(tTree.strFileName.c_str(), bSelected))
		{
			m_CurSelectedMeshFilePath = tTree.strFullPath;
		}
	}
}


void CWindow_Effect::Show_TreeData_Texture(TREE_DATA& tTree)
{
	if (!tTree.vecChildren.empty())
	{
		if (ImGui::TreeNode(tTree.strFileName.c_str()))
		{
			for (auto& tTreeData : tTree.vecChildren)
			{
				Show_TreeData_Texture(tTreeData);
			}

			ImGui::TreePop();
		}

	}
	else
	{
		_bool bSelected = false;

		if (m_CurSelectedTextureFilePath == tTree.strFullPath)
		{
			bSelected = true;
		}


		if (ImGui::Selectable(tTree.strFileName.c_str(), bSelected))
		{
			m_CurSelectedTextureFilePath = tTree.strFullPath;
		}
	}
}

void CWindow_Effect::Show_TreeData_Effect(TREE_DATA& tTree)
{
	if (!tTree.vecChildren.empty())
	{
		if (ImGui::TreeNode(tTree.strFileName.c_str()))
		{
			for (auto& tTreeData : tTree.vecChildren)
			{
				Show_TreeData_Effect(tTreeData);
			}

			ImGui::TreePop();
		}

	}
	else
	{
		_bool bSelected = false;

		if (m_CurSelectedEffectFileKey == tTree.strFileName)
		{
			bSelected = true;
		}


		if (ImGui::Selectable(tTree.strFileName.c_str(), bSelected))
		{
			m_CurSelectedEffectFileKey = tTree.strFileName;
		}
	}
}

void CWindow_Effect::Change_Model(CModel* pModelCom)
{
	pModelCom->Set_NewModel(CFunctor::To_Wstring(m_CurSelectedMeshFilePath));

	CTexture* pTextureCom = pModelCom->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS];
	pTextureCom->Add_Texture(m_vecEffects[m_iCurrentIdx].pEffect->m_wstrColorMapPath.c_str());
	pTextureCom->Set_CurTextureIndex(pTextureCom->Get_TextureSize() - 1);

	pTextureCom = pModelCom->Get_Materials().front().second.pTextures[1];
	pTextureCom->Add_Texture(m_vecEffects[m_iCurrentIdx].pEffect->m_wstrMaskMapPath.c_str());
	pTextureCom->Set_CurTextureIndex(pTextureCom->Get_TextureSize() - 1);

	pModelCom->Set_ShaderFlag(m_vecEffects[m_iCurrentIdx].pEffect->m_vEffectFlag);
}

void CWindow_Effect::Change_Model(CModel* pModelCom, _float4x4 transformMatrix)
{
	pModelCom->Set_NewTransformMatrix(transformMatrix);

	CTexture* pTextureCom = pModelCom->Get_Materials().front().second.pTextures[aiTextureType_DIFFUSE_ROUGHNESS];
	pTextureCom->Add_Texture(m_vecEffects[m_iCurrentIdx].pEffect->m_wstrColorMapPath.c_str());
	pTextureCom->Set_CurTextureIndex(pTextureCom->Get_TextureSize() - 1);

	pTextureCom = pModelCom->Get_Materials().front().second.pTextures[1];
	pTextureCom->Add_Texture(m_vecEffects[m_iCurrentIdx].pEffect->m_wstrMaskMapPath.c_str());
	pTextureCom->Set_CurTextureIndex(pTextureCom->Get_TextureSize() - 1);

	pModelCom->Set_ShaderFlag(m_vecEffects[m_iCurrentIdx].pEffect->m_vEffectFlag);

}
