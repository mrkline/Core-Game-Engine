sampler2D tex0 : register(S0);

float blurOffsets[9];
float blurWeights[9];

float4 PSGaussianBlurH( in float2 t : TEXCOORD0 ) : COLOR
{

    float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    for( int i = 0; i < 9; i++ )
    {
        color += (tex2D( tex0, t + float2( blurOffsets[i], 0.0f ) ) * blurWeights[i] );
    }
        
    return float4( color.rgb, 1.0f );
    
}

float4 PSGaussianBlurV( in float2 t : TEXCOORD0 ) : COLOR
{

    float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    for( int i = 0; i < 9; i++ )
    {
        color += (tex2D( tex0, t + float2(0.0f, blurOffsets[i]) ) * blurWeights[i] );
    }
        
    return float4( color.rgb, 1.0f );
    
}