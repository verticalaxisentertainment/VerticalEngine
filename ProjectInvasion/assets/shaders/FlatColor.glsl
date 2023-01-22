$vertexshader
#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec4 a_Color;
layout (location = 3) in float a_TexIndex;

uniform mat4 projection;

out vec4 Color;
out vec2 TexCoord;
out float TexIndex;
out vec3 Position;

void main()
{
  Position=a_Position;
  TexIndex=a_TexIndex;
  TexCoord=a_TexCoord;
  Color=a_Color;
  gl_Position=projection*vec4(a_Position,1.0);
}



$fragmentshader
#version 330 core

layout (location=0) out vec4 color;

in vec3 Position;
in vec4 Color;
in float TexIndex;
uniform float iTime;

uniform vec3 lightPos;
in vec2 TexCoord;
uniform sampler2D u_Textures[32];

void main()
{
  // color=texture(tex,TexCoord)*vec4(Color.r*sin(iTime)/2+1.0,Color.g*sin(iTime)/2+0.7,Color.b*sin(iTime)/2+0.3,1.0);

  // if(distance(Position,lightPos)<5.0f&&TexIndex==2)
  //   color=texture(u_Textures[1],TexCoord);
  // if(TexIndex==0)
  // {
  //   color=Color;
  // }
  // color=mix(texture(tex,TexCoord),texture(tex2,TexCoord),0.5)*vec4(Color.r*sin(iTime)/2+1.0,Color.g*sin(iTime)/2+0.7,Color.b*sin(iTime)/2+0.3,1.0);
  switch(int(TexIndex))
  {
    case 0: color=Color; break;
    case 1: color=texture(u_Textures[0],TexCoord); break;
    case 2: color=texture(u_Textures[1],TexCoord); break;
    case 3: color=texture(u_Textures[2],TexCoord); break;
    case 4: color=texture(u_Textures[3],TexCoord); break;
    case 5: color=texture(u_Textures[4],TexCoord); break;
    case 6: color=texture(u_Textures[5],TexCoord); break;
  }
  // vec3 zaa=vec3(0.0,5.0,-0.1);
  // if(TexIndex==1)
  // {
  //   color=texture(u_Textures[1],TexCoord);
  // }
  // if(distance(lightPos,zaa)<5.0f&&TexIndex==2)
  // {
  //   color=texture(u_Textures[1],TexCoord);
  // }
  // if(TexIndex==0)
  // {
  //   color=Color;
  // }

  // color=vec4(1.0,1.0,1.0,1.0);
  
  // if(TexIndex==0)
  //   color=Color;
  // else
  //   color=texture(u_Textures[0],TexCoord);
}