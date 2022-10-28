
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix		g_ProjMatrixInv, g_ViewMatrixInv, g_LightViewMatrix, g_LightVeiwProjMatrix;


vector		g_vLightDir;

vector		g_vLightPos;
float		g_fRange;
float		g_fToonStep = 1.f;

vector		g_vCamPosition;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);


float		g_fFogAlpha = 0.f;

texture2D	g_SkyTexture;
texture2D	g_SpecularTexture;
texture2D	g_DepthTexture;
texture2D	g_DiffuseTexture;
texture2D	g_ShadeTexture;
texture2D	g_NormalTexture;
texture2D	g_Texture;
texture2D	g_FlagTexture;
texture2D	g_OutlineTexture;
texture2D	g_ShadowTexture;
texture2D	g_BlurTexture;
texture2D	g_EffectFlagTexture;
texture2D	g_GlowTexture;
texture2D	g_EffectDiffuseTexture;
texture2D	g_EffectBlurTexture;
texture2D	g_BloomTexture;
texture2D	g_BloomOriginTexture;
texture2D	g_DistortionTexture;
texture2D	g_FogTexture;

float		g_fWinCX = 1280.f;
float		g_fWinCY = 720.f;
float		g_fCoord[3] = { -1.f, 0.f, 1.f };
float		g_fLaplacianMask[9] = {
	-1,-1, -1,
	-1, 8, -1,
	-1, -1, -1 };

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler ShadowSampler = sampler_state
{
	filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = clamp;
	AddressV = clamp;
};

float g_fUVPlusX = 0.f;
float g_fUVPlusY = 0.f;

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

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	return Out;
}

struct PS_OUT_LIGHT
{
	vector		vShade : SV_TARGET0;
	vector		vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)1;

	vector			vFlagDesc = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);

	//For Diffuse 
	if (vFlagDesc.r > 0.f)
	{
		vector			vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
		vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
		float			fViewZ = vDepthDesc.y * 1500.f;
		/* 0 -> -1, 1 -> 1*/
		vector			vNormal = normalize(vector(vNormalDesc.xyz * 2.f - 1.f, 0.f));

		float		fShade = saturate(saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)));

		//For Cartoon
		if (vFlagDesc.r >= 0.7f)
		{
			fShade = ceil(fShade * g_fToonStep) / g_fToonStep;
		}
		else if (vFlagDesc.r < 0.9f) //Building Cartoon
		{
			fShade = ceil(fShade * 5.f) / 5.f;
		}

		Out.vShade = g_vLightDiffuse * fShade + (g_vLightAmbient * g_vMtrlAmbient);

		Out.vShade.a = 1.f;



		//For spec
		if (vFlagDesc.r > 0.99f)
		{
			vector			vReflect = reflect(normalize(g_vLightDir), vNormal);

			vector			vWorldPos;

			vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
			vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
			vWorldPos.z = vDepthDesc.x;
			vWorldPos.w = 1.0f;

			vWorldPos *= fViewZ;

			vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

			vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

			vector			vLook = normalize(vWorldPos - g_vCamPosition);

			Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 30.f);

		}
	}
	Out.vSpecular.a = 0.f;






	return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)1;
	vector			vFlagDesc = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);

	if (vFlagDesc.r > 0.f)
	{

		vector			vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
		vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
		float			fViewZ = vDepthDesc.y * 1500.f;
		vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

		vector			vWorldPos;

		vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
		vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
		vWorldPos.z = vDepthDesc.x;
		vWorldPos.w = 1.0f;
		vWorldPos *= fViewZ;

		vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

		vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

		vector			vLightDir = vWorldPos - g_vLightPos;

		float			fDistance = length(vLightDir);

		float			fAtt = saturate((g_fRange - fDistance) / g_fRange);


		float		fShade = saturate(saturate(dot(normalize(vLightDir) * -1.f, vNormal)));

		//For Cartoon
		if (vFlagDesc.r >= 0.7f)
		{
			fShade = ceil(fShade * g_fToonStep) / g_fToonStep;
		}
		else if (vFlagDesc.r < 0.9f) //Building Cartoon
		{
			fShade = ceil(fShade * 5.f) / 5.f;
		}

		

		Out.vShade = g_vLightDiffuse * fShade * fAtt + (g_vLightAmbient * g_vMtrlAmbient) * fAtt;

		//점조명이구, 최대거리보다 작은 범위내에 있으면
		/*if (g_fRange < 100.f && g_fRange > fDistance)
		{
			Out.vShade.a = 0.95f;
		}
		else*/
		Out.vShade.a = 1.f;

		if (vFlagDesc.r > 0.99f)
		{
			vector			vReflect = reflect(normalize(vLightDir), vNormal);
			vector			vLook = normalize(vWorldPos - g_vCamPosition);

			Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 30.f) * fAtt;
		}
	}

	//Out.vShade.xyz = 0.f;
	Out.vSpecular.a = 0.f;

	return Out;
}


