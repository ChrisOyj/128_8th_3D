#include "stdafx.h"
#include "CEffects_Factory.h"

#include "GameInstance.h"
#include "CDodgeWood.h"
#include "Transform.h"
#include "CGuard.h"
#include "CKOLine.h"
#include "CWireKunai.h"
#include "CWire.h"
#include "CKunai.h"
#include "CWindmill.h"
#include "CGoal.h"
#include "CChakraJump_Guide.h"
#include "CCell.h"
#include "CEffect_Text.h"
#include "Model.h"
#include "Functor.h"

#include "CInstancingEffects.h"
#include "CRectEffects.h"

#include "CScreenEffect.h"
#include "CGroundBreak.h"
#include "CEffects_Factory.h"
#include "Texture.h"
#include "CFireBallEffect.h"
#include "CDecalEffect.h"

#include "CWoodHand.h"
#include "CLaughingMonkEffect.h"
#include "CWoodHand_R.h"

#include "CWoodWall.h"

#include "CRasenShurikenEffect.h"
#include "CRasenShurikenBomb.h"
#include "CKamuiEffect.h"

#include "CEightGatesDragon.h"
#include "CEightGatesEffects.h"
#include "CEveElephant.h"
#include "CSharkBombEffect.h"

#include "CRaijinKunai.h"

#include "CBossKunai.h"
#include "CBossAura.h"

#include "CBoneChakra.h"

#include "CBurger.h"

#include "CWaterUlt.h"

IMPLEMENT_SINGLETON(CEffects_Factory);

CEffects_Factory::CEffects_Factory()
{
}

CEffects_Factory::~CEffects_Factory()
{
}

