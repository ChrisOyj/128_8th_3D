
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector		g_vColor = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vFlag = vector(0.f, 0.f, 0.f, 0.f);
vector		g_vGlowFlag = vector(0.f, 0.f, 0.f, 0.f);
float		g_fOutlinePower = 1.f;

texture2D	g_DiffuseTexture, g_NoiseTexture, g_NormalTexture;


struct VS_IN
{
	float3		vPosition : POSITION;

	float3		vInstancePosition0 : TEXCOORD0;
	float3		vInstancePosition1 : TEXCOORD1;
	float3		vInstancePosition2 : TEXCOORD2;
	float4		vColor : TEXCOORD3;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vColor : TEXCOORD0;

};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;


	vector			vPosition;
	matrix			matVP;

	if (In.vPosition.x > 0.5f)
		vPosition = vector(In.vInstancePosition0, 1.f);
	else if (In.vPosition.y > 0.5f)
		vPosition = vector(In.vInstancePosition1, 1.f);
	else
		vPosition = vector(In.vInstancePosition2, 1.f);


	matVP = mul(g_ViewMatrix, g_ProjMatrix);
	Out.vPosition = mul(vPosition, matVP);
	Out.vColor = In.vColor;

	return Out;	
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vColor : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;

};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = In.vColor;

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
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}