PS_OUT PS_MAIN_FORWARDBLEND(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)Out;
	vector			vFlagDesc = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);


	vector			vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vShade = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor = vDiffuse * vShade;

	vector			vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	if (vSpecular.r < 1.f)
		Out.vColor += vSpecular;

	//Shadow
	vector			vShadowDesc = g_ShadowTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor *= vShadowDesc;


	//DOF
	if (Out.vColor.a > 0.f && vDepthDesc.y > 0.01f)
	{
		//멀수록 강해짐
		float fRatio = min((vDepthDesc.y - 0.01f) / 0.15f, 1.f);

		vector			vBlurDesc = g_BlurTexture.Sample(DefaultSampler, In.vTexUV);

		fRatio *= 0.6f;
		fRatio = pow(fRatio, 1.5f);

		Out.vColor = Out.vColor * (1.f - fRatio) + vBlurDesc * fRatio;
		Out.vColor.r -= 0.3f * fRatio;
		Out.vColor.g += (0.2f * fRatio);
		Out.vColor.b += (0.8f * fRatio);

	}

	//white
	Out.vColor.xyz += vFlagDesc.g;

	if (Out.vColor.a <= 0.f)
	{
		vector			vSkyDesc = g_SkyTexture.Sample(DefaultSampler, In.vTexUV);
		Out.vColor = vSkyDesc;
	}

	//Forward Fog
	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;

	vector			vFogColor = g_FogTexture.Sample(DefaultSampler, In.vTexUV * 0.3f);

	if (vFogColor.x < 0.2f)
		return Out;

	vFogColor.xyz = float3(1.1f, 0.f, 0.f) * (vFogColor.x);

	float fDepthRatio = saturate(vDepthDesc.y / 0.01f);

	//0~1을 0.5~1로 바꿔야함
	fDepthRatio = (fDepthRatio + 1.f) * 0.5f * g_fFogAlpha;
	Out.vColor.xyz = Out.vColor.xyz * (1.f - fDepthRatio) + vFogColor * fDepthRatio;

	

	return Out;
}