HRESULT CEffects_Factory::Initialize()
{
	//프로토타입에다가 추가
	if (FAILED(Add_Effect(HASHCODE(CDodgeWood), CDodgeWood::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CChakraJump_Guide), CChakraJump_Guide::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CGuard), CGuard::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CWireKunai), CWireKunai::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CKunai), CKunai::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CWindmill), CWindmill::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CScreenEffect), CScreenEffect::Create(50))))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CFireBallEffect), CFireBallEffect::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CWoodHand), CWoodHand::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CWoodHand_R), CWoodHand_R::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CLaughingMonkEffect), CLaughingMonkEffect::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CWoodWall), CWoodWall::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CRasenShurikenEffect), CRasenShurikenEffect::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CRasenShurikenBomb), CRasenShurikenBomb::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CKamuiEffect), CKamuiEffect::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CEightGatesEffects), CEightGatesEffects::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CEveElephant), CEveElephant::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CEightGatesDragon), CEightGatesDragon::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CRaijinKunai), CRaijinKunai::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CBossKunai), CBossKunai::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CBossAura), CBossAura::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(HASHCODE(CBoneChakra), CBoneChakra::Create())))
		return E_FAIL;
	
	if (FAILED(Add_Effect(HASHCODE(CBurger), CBurger::Create())))
		return E_FAIL;
	
	if (FAILED(Add_Effect(HASHCODE(CSharkBombEffect), CSharkBombEffect::Create())))
		return E_FAIL;
	
	if (FAILED(Add_Effect(HASHCODE(CWaterUlt), CWaterUlt::Create())))
		return E_FAIL;

	if (FAILED(Add_Effect(Convert_ToHash(L"FireDecal"), CDecalEffect::Create(
		L"../bin/resources/textures/effects/decal/T_EFF_Decal_Gokakyu_01_M.dds",
		L"../bin/resources/textures/effects/gradationColor/T_EFF_GMS_Fire_02_BC.png",
		L"FireDecal"))))
		return E_FAIL;

	if (FAILED(Add_Effect(Convert_ToHash(L"GroundBreak_A"), CGroundBreak::Create(L"../bin/resources/meshes/effects/groundBreak/SM_EFF_GroundBreak_A.fbx", L"GroundBreak_A"))))
		return E_FAIL;
	if (FAILED(Add_Effect(Convert_ToHash(L"GroundBreak_B"), CGroundBreak::Create(L"../bin/resources/meshes/effects/groundBreak/SM_EFF_GroundBreak_B.fbx", L"GroundBreak_B"))))
		return E_FAIL;
	if (FAILED(Add_Effect(Convert_ToHash(L"GroundBreak_C"), CGroundBreak::Create(L"../bin/resources/meshes/effects/groundBreak/SM_EFF_GroundBreak_C.fbx", L"GroundBreak_C"))))
		return E_FAIL;


	_float4 vLook = _float4(0.f, 0.f, 1.f, 0.f);
	if (FAILED(Add_Effect(Convert_ToHash(L"Line_0"),
		CKOLine::Create(L"../bin/resources/meshes/effects/lines/SM_EFF_Line_A_02.fbx", vLook))))
		return E_FAIL;
	vLook = _float4(0.f, 0.f, -1.f, 0.f);
	if (FAILED(Add_Effect(Convert_ToHash(L"Line_1"), CKOLine::Create(L"../bin/resources/meshes/effects/lines/SM_EFF_Line_A_01.fbx", vLook))))
		return E_FAIL;
	vLook = _float4(0.f, 1.f, 0.f, 0.f);
	if (FAILED(Add_Effect(Convert_ToHash(L"Line_2"), CKOLine::Create(L"../bin/resources/meshes/effects/lines/SM_EFF_Line_A_03.fbx", vLook))))
		return E_FAIL;
	vLook = _float4(0.f, -1.f, 0.f, 0.f);
	if (FAILED(Add_Effect(Convert_ToHash(L"Line_3"), CKOLine::Create(L"../bin/resources/meshes/effects/lines/SM_EFF_Line_A_04.fbx", vLook))))
		return E_FAIL;
	vLook = _float4(1.f, 0.f, 0.f, 0.f);
	if (FAILED(Add_Effect(Convert_ToHash(L"Line_4"), CKOLine::Create(L"../bin/resources/meshes/effects/lines/SM_EFF_Line_A_05.fbx", vLook))))
		return E_FAIL;
	vLook = _float4(-1.f, 0.f, 0.f, 0.f);
	if (FAILED(Add_Effect(Convert_ToHash(L"Line_5"), CKOLine::Create(L"../bin/resources/meshes/effects/lines/SM_EFF_Line_A_06.fbx", vLook))))
		return E_FAIL;


	vLook = _float4(1.f, 1.f, 0.f, 0.f);
	if (FAILED(Add_Effect(Convert_ToHash(L"Line_6"), CKOLine::Create(L"../bin/resources/meshes/effects/lines/SM_EFF_Line_A_03.fbx", vLook))))
		return E_FAIL;
	vLook = _float4(-1.f, 1.f, 0.f, 0.f);
	if (FAILED(Add_Effect(Convert_ToHash(L"Line_7"), CKOLine::Create(L"../bin/resources/meshes/effects/lines/SM_EFF_Line_A_04.fbx", vLook))))
		return E_FAIL;
	vLook = _float4(1.f, -1.f, 0.f, 0.f);
	if (FAILED(Add_Effect(Convert_ToHash(L"Line_8"), CKOLine::Create(L"../bin/resources/meshes/effects/lines/SM_EFF_Line_A_05.fbx", vLook))))
		return E_FAIL;
	vLook = _float4(-1.f, -1.f, 0.f, 0.f);
	if (FAILED(Add_Effect(Convert_ToHash(L"Line_9"), CKOLine::Create(L"../bin/resources/meshes/effects/lines/SM_EFF_Line_A_06.fbx", vLook))))
		return E_FAIL;


	//Grass
	CInstancingEffects::INSTANCING_CREATE_DATA tData;
	ZeroMemory(&tData, sizeof(CInstancingEffects::INSTANCING_CREATE_DATA));
	tData.fCreateRange = 0.3f;
	tData.fCreateRangeY = 0.f;
	tData.fCurGroundY = -1.f;
	tData.fCurGroundYRange = 0.f;
	tData.fGravity = 4.8f;
	tData.fGravityRange = 3.f;
	tData.fJumpPower = 3.f;
	tData.fJumpPowerRange = 1.5f;
	tData.fSpeed = 2.f;
	tData.fSpeedRange = 2.f;
	tData.fTurnSpeed = 3.f;
	tData.fTurnSpeedRange = 2.f;
	tData.vFadeInTargetScale = _float4(0.8f, 0.8f, 0.8f, 1.f);
	tData.vFadeInTargetScaleRange = _float4(0.3f, 0.3f, 0.3f, 1.f);
	tData.fFadeOutStartTime = 999.f;
	tData.fFadeInStartTime = 0.f;
	tData.fFadeInStartTimeRange = 0.f;
	_uint iNumInstance = 15;

	tData.iNumInstance = iNumInstance;

	if (FAILED(Add_Effect(Convert_ToHash(L"Grass"), CInstancingEffects::Create(L"../bin/resources/meshes/effects/Small/SM_EFF_Grass_01.fbx", iNumInstance, tData, Convert_ToHash(L"Grass")))))
		return E_FAIL;


	tData.fCreateRange = 1.f;
	tData.fCreateRangeY = 0.f;
	tData.fCurGroundY = -1.f;
	tData.fCurGroundYRange = 0.f;
	tData.fGravity = 4.8f;
	tData.fGravityRange = 3.f;
	tData.fJumpPower = 5.f;
	tData.fJumpPowerRange = 1.5f;
	tData.fSpeed = 4.f;
	tData.fSpeedRange = 2.f;
	tData.fTurnSpeed = 3.f;
	tData.fTurnSpeedRange = 2.f;
	tData.vFadeInTargetScale = _float4(1.5f, 1.5f, 1.5f, 1.f);
	tData.vFadeInTargetScaleRange = _float4(0.3f, 0.3f, 0.3f, 1.f);
	tData.fFadeOutStartTime = 999.f;
	iNumInstance = 100;

	tData.iNumInstance = iNumInstance;

	if (FAILED(Add_Effect(Convert_ToHash(L"BigGrass"), CInstancingEffects::Create(L"../bin/resources/meshes/effects/Small/SM_EFF_Grass_01.fbx", iNumInstance, tData, Convert_ToHash(L"BigGrass")))))
		return E_FAIL;


	tData.fCreateRange = 1.3f;
	tData.fCreateRangeY = 0.f;
	tData.fCurGroundY = -200.f;
	tData.fCurGroundYRange = 0.f;
	tData.fGravity = 1.f;
	tData.fGravityRange = 0.1f;
	tData.fJumpPower = 1.5f;
	tData.fJumpPowerRange = 1.f;
	tData.fSpeed = 0.3f;
	tData.fSpeedRange = 0.2f;
	tData.fTurnSpeed = 3.f;
	tData.fTurnSpeedRange = 2.f;

	tData.vScale = _float4(0.1f, 0.1f, 0.1f, 0.f);
	tData.vScaleRange = _float4(0.f, 0.f, 0.f, 0.f);

	tData.vFadeInTargetScale = _float4(0.8f, 0.8f, 0.8f, 1.f);
	tData.vFadeInTargetScaleRange = _float4(0.3f, 0.3f, 0.3f, 1.f);

	tData.vFadeOutTargetScale = _float4(0.2f, 0.2f, 0.2f, 1.f);


	tData.fFadeInStartTime = 0.4f;
	tData.fFadeInStartTimeRange = 0.4f;

	tData.fFadeOutStartTime = 0.4f;
	tData.fFadeOutStartTimeRange = 0.2f;

	tData.fFadeOutTime = 0.25f;
	tData.fFadeOutTimeRange = 0.1f;

	iNumInstance = 100;

	tData.iNumInstance = iNumInstance;

	if (FAILED(Add_Effect(Convert_ToHash(L"EightGatesGrass"),
		CInstancingEffects::Create(L"../bin/resources/meshes/effects/Small/SM_EFF_Grass_01.fbx", iNumInstance, tData, Convert_ToHash(L"EightGatesGrass")))))
		return E_FAIL;

	tData.vFadeInTargetScale = _float4(0.6f, 0.6f, 0.6f, 1.f);
	tData.vFadeInTargetScaleRange = _float4(0.1f, 0.1f, 0.1f, 1.f);

	if (FAILED(Add_Effect(Convert_ToHash(L"EightGatesRock"),
		CInstancingEffects::Create(L"../bin/resources/meshes/effects/Small/SM_EFF_SmallRock_01.fbx", iNumInstance, tData, Convert_ToHash(L"EightGatesRock")))))
		return E_FAIL;



	//LEAF

	tData.fCreateRange = 0.7f;
	tData.fCreateRangeY = 0.5f;
	tData.fCurGroundY = -2.f;
	tData.fCurGroundYRange = 0.5f;
	tData.fGravity = 4.8f;
	tData.fGravityRange = 3.f;
	tData.fJumpPower = 3.f;
	tData.fJumpPowerRange = 2.f;
	tData.fSpeed = 3.f;
	tData.fSpeedRange = 2.f;
	tData.fTurnSpeed = 10.f;
	tData.fTurnSpeedRange = 5.f;
	tData.vFadeInTargetScale = _float4(0.5f, 0.5f, 0.5f, 1.f);
	tData.vFadeInTargetScaleRange = _float4(0.3f, 0.3f, 0.3f, 1.f);
	tData.fFadeOutStartTime = 999.f;
	iNumInstance = 100;
	tData.iNumInstance = iNumInstance;
	tData.fFadeInStartTime = 0.f;
	tData.fFadeInStartTimeRange = 0.f;
	if (FAILED(Add_Effect(Convert_ToHash(L"LeafBomb"), CInstancingEffects::Create(L"../bin/resources/meshes/effects/Small/SM_EFF_Leaf_01.fbx", iNumInstance, tData, Convert_ToHash(L"LeafBomb")))))
		return E_FAIL;


	tData.fCreateRange = 10.f;
	tData.fCreateRangeY = 10.f;
	tData.fGravity = 1.f;
	tData.fGravityRange = 0.9f;
	tData.vFadeInTargetScale = _float4(0.7f, 0.7f, 0.7f, 1.f);

	iNumInstance = 200;
	tData.iNumInstance = iNumInstance;
	if (FAILED(Add_Effect(Convert_ToHash(L"BurgerLeaves"), CInstancingEffects::Create(L"../bin/resources/meshes/effects/Small/SM_EFF_Leaf_01.fbx", iNumInstance, tData, Convert_ToHash(L"BurgerLeaves")))))
		return E_FAIL;


	//SmallRock

	tData.fCreateRange = 0.3f;
	tData.fCreateRangeY = 0.f;
	tData.fCurGroundY = 0.f;
	tData.fCurGroundYRange = 0.f;
	tData.fGravity = 7.8f;
	tData.fGravityRange = 1.f;
	tData.fJumpPower = 4.f;
	tData.fJumpPowerRange = 2.f;
	tData.fSpeed = 3.f;
	tData.fSpeedRange = 2.f;
	tData.fTurnSpeed = 3.f;
	tData.fTurnSpeedRange = 1.f;
	tData.vFadeInTargetScale = _float4(0.5f, 0.5f, 0.5f, 1.f);
	tData.vFadeInTargetScaleRange = _float4(0.2f, 0.2f, 0.2f, 1.f);

	iNumInstance = 6;
	tData.iNumInstance = iNumInstance;

	if (FAILED(Add_Effect(Convert_ToHash(L"SmallRock"), CInstancingEffects::Create(L"../bin/resources/meshes/effects/Small/SM_EFF_SmallRock_01.fbx", iNumInstance, tData, Convert_ToHash(L"SmallRock")))))
		return E_FAIL;


	//Big - SmallRock

	tData.fCreateRange = 1.f;
	tData.fCreateRangeY = 0.f;
	tData.fCurGroundY = 0.f;
	tData.fCurGroundYRange = 0.f;
	tData.fGravity = 7.8f;
	tData.fGravityRange = 1.f;
	tData.fJumpPower = 6.f;
	tData.fJumpPowerRange = 2.f;
	tData.fSpeed = 5.f;
	tData.fSpeedRange = 3.f;
	tData.fTurnSpeed = 3.f;
	tData.fTurnSpeedRange = 1.f;
	tData.vFadeInTargetScale = _float4(1.f, 1.f, 1.f, 1.f);
	tData.vFadeInTargetScaleRange = _float4(0.2f, 0.2f, 0.2f, 1.f);

	iNumInstance = 60;
	tData.iNumInstance = iNumInstance;

	if (FAILED(Add_Effect(Convert_ToHash(L"BigSmallRock"), CInstancingEffects::Create(L"../bin/resources/meshes/effects/Small/SM_EFF_SmallRock_01.fbx", iNumInstance, tData, Convert_ToHash(L"BigSmallRock")))))
		return E_FAIL;



	//WOodWall

	tData.fCreateRange = 3.f;
	tData.fCreateRangeY = 1.f;
	tData.fCurGroundY = -0.5f;
	tData.fCurGroundYRange = 0.f;
	tData.fGravity = 5.8f;
	tData.fGravityRange = 1.f;
	tData.fJumpPower = 3.f;
	tData.fJumpPowerRange = 1.f;
	tData.fSpeed = 0.1f;
	tData.fSpeedRange = 0.1f;
	tData.fTurnSpeed = 3.f;
	tData.fTurnSpeedRange = 1.f;
	tData.vFadeInTargetScale = _float4(1.5f, 1.5f, 1.5f, 1.f);
	tData.vFadeInTargetScaleRange = _float4(0.2f, 0.2f, 0.2f, 1.f);

	iNumInstance = 20;
	tData.iNumInstance = iNumInstance;
	CInstancingEffects* pWoodWallParticle = CInstancingEffects::Create(L"../bin/resources/meshes/effects/ninjutsu/woodhand/SM_WEP_Eff_DomedWall_D_01.fbx", iNumInstance, tData, Convert_ToHash(L"WoodWallParticle"));
	CTexture* pTexture = GET_COMPONENT_FROM(pWoodWallParticle, CModel)->Get_Materials().front().second.pTextures[1];
	pTexture->Add_Texture(L"../bin/resources/textures/animmodeltextures/T_EFF_GMS_Sand_04_BC.png");
	pTexture->Set_CurTextureIndex(1);

	if (FAILED(Add_Effect(Convert_ToHash(L"WoodWallParticle"), pWoodWallParticle)))
		return E_FAIL;





	//ChidoriText

	tData.fCreateRange = 0.3f;
	tData.fCreateRangeY = 0.f;
	tData.fCurGroundY = 0.f;
	tData.fCurGroundYRange = 0.f;
	tData.fGravity = 3.8f;
	tData.fGravityRange = 1.f;
	tData.fJumpPower = 2.5f;
	tData.fJumpPowerRange = 1.5f;
	tData.fSpeed = 3.f;
	tData.fSpeedRange = 2.f;
	tData.fTurnSpeed = 0.f;
	tData.fTurnSpeedRange = 0.f;
	tData.vFadeInTargetScale = _float4(0.7f, 0.7f, 0.7f, 1.f);
	tData.vFadeInTargetScaleRange = _float4(0.1f, 0.1f, 0.1f, 1.f);
	iNumInstance = 2;

	tData.iNumInstance = iNumInstance;

	/* TEXT */

	if (FAILED(Add_Effect(Convert_ToHash(L"ChidoriText"), CInstancingEffects::Create(L"../bin/resources/meshes/effects/Text/SM_EFF_Text_Chidori_02.fbx", iNumInstance, tData, Convert_ToHash(L"ChidoriText")))))
		return E_FAIL;

	iNumInstance = 2;
	tData.fJumpPower = 4.5f;
	tData.fJumpPowerRange = 2.f;
	tData.iNumInstance = iNumInstance;
	tData.fCreateRange = 1.f;
	tData.fCreateRangeY = 1.f;
	tData.fCurGroundY = -200.f;
	tData.vFadeInTargetScale = _float4(0.8f, 0.8f, 0.8f, 1.f);
	tData.vFadeInTargetScaleRange = _float4(0.2f, 0.2f, 0.2f, 1.f);

	tData.fFadeOutStartTime = 0.5f;
	tData.fFadeOutStartTimeRange = 0.1f;

	tData.fFadeOutTime = 0.25f;
	tData.fFadeOutTimeRange = 0.1f;

	tData.vFadeOutTargetScale = _float4(0.1f, 0.1f, 0.1f, 1.f);


	if (FAILED(Add_Effect(Convert_ToHash(L"KamuiText"), CInstancingEffects::Create(L"../bin/resources/meshes/effects/Text/SM_EFF_Text_Reanimation_01.fbx",
		iNumInstance, tData, Convert_ToHash(L"KamuiText")))))
		return E_FAIL;

	if (FAILED(Add_Effect(Convert_ToHash(L"KOTEXT"), CEffect_Text::Create(L"../bin/resources/meshes/effects/Text/SM_EFF_Text_CherryBlossomClash_01.fbx", Convert_ToHash(L"KOTEXT")))))
		return E_FAIL;

	if (FAILED(Add_Effect(Convert_ToHash(L"DODGETEXT"), CEffect_Text::Create(L"../bin/resources/meshes/effects/Text/SM_EFF_Text_kawarimi_01.fbx", Convert_ToHash(L"DODGETEXT")))))
		return E_FAIL;

	CEffect_Text* pText = CEffect_Text::Create(L"../bin/resources/meshes/effects/Text/SM_EFF_Text_Rasengan_01.fbx", Convert_ToHash(L"RASENGANTEXT"));
	pText->Set_GlowFlag(_float4(0.3f, 0.3f, 0.3f, 1.f));
	pText->Set_OffsetPos(_float4(0.f, 1.5f, 1.f, 1.f));
	if (FAILED(Add_Effect(Convert_ToHash(L"RASENGANTEXT"), pText)))
		return E_FAIL;


	pText = CEffect_Text::Create(L"../bin/resources/meshes/effects/Text/SM_EFF_Text_TrueRasenShuriken_01.fbx", Convert_ToHash(L"RASENSHURIKENTEXT"));
	pText->Set_OffsetPos(_float4(3.5f, 1.f, 2.5f, 1.f));
	pText->Set_GlowFlag(_float4(1.f, 1.f, 1.f, 1.f));

	if (FAILED(Add_Effect(Convert_ToHash(L"RASENSHURIKENTEXT"), pText)))
		return E_FAIL;

	if (FAILED(Add_Effect(Convert_ToHash(L"CHIDORITEXT1"), CEffect_Text::Create(L"../bin/resources/meshes/effects/Text/SM_EFF_Text_Chidori_01.fbx", Convert_ToHash(L"CHIDORITEXT1")))))
		return E_FAIL;


	pText = CEffect_Text::Create(L"../bin/resources/meshes/effects/Text/SM_EFF_Text_Gokakyu_01.fbx", Convert_ToHash(L"FIRETEXT"));

	pText->Set_OffsetPos(_float4(0.f, 2.f, 1.f, 1.f));

	if (FAILED(Add_Effect(Convert_ToHash(L"FIRETEXT"), pText)))
		return E_FAIL;



	pText = CEffect_Text::Create(L"../bin/resources/meshes/effects/Text/SM_EFF_Text_LaughingMonk_01.fbx", Convert_ToHash(L"LaughingMonkText1"));
	pText->Set_OffsetPos(_float4(0.f, 2.f, 1.f, 1.f));
	if (FAILED(Add_Effect(Convert_ToHash(L"LaughingMonkText1"), pText)))
		return E_FAIL;


	pText = CEffect_Text::Create(L"../bin/resources/meshes/effects/Text/SM_EFF_Text_LaughingMonk_02.fbx", Convert_ToHash(L"LaughingMonkText2"));
	pText->Set_OffsetPos(_float4(0.f, 2.f, 1.f, 1.f));
	if (FAILED(Add_Effect(Convert_ToHash(L"LaughingMonkText2"), pText)))
		return E_FAIL;



	if (FAILED(SetUp_MultiEffects()))
		return E_FAIL;

	return S_OK;
}

