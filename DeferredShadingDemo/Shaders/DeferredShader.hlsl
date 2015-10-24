Texture2D txPosition: register(t0);
Texture2D txNormal : register(t1);
Texture2D txAlbedo : register(t2);

SamplerState samLinear : register (s0);

float4 VS( float3 position : POSITION  ) : SV_POSITION
{
	return  float4( position, 1.0f ) ;
}

float4 PS( float4 pos : SV_POSITION ) : SV_TARGET
{
	float3 loadpos = float3( pos.xy, 0 );
	return txPosition.Load( loadpos );
}