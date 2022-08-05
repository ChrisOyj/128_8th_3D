
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector	g_vCamPosition;

vector	g_vLightDir;
vector	g_vLightPos;
float	g_fRange = 1.f;

vector	g_vLightDiffuse;
vector	g_vLightAmbient;
vector	g_vLightSpecular;

// vector	g_vMtrlDiffuse;
texture2D	g_DiffuseTexture;
vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
float		g_fPower = 30.f;



struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float		fShade : TEXCOORD1;
	float		fSpecular : TEXCOORD2;
};

VS_OUT VS_MAIN_TERRAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	vector		vWorldNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
	Out.fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(vWorldNormal)), 0.f);

	vector		vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);


	vector		vLook = vWorldPos - g_vCamPosition;
	vector		vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));

	Out.fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower);

	return Out;	
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float		fShade : TEXCOORD1;
	float		fSpecular : TEXCOORD2;
};

struct PS_OUT
{	
	vector		vColor : SV_TARGET0;	
};

PS_OUT PS_MAIN_TERRAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * 30.f);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(In.fShade + (g_vLightAmbient * g_vMtrlAmbient)) 
		+ (g_vLightSpecular * g_vMtrlSpecular) * In.fSpecular;

	// Out.vColor.a = 1.f;

	return Out;	
}

struct VS_OUT_PHONG
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;	
	float4		vWorldPos : TEXCOORD1;
};


VS_OUT_PHONG VS_MAIN_TERRAIN_PHONG(VS_IN In)
{
	VS_OUT_PHONG		Out = (VS_OUT_PHONG)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	return Out;
}

struct PS_IN_PHONG
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
};

PS_OUT PS_MAIN_TERRAIN_PHONG(PS_IN_PHONG In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * 30.f);

	float		fShade, fSpecular;
	fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);

	vector		vLook = In.vWorldPos - g_vCamPosition;
	vector		vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));

	fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))
		+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

	// Out.vColor.a = 1.f;

	return Out;
}

PS_OUT PS_MAIN_TERRAIN_PHONG_POINT(PS_IN_PHONG In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV * 30.f);

	vector		vLightDir = In.vWorldPos - g_vLightPos;
	float		fDistance = length(vLightDir);

	float		fAtt = max(g_fRange - fDistance, 0.f) / g_fRange;

	float		fShade, fSpecular;
	fShade = max(dot(normalize(vLightDir) * -1.f, normalize(In.vNormal)), 0.f) * fAtt;

	vector		vLook = In.vWorldPos - g_vCamPosition;
	vector		vReflect = reflect(normalize(vLightDir), normalize(In.vNormal));

	fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower) * fAtt;

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))
		+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;



	return Out;
}


technique11 DefaultTechnique
{
	pass Terrain
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);


		VertexShader = compile vs_5_0 VS_MAIN_TERRAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN();
	}	

	pass Terrain_Phong
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_TERRAIN_PHONG();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN_PHONG();
	}

	pass Terrain_Point
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_TERRAIN_PHONG();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN_PHONG_POINT();
	}
}