void CEffects_Factory::On_EnterLevel()
{
	for (auto& hcCode : m_EffectsResourcesCode)
	{
		CGameObject* pGameObject = nullptr;
		pGameObject = GAMEINSTANCE->Clone_GameObject(hcCode);
		if (!pGameObject)
			continue;
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		DISABLE_GAMEOBJECT(pGameObject);
	}

}

void CEffects_Factory::On_ExitLevel()
{
	m_Effects.clear();
}

list<CGameObject*> CEffects_Factory::Create_MultiEffects(wstring wstrKey, CGameObject* pFollowTarget, _float4 vPos)
{
	list<CGameObject*> EffectsList;

	if (m_MultiEffects.find(Convert_ToHash(wstrKey)) == m_MultiEffects.end())
	{
		Call_MsgBox(L"Cant Find MultiEffects");
		return EffectsList;
	}

	list<_hashcode> hcEffects = m_MultiEffects.find(Convert_ToHash(wstrKey))->second;

	for (auto& hcCode : hcEffects)
	{
		EffectsList.push_back(Create_Effects(hcCode, pFollowTarget, vPos));
	}

	return EffectsList;
}

CGameObject* CEffects_Factory::Create_Effects(_hashcode _hcCode, CGameObject* pFollowTarget, _float4 vPos)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		static_cast<CEffect*>(pGameObject)->Self_Reset(pFollowTarget, vPos);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		pEffect->Self_Reset(pFollowTarget, vPos);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_Effects(_hashcode _hcCode, _float4 vPos)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CEffect*>(pGameObject)->Reset(vPos);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		pEffect->Reset(vPos);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_Effects(_hashcode _hcCode, _float4x4 worldMat)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CEffect*>(pGameObject)->Reset(worldMat);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		pEffect->Reset(worldMat);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_Effects_Turn(_hashcode _hcCode, CGameObject* pFollowTarget, _float4 vPos)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		static_cast<CEffect*>(pGameObject)->Self_Reset_Turn(pFollowTarget, vPos);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		pEffect->Self_Reset_Turn(pFollowTarget, vPos);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_LandingEffects(_float4 vPos, CCell* pCurCell)
{
	CGameObject* pGameObject = nullptr;

	if (pCurCell->IsGrass())
	{
		pGameObject = Create_Effects(Convert_ToHash(L"Grass"), vPos);
	}
	else if (pCurCell->IsGround())
	{
		pGameObject = Create_Effects(Convert_ToHash(L"SmallRock"), vPos);

	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_BigLandingEffects(_float4 vPos, CCell* pCurCell)
{
	CGameObject* pGameObject = nullptr;

	if (pCurCell->IsGrass())
	{
		pGameObject = Create_Effects(Convert_ToHash(L"BigGrass"), vPos);
	}
	else if (pCurCell->IsGround())
	{
		pGameObject = Create_Effects(Convert_ToHash(L"BigSmallRock"), vPos);

	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_EightGatesLandingEffects(_float4 vPos, CCell* pCurCell)
{
	CGameObject* pGameObject = nullptr;

	if (pCurCell->IsGrass())
	{
		pGameObject = Create_Effects(Convert_ToHash(L"EightGatesGrass"), vPos);
	}
	else if (pCurCell->IsGround())
	{
		pGameObject = Create_Effects(Convert_ToHash(L"EightGatesRock"), vPos);

	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_ScreenEffects(CGameObject* pFollowTarget)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[HASHCODE(CScreenEffect)].empty())
	{
		return nullptr;
	}
	else
	{
		CEffect* pEffect = m_Effects[HASHCODE(CScreenEffect)].front();
		pEffect->Reset(pFollowTarget);
		m_Effects[HASHCODE(CScreenEffect)].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;
}

CGameObject* CEffects_Factory::Create_Effects(_hashcode _hcCode, _float4 vPos, _float4 vTargetPosition)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CEffect*>(pGameObject)->Reset(vPos, vTargetPosition);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		pEffect->Reset(vPos, vTargetPosition);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;

}

CGameObject* CEffects_Factory::Create_Effects(_hashcode _hcCode, CGameObject* pFollowTarget)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
		static_cast<CEffect*>(pGameObject)->Reset(pFollowTarget);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		pEffect->Reset(pFollowTarget);
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
	}

	return pGameObject;

}

CGameObject* CEffects_Factory::Create_Effects(_hashcode _hcCode, CGameObject* pOwner, CHierarchyNode* pRightHandBone)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[_hcCode].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(_hcCode);
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
	}
	else
	{
		CEffect* pEffect = m_Effects[_hcCode].front();
		m_Effects[_hcCode].pop_front();
		pGameObject = pEffect;
		ENABLE_GAMEOBJECT(pEffect);
	}

	static_cast<CEffect*>(pGameObject)->Set_FollowTarget(pOwner);
	static_cast<CEffect*>(pGameObject)->Set_RefBone(pRightHandBone);

	return static_cast<CWindmill*>(pGameObject);
}

