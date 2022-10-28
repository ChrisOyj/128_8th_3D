#include "CResource_Material.h"

#include "CUtility_File.h"

CResource_Material::CResource_Material()
{
}

CResource_Material::~CResource_Material()
{
}

CResource_Material* CResource_Material::Create(ifstream* pReadFile, string strName)
{
	CResource_Material* pResource = new CResource_Material;

	if (FAILED(pResource->Initialize(pReadFile, strName)))
	{
		Call_MsgBox(L"Failed to Initiailize : CResource_Material");
		SAFE_DELETE(pResource);
	}

	return pResource;
}

HRESULT CResource_Material::Initialize(ifstream* pReadFile, string strName)
{
	__super::Initialize(pReadFile, strName);

	for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
	{
		m_tMatDesc.TexturePaths[i] = CUtility_File::Read_Text(pReadFile);
	}

	if (!m_tMatDesc.TexturePaths[6].empty())
	{
		string strTemp = m_tMatDesc.TexturePaths[6];
		_int iFind = 0;
		iFind = (_int)strTemp.rfind("CopicMask");
		if (iFind != -1)
		{
			if ((_int)strTemp.rfind("WoodBoard") == -1)
			{
				m_tMatDesc.TexturePaths[6].clear();
			}

			m_tMatDesc.TexturePaths[aiTextureType_DIFFUSE_ROUGHNESS] = "../bin/resources/textures/modeltextures/T_ENV_KNVLLG_Noize_02_UC.dds";
		}
		else if ((_int)strTemp.rfind("Hatching") != -1)
		{
			//m_tMatDesc.TexturePaths[6].clear();
			m_tMatDesc.TexturePaths[aiTextureType_DIFFUSE_ROUGHNESS] = "../bin/resources/textures/modeltextures/T_ENV_KNVLLG_Noize_05_UC.dds";
		}
		else if ((_int)strTemp.rfind("Ground") != -1)
		{
			m_tMatDesc.TexturePaths[6].clear(); //= "../bin/resources/textures/modeltextures/T_ENV_LKNVLD_Ground_03_N.dds";
			m_tMatDesc.TexturePaths[aiTextureType_DIFFUSE_ROUGHNESS] = "../bin/resources/textures/modeltextures/T_ENV_KNVLLG_Noize_06_UC.dds";
		}
		else if ((_int)strTemp.rfind("Sample") != -1)
		{
			m_tMatDesc.TexturePaths[6].clear(); //= "../bin/resources/textures/modeltextures/T_ENV_LKNVLD_Ground_03_N.dds";
			m_tMatDesc.TexturePaths[aiTextureType_DIFFUSE_ROUGHNESS] = "../bin/resources/textures/modeltextures/T_ENV_KNVLLG_Noize_06_UC.dds";
		}
		else
			m_tMatDesc.TexturePaths[aiTextureType_DIFFUSE_ROUGHNESS] = "../bin/resources/textures/materials/T_noise_Type_2.dds";

	}
	else
	{
		m_tMatDesc.TexturePaths[aiTextureType_DIFFUSE_ROUGHNESS] = "../bin/resources/textures/materials/T_noise_Type_2.dds";

	}

	m_tMatDesc.vMixColor = Check_FileName(m_strName);

	return S_OK;
}

