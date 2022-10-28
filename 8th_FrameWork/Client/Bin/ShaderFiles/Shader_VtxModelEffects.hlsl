
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix, g_ViewMatrixInv, g_ProjMatrixInv, g_WorldMatrixInv;

texture2D	g_DiffuseTexture, g_BlendTexture, g_FilterTexture, g_NormalTexture, g_NoiseTexture, g_StaticShadowTexture;
texture2D	g_DepthTexture;

float		g_fRange = 1.f;

float		g_fUVPlusX = 0.f;
float		g_fUVPlusY = 0.f;

float		g_fAlpha = 0.f;
float		g_fNoiseScale = 0.1f;


vector		g_vColor = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vFlag = vector(0.f, 0.f, 0.f, 0.f);
vector		g_vGlowFlag = vector(0.f, 0.f, 0.f, 0.f);

vector		g_vCamPosition;
float		g_fOutlinePower = 1.f;





struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
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

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )



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
	vector		vDepth : SV_TARGET1;
	vector		vEffectFlag : SV_TARGET2;
	vector		vEffectDiffuse : SV_TARGET3;
	vector		vGlowFlag : SV_TARGET4;
};

PS_OUT PS_MAIN_DEFAULT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vEffectFlag = g_vFlag;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);

	//Noise : Color
	//Diffuse : AlphaMap
	vector vColor = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;
	vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse = vMtrlDiffuse;

	Out.vDiffuse.a = Out.vDiffuse.r;



	Out.vDiffuse.a *= g_fAlpha;

	Out.vDiffuse.xyz = vColor.xyz;




	Out.vEffectDiffuse = Out.vDiffuse;
	Out.vGlowFlag = g_vGlowFlag;


	if (Out.vDiffuse.a <= 0.05f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_NOISEMAPPING(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vEffectFlag = g_vFlag;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);

	//Noise : Color
	//Diffuse : AlphaMap
	vector vColor = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;
	vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse.xyz = vColor.xyz;
	Out.vDiffuse *= vMtrlDiffuse.r;
	Out.vDiffuse *= g_vColor;

	Out.vDiffuse.a = g_fAlpha;
	Out.vEffectDiffuse = Out.vDiffuse;
	Out.vGlowFlag = g_vGlowFlag;


	if (Out.vDiffuse.a <= 0.05f)
		discard;

	return Out;
}
PS_OUT PS_MAIN_LINEX(PS_IN In)
{
	if (In.vTexUV.x < g_fUVPlusX)
		discard;

	PS_OUT		Out = PS_MAIN_DEFAULT(In);

	return Out;
}

PS_OUT PS_MAIN_LINEY(PS_IN In)
{
	if (In.vTexUV.y < g_fUVPlusY)
		discard;

	PS_OUT		Out = PS_MAIN_DEFAULT(In);

	return Out;
}

PS_OUT PS_MAIN_TEXT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vEffectFlag = g_vFlag;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse = vMtrlDiffuse;
	Out.vDiffuse.a *= g_fAlpha;

	Out.vEffectDiffuse = Out.vDiffuse;
	Out.vGlowFlag = g_vGlowFlag;

	if (Out.vDiffuse.a <= 0.05f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_CARTOON(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	//Noise : Color
	//Diffuse : AlphaMap
	vector vColor = g_NoiseTexture.Sample(DefaultSampler, float2(In.vTexUV.x, In.vTexUV.y * 2.f));

	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;

	vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse = vMtrlDiffuse;
	Out.vDiffuse.a = Out.vDiffuse.r;
	Out.vDiffuse.a *= g_fAlpha;

	Out.vDiffuse.xyz = vColor.xyz;

	if (Out.vDiffuse.a <= 0.2f)
		discard;

	Out.vDiffuse.a = 1.f;

	Out.vEffectDiffuse = Out.vDiffuse;
	Out.vGlowFlag = g_vGlowFlag;
	Out.vEffectFlag = g_vFlag;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);




	return Out;
}


struct PS_DISTORTION_OUT
{
	vector		vDistortionFlag : SV_TARGET0;
};

PS_DISTORTION_OUT PS_MAIN_DISTORTION(PS_IN In)
{
	PS_DISTORTION_OUT		Out = (PS_DISTORTION_OUT)0;

	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;

	vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	vector vDiffuse;

	vDiffuse = vMtrlDiffuse;
	vDiffuse.a = vDiffuse.r;
	vDiffuse.a *= g_fAlpha;

	Out.vDistortionFlag.b = vDiffuse.a;

	if (vDiffuse.a <= 0.05f)
		discard;

	Out.vDistortionFlag.xyz = vDiffuse.a;
	Out.vDistortionFlag.a = 1;

	return Out;
}



technique11 DefaultTechnique
{
	pass Default
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}

	pass LINEX
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LINEX();
	}

	pass LINEY
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LINEY();
	}

	pass TEXT
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TEXT();
	}

	pass CARTOON
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CARTOON();
	}

	pass DISTORTION
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DISTORTION();
	}

	pass NOISEMAPPING
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NOISEMAPPING();
	}

}