
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture, g_NoiseTexture;

float	g_fAlpha = 1.f;
float	g_fProgress = 1.0f;
float	g_fProgressY = 0.4f;

vector	g_vColor;
vector	g_vFlag;
vector		g_vGlowFlag = vector(0.f, 0.f, 0.f, 0.f);




struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;	
}


struct VS_TRAIL_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

VS_TRAIL_OUT VS_TRAIL_MAIN(VS_IN In)
{
	VS_TRAIL_OUT		Out = (VS_TRAIL_OUT)0;

	matrix			matWV, matWVP;

	matWV = g_ViewMatrix;
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct VS_SKILLICON_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float3		vLocalPos : TEXCOORD1;
};

VS_SKILLICON_OUT VS_SKILLICON_MAIN(VS_IN In)
{
	VS_SKILLICON_OUT		Out = (VS_SKILLICON_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vLocalPos = In.vPosition;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{	
	vector		vColor : SV_TARGET0;	
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.a < 0.1f)
		discard;

	return Out;	
}

PS_OUT PS_UIColor_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.w *= g_fAlpha;

	if (Out.vColor.w < 0.01f)
		discard;

	return Out;
}

PS_OUT PS_LoadingBar_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (In.vTexUV.x >= g_fProgress)
	{
		Out.vColor.rgb = 0.6f;
	}

	Out.vColor.w *= g_fAlpha;

	if (Out.vColor.w < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_NOBLACK_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.w = Out.vColor.r * g_fAlpha;

	if (Out.vColor.w < 0.3f)
		discard;


	return Out;
}

PS_OUT PS_HPBAREFFECT_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.w *= g_fAlpha;
	Out.vColor.x = 0.8f;
	Out.vColor.y = 0.f;
	Out.vColor.z = 0.f;

	/*Out.vColor.w = (Out.vColor.r * 2.f) * g_fAlpha;

	if (Out.vColor.w < 0.3f)
		discard;*/


	return Out;
}

PS_OUT PS_CROSSHAIR_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.w *= g_fAlpha;
	if (Out.vColor.w >= 1.f)
		Out.vColor.w = 0.7f;

	if (Out.vColor.w < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_COLORNOBLACK_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.w = Out.vColor.r * g_fAlpha;

	if (Out.vColor.w < 0.3f)
		discard;

	Out.vColor *= g_vColor;


	return Out;
}

PS_OUT PS_HPBAR_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.w *= g_fAlpha;

	if (Out.vColor.w < 0.1f)
		discard;


	Out.vColor.xyz = g_vColor.xyz;

	return Out;
}

PS_OUT PS_SKILLICON_MAIN(VS_SKILLICON_OUT In)
{
	//2. 원으로 출력
	float fRadius = length(In.vLocalPos);
	if (fRadius > 0.5f)
	{
		//반지름이 0.5보다 크면 안그림
		discard;
	}
	
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//1. 스킬 쿨타임에 맞춰 아래부터
	if (In.vLocalPos.y > -g_fProgressY)
	{
		Out.vColor.xyz *= 0.3f;

		//전역으로 -0.5~0.5의 Y값을 던져줌.
		//픽셀 y가 전역으로 던져준 높이보다 낮으면 원래 색으로, 아니면ㅇ ㅓ둡게

		//쿨 없을때 0.5고 쓸 수 이을때 -0.5임

		//- 0.5일때 다 출력되어야함
	}

	Out.vColor.w *= g_fAlpha;


	if (Out.vColor.w < 0.1f)
		discard;



	return Out;
}

PS_OUT PS_ENEMYHPBAR_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.w = Out.vColor.r * g_fAlpha;

	if (Out.vColor.w < 0.1f)
		discard;

	Out.vColor = vector(1.f, 0.f, 0.f, 1.f);


	return Out;
}

PS_OUT PS_ENEMYHPBARLINE_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.w *= g_fAlpha;

	if (Out.vColor.w < 0.8f)
		discard;

	Out.vColor = vector(0.f, 0.f, 0.f, 1.f);


	return Out;
}

PS_OUT PS_KOTO_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.x *= g_fAlpha;

	if (Out.vColor.x < 0.05f)
		discard;

	Out.vColor = vector(0.f, 0.f, 0.f, Out.vColor.x * 0.3f);


	return Out;
}