CWindmill* CEffects_Factory::Create_Windmill(CGameObject* pOwner, CHierarchyNode* pRightHandBone)
{
	CGameObject* pGameObject = nullptr;

	if (m_Effects[HASHCODE(CWindmill)].empty())
	{
		pGameObject = GAMEINSTANCE->Clone_GameObject(HASHCODE(CWindmill));
		//없으면 새로 집어넣음
		pGameObject->Initialize();
		CREATE_GAMEOBJECT(pGameObject, GROUP_EFFECT);
	}
	else
	{
		CEffect* pEffect = m_Effects[HASHCODE(CWindmill)].front();
		m_Effects[HASHCODE(CWindmill)].pop_front();
		pGameObject = pEffect;
		ENABLE_GAMEOBJECT(pEffect);
	}

	static_cast<CEffect*>(pGameObject)->Set_FollowTarget(pOwner);
	static_cast<CEffect*>(pGameObject)->Set_RefBone(pRightHandBone);

	return static_cast<CWindmill*>(pGameObject);
}

HRESULT CEffects_Factory::Add_Effect(_hashcode _hcCode, CEffect* pEffect)
{
	if (!pEffect)
		return E_FAIL;

	/*if (FAILED(pEffect->Initialize()))
		return E_FAIL;*/

	if (FAILED(GAMEINSTANCE->Add_GameObject_Prototype(pEffect, _hcCode)))
		return E_FAIL;

	m_Effects.emplace(_hcCode, list<CEffect*>());


	return S_OK;
}

