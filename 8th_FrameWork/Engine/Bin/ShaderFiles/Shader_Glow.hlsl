matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix		g_ProjMatrixInv, g_ViewMatrixInv;

float		g_fRange; 

vector		g_vCamPosition;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);


texture2D	g_ShaderTexture;
texture2D	g_FlagTexture;
texture2D	g_GlowFlagTexture;


float		g_fWinCX = 640.f;
float		g_fWinCY = 360.f;

static const float g_fWeight[7] =
{
	1, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};


sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
};

struct VS_IN
{
	float3		position : POSITION;
	float2		tex : TEXCOORD0;
};

struct VS_DOWNSCALE_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_DOWNSCALE_OUT VS_DOWNSCALE_MAIN(VS_IN In)
{
	VS_DOWNSCALE_OUT		Out = (VS_DOWNSCALE_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.position, 1.f), matWVP);
	Out.vTexUV = In.tex;

	return Out;
}

struct PS_DOWNSCALE_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_GLOWABLE_MAIN(PS_DOWNSCALE_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vEffectFlagDesc = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);

	if (vEffectFlagDesc.r < 0.99f)
	{
			discard;
	}

	Out.vColor = g_ShaderTexture.Sample(DefaultSampler, In.vTexUV);

	return Out;
}


struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
};

VS_OUT VS_VERTICAL_MAIN(VS_IN input)
{
	VS_OUT		output = (VS_OUT)0;

	matrix			matWV, matWVP;

	float texelSize;


	// Change the position vector to be 4 units for proper matrix calculations.

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(vector(input.position, 1.f), g_WorldMatrix);
	output.position = mul(output.position, g_ViewMatrix);
	output.position = mul(output.position, g_ProjMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Determine the floating point size of a texel for a screen with this specific height.
	texelSize = 1.0f / g_fWinCY;

	// Create UV coordinates for the pixel and its four vertical neighbors on either side.
	output.texCoord1 = input.tex + float2(0.0f, texelSize * -4.0f);
	output.texCoord2 = input.tex + float2(0.0f, texelSize * -3.0f);
	output.texCoord3 = input.tex + float2(0.0f, texelSize * -2.0f);
	output.texCoord4 = input.tex + float2(0.0f, texelSize * -1.0f);
	output.texCoord5 = input.tex + float2(0.0f, texelSize * -0.0f);
	output.texCoord6 = input.tex + float2(0.0f, texelSize * 1.0f);
	output.texCoord7 = input.tex + float2(0.0f, texelSize * 2.0f);
	output.texCoord8 = input.tex + float2(0.0f, texelSize * 3.0f);
	output.texCoord9 = input.tex + float2(0.0f, texelSize * 4.0f);

	return output;
}

VS_OUT VS_HORIZONTAL_MAIN(VS_IN input)
{
	VS_OUT		output = (VS_OUT)0;

	matrix			matWV, matWVP;

	float texelSize;


	// Change the position vector to be 4 units for proper matrix calculations.

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(vector(input.position, 1.f), g_WorldMatrix);
	output.position = mul(output.position, g_ViewMatrix);
	output.position = mul(output.position, g_ProjMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Determine the floating point size of a texel for a screen with this specific height.
	texelSize = 1.0f / g_fWinCX;

	// Create UV coordinates for the pixel and its four vertical neighbors on either side.
	output.texCoord1 = input.tex + float2(texelSize * -4.0f, 0.0f);
	output.texCoord2 = input.tex + float2(texelSize * -3.0f, 0.0f);
	output.texCoord3 = input.tex + float2(texelSize * -2.0f, 0.0f);
	output.texCoord4 = input.tex + float2(texelSize * -1.0f, 0.0f);
	output.texCoord5 = input.tex + float2(texelSize * -0.0f, 0.0f);
	output.texCoord6 = input.tex + float2(texelSize * 1.0f, 0.0f);
	output.texCoord7 = input.tex + float2(texelSize * 2.0f, 0.0f);
	output.texCoord8 = input.tex + float2(texelSize * 3.0f, 0.0f);
	output.texCoord9 = input.tex + float2(texelSize * 4.0f, 0.0f);

	return output;
}

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
};


PS_OUT	PS_DEFAULTBLUR_MAIN(PS_IN input)
{
	PS_OUT	Output = (PS_OUT)1;

	float weight0, weight1, weight2, weight3, weight4, normalization;

	// Create the weights that each neighbor pixel will contribute to the blur.
	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	// Create a normalized value to average the weights out a bit.
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	// Normalize the weights.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	// Initialize the color to black.
	Output.vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);


	// Add the nine vertical pixels to the color by the specific weight of each.

	//Default에서는 그냥 알파만 섞어주기. 색이 있따면 색도 훔쳐와

	vector BlurDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord1);
	if (length(BlurDesc.xyz) > 0.1f)
		Output.vColor.xyz = BlurDesc.xyz;
	Output.vColor.a += BlurDesc.a * weight4;

	BlurDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord2);
	if (length(BlurDesc.xyz) > 0.1f)
		Output.vColor.xyz = BlurDesc.xyz;
	Output.vColor.a += BlurDesc.a * weight3;
	BlurDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord3);
	if (length(BlurDesc.xyz) > 0.1f)
		Output.vColor.xyz = BlurDesc.xyz;
	Output.vColor.a += BlurDesc.a * weight2;

	BlurDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord4);
	if (length(BlurDesc.xyz) > 0.1f)
		Output.vColor.xyz = BlurDesc.xyz;
	Output.vColor.a += BlurDesc.a * weight1;

	BlurDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord5);
	if (length(BlurDesc.xyz) > 0.1f)
		Output.vColor.xyz = BlurDesc.xyz;
	Output.vColor.a += BlurDesc.a * weight0;

	BlurDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord6);
	if (length(BlurDesc.xyz) > 0.1f)
		Output.vColor.xyz = BlurDesc.xyz;
	Output.vColor.a += BlurDesc.a * weight1;

	BlurDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord7);
	if (length(BlurDesc.xyz) > 0.1f)
		Output.vColor.xyz = BlurDesc.xyz;
	Output.vColor.a += BlurDesc.a * weight2;

	BlurDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord8);
	if (length(BlurDesc.xyz) > 0.1f)
		Output.vColor.xyz = BlurDesc.xyz;
	Output.vColor.a += BlurDesc.a * weight3;

	BlurDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord9);
	if (length(BlurDesc.xyz) > 0.1f)
		Output.vColor.xyz = BlurDesc.xyz;
	Output.vColor.a += BlurDesc.a * weight4;



	return Output;
}


