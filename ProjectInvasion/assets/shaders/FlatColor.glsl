$vertexshader
#version 450 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec4 a_Color;
layout (location = 3) in float a_TexIndex;
layout (location = 4) in int a_EntityID;

uniform mat4 projection;

out vec4 Color;
out vec2 TexCoord;
out float TexIndex;
out vec3 Position;
out flat int EntityID;


void main()
{
  EntityID=a_EntityID;
  Position=a_Position;
  TexIndex=a_TexIndex;
  TexCoord=a_TexCoord;
  Color=a_Color;
  gl_Position=projection*vec4(a_Position,1.0);
}



$fragmentshader
#version 450 core

layout (location = 0) out vec4 color;
layout (location = 1) out int color2;


in vec3 Position;
in vec4 Color;
in float TexIndex;
in flat int EntityID;

uniform float iTime;

uniform vec3 lightPos;
in vec2 TexCoord;
layout (binding = 0) uniform sampler2D u_Textures[32];

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
    case 0: color=Color;break;
    case 1: color=texture(u_Textures[0],TexCoord); break;
    case 2: color=texture(u_Textures[1],TexCoord); break;
    case 3: color=texture(u_Textures[2],TexCoord); break;
    case 4: color=texture(u_Textures[3],TexCoord); break;
    case 5: color=texture(u_Textures[4],TexCoord); break;
    case 6: color=texture(u_Textures[5],TexCoord); break;
    case 7: color=texture(u_Textures[6],TexCoord); break;
    case 8: color=texture(u_Textures[7],TexCoord); break;
    case 9: color=texture(u_Textures[8],TexCoord); break;
    case 10: color=texture(u_Textures[9],TexCoord); break;
    case 11: color=texture(u_Textures[10],TexCoord); break;
    case 12: color=texture(u_Textures[11],TexCoord); break;
    case 13: color=texture(u_Textures[12],TexCoord); break;
    case 14: color=texture(u_Textures[13],TexCoord); break;
    case 15: color=texture(u_Textures[14],TexCoord); break;
    case 16: color=texture(u_Textures[15],TexCoord); break;
    case 17: color=texture(u_Textures[16],TexCoord); break;
    case 18: color=texture(u_Textures[17],TexCoord); break;
    case 19: color=texture(u_Textures[18],TexCoord); break;
    case 20: color=texture(u_Textures[19],TexCoord); break;
    case 21: color=texture(u_Textures[20],TexCoord); break;
    case 22: color=texture(u_Textures[21],TexCoord); break;
    case 23: color=texture(u_Textures[22],TexCoord); break;
    case 24: color=texture(u_Textures[23],TexCoord); break;
    case 25: color=texture(u_Textures[24],TexCoord); break;
    case 26: color=texture(u_Textures[25],TexCoord); break;
    case 27: color=texture(u_Textures[26],TexCoord); break;
    case 28: color=texture(u_Textures[27],TexCoord); break;
    case 29: color=texture(u_Textures[28],TexCoord); break;
    case 30: color=texture(u_Textures[29],TexCoord); break;
    case 31: color=texture(u_Textures[30],TexCoord); break;
  }
  // if(EntityID==1)
  // {
  // color2=vec4(EntityID,EntityID,EntityID,1);

  // }

  color2=EntityID;

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