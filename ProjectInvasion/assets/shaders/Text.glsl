$vertexshader
#version 450 core
layout (location=0) in vec2 a_Position;
layout (location=1) in vec2 a_TexCoords;
layout (location=2) in vec4 a_Color;
layout (location=3) in float a_TexIndex;

out vec2 TexCoords;
out vec4 TextColor;
out float TexIndex;
uniform mat4 projection;

void main()
{
    TexIndex=a_TexIndex;
    TextColor=a_Color;
    TexCoords=a_TexCoords;
    gl_Position=projection*vec4(a_Position,0.2,1.0);
}



$fragmentshader
#version 450 core

in vec2 TexCoords;
in vec4 TextColor;
in float TexIndex;

out vec4 Color;
uniform sampler2D text;

layout(binding = 0) uniform sampler2D u_Textures[32];

void main()
{
    vec4 sampled;
    switch(int(TexIndex))
    {
        case 0: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[0], TexCoords).r); break;
        case 1: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[1], TexCoords).r); break;
        case 2: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[2], TexCoords).r); break;
        case 3: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[3], TexCoords).r); break;
        case 4: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[4], TexCoords).r); break;
        case 5: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[5], TexCoords).r); break;
        case 6: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[6], TexCoords).r); break;
        case 7: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[7], TexCoords).r); break;
        case 8: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[8], TexCoords).r); break;
        case 9: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[9], TexCoords).r); break;
        case 10: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[10], TexCoords).r); break;
        case 11: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[11], TexCoords).r); break;
        case 12: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[12], TexCoords).r); break;
        case 13: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[13], TexCoords).r); break;
        case 14: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[14], TexCoords).r); break;
        case 15: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[15], TexCoords).r); break;
        case 16: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[16], TexCoords).r); break;
        case 17: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[17], TexCoords).r); break;
        case 18: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[18], TexCoords).r); break;
        case 19: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[19], TexCoords).r); break;
        case 20: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[20], TexCoords).r); break;
        case 21: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[21], TexCoords).r); break;
        case 22: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[22], TexCoords).r); break;
        case 23: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[23], TexCoords).r); break;
        case 24: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[24], TexCoords).r); break;
        case 25: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[25], TexCoords).r); break;
        case 26: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[26], TexCoords).r); break;
        case 27: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[27], TexCoords).r); break;
        case 28: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[28], TexCoords).r); break;
        case 29: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[29], TexCoords).r); break;
        case 30: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[30], TexCoords).r); break;
        case 31: Color=vec4(TextColor)*vec4(1.0,1.0,1.0,texture(u_Textures[31], TexCoords).r); break;
    }

    // sampled=vec4(1.0,1.0,1.0,texture(u_Textures[2], TexCoords).r);

    // Color =vec4(TextColor)*sampled;



}