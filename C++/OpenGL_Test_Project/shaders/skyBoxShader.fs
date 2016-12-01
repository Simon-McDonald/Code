#version 330

in vec3 TexCoord0;

out vec4 FragColor;
//sampler2D
uniform samplerCube gSamplerCube;

void main()
{
    FragColor = texture(gSamplerCube, TexCoord0);
    //FragColor = gl_FragCoord;
    //FragColor = vec4(TexCoord0, 1.0);
}
