sampler2D tex0 : register(S0);
sampler2D tex1 : register(S1);

float4 PSTextureAdder( in float2 t : TEXCOORD0 ) : COLOR0
{
	float4 sum = tex2D(tex0, t) + tex2D(tex1, t);
	sum.a = saturate(sum.a);
	return sum;
}