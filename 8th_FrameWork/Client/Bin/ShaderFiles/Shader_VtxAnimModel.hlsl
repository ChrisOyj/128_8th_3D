#include "Client_Shader_Defines.hpp"


matrix	g_matViewList[MAX_CASCADE_NUM];
matrix	g_matProjList[MAX_CASCADE_NUM];

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture, g_BlendTexture, g_FilterTexture , g_NoiseTexture, g_NormalTexture;
texture2D	g_StaticShadowTexture;


vector		g_vCamPosition;
float		g_fNoiseScale = 0.1f;

float		g_fRange = 1.f;
vector		g_vColor = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vFlag = vector(0.f, 0.f, 0.f, 0.f);
float		g_fWhite = 0.f;

float		g_rimWidth = 1.f;
float		g_fOffset = 0.2f;
float		g_fOutlinePower = 1.f;

float		g_fUVPlusX = 0.f;
float		g_fUVPlusY = 0.f;

float		g_fAlpha = 0.f;

vector		g_vGlowFlag = vector(0.f, 0.f, 0.f, 0.f);


struct		tagBoneMatrix
{
	matrix		BoneMatrices[256];
};

tagBoneMatrix		g_Bones;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;	
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float			fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix			BoneMatrix = g_Bones.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_Bones.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_Bones.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_Bones.BoneMatrices[In.vBlendIndex.w] * fWeightW;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	Out.vNormal = normalize(mul(Out.vNormal, g_WorldMatrix));
	Out.vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;	
}

VS_OUT VS_PREVIEW_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float			fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix			BoneMatrix = g_Bones.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_Bones.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_Bones.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_Bones.BoneMatrices[In.vBlendIndex.w] * fWeightW;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	Out.vNormal = normalize(mul(Out.vNormal, g_WorldMatrix));
	Out.vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vTexUV = In.vTexUV;

	Out.vProjPos = Out.vPosition;
	Out.vPosition.x += 0.59f;

	return Out;
}

VS_OUT VS_PREVIEW_REF_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vWorldPos = mul(vector(In.vPosition,1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;

	//z = 3.5f ~ 0.5f
	//2 * ? = 0.8f
	Out.vProjPos = Out.vPosition;
	Out.vPosition.x += 0.59f;


	return Out;
}





struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

struct PS_OUT
{	
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
	vector	vFlag : SV_TARGET3;
};

struct PS_SHADOW_OUT
{
	vector		vLightDepth : SV_TARGET0;
};
PS_SHADOW_OUT PS_SHADOW_MAIN(PS_IN In)
{
	PS_SHADOW_OUT		Out = (PS_SHADOW_OUT)0;

	//기존에 있던 녀석과 깊이 테스트를 해주어야함.

	float2 staticUV = float2(
		(In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f,
		(In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f
		);

	Out.vLightDepth.rgb = In.vProjPos.w / 1500.f;

	vector vStaticDesc = g_StaticShadowTexture.Sample(ShadowSampler, staticUV);

	if (vStaticDesc.r < Out.vLightDepth.r)
		discard;


	Out.vLightDepth.a = 1.f;


	return Out;
}

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse *= g_vColor;

	float fTemp = Out.vDiffuse.x + Out.vDiffuse.y + Out.vDiffuse.z;

	if (fTemp < 2.5f)
	{
		Out.vDiffuse *= saturate(g_NoiseTexture.Sample(DefaultSampler, In.vTexUV) * 1.3f);
		Out.vDiffuse *= 1.1f;
	}
		

	Out.vDiffuse.a = 1.f;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vFlag.b = g_fOutlinePower;



	return Out;	
}

PS_OUT PS_FACE_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse *= g_vColor;
	/*Out.vDiffuse *= saturate(g_NoiseTexture.Sample(DefaultSampler, In.vTexUV) * 0.9f);
	Out.vDiffuse *= 1.25f;*/
	Out.vDiffuse.a = 1.f;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;



	return Out;
}

PS_OUT PS_HIT_MAIN(PS_IN In)
{
	PS_OUT		Out = PS_MAIN(In);

	Out.vFlag.g = g_fWhite;

	return Out;
}

struct PS_EFFECT_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vDepth : SV_TARGET1;
	vector		vEffectFlag : SV_TARGET2;
	vector		vEffectDiffuse : SV_TARGET3;
	vector		vGlowFlag : SV_TARGET4;
};

