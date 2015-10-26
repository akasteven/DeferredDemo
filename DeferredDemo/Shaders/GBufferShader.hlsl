Texture2D txDiffuse: register(t0);
SamplerState samLinear : register (s0);

cbuffer cbPerObject: register( b3 )
{
	float4x4 matWorld;
    float4x4 matWVP;
	float4x4 matWorldInvTranspose;
	int isInstancing;
	float3 padding;
};


struct VSINPUT
{
	float3 PosL : POSITION;
	float3 NorL : NORMAL;
	float2 Tex : TEXCOORD;
	row_major float4x4 World : WORLD;
	uint InstanceId : SV_InstanceID;
};

struct PSINPUT
{
	float4 PosH : SV_POSITION ;
    float3 NorW : NORMAL ;
	float2 Tex  : TEXCOORD;	
	float3 PosW : POSITION;
};


struct PSOUTPUT
{
	float4 Position : SV_TARGET0;
	float4 Normal : SV_TARGET1;
	float4 Albedo : SV_TARGET2;
};


PSINPUT VS( VSINPUT input )
{
    PSINPUT output = (PSINPUT)0;
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

PSOUTPUT PS( PSINPUT input )
{
	PSOUTPUT output = (PSOUTPUT) 0 ;
	float3 diffuseAlbedo = txDiffuse.Sample(samLinear, input.Tex).rgb ;
	output.Position = float4( input.PosW, 1.0f );
	output.Normal = float4(normalize(input.NorW), 1.0f ) ;
	output.Albedo = float4(diffuseAlbedo, 1.0f);
	
	return output ;
}
