#version 330                                                                        
                                                                                    
in vec4 LightSpacePos;                                                              
in vec2 TexCoord0;                                                                  
in vec3 Normal0;                                                                    
in vec3 WorldPos0;                                                                  
                                                                                    
out vec4 FragColor;                                                                 
                                                                                    
struct BaseLight                                                                    
{                                                                                   
    vec3 Color;                                                                     
    float AmbientIntensity;                                                         
    float DiffuseIntensity;                                                         
};                                                                                  
                                                                                    
struct DirectionalLight                                                             
{                                                                                   
    BaseLight Base;                                                          
    vec3 Direction;                                                                 
};                                                                                  
                                                               
uniform DirectionalLight gDirectionalLight;                                                                                             
uniform sampler2D gSampler;                                                                 
uniform sampler2D gShadowMap;                                                               
uniform vec3 gEyeWorldPos;                                                                  
uniform float gMatSpecularIntensity;                                                        
uniform float gSpecularPower;                                                               

float CalcShadowFactor(vec4 aLightSpacePos)                                                  
{                                                                       
    vec3 ProjCoords = aLightSpacePos.xyz / aLightSpacePos.w;                                  
    vec2 UVCoords;                                                                          
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;                                                  
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;                                                  
    float z = 0.5 * ProjCoords.z + 0.5;                                                     
    float Depth = texture(gShadowMap, UVCoords).x * 255;                                          
    if (Depth < z + 0.0001)                                                                 
        return 0.0;
    else                                                                                    
        return 1.0;                                                                         
}
                                                                                           
vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal,            
                       float ShadowFactor)                                                  
{                                                                                           
    vec4 AmbientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0f);
    float DiffuseFactor = dot(Normal, -LightDirection);                                     
                                                                                            
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                                  
    vec4 SpecularColor = vec4(0, 0, 0, 0);                                                  
                                                                                            
    if (DiffuseFactor > 0) {                                                                
        DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0f);    
                                                                                            
        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);                             
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));                     
        float SpecularFactor = dot(VertexToEye, LightReflect);                                      
        if (SpecularFactor > 0) {                                                           
            SpecularFactor = pow(SpecularFactor, gSpecularPower);                               
            SpecularColor = vec4(Light.Color, 1.0f) * gMatSpecularIntensity * SpecularFactor;                         
        }                                                                                   
    }                                                                                       
//return (AmbientColor + ShadowFactor * (DiffuseColor + SpecularColor));
    return (AmbientColor + ShadowFactor * (DiffuseColor));                  
}                                                                                           
                                                                                            
vec4 CalcDirectionalLight(vec3 Normal)                                                      
{      
	return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal, 1.0);                                                                                            
    //return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal, CalcShadowFactor(LightSpacePos));  
}                                                                                                
                                                   
void main()                                                                                 
{                                                                                           
    vec3 Normal = normalize(Normal0);                                                       
    vec4 TotalLight = CalcDirectionalLight(Normal);                                         
    //vec4 TotalLight = vec4(1.0,1.0,1.0,1.0);                                                                                      
    vec4 SampledColor = texture2D(gSampler, TexCoord0.xy);                                  
    FragColor = SampledColor * TotalLight;
	//FragColor = vec4(FragColor.x, FragColor.y, FragColor.z, 1.0);
	//FragColor = texture2D(gShadowMap, TexCoord0.xy);  
	//FragColor.z = 0.1;                                          
}
