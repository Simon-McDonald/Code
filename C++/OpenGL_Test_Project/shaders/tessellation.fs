#version 410 core
                                                          
in vec2 TexCoord_FS_in;
in vec3 Normal_FS_in;
in vec3 WorldPos_FS_in;                                                           
                                              
out vec4 FragColor;                                                                 
                                                                                          
uniform sampler2D gSampler;                                                                 

void main() {                                                                                           
    //vec3 Normal = normalize(Normal0);                                                       
    //vec4 TotalLight = dot(vec3(1.0, 0.0, 0.0), Normal);                                                                                 
    FragColor = texture(gSampler, TexCoord_FS_in.xy);                                  
    //FragColor = SampledColor * TotalLight;                                   
}


