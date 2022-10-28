
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix, g_ViewMatrixInv, g_ProjMatrixInv, g_WorldMatrixInv;

texture2D	g_DiffuseTexture, g_BlendTexture, g_FilterTexture, g_NormalTexture, g_NoiseTexture;
texture2D	g_DepthTexture;


float		g_fUVPlusX = 0.f;
float		g_fUVPlusY = 0.f;


float		g_fAlpha = 0.f;

vector		g_vColor = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vFlag = vector(0.f, 0.f, 0.f, 0.f);
vector		g_vGlowFlag = vector(0.f, 0.f, 0.f, 0.f);




/* SSD */

struct VS_DECAL_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_DECAL_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

};

VS_DECAL_OUT VS_MAIN_SSD(VS_DECAL_IN In)
{
	VS_DECAL_OUT		Out = (VS_DECAL_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vEffectDiffuse : SV_TARGET1;
	vector		vEffectFlag : SV_TARGET2;
	vector		vGlowFlag : SV_TARGET3;
};

PS_OUT PS_MAIN_SSD(VS_DECAL_OUT In)
{
	PS_OUT		Out = (PS_OUT)0;

	float3 vScreenPosition = In.vProjPos.xyz / In.vProjPos.w;

	vScreenPosition.x = vScreenPosition.x * 0.5f + 0.5f;
	vScreenPosition.y = vScreenPosition.y * -0.5f + 0.5f;

	//vScreenPosition.xy += 0.003f;

	vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vScreenPosition.xy);


	float			fViewZ = vDepthDesc.y * 1500.f;

	//Depth에서 월드로 옮겨야함

	vector			vWorldPos;
	vWorldPos.x = vScreenPosition.x * 2.f - 1.f;
	vWorldPos.y = vScreenPosition.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.0f;
	vWorldPos *= fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector vLocalPos = mul(vWorldPos, g_WorldMatrixInv);

	//discarding
	clip(0.5f - abs(vLocalPos.xyz));


	float2 DecalUV = vLocalPos.xz;
	DecalUV += 0.5f;

	Out.vEffectFlag = g_vFlag;

	//Noise : Color
	//Diffuse : AlphaMap
	vector vColor = g_NoiseTexture.Sample(DefaultSampler, DecalUV);
	vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, DecalUV);

	Out.vDiffuse = vMtrlDiffuse;
	Out.vDiffuse.a = 1.f;//Out.vDiffuse.r;
	Out.vDiffuse.a *= g_fAlpha;


	Out.vDiffuse.xyz = vColor.xyz;
	Out.vDiffuse = 1;
	Out.vEffectDiffuse = Out.vDiffuse;
	Out.vGlowFlag = g_vGlowFlag;

	if (Out.vDiffuse.a <= 0.05f)
		discard;

	return Out;
}


technique11 DefaultTechnique
{
	pass SSD
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN_SSD();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SSD();
	}

}