PS_EFFECT_OUT PS_DRAGON_MAIN(PS_IN In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	vector NoiseDesc = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * 2.f);

	if (sqrt(NoiseDesc.r) > g_fAlpha)
		discard;

	//Diffuse : 용 선, 검은부분이 몸
	//Normal : ColorMap
	//Noise : ColorMap에 곱해줘서 UV 흐르기

	vector MaskDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector ColorDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;
	NoiseDesc = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * 2.f);



	Out.vDiffuse = vector(1.f, 0.4f, 0.4f, 1.f);
	Out.vDiffuse.xyz *= NoiseDesc.x;
	Out.vDiffuse.x = 1.f;

	if (MaskDesc.r > 0.5f)
	{
		Out.vDiffuse.xyz = float3(1.f, 0.6f, 0.6f);
	}

	Out.vDiffuse.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vEffectFlag = g_vFlag;
	Out.vEffectDiffuse = Out.vDiffuse;
	Out.vGlowFlag = g_vGlowFlag;



	return Out;
}


struct VS_OUT_NORMAL
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};

VS_OUT_NORMAL VS_NORMAL_MAIN(VS_IN In)
{
	VS_OUT_NORMAL		Out = (VS_OUT_NORMAL)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float			fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix			BoneMatrix = g_Bones.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_Bones.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_Bones.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_Bones.BoneMatrices[In.vBlendIndex.w] * fWeightW;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix)).xyz;
	Out.vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), BoneMatrix)).xyz;
	Out.vTangent = normalize(mul(vector(Out.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));

	return Out;
}
struct PS_IN_NORMAL
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;

};
PS_OUT PS_NORMAL_MAIN(PS_IN_NORMAL In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse *= g_vColor;
	Out.vDiffuse *= g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * g_fNoiseScale);
	Out.vDiffuse *= 2.f;
	Out.vDiffuse.a = 1.f;

	float fDistance = length(In.vWorldPos - g_vCamPosition);

	/* 0 ~ 1 */
	float3		vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV * 1.f).xyz;

	/* -1 ~ 1 */
	vPixelNormal = normalize(vPixelNormal * 2.f - 1.f);// *-1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vPixelNormal = mul(vPixelNormal, WorldMatrix);

	Out.vNormal = normalize(vector(vPixelNormal * 0.5f + 0.5f, 0.f));


	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.0f, 0.f, 0.f);

	Out.vFlag = g_vFlag;
	Out.vFlag.b = g_fOutlinePower;



	return Out;
}



/* CASCADE SHADOWING */

struct GS_IN_CASCADE
{
	float4		vWorldPos : POSITION;

};

struct PS_OUT_CASCADECASTER
{
	float4		vPosition : SV_POSITION;
	float4		vProjPos : TEXCOORD0;
	uint		iRTIndex : SV_RenderTargetArrayIndex;

};

GS_IN_CASCADE VS_MAIN_CASCADECASTER(VS_IN In)
{
	GS_IN_CASCADE		Out = (GS_IN_CASCADE)0;

	float			fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix			BoneMatrix = g_Bones.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_Bones.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_Bones.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_Bones.BoneMatrices[In.vBlendIndex.w] * fWeightW;

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);

	Out.vWorldPos = mul(vPosition, g_WorldMatrix);

	return Out;
}


[maxvertexcount(3*MAX_CASCADE_NUM)]
void GS_MAIN_CASCADECASTER(triangle GS_IN_CASCADE In[3], inout TriangleStream<PS_OUT_CASCADECASTER> TriStream)
{
	PS_OUT_CASCADECASTER	Out[3];
	for (uint i = 0; i < MAX_CASCADE_NUM; ++i)
	{
		for (uint j = 0; j < 3; ++j)
		{
			float4 vViewPos = mul(In[j].vWorldPos, g_matViewList[i]);
			vViewPos.z += 2.f; // bias
			Out[j].vPosition = mul(vViewPos, g_matProjList[i]);
			Out[j].vProjPos = Out[j].vPosition;
			Out[j].iRTIndex = i;
			TriStream.Append(Out[j]);
		}

		TriStream.RestartStrip();
	}
}

PS_SHADOW_OUT PS_MAIN_CASCADECASTER(PS_OUT_CASCADECASTER In)
{
	PS_SHADOW_OUT		Out = (PS_SHADOW_OUT)0;

	Out.vLightDepth.rgb = In.vProjPos.w / 1500.f;

	Out.vLightDepth.a = 1.f;


	return Out;
}






technique11 DefaultTechnique
{
	pass Default
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Face
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FACE_MAIN();
	}

	pass Eye
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass PREVIEW
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_PREVIEW_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass PREVIEW_REF
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_PREVIEW_REF_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Hit
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_HIT_MAIN();
	}

	pass PREVIEW_FACE
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_PREVIEW_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FACE_MAIN();
	}

	pass SHADOW
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOW_MAIN();
	}

	pass NORMAL
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_NORMAL_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_NORMAL_MAIN();
	}

	pass Dragon
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DRAGON_MAIN();
	}

	pass CASCADECASTER
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_CASCADECASTER();
		GeometryShader = compile gs_5_0 GS_MAIN_CASCADECASTER();
		PixelShader = compile ps_5_0 PS_MAIN_CASCADECASTER();
	}
}