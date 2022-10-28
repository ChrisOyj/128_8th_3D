#include "stdafx.h"
#include "CRoleWindow_Customizing.h"

#include "CGameObject_Factory.h"

#include "GameInstance.h"

#include "Texture.h"
#include "CFader.h"
#include "Transform.h"
#include "Model.h"
#include "MeshContainer.h"

#include "CDefault_UI.h"

#include "CCustomizingPreview.h"

#include "Loading_Manager.h"

#include "CUser.h"

CRoleWindow_Customizing::CRoleWindow_Customizing()
{

}
CRoleWindow_Customizing::~CRoleWindow_Customizing()
{
}

CRoleWindow_Customizing* CRoleWindow_Customizing::Create(CCustomizingPreview* pPreview)
{
    CRoleWindow_Customizing* pInstance = new CRoleWindow_Customizing;

    pInstance->m_pPreview = pPreview;

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Call_MsgBox(L"Failed to Initialize_Prototype : CRoleWindow_Customizing");
        SAFE_DELETE(pInstance);
    }

	return pInstance;
}

void CRoleWindow_Customizing::OnSelect()
{


    if (!m_bFinish && m_iCurSelection == m_arrPhaseDesc[m_iCurPhase].iNumButtons)
    {
        m_bFinish = true;
        GAMEINSTANCE->Play_Sound(L"Select_Big", (CHANNEL_GROUP)CHANNEL_UI);


        list <CGameObject*>& pList = CGameInstance::Get_Instance()->Get_ObjGroup(GROUP_UI);

        for (auto& elem : pList)
        {
            if (elem->Get_Component<CFader>().empty())
                continue;

            GET_COMPONENT_FROM(elem, CFader)->Get_FadeDesc().eFadeOutType = FADEDESC::FADEOUT_NONE;
            GET_COMPONENT_FROM(elem, CFader)->Force_KeyInput();
        }

        CUser::Get_Instance()->Set_PlayerModelData(m_pPreview->Get_ModelData());

        return;
    }

    GAMEINSTANCE->Play_Sound(L"Select_Small", (CHANNEL_GROUP)CHANNEL_UI);

    switch (m_iCurPhase)
    {
    case PHASE_DEFAULT:
        Set_CurPhase(m_iCurSelection+1);
        break;

    case PHASE_HEAD:
        Set_Head(m_iCurSelection);
        break;

    case PHASE_EYE:
        Set_Eye(m_iCurSelection);
        break;

    case PHASE_FACE:
        Set_Face(m_iCurSelection);
        break;

    case PHASE_OVERALL:
        Set_Overall(m_iCurSelection);
        break;

    case PHASE_BODYUPPER:
        Set_BodyUpper(m_iCurSelection);
        break;

    case PHASE_BODYLOWER:
        Set_BodyLower(m_iCurSelection);
        break;

    case PHASE_ACCESSORY:
        Set_Accessorry(m_iCurSelection);
        break;

    default:
        break;
    }
}

