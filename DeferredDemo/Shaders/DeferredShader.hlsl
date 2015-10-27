Texture2D txPosition: register(t0);
Texture2D txNormal : register(t1);
Texture2D txAlbedo : register(t2);

SamplerState samLinear : register (s0);


cbuffer cbPerFrame : register(b2)
{
	float3 eyePos;
	float pad;
};

cbuffer cbPerLight : register(b3)
{	
	float3 LightDir;
	float  LightRange;
	float3 LightColor;
	float SpotAngle;
	float3 LightPos;
	float padding;
};

float4 VS( float3 position : POSITION  ) : SV_POSITION
{
	return  float4( position, 1.0f ) ;
}

void GetAtttributes( in float2 screenpos,  out float3 pos, out float3 norm, out float3 abd, out float specPower)
{
	int3 idx = int3(screenpos.xy, 0);
	pos = txPosition.Load(idx).xyz;
	norm = txNormal.Load(idx).xyz;
	float4 albedo = txAlbedo.Load(idx);
	abd = albedo.xyz;
	specPower = albedo.w;
}

float4 PS_POINTLIGHT(float4 pos : SV_POSITION) : SV_TARGET
{
	float3 normal;
	float3 position;
	float3 albedo;
	float specularPower;

	//Sample attributes 
	GetAtttributes(pos, position, normal, albedo, specularPower);
	//Light direction 
	float3 dir = LightPos - position;
	//Light distance
		float dist = length(dir);
	//Linear attenuation factor
	float attenuation = max(0 , 1 - dist / LightRange );
	//Normalize light direction vector
	dir /= dist;

	//Calculate diffuse
	float NDL = saturate(dot(normal, dir));
	float3 diffuse = NDL * LightColor * albedo;

	//Calculate Specular
	float3 V = eyePos - position;
	float3 H = normalize(V + dir);
	float3 specular = pow(saturate(dot(normal, H)), specularPower) * LightColor * NDL;

	return float4((diffuse + specular) * attenuation , 1.0f);
}

float4 PS_SPOTLIGHT(float4 pos : SV_POSITION) : SV_TARGET
{
	float3 normal;
	float3 position;
	float3 albedo;
	float specularPower;

	//Sample attributes 
	GetAtttributes(pos, position, normal, albedo, specularPower);

	//Light direction 
	float3 dir = LightPos - position;

	//Light distance
	float dist = length(dir);

	//Normalize light direction vector
	dir /= dist;

	//Attenuation factor
	float rho = dot(dir, -LightDir);
	float attenuation = max(0, 1 - dist / LightRange);// *max(0, 1 - rho / SpotAngle);

	//Calculate diffuse
	float NDL = saturate(dot(normal, dir));
	float3 diffuse = NDL * LightColor * albedo;

	return float4(diffuse * attenuation, 1.0f);
}

float4 PS_DIRECTIONALLIGHT(float4 pos : SV_POSITION) : SV_TARGET
{
	float3 normal;
	float3 position;
	float3 albedo;
	float specularPower;

	//Sample attributes 
	GetAtttributes(pos, position, normal, albedo, specularPower);

	//Light direction 
	float3 dir = normalize(-LightDir) ;

	//Calculate diffuse
	float NDL = saturate(dot(normal, dir));
	float3 diffuse = NDL * LightColor * albedo;

	//Calculate Specular
	float3 V = eyePos - position;
	float3 H = normalize(V + dir);
	float3 specular = pow(saturate(dot(normal, H)), specularPower) * LightColor * NDL;

	return float4((diffuse + specular), 1.0f);
}