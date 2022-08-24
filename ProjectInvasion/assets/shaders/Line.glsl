$vertexshader
#version 330 core

layout (location = 0) in vec3 a_Position;
layout(location=1) in vec3 a_Color;

out vec3 Color;
uniform mat4 projection;

void main(){
    Color=a_Color;
    gl_Position=projection*vec4(a_Position,1.0);
}


$fragmentshader
#version 330 core

layout (location = 0) out vec4 color;

in vec3 Color;


void main(){
 color=vec4(Color,1.0);   
}