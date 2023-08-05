$vertexshader
#version 450 core
layout (location=0) in vec2 a_Position;
layout (location=1) in vec2 a_TexCoords;
layout (location=2) in vec4 a_Color;
layout (location=2) in float a_TexIndex;

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

out vec4 Color;
uniform sampler2D text;

void main()
{
    vec4 sampled=vec4(1.0,1.0,1.0,texture(text, TexCoords).r);
    Color =vec4(TextColor)*sampled;
}