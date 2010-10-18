sampler2D tex0 : register(S0);

float2 dsOffsets[4];
float brightThreshold;

float4 PSBrightPass( in float2 t : TEXCOORD0 ) : COLOR
{
    
    float4 average = { 0.0f, 0.0f, 0.0f, 0.0f };

    // load in and combine the 4 samples from the source HDR texture
        for( int i = 0; i < 4; i++ )
        {
            average += tex2D( tex0, t + float2( dsOffsets[i].x, dsOffsets[i].y ) );
        }
            
        average *= 0.25f;

    // Determine the brightness of this particular pixel. As with the luminance calculations
    // there are 4 possible variations on this calculation:
    
    // 1. Do a very simple mathematical average:
        //float luminance = dot( average.rgb, float3( 0.33f, 0.33f, 0.33f ) );
    
    // 2. Perform a more accurately weighted average:
        //float luminance = dot( average.rgb, float3( 0.299f, 0.587f, 0.114f ) );
    
    // 3. Take the maximum value of the incoming, same as computing the
    //    brightness/value for an HSV/HSB conversion:
       //float luminance = max( average.r, max( average.g, average.b ) );
    
    // 4. Compute the luminance component as per the HSL colour space:
        //float luminance = 0.5f * ( max( average.r, max( average.g, average.b ) ) + min( average.r, min( average.g, average.b ) ) );
    
    // 5. Use the magnitude of the colour
        float luminance = length( average.rgb );
            
    // Determine whether this pixel passes the test...
        if( luminance < brightThreshold)
            average = float4( 0.0f, 0.0f, 0.0f, 1.0f );
    
    // Write the colour to the bright-pass render target
        return average;
    
}