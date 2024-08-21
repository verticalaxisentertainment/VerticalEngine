$vertexshader
#version 330 core
layout (location=0) in vec2 a_Position;
layout(location=1) in vec2 a_TexCoord;
layout(location=2) in vec3 a_Color;

uniform mat4 projection;
uniform mat4 model;
out vec3 Color;
out vec2 TexCoord;

void main()
{
  TexCoord=a_TexCoord;
  Color=a_Color;  
  gl_Position=projection*model*vec4(a_Position,0.0,1.0);
}



$fragmentshader
#version 330 core

layout (location=0) out vec4 color;

in vec3 Color;
uniform float iTime;
uniform sampler2D tex;
uniform sampler2D tex2;
in vec2 TexCoord;

void main()
{
  // color=texture(tex,TexCoord)*vec4(Color.r*sin(iTime)/2+1.0,Color.g*sin(iTime)/2+0.7,Color.b*sin(iTime)/2+0.3,1.0);

  
  color=mix(texture(tex,TexCoord),texture(tex2,TexCoord),0.5)*vec4(Color.r*sin(iTime)/2+1.0,Color.g*sin(iTime)/2+0.7,Color.b*sin(iTime)/2+0.3,1.0);
 
  // color=vec4(Color,1.0);
}