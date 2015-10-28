Texture2D txDiffuse: register(t0);
SamplerState samLinear : register (s0);

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
	float4x4 matWorldInvTranspose;
	int isInstancing;
	float specularPower;
	float2 padding;
};

cbuffer cbPerLight : register( b4 )
{
	float3 LightDir;
	float  LightRange;
	float3 LightColor;
	float SpotAngle;
	float3 LightPos;
	float padding2;
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
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
	float3 posL = input.PosL;

	[flatten]
	if ( isInstancing == 1)
		posL = mul(float4(input.PosL, 1.0f), input.World).xyz;

	output.PosW = mul(float4(posL, 1.0f), matWorld).xyz;
	output.NorW = mul(input.NorL, (float3x3)matWorldInvTranspose);
	output.PosH = mul(float4(posL, 1.0f), matWVP);
	output.Tex = input.Tex;
    return output;
}

float4 PS_POINTLIGHT(PS_INPUT input) : SV_TARGET
{
	//Light direction 
	float3 dir = LightPos - input.PosW ;
		//Light distance
	float dist = length(dir);
	//Linear attenuation factor
	float attenuation = max(0, 1 - dist / LightRange);
	//Normalize light direction vector
	dir /= dist;

	float3 normal = normalize(input.NorW);
	//Calculate diffuse
	float NDL = saturate(dot(normal, dir));
	float3 albedo = txDiffuse.Sample(samLinear, input.Tex).xyz;
	float3 diffuse = NDL * LightColor * albedo;

		//Calculate Specular
	float3 V = normalize(eyePos - input.PosW);
	float3 H = normalize(V + dir);
	float3 specular = pow(saturate(dot(normal, H)), specularPower) * LightColor * NDL;
	return float4((diffuse + specular) * attenuation, 1.0f);
}

float4 PS_DIRECTIONALLIGHT(PS_INPUT input) : SV_TARGET
{
	//Light direction 
	float3 dir = normalize(-LightDir);

	//Normal
	float3 normal = normalize(input.NorW);

	//Calculate diffuse
	float NDL = saturate(dot(normal, dir));
	float3 albedo = txDiffuse.Sample(samLinear, input.Tex).xyz;
	float3 diffuse = NDL * LightColor * albedo;

	//Calculate Specular
	float3 V = normalize(eyePos - input.PosW);
	float3 H = normalize(V + dir);
	float3 specular = pow(saturate(dot(normal, H)), specularPower) * LightColor * NDL;

	return float4((diffuse + specular), 1.0f);
}