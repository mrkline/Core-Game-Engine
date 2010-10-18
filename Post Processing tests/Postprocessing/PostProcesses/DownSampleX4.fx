sampler2D tex0 : register(S0);

float2 dsOffsets[16];

float4 PSDownSample( in float2 t : TEXCOORD0 ) : COLOR
{
    
    float4 average = { 0.0f, 0.0f, 0.0f, 0.0f };

    for( int i = 0; i < 16; i++ )
    {
        average += tex2D( tex0, t + dsOffsets[i]);
    }
        
    average /= 16.0f ;

    return average;
}