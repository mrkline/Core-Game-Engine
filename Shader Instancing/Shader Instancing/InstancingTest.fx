sampler2D tex0 : register(S0);

//Transform vertices to world space
float4x4 worldTransforms[60];
//Transform coordinates to screen space
float4x4 viewProjection;

struct VertexInput
{
	float3 position: POSITION;
	float4 normal : NORMAL;
	float2 uv : TEXCOORD0;
	float2 instanceIndex : TEXCOORD1;
	float4 color : COLOR0;
};

struct VertexOutput
{
	float4 screenPos : POSITION;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};

VertexOutput InstancingVS(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;
	int index = IN.instanceIndex.x;
	float4x4 worldViewProjection = mul(worldTransforms[index], viewProjection);
	OUT.screenPos = mul(float4(IN.position, 1), worldViewProjection);
	OUT.color = IN.color;
	OUT.uv = IN.uv;

	return OUT;
}

float4 InstancingPS(VertexOutput IN) : COLOR0
{
	return tex2D(tex0, IN.uv);
}