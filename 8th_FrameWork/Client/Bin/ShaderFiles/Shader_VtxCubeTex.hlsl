
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

textureCUBE	g_DiffuseTexture;



struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN_SKY(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;	
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vTexUV : TEXCOORD0;
};

struct PS_OUT
{	
	vector		vColor : SV_TARGET0;	
	/*vector		vColor : SV_TARGET1;
	vector		vColor : SV_TARGET2;
	vector		vColor : SV_TARGET3;
	vector		vColor : SV_TARGET4;
	vector		vColor : SV_TARGET5;
	vector		vColor : SV_TARGET6;
	vector		vColor : SV_TARGET7;*/
};

PS_OUT PS_MAIN_SKY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.a = 0.5f;

	return Out;	
}


technique11 DefaultTechnique
{
	pass Sky
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Sky);

		VertexShader = compile vs_5_0 VS_MAIN_SKY();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SKY();
	}

}