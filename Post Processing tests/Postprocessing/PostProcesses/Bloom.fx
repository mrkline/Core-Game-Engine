sampler2D source: register(S0);

sampler2D bloom: register(S1);

float bloomLimit;

float4 PSBloom( in float2 t : TEXCOORD0 ) : COLOR0
{
	float4 bloomContribution = tex2D(bloom, t);

	float4 OUT = tex2D(source, t);
	OUT.r += min(bloomContribution.r, bloomLimit);
	OUT.g += min(bloomContribution.g, bloomLimit);
	OUT.b += min(bloomContribution.b, bloomLimit);
	return OUT;
}