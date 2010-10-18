sampler2D tex0 : register(S0);

float2 lumOffsets[4];

float4 PSLuminance( in float2 t : TEXCOORD0 ) : COLOR0
{

    // Compute the average of the 4 necessary samples
        float average = 0.0f;
        float4 color = 0.0f;
		float maximum = -1e20;
        float3 WEIGHT = float3( 0.299f, 0.587f, 0.114f );
        
        for( int i = 0; i < 4; i++ )
        {
            color = tex2D( tex0, t + lumOffsets[i]);
            
            // There are a number of ways we can try and convert the RGB value into
            // a single luminance value:
            
            // 1. Do a very simple mathematical average:
            //float GreyValue = dot( color.rgb, float3( 0.33f, 0.33f, 0.33f ) );
            
            // 2. Perform a more accurately weighted average:
            //float GreyValue = dot( color.rgb, WEIGHT );
            
            // 3. Take the maximum value of the incoming, same as computing the
            //    brightness/value for an HSV/HSB conversion:
            //float GreyValue = max( color.r, max( color.g, color.b ) );
            
            // 4. Compute the luminance component as per the HSL colour space:
           //float GreyValue = 0.5f * ( max( color.r, max( color.g, color.b ) ) + min( color.r, min( color.g, color.b ) ) );
            
            // 5. Use the magnitude of the colour
            float GreyValue = length( color.rgb );

			maximum = max( maximum, GreyValue );
            average += (0.25f * log( 1e-5 + GreyValue )); //1e-5 necessary to stop the singularity at GreyValue=0
        }
        
        average = exp( average );
        
    // Output the luminance to the render target
        return float4(average, maximum, 0.0f, 1.0f);
        
}

float2 dsOffsets[9];
float halfDestPixelSize;

float4 PSLuminanceDownsample( in float2 t : TEXCOORD0 ) : COLOR0
{
    
    // Compute the average of the 10 necessary samples
        float4 color = 0.0f;
        float average = 0.0f;
        
        for( int i = 0; i < 9;  i++ )
        {
            color = tex2D( tex0, t + float2(halfDestPixelSize, halfDestPixelSize) + dsOffsets[i]);
            average += color.r;
        }
        
    // We've just taken 9 samples from the
    // high resolution texture, so compute the
    // actual average that is written to the
    // lower resolution texture (render target).
        average /= 9.0f;
        
    // Return the new average luminance
        return average;
}