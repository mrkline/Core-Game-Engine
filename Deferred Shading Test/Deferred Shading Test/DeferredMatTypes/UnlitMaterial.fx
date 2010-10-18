struct VertexInput
{
   float4 position : POSITION;
   float4 normal : NORMAL;
   float4 color : COLOR0;
   float2 uvCoord : TEXCOORD0;
};

struct PixelInput
{
	float4 position : POSITION;
	float4 color : COLOR0;
	float3 normal : TEXCOORD0;
	float2 uvCoord : TEXCOORD1;
	float depth : TEXCOORD2;
};

struct PixelOutput
{
	float4 rt0 : COLOR0;
	float4 rt1 : COLOR1;
	float4 rt2 : COLOR2;
	float4 rt3 : COLOR3;
};

//Transform coordinates to world space
float4x4 world;
//Transform coordinates to screen space
float4x4 worldViewProjection;
//Transform normals to world space
float4x4 worldInverseTranspose;
float3 cameraDirection;
float3 cameraPosition;

PixelInput VSUnlit(VertexInput IN)
{
	PixelInput OUT = (PixelInput)0;
	OUT.color = IN.color;
	OUT.normal = normalize(mul(IN.normal, worldInverseTranspose).xyz);
	OUT.depth = dot(cameraDirection, mul(IN.position, world).xyz - cameraPosition);
	OUT.position = mul(IN.position, worldViewProjection);
	OUT.uvCoord = IN.uvCoord;

	return OUT;
}


PixelOutput PSUnlit(PixelInput IN)
{
	PixelOutput OUT = (PixelOutput)0;
	OUT.rt0 = IN.color;
	OUT.rt1 = float4(IN.normal.xyz, IN.depth);
	OUT.rt2 = float4(IN.depth, IN.depth, IN.depth, 1);
	OUT.rt3 = float4(0.0f, 0.0f, 1.0f, 1.0f);
	return OUT;
}