sampler	depthBuffer  : register( s0 );

struct VertexInput
{
   float4 position : POSITION;
   float2 uvCoord : TEXCOORD0;
   float2 vertexIndex : TEXCOORD1;
};

struct PixelInput
{
	float4 position : POSITION;
	float2 uvCoord : TEXCOORD0;
	float3 nearFrustumValue : TEXCOORD1;
	float3 farFrustumValue : TEXCOORD2;
};


float3 nearFrustumCorners[4];
float3 farFrustumCorners[4];

PixelInput VSPositionCreator(VertexInput IN)
{
	PixelInput OUT = (PixelInput)0;
	OUT.position = IN.position;
	OUT.uvCoord = IN.uvCoord;
	OUT.nearFrustumValue = nearFrustumCorners[IN.vertexIndex.x];
	OUT.farFrustumValue = farFrustumCorners[IN.vertexIndex.x];
	return OUT;
}

float4 PSPositionCreator(PixelInput IN) : COLOR
{
	float depth = 1.0f - tex2D(depthBuffer, IN.uvCoord).a;
	return float4(IN.nearFrustumValue + (IN.farFrustumValue - IN.nearFrustumValue) * depth, depth);
}