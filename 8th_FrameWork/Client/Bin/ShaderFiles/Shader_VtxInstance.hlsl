
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector		g_vColor = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vFlag = vector(0.f, 0.f, 0.f, 0.f);
float		g_fOutlinePower = 1.f;

texture2D	g_DiffuseTexture, g_NoiseTexture, g_NormalTexture;
texture2D	g_StaticShadowTexture;


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
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

	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
	Out.vPosition = mul(vPosition, matWVP);
	Out.vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vNormal = mul(vector(In.vNormal, 0.f), TransformMatrix);
	Out.vNormal = normalize(mul(Out.vNormal, g_WorldMatrix));
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
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
	vector		vFlag : SV_TARGET3;
};

struct PS_SHADOW_OUT
{
	vector		vLightDepth : SV_TARGET0;
};
PS_SHADOW_OUT PS_SHADOW_MAIN(PS_IN In)
{
	PS_SHADOW_OUT		Out = (PS_SHADOW_OUT)0;

	//기존에 있던 녀석과 깊이 테스트를 해주어야함.
	//In.vProjPos : w나누기 수행 전의 투영좌표
	//w 나누고나면 -1~1사이의 좌표


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

PS_SHADOW_OUT PS_STATICSHADOW_MAIN(PS_IN In)
{
	PS_SHADOW_OUT		Out = (PS_SHADOW_OUT)0;

	Out.vLightDepth.rgb = In.vProjPos.w / 1500.f;

	Out.vLightDepth.a = 1.f;

	return Out;
}



PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse *= g_vColor;
	Out.vDiffuse.a = 1.f;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vFlag.b = g_fOutlinePower;
	return Out;	
}
struct PS_Particle_OUT
{
	vector		vDiffuse : SV_TARGET0;

};

PS_Particle_OUT PS_Particle_MAIN(PS_IN In)
{
	PS_Particle_OUT		Out = (PS_Particle_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse *= g_vColor;
	Out.vDiffuse.a = 1.f;

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


	pass Particle
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Particle_MAIN();
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

	pass STATICSHADOW
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_STATICSHADOW_MAIN();
	}
}