_bool CEffects_Factory::Find_Effect(_hashcode _hcCode)
{
	auto iter = m_Effects.find(_hcCode);

	if (iter == m_Effects.end())
		return false;

	return true;
}

void CEffects_Factory::Collect_Effect(_hashcode _hcCode, CEffect* pEffect)
{
	m_Effects[_hcCode].push_back(pEffect);
}

list<string> CEffects_Factory::Read_AllEffectsFile(const char* pFolderPath)
{
	list<string>	EffectsTemp;

	for (filesystem::directory_iterator FileIter(pFolderPath);
		FileIter != filesystem::end(FileIter); ++FileIter)
	{
		const filesystem::directory_entry& entry = *FileIter;

		wstring wstrPath = entry.path().relative_path();
		string strFullPath;
		strFullPath.assign(wstrPath.begin(), wstrPath.end());

		_int iFind = (_int)strFullPath.rfind("\\") + 1;
		string strFileName = strFullPath.substr(iFind, strFullPath.length() - iFind);

		_int iFind2 = (_int)strFileName.find(".");
		string strFileKey = strFileName.substr(0, iFind2);

		CEffect* pNewEffect = CEffect::Create_Effect_FromBinFile(strFileKey);

		if (!pNewEffect)
		{
			Call_MsgBox(L"Failed to Create_Effect");
			return EffectsTemp;

		}

		_hashcode hcCode = Convert_ToHash(CFunctor::To_Wstring(strFileKey));

		if (FAILED(Add_Effect(hcCode, pNewEffect)))
		{
			Call_MsgBox(L"Failed to Add_Effect");
			return EffectsTemp;
		}

		EffectsTemp.push_back(strFileKey);
		m_EffectsResourcesCode.push_back(hcCode);
	}







	return EffectsTemp;
}