HRESULT CRoleWindow_Customizing::Initialize_Prototype()
{
    vector<CGameObject*>    vecUI;
    vecUI = CGameObject_Factory::Create_vecUI(L"CustomizingMenu");

    for (_uint i = 0; i < vecUI.size(); ++i)
    {
        m_vecUIs.push_back(static_cast<CDefault_UI*>(vecUI[i]));
    }

    CDefault_UI* pRoleWindowButton = CGameObject_Factory::Create_UI(L"CustomizingMenuButton_RoleWindow");
    CDefault_UI* pRoleWindowButtonText = CGameObject_Factory::Create_UI(L"CustomizingMenuButton_RoleWindowText");
    m_vecButtons.push_back(pRoleWindowButton);
    m_vecButtonTexts.push_back(pRoleWindowButtonText);

    for (_uint i = 1; i < 8; ++i)
    {
        CDefault_UI* pButton = pRoleWindowButton->Clone();
        CDefault_UI* pText = pRoleWindowButtonText->Clone();
        GET_COMPONENT_FROM(pButton, CFader)->Get_FadeDesc().fFadeInStartTime += (0.03f * (_float)i);
        GET_COMPONENT_FROM(pText, CFader)->Get_FadeDesc().fFadeInStartTime += (0.03f * (_float)i);

        _float4 vPos = pButton->Get_Transform()->Get_MyWorld(WORLD_POS);
        _float4 vPos2 = pButton->Get_Transform()->Get_MyWorld(WORLD_POS);
        vPos.y -= (40.f * i);
        vPos2.y -= (40.f * i);
        pButton->Get_Transform()->Set_World(WORLD_POS, vPos);
        pText->Get_Transform()->Set_World(WORLD_POS, vPos2);

        m_vecButtons.push_back(pButton);
        m_vecButtonTexts.push_back(pText);
    }

    m_iCurPhase = PHASE_DEFAULT;

    /* ============ Phase Baking ============*/

    for (auto& elem : m_vecUIs)
        m_vecAllUIs.push_back(elem);

    for (auto& elem : m_vecButtons)
        m_vecAllUIs.push_back(elem);

    for (auto& elem : m_vecButtonTexts)
        m_vecAllUIs.push_back(elem);

    /*for (_uint i = 0; i < PHASE_END; ++i)
    {
        Bake_PhaseDesc((PHASE)i, 7, 0, i, i, 0);
    }*/
    //ePhase,               NumButton, MainTextIdx, TitleTexIdx, BGInfoIdx, ButtonStartIdx

    Bake_PhaseDesc(PHASE_DEFAULT,       7, 0, 0, 0, 0);
    Bake_PhaseDesc(PHASE_HEAD,          8, 0, 1, 0, 7);
    Bake_PhaseDesc(PHASE_EYE,           6, 0, 2, 1, 15);
    Bake_PhaseDesc(PHASE_FACE,          4, 0, 3, 2, 21);
    Bake_PhaseDesc(PHASE_OVERALL,       8, 0, 4, 3, 25);
    Bake_PhaseDesc(PHASE_BODYUPPER,     8, 0, 5, 4, 33);
    Bake_PhaseDesc(PHASE_BODYLOWER,     8, 0, 6, 5, 41);
    Bake_PhaseDesc(PHASE_ACCESSORY,     8, 0, 7, 6, 49);


	return S_OK;
}

void CRoleWindow_Customizing::On_FadeOut(_uint iPhase)
{
    if (m_bFinish)
    {
        CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_STAGE1);
        return;
    }

    __super::On_FadeOut(iPhase);
}

HRESULT CRoleWindow_Customizing::Start()
{
    

    On_FadeOut(m_iCurPhase);

    return S_OK;
}

void CRoleWindow_Customizing::OnSelectionSwitch(_int iSelection)
{
    __super::OnSelectionSwitch(iSelection);

    if (m_iCurSelection == m_arrPhaseDesc[m_iCurPhase].iNumButtons)
        GET_COMPONENT_FROM(m_vecUIs[m_iBGInfoTextIndex], CTexture)->Set_CurTextureIndex(PHASE_END - 1);
    else
    {
        if (m_iCurPhase == PHASE_DEFAULT)
            GET_COMPONENT_FROM(m_vecUIs[m_iBGInfoTextIndex], CTexture)->Set_CurTextureIndex(m_iCurSelection);
        else
            GET_COMPONENT_FROM(m_vecUIs[m_iBGInfoTextIndex], CTexture)->Set_CurTextureIndex(m_arrPhaseDesc[m_iCurPhase].iBGInfoTextIndex);
    }
    


}

void CRoleWindow_Customizing::Bake_PhaseDesc(PHASE ePhase, _uint iNumButtons, _uint iMainTextIndex, _uint iTitleTextIndex, _uint iBGInfoTextIndex, _uint iButtonTextIndex)
{
    m_arrPhaseDesc[ePhase].iNumButtons = iNumButtons;
    m_arrPhaseDesc[ePhase].iMainTextIndex = iMainTextIndex;
    m_arrPhaseDesc[ePhase].iTitleTextIndex = iTitleTextIndex;
    m_arrPhaseDesc[ePhase].iBGInfoTextIndex = iBGInfoTextIndex;

    for (_uint i = 0; i < m_arrPhaseDesc[ePhase].iNumButtons; ++i)
        m_arrPhaseDesc[ePhase].vecButtonTextIndex.push_back(i + iButtonTextIndex);
}