PS_OUT	PS_FIRSTBLUR_MAIN(PS_IN input)
{
	PS_OUT	Output = (PS_OUT)1;

	float weight0, weight1, weight2, weight3, weight4, normalization;

	// Create the weights that each neighbor pixel will contribute to the blur.
	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	// Create a normalized value to average the weights out a bit.
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	// Normalize the weights.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	// Initialize the color to black.
	Output.vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 vGlowColor = 0;
	vector vGlowFlagDesc, vEffectFlagDesc;
	float	fGlowPower = 0;

	// Add the nine vertical pixels to the color by the specific weight of each.
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord1) * weight4;
	vGlowFlagDesc = g_GlowFlagTexture.Sample(DefaultSampler, input.texCoord1 );
	vEffectFlagDesc = g_FlagTexture.Sample(DefaultSampler, input.texCoord1 );


	if (vEffectFlagDesc.x > 0.99f && length(vGlowFlagDesc.xyz) > 0.1f)
	{
		vGlowColor = vGlowFlagDesc.xyz;
		fGlowPower = vGlowFlagDesc.a;
	}

	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord2) * weight3;
	vGlowFlagDesc = g_GlowFlagTexture.Sample(DefaultSampler, input.texCoord2 );
	vEffectFlagDesc = g_FlagTexture.Sample(DefaultSampler, input.texCoord2);

	if (vEffectFlagDesc.x > 0.99f && length(vGlowFlagDesc.xyz) > 0.1f)
	{
		vGlowColor = vGlowFlagDesc.xyz;
		fGlowPower = vGlowFlagDesc.a;
	}
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord3) * weight2;
	vGlowFlagDesc = g_GlowFlagTexture.Sample(DefaultSampler, input.texCoord3 );
	vEffectFlagDesc = g_FlagTexture.Sample(DefaultSampler, input.texCoord3);
	if (vEffectFlagDesc.x > 0.99f && length(vGlowFlagDesc.xyz) > 0.1f)
	{
		vGlowColor = vGlowFlagDesc.xyz;
		fGlowPower = vGlowFlagDesc.a;
	}
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord4) * weight1;
	vGlowFlagDesc = g_GlowFlagTexture.Sample(DefaultSampler, input.texCoord4 );
	vEffectFlagDesc = g_FlagTexture.Sample(DefaultSampler, input.texCoord4);
	if (vEffectFlagDesc.x > 0.99f && length(vGlowFlagDesc.xyz) > 0.1f)
	{
		vGlowColor = vGlowFlagDesc.xyz;
		fGlowPower = vGlowFlagDesc.a;
	}
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord5) * weight0;
	vGlowFlagDesc = g_GlowFlagTexture.Sample(DefaultSampler, input.texCoord5 );
	vEffectFlagDesc = g_FlagTexture.Sample(DefaultSampler, input.texCoord5);
	if (vEffectFlagDesc.x > 0.99f && length(vGlowFlagDesc.xyz) > 0.1f)
	{
		vGlowColor = vGlowFlagDesc.xyz;
		fGlowPower = vGlowFlagDesc.a;
	}
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord6) * weight1;
	vGlowFlagDesc = g_GlowFlagTexture.Sample(DefaultSampler, input.texCoord6 );
	vEffectFlagDesc = g_FlagTexture.Sample(DefaultSampler, input.texCoord6);
	if (vEffectFlagDesc.x > 0.99f && length(vGlowFlagDesc.xyz) > 0.1f)
	{
		vGlowColor = vGlowFlagDesc.xyz;
		fGlowPower = vGlowFlagDesc.a;
	}
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord7) * weight2;
	vGlowFlagDesc = g_GlowFlagTexture.Sample(DefaultSampler, input.texCoord7 );
	vEffectFlagDesc = g_FlagTexture.Sample(DefaultSampler, input.texCoord7);
	if (vEffectFlagDesc.x > 0.99f && length(vGlowFlagDesc.xyz) > 0.1f)
	{
		vGlowColor = vGlowFlagDesc.xyz;
		fGlowPower = vGlowFlagDesc.a;
	}
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord8) * weight3;
	vGlowFlagDesc = g_GlowFlagTexture.Sample(DefaultSampler, input.texCoord8 );
	vEffectFlagDesc = g_FlagTexture.Sample(DefaultSampler, input.texCoord8);
	if (vEffectFlagDesc.x > 0.99f && length(vGlowFlagDesc.xyz) > 0.1f)
	{
		vGlowColor = vGlowFlagDesc.xyz;
		fGlowPower = vGlowFlagDesc.a;
	}
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord9) * weight4;
	vGlowFlagDesc = g_GlowFlagTexture.Sample(DefaultSampler, input.texCoord9 );
	vEffectFlagDesc = g_FlagTexture.Sample(DefaultSampler, input.texCoord9);
	if (vEffectFlagDesc.x > 0.99f && length(vGlowFlagDesc.xyz) > 0.1f)
	{
		vGlowColor = vGlowFlagDesc.xyz;
		fGlowPower = vGlowFlagDesc.a;
	}

	//첫 블러에 글로우 컬러로 모두 고정.
	Output.vColor.xyz = vGlowColor;
	Output.vColor.a *= fGlowPower;

	return Output;
}