_float4 CResource_Material::Check_FileName(string strName)
{
	_float4 vReturnColor = _float4(1.f, 1.f, 1.f, 1.f);

	//Model
	if (strName == "MI_CHR_Face")
	{
		vReturnColor = _float4(1.f, 0.9f, 0.8f, 1.f);
		//m_tMatDesc.TexturePaths[aiTextureType_DIFFUSE_ROUGHNESS] = "../bin/resources/textures/materials/T_Faceparts_base_coptes.dds";

	}
	else if (strName == "MI_CHR_Man_ShortSpikyHair_04_Metal")
	{
		vReturnColor = _float4(0.1f, 0.1f, 0.4f, 1.f);
	}
	else if (strName == "MI_CHR_Man_Beach_01_C")
	{
		vReturnColor = _float4(1.15f, 1.15f, 1.15f, 1.f);
	}
	else if (strName == "MI_CHR_Man_Beach_01_B")
	{
		vReturnColor = _float4(1.15f, 1.15f, 1.15f, 1.f);
	}
	else if (strName == "M_ENV_LKNVLD_PottedPlants")
	{
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/modeltextures/T_ENV_KNVLLG_PropAtlas_BC.dds";
	}
	else if (strName == "MI_ENV_KNVLLG_AnimatedScreen_01")
	{
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/modeltextures/jusin0.png";
		m_tMatDesc.TexturePaths[2] = "../bin/resources/textures/modeltextures/jusin1.png";
		m_tMatDesc.TexturePaths[3] = "../bin/resources/textures/modeltextures/jusin2.png";
	}

	else if (strName == "MI_EFF_GroundBreak_A_01")
	{
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/effects/decal/T_EFF_GroundBreak_A_01_BC.dds";
		m_tMatDesc.TexturePaths[aiTextureType_NORMALS] = "../bin/resources/textures/effects/decal/T_EFF_GroundBreak_A_01_N.dds";
		m_tMatDesc.TexturePaths[aiTextureType_DIFFUSE_ROUGHNESS] = "../bin/resources/textures/effects/GradationColor/T_EFF_GMS_Ground_01.png";

	}
	else if (strName == "MI_EFF_GroundBreak_B_01")
	{
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/effects/decal/T_EFF_GroundBreak_B_01_BC.dds";
		m_tMatDesc.TexturePaths[aiTextureType_NORMALS] = "../bin/resources/textures/effects/decal/T_EFF_GroundBreak_B_01_N.dds";
		m_tMatDesc.TexturePaths[aiTextureType_DIFFUSE_ROUGHNESS] = "../bin/resources/textures/effects/GradationColor/T_EFF_GMS_Ground_01.png";

	}

	/*else if (strName == "MI_ENV_KNVLLG_StoneWall_Blend")
	{
		m_tMatDesc.TexturePaths[aiTextureType_NORMALS].clear();
	}*/
	//Grass
	else if (strName == "MI_CHR_Man_ShortSpikyHair_06" || strName == "MI_CHR_Man_TopKnot_01")
	{
		vReturnColor = _float4(0.4f, 0.34f, 0.4f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Windows_A" || strName == "MI_ENV_KNVLLG_Windows_B")
	{
		vReturnColor = _float4(1.1f, 1.1f, 1.2f, 1.f);
	}
	//ConcreteSlabs
	else if (strName == "MI_ENV_KNVLLG_ConcreteSlabs_Bright" || strName == "MI_ENV_TCHEXA_ConcreteSlabs_Bright")
	{
		vReturnColor = _float4(0.3f, 0.28f, 0.238f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_ConcreteSlabs_Dark2" || strName == "MI_ENV_TCHEXA_ConcreteSlabs_Dark")
	{
		vReturnColor = _float4(0.18f, 0.16807f, 0.1431f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_ConcreteSlabs_Light")
	{
		vReturnColor = _float4(0.53f, 0.494872f, 0.42135f, 1.f);
	}
	else if (strName == "MI_ENV_TCHEXA_ConcreteSlabs_Dark")
	{
		vReturnColor = _float4(0.53f, 0.494872f, 0.42135f, 1.f);
	}
	else if (strName == "MI_ENV_TCHEXA_ConcreteSlabs_Light_01")
	{
		vReturnColor = _float4(0.34f, 0.29493f, 0.2006f, 1.f);
	}
	else if (strName == "MI_ENV_TCHEXA_ConcreteSlabs_Red")
	{
		vReturnColor = _float4(0.13, 0.096146f, 0.0715f, 1.f);
	}

	//Metal Roof
	else if (strName == "MI_ENV_KNVLLG_MetalRoof_BlueLight")
	{
		vReturnColor = _float4(0.175225f, 0.694128f, 0.701102f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_MetalRoof_BlueMarine")
	{
		vReturnColor = _float4(0.28835f, 0.455566f, 0.79f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_MetalRoof_Red")
	{
		vReturnColor = _float4(1.1f, 0.228067f, 0.198f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_MetalRoof_GreenLight")
	{
		vReturnColor = _float4(0.256f, 0.64f, 0.2944f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_MetalRoof_Orange" || strName == "MI_ENV_TCHEXA_MetalRoof_Orange")
	{
		vReturnColor = _float4(1.1f, 0.4939f, 0.143f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_MetalRoof_Purple")
	{
		vReturnColor = _float4(0.617187f, 0.37525f, 0.79f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_MetalRoof_Yellow")
	{
		vReturnColor = _float4(1.2f, 0.749147f, 0.156f, 1.f);
	}
	else if (strName == "MI_ENV_TCHEXA_MetalRoof_Dark")
	{
		vReturnColor = _float4(0.365f, 0.294623f, 0.218287f, 1.f);
	}
	else if (strName == "MI_ENV_TCHEXA_MetalRoof_White")
	{
		vReturnColor = _float4(0.8525f, 1.1f, 1.03192f, 1.f);
	}

	//pipe
	else if (strName == "MI_ENV_KNVLLG_Pipe_01")
	{
		vReturnColor = _float4(0.21f, 0.05563f, 0.033f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Pipe_02")
	{
		vReturnColor = _float4(0.085f, 0.03116f, 0.02635f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Pipe_04")
	{
		vReturnColor = _float4(0.0574f, 0.087687f, 0.14f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Pipe_05")
	{
		vReturnColor = _float4(0.18f, 0.118125f, 0.0675f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Pipe_Dark")
	{
		vReturnColor = _float4(0.073525f, 0.08072f, 0.085f, 1.f);
	}
	else if (strName == "T_ENV_TCHEXA_Pipe_Brown")
	{
		vReturnColor = _float4(0.09f, 0.070633f, 0.05805f, 1.f);
	}

	//pipe Details
	else if (strName == "MI_ENV_KNVLLG_PipeDetails_01")
	{
		vReturnColor = _float4(0.15f, 0.132303f, 0.117f, 1.f);
	}

	//plaster
	else if (strName == "MI_ENV_KNVLLG_Plaster")
	{
		vReturnColor = _float4(0.44f, 0.408388f, 0.3476f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Plaster_Green")
	{
		vReturnColor = _float4(0.347905f, 0.59f, 0.33335f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Plaster_Light" || strName == "MI_ENV_TCHEXA_Plaster_Light")
	{
		vReturnColor = _float4(0.9f, 0.858433f, 0.7785f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Plaster_LightBlue")
	{
		vReturnColor = _float4(0.410263f, 0.660724f, 0.73f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Plaster_Orange")
	{
		vReturnColor = _float4(0.44f, 0.228067f, 0.066f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Plaster_Red")
	{
		vReturnColor = _float4(0.448f, 0.10752f, 0.113195f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Plaster_Yellow")
	{
		vReturnColor = _float4(0.635f, 0.490531f, 0.212725f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Plaster_Dark")
	{
		vReturnColor = _float4(0.1f, 0.092815f, 0.079f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Plaster_DarkGrey")
	{
		vReturnColor = _float4(0.17f, 0.157785f, 0.1343f, 1.f);
	}
	else if (strName == "MI_ENV_TCHEXA_Plaster_White")
	{
	vReturnColor = _float4(0.399634f, 0.44f, 0.3454f, 1.f);
	}

	//Wood Plain
	else if (strName == "MI_ENV_KNVLLG_WoodPlain_Red")
	{
		vReturnColor = _float4(0.685f, 0.174675f, 0.174675f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_WoodPlain_Yellow")
	{
		vReturnColor = _float4(1.f, 0.612888f, 0.f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_WoodPlain_Black")
	{
		vReturnColor = _float4(0.14f, 0.14f, 0.14f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_WoodPlain_Brown")
	{
		vReturnColor = _float4(0.55f, 0.295724f, 0.18975f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_WoodPlain_LightBrown")
	{
		vReturnColor = _float4(1.f, 0.749386f, 0.42218f, 1.f);
	}

	//wall
	else if (strName == "MI_ENV_KNVLLG_Wall_A")
	{
		vReturnColor = _float4(0.671625f, 0.800975f, 0.995f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Stonewall_04")
	{
		vReturnColor = _float4(0.625f, 0.625f, 0.625f, 1.f);
	}
	else if (strName == "T_ENV_TCHEXA_WallBoards_01")
	{
	vReturnColor = _float4(0.485f, 0.325478f, 0.2522f, 1.f);
	}


	//window
	else if (strName == "MI_ENV_KNVLLG_WoodWindow_A")
	{
		vReturnColor = _float4(0.845f, 0.792526f, 0.682698f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Window_Paper")
	{
		vReturnColor = _float4(0.4f, 0.391209f, 0.356f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_Windows_A" || strName == "MI_ENV_KNVLLG_Windows_B")
	{
		vReturnColor = _float4(0.321751f, 0.568852f, 0.92f, 1.f);
	}

	//wood board
	else if (strName == "MI_ENV_KNVLLG_WoodBoard_Natural_Lighter" || strName == "MI_ENV_TCHEXA_WoodBoard_Natural_Lighter")
	{
		vReturnColor = _float4(0.565f, 0.346465f, 0.243387f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_WoodBoard_Natural_Red")
	{
		vReturnColor = _float4(0.708f, 0.13098f, 0.13098f, 1.f);
	}
	else if (strName == "MI_ENV_KNVLLG_WoodBoard_Natural_Yellowed")
	{
		vReturnColor = _float4(1.f, 0.749386f, 0.42218f, 1.f);
	}


	//roof tile
	else if (strName == "MI_ENV_TCHEXA_RoofTile_Orange")
	{
		vReturnColor = _float4(0.9f, 0.415624f, 0.063f, 1.f);
	}
	else if (strName == "MI_ENV_TCHEXA_RoofTile_White")
	{
		vReturnColor = _float4(0.9f, 0.768267f, 0.7335f, 1.f);
	}

	// leaf
	else if (strName == "MI_ENV_TCHEXA_Leaf_A_01")
	{
		vReturnColor = _float4(0.441546f, 0.635f, 0.441325f, 1.f);
	}


	else if (strName == "MI_ENV_KNVLLG_Road_Simple")
	{
		vReturnColor = _float4(0.295f, 0.294002f, 0.278775f, 1.f);
	}
	return vReturnColor;
}