void CRoleWindow_Customizing::Set_Head(_int iSelection)
{
    if (iSelection >= m_arrPhaseDesc[PHASE_HEAD].iNumButtons || iSelection < 0)
        return;

    MODEL_PART_TYPE eType = MODEL_PART_HEADGEAR;

    CModel* pModel = GET_COMPONENT_FROM(m_pPreview, CModel);
    pModel->Delete_ModelParts(eType);

    switch (iSelection)
    {
    case 0:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_ShortSpikyHair_07.fbx", eType);
        break;

    case 1:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_TopKnot_01.fbx", eType);
        break;

    case 2:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_MediumStraight_05_S.fbx", eType);
        break;

    case 3:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_ShortSpikyHair_04.fbx", eType);
        break;

    case 4:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_Helmet_02_Z.fbx", eType);
        break;

    case 5: 
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_Hat_01.fbx", eType);
        break;

    case 6:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_HatTurban_01.fbx", eType);
        break;

    case 7:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/head/SK_CHR_Man_PajamaHat_01.fbx", eType);
        break;

    default:
        break;
    }


    vector<pair<_uint, CMeshContainer*>>& vecMC = pModel->Get_MeshContainers();

    for (auto& elem : vecMC)
    {
        if (elem.first == 0)
            elem.second->Set_CurPass(VTXANIM_PASS_PREVIEWFACE);
        else
            elem.second->Set_CurPass(VTXANIM_PASS_PREVIEW);
    }


}

void CRoleWindow_Customizing::Set_BodyUpper(_int iSelection)
{
    if (iSelection >= m_arrPhaseDesc[PHASE_BODYUPPER].iNumButtons || iSelection < 0)
        return;

    MODEL_PART_TYPE eType = MODEL_PART_BODYUPPER;

    CModel* pModel = GET_COMPONENT_FROM(m_pPreview, CModel);
    pModel->Delete_ModelParts(eType);

    switch (iSelection)
    {
    case 0:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodyupper/SK_CHR_Man_Trainer_01.fbx", eType);
        break;

    case 1:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodyupper/SK_CHR_Man_Shirt_01.fbx", eType);
        break;

    case 2:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodyupper/SK_CHR_Man_AnbuBlackOps.fbx", eType);
        break;

    case 3:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodyupper/SK_CHR_Man_Shirt_02_D.fbx", eType);
        break;

    case 4:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodyupper/SK_CHR_Man_SandVest_02.fbx", eType);
        break;

    case 5:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodyupper/SK_CHR_Man_Shirt_05.fbx", eType);
        break;

    case 6:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodyupper/SK_CHR_Man_Vest_04_A.fbx", eType);
        break;

    case 7:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodyupper/SK_CHR_Man_Vest_05.fbx", eType);
        break;

    default:
        break;
    }


    vector<pair<_uint, CMeshContainer*>>& vecMC = pModel->Get_MeshContainers();

    for (auto& elem : vecMC)
    {
        elem.second->Set_CurPass(VTXANIM_PASS_PREVIEW);

        if (elem.first == 0)
            elem.second->Set_CurPass(VTXANIM_PASS_PREVIEWFACE);
        else if (elem.first == MODEL_PART_OVERALL)
            DISABLE_COMPONENT(elem.second);

        else if (elem.first == MODEL_PART_BODYLOWER)
            ENABLE_COMPONENT(elem.second);
    }
}

void CRoleWindow_Customizing::Set_Eye(_int iSelection)
{
    if (iSelection >= m_arrPhaseDesc[PHASE_EYE].iNumButtons || iSelection < 0)
        return;

    vector<pair<_uint, MODEL_MATERIAL>>& vecMaterials = GET_COMPONENT_FROM(m_pPreview, CModel)->Get_Materials();

    for (auto& elem : vecMaterials)
    {
        if (elem.second.strName == "MI_CHR_Eyes_WThinL_01" ||
            elem.second.strName == "MI_CHR_Eyes_WThinR_01")
        elem.second.pTextures[1]->Set_CurTextureIndex(iSelection+1);
    }

    m_pPreview->Get_ModelData().iEyeIndex = iSelection + 1;
}

