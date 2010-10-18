sampler2D    original_scene  : register( s0 );   // The HDR data
//sampler     luminance       : register( s1 );   // The 1x1 luminance map

float       exposure;                          // A user configurable bias to under/over expose the image
float		avgLuminance; //average luminane of the scene (hopefully better than continuous texture lookups)

float4 PSHDR( in float2 t : TEXCOORD0 ) : COLOR0
{
	// Read the HDR value that was computed as part of the original scene
	float4 c = tex2D( original_scene, t );
    
	// Read the luminance value, target the centre of the texture
	// which will map to the only pixel in it!
	//float4 l = tex2D( luminance, float2( 0.5f, 0.5f ) );
            
	//Since we'll be spending most of our time staring at the black depths of space,
	//we don't want to get any more sensative to light.
	float4 adjustedColor = c / avgLuminance * exposure;
	
    c = adjustedColor;
    c.a = 1.0f;
    return c;

}