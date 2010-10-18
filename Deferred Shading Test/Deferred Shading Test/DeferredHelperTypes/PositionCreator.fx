sampler	depthBuffer  : register( s0 );

float3 nearFrustumCorners[4];
float3 farFrustumCorners[4];

/*float3 GetNearPlanePosition(float2 t)
{
	float3 ret;
	ret.x = nearFrustumCorners
}*/

float4 PSPositionCreator( in float2 t : TEXCOORD0 ) : COLOR
{
	//float4 OUT;
	return float4(1,1,1,1);
}