void CRoleWindow_Customizing::Set_Face(_int iSelection)
{

    if (iSelection >= m_arrPhaseDesc[PHASE_FACE].iNumButtons || iSelection < 0)
        return;
    _uint iCurEyeIndex = 0;

    vector<pair<_uint, MODEL_MATERIAL>>& vecMaterials = GET_COMPONENT_FROM(m_pPreview, CModel)->Get_Materials();

    for (auto& elem : vecMaterials)
    {
        if (elem.second.strName == "MI_CHR_Eyes_WThinL_01" ||
            elem.second.strName == "MI_CHR_Eyes_WThinR_01")
        {
            iCurEyeIndex = elem.second.pTextures[1]->Get_CurTextureIndex();
            break;
        }
    }


    MODEL_PART_TYPE eType = MODEL_PART_FACE;

    CModel* pModel = GET_COMPONENT_FROM(m_pPreview, CModel);
    pModel->Delete_ModelParts(eType);

    switch (iSelection)
    {
    case 0:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/face/SK_CHR_Face.fbx", eType);
        break;

    case 1:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/face/SK_CHR_FaceMask2.fbx", eType);
        break;

    case 2:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/face/SK_CHR_FaceMask.fbx", eType);
        break;

    case 3:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/face/SK_CHR_FaceBandage2.fbx", eType);
        break;

    default:
        break;
    }


    vector<pair<_uint, CMeshContainer*>>& vecMC = pModel->Get_MeshContainers();

    for (auto& elem : vecMC)
    {
        if (vecMaterials[elem.second->Get_MaterialIndex()].second.strName == "MI_CHR_Hachigane_Konoha_01" ||
            vecMaterials[elem.second->Get_MaterialIndex()].second.strName == "MI_WEP_Sword_01" ||
            vecMaterials[elem.second->Get_MaterialIndex()].second.strName == "MI_WEP_Kurosawa")
            elem.second->Set_ShaderFlag(SH_LIGHT_DEFAULT);

        if (elem.first == 0)
            elem.second->Set_ShaderFlag(SH_LIGHT_NOOUTLINE);

        /*if (vecMaterials[elem.second->Get_MaterialIndex()].second.strName.find("Eye"))
            elem.second->Set_ShaderFlag(SH_LIGHT_NOOUTLINE);*/

        if (elem.first == 0)
            elem.second->Set_CurPass(VTXANIM_PASS_PREVIEWFACE);
        else
            elem.second->Set_CurPass(VTXANIM_PASS_PREVIEW);
    }



    for (auto& elem : vecMaterials)
    {
        if (elem.second.strName == "MI_CHR_Eyes_WThinL_01" ||
            elem.second.strName == "MI_CHR_Eyes_WThinR_01")
        {
            for (_uint i = 0; i < EYE_END; ++i)
            {
                _tchar szBuff[MAX_STR] = L"";
                swprintf_s(szBuff, L"../bin/resources/textures/animModelTextures/Eyes/Eye_%d.dds", i);
                elem.second.pTextures[1]->Add_Texture(szBuff);
            }

            elem.second.pTextures[1]->Set_CurTextureIndex(iCurEyeIndex);

        }

    }


}

void CRoleWindow_Customizing::Set_BodyLower(_int iSelection)
{
    if (iSelection >= m_arrPhaseDesc[PHASE_BODYLOWER].iNumButtons || iSelection < 0)
        return;

    MODEL_PART_TYPE eType = MODEL_PART_BODYLOWER;

    CModel* pModel = GET_COMPONENT_FROM(m_pPreview, CModel);
    pModel->Delete_ModelParts(eType);

    switch (iSelection)
    {
    case 0:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodylower/SK_CHR_Man_LeafPants_01.fbx", eType);
        break;

    case 1:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodylower/SK_CHR_Man_AnbuBlackOps_01.fbx", eType);
        break;

    case 2:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodylower/SK_CHR_Man_LeatherPants_01.fbx", eType);
        break;

    case 3:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodylower/SK_CHR_Man_PantsCut_01_A.fbx", eType);
        break;

    case 4:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodylower/SK_CHR_Man_Skirt_01.fbx", eType);
        break;

    case 5:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodylower/SK_CHR_Man_Skirt_01_Z.fbx", eType);
        break;

    case 6:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodylower/SK_CHR_Man_StonePants_01.fbx", eType);
        break;

    case 7:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/bodylower/SK_CHR_Man_TrainerPants_05.fbx", eType);
        break;

    default:
        break;
    }


    vector<pair<_uint, CMeshContainer*>>& vecMC = pModel->Get_MeshContainers();

    for (auto& elem : vecMC)
    {

        if (elem.first == 0)
            elem.second->Set_CurPass(VTXANIM_PASS_PREVIEWFACE);
        else
            elem.second->Set_CurPass(VTXANIM_PASS_PREVIEW);

        if (elem.first == MODEL_PART_OVERALL)
            DISABLE_COMPONENT(elem.second);

        if (elem.first == MODEL_PART_BODYUPPER)
            ENABLE_COMPONENT(elem.second);
    }
}