PS_OUT PS_MAIN_FINALBLEND(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)Out;
	//vector			vFlagDesc = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);

	vector			vForwardDesc = g_Texture.Sample(DefaultSampler, In.vTexUV);
	vector			vEffectDiffuseDesc = g_EffectDiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vEffectBlurDesc = g_EffectBlurTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vEffectFlagDesc = g_EffectFlagTexture.Sample(DefaultSampler, In.vTexUV);

	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;
	vector			vFogColor = g_FogTexture.Sample(DefaultSampler, In.vTexUV * 0.3f);

	In.vTexUV.x -= g_fUVPlusX;
	In.vTexUV.y -= g_fUVPlusY;

	bool bFog = false;

	Out.vColor = vForwardDesc;

	if (vEffectDiffuseDesc.a > 0.f)
		bFog = true;


	


	//Blur
	if (vEffectFlagDesc.a > 0.9f && vEffectFlagDesc.a < 0.99f)
	{
		Out.vColor = vEffectBlurDesc;
		bFog = true;
	}


	//Glow
	


	vector			vGlowDesc = g_GlowTexture.Sample(DefaultSampler, In.vTexUV);
	vGlowDesc.a *= 2.f;
	saturate(vGlowDesc.a);
	vGlowDesc.a = sqrt(vGlowDesc.a);

	//멀면 글로우 약하게
	/*if (vDepthDesc.y > 0.05f)
	{
		vGlowDesc.a *= saturate( (0.2f - vDepthDesc.y) / 0.2f);
	}*/

	if (vGlowDesc.a > 0.01f)
	{
		//이펙트 안그려진 부분만 섞기
		if (vEffectFlagDesc.r <= 0.1f)
		{
			//안개켜져있고, 그리는곳 depth가 안개범위 밖이면
			if (vFogColor.x > 0.1f && vDepthDesc.y > 0.01f)
			{
				
			}
			else
			{
				//차후 깊이 비교까지 추가
				Out.vColor.xyz = Out.vColor.xyz * (1.f - vGlowDesc.a) + vGlowDesc.xyz * (vGlowDesc.a);
			}

		}
		bFog = true;
	}

	//Bloom

	//BloomDesc의 알파가 반영되어야함.
	vector			vBloomDesc = g_BloomTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vBloomOriginDesc = g_BloomOriginTexture.Sample(DefaultSampler, In.vTexUV);

	if (vBloomDesc.a > 0.f)
		bFog = true;

	float4		vBloom = pow(pow(abs(vBloomDesc), 2.2f) + pow(abs(vBloomOriginDesc), 2.2f), 1.f / 2.2f);
	//Out.vColor.xyz += vBloom.xyz;


	Out.vColor.xyz += vBloom.xyz * (sqrt(vBloom.a));



	//Outline

	//a에다가 outline 깊이는 저장할 수 있음

	//이펙트의 깊이를 알아야함
	vector			vOutlineDesc = g_OutlineTexture.Sample(DefaultSampler, In.vTexUV);

	//1. OutLine 그려야할 때
	if (vOutlineDesc.x < 0.8f)
	{
		//2. 그냥 depth 확인
		//그릴곳의 W가 
		//if (vDepthDesc.w > vOutlineDesc.a)
		if (vFogColor.x > 0.1f && vOutlineDesc.a > 0.01f)
		{

		}
		else
		{
			if (bFog)
			{
				//이펙트가 그려진곳은
				/*if (vEffectDiffuseDesc.a < 0.3f)
					Out.vColor *= vOutlineDesc;*/
			}
			else
			{
				Out.vColor *= vOutlineDesc;
			}

			Out.vColor.a = 1.f;

		}
	}



	if (Out.vColor.a <= 0.f)
		discard;

	//이펙트나 OutLine 그려진 곳만 안개 추가로 씌우기


	if (bFog == true)
	{

		if (vFogColor.x < 0.2f)
			return Out;

		vFogColor.xyz = float3(1.1f, 0.f, 0.f) * (vFogColor.x);

		float fDepthRatio = saturate(vDepthDesc.y / 0.01f);


		//0~1을 0.5~1로 바꿔야함
		fDepthRatio = (fDepthRatio + 1.f) * 0.5f * g_fFogAlpha;

		if (fDepthRatio >= 1.f)
			Out.vColor.xyz = vFogColor;
		else
		{
			fDepthRatio *= vEffectDiffuseDesc.a;
			Out.vColor.xyz = Out.vColor.xyz * (1.f - fDepthRatio) + vFogColor * fDepthRatio;

		}
	}

	



	return Out;
}

