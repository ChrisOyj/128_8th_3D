
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture, g_BlendTexture, g_FilterTexture, g_NormalTexture, g_NoiseTexture, g_StaticShadowTexture;


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
	Out.vDiffuse *= g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * g_fNoiseScale);
	Out.vDiffuse *= 2.f;

	Out.vDiffuse.a = 1.f;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vFlag.b = g_fOutlinePower;


	return Out;	
}

PS_OUT PS_GROUND_MAIN(PS_IN In)
{
	PS_OUT		Out = PS_MAIN(In);

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse *= g_vColor;
	Out.vDiffuse *= saturate(g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * g_fNoiseScale) * 1.5f);
	Out.vDiffuse.a = 1.f;

	vector	vFilter =		g_FilterTexture.Sample(DefaultSampler, In.vTexUV * 0.1f);

	if (vFilter.r > 0.7f)
	{
		vector vSourDiffuse = g_BlendTexture.Sample(DefaultSampler, In.vTexUV * 1.f);
		Out.vDiffuse = vSourDiffuse;
		Out.vDiffuse *= g_NoiseTexture.Sample(DefaultSampler, In.vTexUV* g_fNoiseScale);
		Out.vDiffuse *= 1.5f;

	}
	else
	{
		//땅 색 보정
		Out.vDiffuse.r += 0.1f;
		Out.vDiffuse.g += 0.05f;
		Out.vDiffuse.b -= 0.f;

	}
	/*else
	{
		Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * 1.f);
		Out.vDiffuse.a = 1.f;
	}*/

	

	return Out;
}

PS_OUT PS_GROUNDEXAM_MAIN(PS_IN In)
{
	PS_OUT		Out = PS_MAIN(In);

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse *= g_vColor;
	Out.vDiffuse *= saturate(g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * g_fNoiseScale) * 1.5f);
	Out.vDiffuse.a = 1.f;

	vector	vFilter = g_FilterTexture.Sample(DefaultSampler, In.vTexUV * 0.1f);

	if (vFilter.r > 0.5f)
	{
		vector vSourDiffuse = g_BlendTexture.Sample(DefaultSampler, In.vTexUV * 1.f);
		Out.vDiffuse = vSourDiffuse;
		Out.vDiffuse *= (g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * g_fNoiseScale) * 2.f);

	}
	else
	{
		//땅 색 보정
		Out.vDiffuse.r += 0.1f;
		Out.vDiffuse.g += 0.05f;
		Out.vDiffuse.b -= 0.f;

	}
	/*else
	{
		Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * 1.f);
		Out.vDiffuse.a = 1.f;
	}*/
	

	return Out;
}


PS_OUT PS_SAMPLE_MAIN(PS_IN In)
{
	PS_OUT		Out = PS_MAIN(In);

	Out.vDiffuse.z *= 1.5f;
	Out.vDiffuse.w *= 0.7f;

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

VS_OUT_NORMAL VS_MAIN_NORMAL(VS_IN In)
{
	VS_OUT_NORMAL		Out = (VS_OUT_NORMAL)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix)).xyz;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
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


PS_OUT PS_MAIN_NORMAL(PS_IN_NORMAL In)
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
		vPixelNormal = normalize(vPixelNormal * 2.f - 1.f) * -1.f;

		float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

		vPixelNormal = mul(vPixelNormal, WorldMatrix);

		Out.vNormal = normalize(vector(vPixelNormal * 0.5f + 0.5f, 0.f));

	
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.0f, 0.f, 0.f);

	Out.vFlag = g_vFlag;
	Out.vFlag.b = g_fOutlinePower;

	

	return Out;
}


PS_OUT PS_MAIN_GROUNDBREAK(PS_IN_NORMAL In)
{
	PS_OUT		Out = (PS_OUT)0;


	//실제 색상
	Out.vDiffuse.xyz = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV).xyz;

	//마스크맵
	Out.vDiffuse.xyz *= g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV).r;
	Out.vDiffuse.a = 1.f;


	/* 0 ~ 1 */
	float3		vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV * 1.f).xyz;

	/* -1 ~ 1 */
	vPixelNormal = normalize(vPixelNormal * 2.f - 1.f) * -1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vPixelNormal = mul(vPixelNormal, WorldMatrix);

	Out.vNormal = normalize(vector(vPixelNormal * 0.5f + 0.5f, 0.f));


	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.0f, 0.f, 0.f);

	Out.vFlag = g_vFlag;
	Out.vFlag.b = g_fOutlinePower;



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
	pass CW
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Sky);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass None
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Building
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Ground
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GROUND_MAIN();
	}

	pass Ground_Exam
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GROUNDEXAM_MAIN();
	}

	pass Sample
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SAMPLE_MAIN();
	}

	pass NORMALMAPPING
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
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

	pass GROUNDBREAK
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_GROUNDBREAK();
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