void CRoleWindow_Customizing::Set_Overall(_int iSelection)
{
    if (iSelection >= m_arrPhaseDesc[PHASE_OVERALL].iNumButtons || iSelection < 0)
        return;

    MODEL_PART_TYPE eType = MODEL_PART_OVERALL;

    CModel* pModel = GET_COMPONENT_FROM(m_pPreview, CModel);
    pModel->Delete_ModelParts(eType);

    switch (iSelection)
    {
    case 0:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_Coat.fbx", eType);
        break;

    case 1:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_Coat_15.fbx", eType);
        break;

    case 2:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_Coat_09.fbx", eType);
        break;

    case 3:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_LightningRobe_01.fbx", eType);
        break;

    case 4:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_Overall_05.fbx", eType);
        break;

    case 5:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_Santa_01.fbx", eType);
        break;

    case 6:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_Beach_01_C.fbx", eType);
        break;

    case 7:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/overall/SK_CHR_Man_Tuxedo_02.fbx", eType);
        break;


    default:
        break;
    }

    vector<pair<_uint, CMeshContainer*>>& vecMC = pModel->Get_MeshContainers();

    for (auto& elem : vecMC)
    {

        if (elem.first == 0)
            elem.second->Set_CurPass(VTXANIM_PASS_PREVIEWFACE);
        else
            elem.second->Set_CurPass(VTXANIM_PASS_PREVIEW);

        if (elem.first == MODEL_PART_BODYUPPER || elem.first == MODEL_PART_BODYLOWER)
            DISABLE_COMPONENT(elem.second);
    }

    
}

void CRoleWindow_Customizing::Set_Accessorry(_int iSelection)
{
    if (iSelection >= m_arrPhaseDesc[PHASE_ACCESSORY].iNumButtons || iSelection < 0)
        return;

    MODEL_PART_TYPE eType = MODEL_PART_ACCESSORY;

    CModel* pModel = GET_COMPONENT_FROM(m_pPreview, CModel);
    pModel->Delete_ModelParts(eType);

    switch (iSelection)
    {
    case 0:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/Accessories/SK_CHR_Bell2.fbx", eType);
        break;

    case 1:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/Accessories/SK_CHR_Book_MakeOut.fbx", eType);
        break;

    case 2:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/Accessories/SK_CHR_Glasses_Shooter.fbx", eType);
        break;

    case 3:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/Accessories/SK_CHR_FinalGear.fbx", eType);
        break;

    case 4:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/Accessories/SK_LightningOrnaments_02.fbx", eType);
        break;

    case 5:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/Accessories/SK_WaterOrnaments_01.fbx", eType);
        break;

    case 6:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/Accessories/SM_CHR_Earrings_Delta.fbx", eType);
        break;

    case 7:
        m_pPreview->Add_ModelParts(L"../bin/resources/meshes/characters/custom/Accessories/SM_DarkRoseEarrings.fbx", eType);
        break;


    default:
        break;
    }

    vector<pair<_uint, CMeshContainer*>>& vecMC = pModel->Get_MeshContainers();

    for (auto& elem : vecMC)
    {

        if (elem.first == 0)
            elem.second->Set_CurPass(VTXANIM_PASS_PREVIEWFACE);
        else
            elem.second->Set_CurPass(VTXANIM_PASS_PREVIEW);
    }
}

void CRoleWindow_Customizing::My_Tick()
{
    if (KEY(Q, TAP))
    {
            _uint iRand = 0;
        _bool   bOverall = false;
        GAMEINSTANCE->Play_Sound(L"Select_Small", (CHANNEL_GROUP)CHANNEL_UI);

        //한벌옷만 가능
        //상의에서 하의를 Enable하는데 그 하의를 지워버림
        if (random(0, 2) <= 3)
        {
            bOverall = true;
        }

        for (_uint i = PHASE_HEAD; i < PHASE_END; ++i)
        {
            if (i == PHASE_OVERALL)
            {
                if (!bOverall)
                    continue;
            }
            else if (i == PHASE_BODYUPPER || i == PHASE_BODYLOWER)
            {
                if (bOverall)
                    continue;
            }


            iRand = random(0, m_arrPhaseDesc[i].iNumButtons);
            

            switch (i)
            {
            case PHASE_DEFAULT:
                Set_CurPhase(m_iCurSelection + 1);
                break;

            case PHASE_HEAD:
                Set_Head(iRand);
                break;

            case PHASE_EYE:
                Set_Eye(iRand);
                break;

            case PHASE_FACE:
                Set_Face(iRand);
                break;

            case PHASE_OVERALL:
                Set_Overall(iRand);
                break;

            case PHASE_BODYUPPER:
                Set_BodyUpper(iRand);
                break;

            case PHASE_BODYLOWER:
                Set_BodyLower(iRand);
                break;

            case PHASE_ACCESSORY:
                Set_Accessorry(iRand);
                break;

            default:
                break;
            }




        }

    }



    __super::My_Tick();

    
}
