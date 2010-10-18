sampler2D tex0 : register(S0);

float4 PSInvert(float2 texCoord : TEXCOORD0) : COLOR0
{
	float4 texColor = tex2D( tex0, texCoord);
	float4 OUT = float4(1, 1, 1, 1);
	OUT.rgb -= texColor.rgb;
	return OUT;
}