//PS_OUT	PS_VERTICAL_MAIN(PS_IN input)
//{
//	PS_OUT	Output = (PS_OUT)1;
//
//	float weight0, weight1, weight2, weight3, weight4, weight5, weight6;
//	float normalization;
//
//	// Create a normalized value to average the weights out a bit.
//	normalization = (g_fWeight[0] + 2.0f * (g_fWeight[6] + g_fWeight[1] + g_fWeight[2] + g_fWeight[3] + g_fWeight[4] + g_fWeight[5]));
//
//	// Normalize the weights.
//	weight0 = g_fWeight[0] / normalization;
//	weight1 = g_fWeight[1] / normalization;
//	weight2 = g_fWeight[2] / normalization;
//	weight3 = g_fWeight[3] / normalization;
//	weight4 = g_fWeight[4] / normalization;
//	weight5 = g_fWeight[5] / normalization;
//	weight6 = g_fWeight[6] / normalization;
//
//	// Initialize the color to black.
//	Output.vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
//
//	vector		vGlowFlagDesc = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	//xyz = color
//	//a = power
//	float3 vGlowColor = 1;
//	float fGlowPower = 1.f;
//
//
//	// Add the nine vertical pixels to the color by the specific weight of each.
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord1) * weight6;
//
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord1);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord2) * weight5;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord2);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord3) * weight4;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord3);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord4) * weight3;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord4);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord5) * weight2;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord5);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord6) * weight1;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord6);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord7) * weight0;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord7);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord8) * weight1;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord8);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord9) * weight2;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord9);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord10) * weight3;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord10);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord11) * weight4;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord11);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord12) * weight5;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord12);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord13) * weight6;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord13);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//
//
//
//	//섞는 픽셀의 이펙트 플래그의 색을 받고, 강도를 받기.
//
//	/*if (Output.vColor.a > 0.001f)
//	{
//		Output.vColor.a += 0.1f;
//	}
//	Output.vColor.a *= 2.f;*/
//	//Output.vColor.a = sqrt(Output.vColor.a);
//
//	if (fGlowPower > 0.f)
//		Output.vColor.a *= fGlowPower;
//
//
//	Output.vColor.xyz = vGlowColor;
//
//	return Output;
//}
//
//
//PS_OUT	PS_HORIZONTAL_MAIN(PS_IN input)
//{
//	PS_OUT	Output = (PS_OUT)1;
//
//	float weight0, weight1, weight2, weight3, weight4, weight5, weight6;
//	float normalization;
//
//	// Create a normalized value to average the weights out a bit.
//	normalization = (g_fWeight[0] + 2.0f * (g_fWeight[6] + g_fWeight[1] + g_fWeight[2] + g_fWeight[3] + g_fWeight[4] + g_fWeight[5]));
//
//	// Normalize the weights.
//	weight0 = g_fWeight[0] / normalization;
//	weight1 = g_fWeight[1] / normalization;
//	weight2 = g_fWeight[2] / normalization;
//	weight3 = g_fWeight[3] / normalization;
//	weight4 = g_fWeight[4] / normalization;
//	weight5 = g_fWeight[5] / normalization;
//	weight6 = g_fWeight[6] / normalization;
//
//	// Initialize the color to black.
//	Output.vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
//
//	vector		vGlowFlagDesc = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	//xyz = color
//	//a = power
//	float3 vGlowColor = 1;
//	float fGlowPower = 1.f;
//
//
//	// Add the nine vertical pixels to the color by the specific weight of each.
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord1) * weight6;
//
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord1);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord2) * weight5;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord2);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord3) * weight4;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord3);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord4) * weight3;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord4);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord5) * weight2;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord5);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord6) * weight1;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord6);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord7) * weight0;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord7);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord8) * weight1;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord8);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord9) * weight2;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord9);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord10) * weight3;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord10);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord11) * weight4;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord11);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord12) * weight5;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord12);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord13) * weight6;
//	vGlowFlagDesc = g_ShaderTexture.Sample(DefaultSampler, input.texCoord13);
//
//	//Check Power
//	if (vGlowFlagDesc.a > 0.f)
//		fGlowPower = vGlowFlagDesc.a;
//	//Check Color
//	if (length(vGlowFlagDesc) > 0)
//		vGlowColor = vGlowFlagDesc.xyz;
//
//
//
//
//	//섞는 픽셀의 이펙트 플래그의 색을 받고, 강도를 받기.
//
//	/*if (fGlowPower > 0.f)
//		Output.vColor.a *= fGlowPower;*/
//	Output.vColor.xyz = vGlowColor;
//
//	return Output;
//}

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
	pass GLOWABLE
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GLOWABLE_MAIN();
	}

	pass VERTICAL
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_VERTICAL_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DEFAULTBLUR_MAIN();
	}

	pass HORIZONTAL
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_HORIZONTAL_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DEFAULTBLUR_MAIN();
	}

	pass HORIZONTALFIRST
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_HORIZONTAL_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_FIRSTBLUR_MAIN();
	}

}