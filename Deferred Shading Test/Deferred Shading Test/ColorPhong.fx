//Transform vertices to world space
float4x4 world;
//Transform normals to world space
float4x4 worldInverseTranspose;
//transform view coordinates to world space
float4x4 viewInverse;
//Transform coordinates to screen space
float4x4 worldViewProjection;

//Light position in world space
float3 lightPosition;

struct VertexInput
{
	float3 position: POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR0;
};

struct VertexOutput
{
	float4 screenPos : POSITION;
	float3 lightVector : TEXCOORD1;
	float3 worldNormal : TEXCOORD2;
	float3 worldView : TEXCOORD3;
	float4 color : COLOR0;
};


VertexOutput ColorPhongVS(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;
	OUT.worldNormal = mul(IN.normal, worldInverseTranspose).xyz;
	
	float4 worldPosition = mul(float4(IN.position.xyz, 1), world);
	OUT.lightVector = lightPosition.xyz - worldPosition.xyz;
	OUT.worldView = normalize(viewInverse[3].xyz - worldPosition.xyz);
	OUT.screenPos = mul(float4(IN.position, 1), worldViewProjection);
	OUT.color = IN.color;	

	return OUT;
}

float3 lightColor;
float3 ambientColor;
float specularAmount;
float specularExponent;

float4 ColorPhongPS(VertexOutput IN) : COLOR
{
	float3 light = normalize(IN.lightVector);
	float3 view = normalize(IN.worldView);
	float3 norm = normalize(IN.worldNormal);
	float3 halfAngle = normalize(view + light);
	float4 litResult = lit(dot(light, norm), 
		dot(halfAngle, norm), specularExponent);
	float3 diffuseContrib = litResult.y * lightColor;
	float3 specularContrib = litResult.y * litResult.z * 
		specularAmount * lightColor;
	float3 diffuse = IN.color.rgb;
	float4 OUT = float4(specularContrib +
		(diffuse * (diffuseContrib + ambientColor)), 1);
	return OUT;
}
