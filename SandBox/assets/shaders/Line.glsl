$vertexshader
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 projection;
out vec4 Color;

void main(){
    Color=a_Color;
    gl_Position=projection*vec4(a_Position,1.0);
}


$fragmentshader
#version 330 core

out vec4 FragColor;
in vec4 Color;

void main(){
 FragColor=Color;   
}