PS_OUT PS_MAIN_POSTEFFECT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)Out;

	vector			vDistortionDesc = g_DistortionTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);

	//vector			vFogColor = g_FogTexture.Sample(DefaultSampler, In.vTexUV);

	if (vDistortionDesc.a > 0.f)
	{
		//vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
		//depth에 비례하자
		//depth.w는 0~1사이구 가까울수록 값이 작다?
		//vDepthDesc.y = saturate(vDepthDesc.y / 0.1f);
		In.vTexUV += vDistortionDesc.r * 0.03f;
	}


	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);


	if (Out.vColor.a <= 0.f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_OUTLINE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float fOutlinePower = 1.f;

	vector DepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	vector NormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float	fCurDepth = DepthDesc.y;

	//멀수록 y가 높음
	//너무 멀면 Outline없나본디
	if (DepthDesc.y > 0.1f)
	{
		Out = (PS_OUT)1;
		return Out;

	}

	for (unsigned int i = 0; i < 9; ++i)
	{
		float2 OtherTexUV = In.vTexUV + float2(g_fCoord[i % 3] / g_fWinCX, g_fCoord[i / 3] / g_fWinCY);

		vector			vFlagDesc = g_FlagTexture.Sample(DefaultSampler, OtherTexUV);
		vector OtherDepthDesc = g_DepthTexture.Sample(DefaultSampler, OtherTexUV);
		vector OtherNormalDesc = g_NormalTexture.Sample(DefaultSampler, OtherTexUV);

		if (OtherDepthDesc.y < fCurDepth)
			fCurDepth = OtherDepthDesc.y;

		if (vFlagDesc.r > 0.8f)
		{
			vector vPlusColor = (g_fLaplacianMask[i] * g_DiffuseTexture.Sample(DefaultSampler, OtherTexUV));

			//Add Normal and Depth
			float fDepthSub = length(OtherDepthDesc - DepthDesc);
			float fNormalSub = abs(dot(normalize(OtherNormalDesc), normalize(NormalDesc)));

			float fMaxRange = 0.03f;
			float fRatio = 1.f - min((DepthDesc.y * 1.f / fMaxRange), 1.f);

			if (DepthDesc.y < fMaxRange)
			{
				//Depth
				if (fDepthSub > 0.005f)
				{
					vPlusColor += 0.5f * fRatio;
				}
				if (fNormalSub <= 0.75f)
				{
					vPlusColor += 0.5f * fRatio;

				}
			}


			Out.vColor += vPlusColor;


			if (vFlagDesc.b < 0.9f)
				fOutlinePower = (vFlagDesc.b);
		}



	}
	float fBlack = 1 - min((Out.vColor.r * 0.33f + Out.vColor.g * 0.59f + Out.vColor.b * 0.11f), 1.f);
	/*if (fBlack < 0.8f)
		fBlack *= 0.5f;*/
	fBlack += (1.f - fOutlinePower);
	Out.vColor = vector(fBlack, fBlack, fBlack, fCurDepth);

	return Out;
}



BlendState BS_Default
{
	BlendEnable[0] = false;
};

BlendState BS_AlphaBlending
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = one;
	DestBlend = one;
	BlendOp = add;
};

BlendState BS_OriginAlphaBlending
{
	BlendEnable[0] = true;

	SrcBlend = src_alpha;
	DestBlend = inv_src_alpha;
	BlendOp = add;
};


RasterizerState RS_Default
{
	FillMode = solid;
	CullMode = back;
	FrontCounterClockwise = false;
};

DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_ZEnable_ZWriteEnable_false
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

technique11 DefaultTechnique
{
	pass Debug
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

	pass Light_Directional
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass Light_Point
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
	}

	pass ForwardBlend
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_FORWARDBLEND();
	}

	pass Outline
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_OUTLINE();
	}

	pass FinalBlend
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_FINALBLEND();
	}

	pass POSTEFFECT
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POSTEFFECT();
	}
}