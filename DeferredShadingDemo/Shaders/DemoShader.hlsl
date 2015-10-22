#include "LightDef.hlsl"
#include "SimpleLighting.hlsl"

Texture2D txDiffuse: register(t0);
Texture2D txShadowMap : register(t2);

SamplerState samLinear : register (s0);
SamplerComparisonState samShadowMap : register(s1);

cbuffer cbNeverChanges : register( b0 )
{
	DirectionalLight DirLight;
	float ShadowMapSize;
	float3 pad3;
};

cbuffer cbChangeOnResize : register( b1 )
{
};

cbuffer cbPerFrame : register( b2 )
{
	float3 eyePos;
	float pad;
};

cbuffer cbPerObject: register( b3 )
{
	float4x4 matWorld;
    float4x4 matWVP;
	Material material;	
	float4x4 matWorldInvTranspose;
	float4x4 lightWVPT;
	int isInstancing;
	float3 padding;
};

struct VS_INPUT
{
	float3 PosL : POSITION;
	float3 NorL : NORMAL;
	float2 Tex : TEXCOORD;
	row_major float4x4 World : WORLD;
	uint InstanceId : SV_InstanceID;
};

struct PS_INPUT
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NorW : NORMAL ;
	float2 Tex : TEXCOORD;
	float4 ShadowH : TEXCOORD1;
};


float ShadowFactorPCF(SamplerComparisonState samShadow,
	Texture2D shadowMap,
	float4 shadowPosH)
{
	// Complete projection by doing division by w.
	shadowPosH.xyz /= shadowPosH.w;

	// Depth in NDC space.
	float depth = shadowPosH.z;

	// Texel size.
	const float dx = 1.0f / ShadowMapSize;

	float percentLit = 0.0f;
	const float2 offsets[9] =
	{
		float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
	};

	[unroll]
	for (int i = 0; i < 9; ++i)
	{
		percentLit += shadowMap.SampleCmpLevelZero(samShadow,
			shadowPosH.xy + offsets[i], depth).r;
	}

	return percentLit /= 9.0f;
}

PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	float3 posL = input.PosL;
	if ( isInstancing == 1)
		posL = mul(float4(input.PosL, 1.0f), input.World).xyz;

	output.PosW = mul(float4(posL, 1.0f), matWorld).xyz;
	output.NorW = mul(input.NorL, (float3x3)matWorldInvTranspose);
	output.PosH = mul(float4(posL, 1.0f), matWVP);
	output.ShadowH = mul(float4(posL, 1.0f), lightWVPT);
	output.Tex = input.Tex;
    return output;
}

float4 PS( PS_INPUT input) : SV_Target
{
	input.NorW = normalize(input.NorW);
	float3 toEye = eyePos - input.PosW;
	float distToEye = length(toEye);
	toEye /= distToEye;
	

	float PCF = ShadowFactorPCF(samShadowMap, txShadowMap, input.ShadowH);

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D, S;
	ComputeDirectionalLight(material, DirLight, input.NorW, toEye, A, D, S);

	ambient += A;
	diffuse += D * PCF;
	specular += S * PCF;

	float texColor = txDiffuse.Sample(samLinear, input.Tex);

	float4 litColor = texColor * (ambient + diffuse) + specular;
	litColor.a = 1.0f;

	return litColor;
}