HRESULT CEffects_Factory::Add_MultiEffects(_hashcode MultiEffectsName, _hashcode EffectsCode)
{
	/*GAMEINSTANCE->Find_GameObject_Prototype()

	auto iter = m_Effects.find(EffectsCode);

	if (iter == m_Effects.end())
	{
		Call_MsgBox(L"No Effects : Add_MultiEffects");
		return E_FAIL;
	}*/

	m_MultiEffects[MultiEffectsName].push_back(EffectsCode);

	return S_OK;

}

HRESULT CEffects_Factory::Combine_EffectsGroup(list<string>& listTemp, _hashcode MultiEffectsName, string strKey)
{
	//폴더 뒤져서 이름 다 가져오기

	//strKey가 있으면 넣어

	for (auto& elem : listTemp)
	{
		_int iFind = elem.find(strKey);
		if (iFind >= 0)
		{
			if (FAILED(Add_MultiEffects(MultiEffectsName, Convert_ToHash(CFunctor::To_Wstring(elem)))))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CEffects_Factory::SetUp_MultiEffects()
{
	list<string>	listTemp = Read_AllEffectsFile("../bin/effects");

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"CJ_Start"), "CJ_Start")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"CJ_MaxCharge"), "CJ_MaxCharge")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"LeafHurricane"), "LeafHurricane")))
		return E_FAIL;

	if (FAILED(Add_MultiEffects(Convert_ToHash(L"LeafHurricane"), Convert_ToHash(L"LeafBomb"))))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"HandHitFinish"), "HandHit")))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"HandHitFinish"), Convert_ToHash(L"Hit_Distortion"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"HandHit"), Convert_ToHash(L"Hit_Distortion_1"))))
		return E_FAIL;
	/* HandHit */
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"HandHit"), Convert_ToHash(L"HandHit_1"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"HandHit"), Convert_ToHash(L"HandHit_2"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"HandHit"), Convert_ToHash(L"Hit_Distortion"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"HandHit"), Convert_ToHash(L"Hit_Distortion_1"))))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"NSHit"), "NSHit")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"Smoke"), "Smoke")))
		return E_FAIL;

	/* Dash Effects */
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"Dash_Effects_Air"), Convert_ToHash(L"AirDash_0"))))
		return E_FAIL;
	/*if (FAILED(Add_MultiEffects(Convert_ToHash(L"Dash_Effects_Air"), Convert_ToHash(L"Dash_Effect"))))
		return E_FAIL;*/

	if (FAILED(Add_MultiEffects(Convert_ToHash(L"Dash_Effects_Ground"), Convert_ToHash(L"AirDash_0"))))
		return E_FAIL;
	/*if (FAILED(Add_MultiEffects(Convert_ToHash(L"Dash_Effects_Ground"), Convert_ToHash(L"Dash_Effect_Ground"))))
		return E_FAIL;*/

	/* Rasengan */
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"RasenganStart"), Convert_ToHash(L"CJ_LoopEffect_0"))))
		return E_FAIL;

	if (FAILED(Add_MultiEffects(Convert_ToHash(L"RasenganStart"), Convert_ToHash(L"RASENGANTEXT"))))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"RasenganStart"), "Rasengan_Start")))
		return E_FAIL;


	/*
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"RasenganStart"), Convert_ToHash(L"CJ_MaxCharge_1"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"RasenganStart"), Convert_ToHash(L"CJ_MaxCharge_2"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"RasenganStart"), Convert_ToHash(L"CJ_MaxCharge_3"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"RasenganStart"), Convert_ToHash(L"CJ_MaxCharge_4"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"RasenganStart"), Convert_ToHash(L"CJ_MaxCharge_5"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"RasenganStart"), Convert_ToHash(L"CJ_LoopEffect_0"))))
		return E_FAIL;*/
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"RasenganStart"), Convert_ToHash(L"Rasengan_Ball"))))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"RasenganRun"), "Rasengan_Run")))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"RasenganRun"), Convert_ToHash(L"Rasengan_Ball"))))
		return E_FAIL;


	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"RasenganAttack"), "RasenganAttack")))
		return E_FAIL;


	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"ChidoriStart"), "Chidori")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"ChidoriRun"), "ChidoRun")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"ChidoriDistortion"), "ChidoDistortion")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"ChidoriHitParticle"), "ChidoHitParticle")))
		return E_FAIL;


	/* Fire Ball */
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"FireBalls"), "FireBall_Ball")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"FireBallParticle"), "FireBall_Particle")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"FireBallStart"), "FireBall_Start")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"FireBallLaunch"), "FireBall_Launch")))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"FireBallLaunch"), Convert_ToHash(L"FIRETEXT"))))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"FireBallExplosion"), "FireBall_Explosion")))
		return E_FAIL;


	/*Laughing Monk */
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"WoodHands"), HASHCODE(CWoodHand))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"WoodHands"), HASHCODE(CWoodHand_R))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"WoodHands"), Convert_ToHash(L"LaughingMonkText1"))))
		return E_FAIL;


	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"BigSmoky"), "BigSmoky")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"LaughingMonkHit"), "LaughingMonkHit")))
		return E_FAIL;

	/* Wood Wall */
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"WoodWallSmoke"), "WallSmoky")))
		return E_FAIL;

	/*if (FAILED(Add_MultiEffects(Convert_ToHash(L"FireBallExplosion"), Convert_ToHash(L"FireDecal"))))
		return E_FAIL;*/

		/* Rasen Shuriken */
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"RasenShuriken_Start"), "RasenShuriken_Start")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"RasenShuriken_Ball"), "RasenShuriken_Ball")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"RasenShuriken_Particle"), "RasenShuriken_Particle")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"RasenShuriken_Attack"), "RasenSurikenAttack")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"RasenShuriken_Bomb"), "RasenSurikenBomb")))
		return E_FAIL;

	/* KAMUI */

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"KamuiEnter"), "KamuiEnter")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"KamuiAttack"), "KamuiAttack")))
		return E_FAIL;

	/* EightGates */

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"EightGatesStart"), "EightGatesStart")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"EightGatesLoop"), "EightGatesLoop")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"EightGatesParticle"), "EightGatesParticle")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"EightGatesLaunch"), "EightGates_Launch")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"EightGatesAttack"), "EightGatesAttack")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"EightGatesHit"), "EightGatesHit")))
		return E_FAIL;

	if (FAILED(Add_MultiEffects(Convert_ToHash(L"EightGatesDash"), Convert_ToHash(L"EightGates_Launch_0"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"EightGatesDash"), Convert_ToHash(L"EightGates_Launch_2"))))
		return E_FAIL;
	if (FAILED(Add_MultiEffects(Convert_ToHash(L"EightGatesDash"), Convert_ToHash(L"EightGates_Launch_3"))))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"NightGuyLoop"), "NightGuy_Loop")))
		return E_FAIL;

	/* */

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"GroundSmoke"), "GroundSmoky")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"WallDust"), "WallDust")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"RaijinEffect"), "RaijinEffect")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"BossAuraLoop"), "BossAuraLoop")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"BossAuraParticle"), "BossAuraParticle")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"BossAuraStart"), "BossAuraStart")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"KOEffect"), "KOEffect")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"BurgerStart"), "BurgerStart")))
		return E_FAIL;

	/*Water*/
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"WaterPrison_Start"), "WaterPrison_Start")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"WaterBall"), "WaterTest")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"WaterParticle"), "WaterParticle")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"SharkBomb"), "SharkBomb")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"SharkParticle"), "SharkParticle")))
		return E_FAIL;
	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"SharkBoom"), "SharkBoom")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"WaterUlt"), "WaterUlt")))
		return E_FAIL;

	if (FAILED(Combine_EffectsGroup(listTemp, Convert_ToHash(L"WaterCam"), "WaterCam")))
		return E_FAIL;

	return S_OK;
}
