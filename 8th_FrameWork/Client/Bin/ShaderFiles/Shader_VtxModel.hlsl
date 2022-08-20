
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture;
vector		g_vCamPosition;

vector		g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector		g_vLightPos;
float		g_fRange = 1.f;


vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
float		g_fPower = 30.f;

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
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;

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

};

struct PS_OUT
{	
	vector		vColor : SV_TARGET0;	
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float		fShade, fSpecular;
	fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);

	//vector		vLook = In.vWorldPos - g_vCamPosition;
	//vector		vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));

	//fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower);

	//Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient));

	Out.vColor = vMtrlDiffuse * 0.3f + vMtrlDiffuse * fShade;

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
}