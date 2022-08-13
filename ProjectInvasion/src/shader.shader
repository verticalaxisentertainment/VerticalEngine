$vertexshader
#version 330 core
layout (location=0) in vec2 a_Position;
layout(location=1) in vec2 a_Color;

uniform mat4 projection;

    void main()
    {
		    gl_Position=projection*vec4(a_Position,0.0,1.0);
    }



$fragmentshader
#version 330 core

	layout (location=0) out vec4 color;
  in vec2 TexCoords;


    void main()
    {
		  color=vec4(1.0,0.2,0.3,1.0);
    }