PS_OUT PS_DIALOG_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.w *= g_fAlpha;

	if (In.vTexUV.x > 0.7f)
	{
		float fRatio = max(0.f, ((0.9f - In.vTexUV.x) / 0.2f));
		Out.vColor.w *= fRatio;
	}


	return Out;
}

struct PS_EFFECT_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vDepth : SV_TARGET1;
	vector		vFlag : SV_TARGET2;
	vector		vEffectDiffuse : SV_TARGET3;
	vector		vGlowFlag : SV_TARGET4;

};

PS_EFFECT_OUT PS_TRAIL_MAIN(VS_TRAIL_OUT In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	In.vTexUV.x = 1.f - In.vTexUV.x;
	//Mask
	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse.a = Out.vDiffuse.r;


	In.vTexUV.x = 1.f - In.vTexUV.x;
	In.vTexUV.y *= 5.f;
	Out.vDiffuse.xyz = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV).xyz;

	Out.vDiffuse *= g_vColor;

	if (Out.vDiffuse.a < 0.7f)
		discard;

	Out.vDiffuse.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vGlowFlag = g_vGlowFlag;
	Out.vEffectDiffuse = Out.vDiffuse;

	return Out;
}


PS_EFFECT_OUT PS_FOOTTRAIL_MAIN(VS_TRAIL_OUT In)
{
	PS_EFFECT_OUT		Out = (PS_EFFECT_OUT)0;

	In.vTexUV.x = 1.f - In.vTexUV.x;
	//Mask
	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse.a = Out.vDiffuse.r;


	In.vTexUV.x = 1.f - In.vTexUV.x;
	In.vTexUV.y *= 1.f;
	Out.vDiffuse.xyz = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV).xyz;

	Out.vDiffuse *= g_vColor;

	if (Out.vDiffuse.a < 0.7f)
		discard;

	Out.vDiffuse.a = 0.4f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vGlowFlag = g_vGlowFlag;
	Out.vEffectDiffuse = Out.vDiffuse;

	return Out;
}

PS_OUT PS_BOSSHP_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.w *= g_fAlpha;
	Out.vColor.yz = 0.03f;
	Out.vColor.x = 1.f;
	if (In.vTexUV.x >= 0.6f)
	{
		//Ratio가 0부터 1까지 나와야함
		//x는 지금 0.~.0.3임
		float fRatio = max(0.f, ((In.vTexUV.x - 0.6f) / 0.4f));
		Out.vColor.y += 0.6f * fRatio;
	}

	if (In.vTexUV.y >= 0.35f && In.vTexUV.y <= 0.45f)
	{
		//0.6에서 0.8까지는 하얗게
		//Ratio가 0~1로 나옴
		float fRatio = max(0.f, ((In.vTexUV.y - 0.35f) / 0.1f));
		Out.vColor.xyz += 0.7f * fRatio;
	}

	else if (In.vTexUV.y > 0.45f && In.vTexUV.y <= 0.55f)
	{
		//0.45에서 0.55까지는 하얗게
		//Ratio가 0~1로 나옴
		float fRatio = max(0.f, ((In.vTexUV.y - 0.45f) / 0.1f));
		Out.vColor.xyz += 0.7f * (1.f - fRatio);
	}


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

	pass UIColor
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_UIColor_MAIN();
	}

	pass LoadingBar
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_LoadingBar_MAIN();
	}

	pass NoBlack
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_NOBLACK_MAIN();
	}

	pass Crosshair
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CROSSHAIR_MAIN();
	}

	pass ColorNoBlack
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_COLORNOBLACK_MAIN();
	}

	pass SkillIcon
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_SKILLICON_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SKILLICON_MAIN();
	}

	pass HPBAR
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_HPBAR_MAIN();
	}

	pass HPBAREffect
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_HPBAREFFECT_MAIN();
	}

	pass ENEMYHPBAR
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ENEMYHPBAR_MAIN();
	}

	pass ENEMYHPBARLINE
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ENEMYHPBARLINE_MAIN();
	}

	pass DIALOG
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DIALOG_MAIN();
	}

	pass BOSSHP
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BOSSHP_MAIN();
	}

	pass TRAIL
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_TRAIL_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_TRAIL_MAIN();
	}

	pass KOTO
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_KOTO_MAIN();
	}

	pass FOOTTRAIL
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_TRAIL_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FOOTTRAIL_MAIN();
	}
}