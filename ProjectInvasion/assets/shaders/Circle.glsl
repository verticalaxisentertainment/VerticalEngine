$vertexshader
#version 330 core
layout (location=0) in vec3 a_WorldPosition;
layout (location=1) in vec3 a_LocalPosition;
layout (location=2) in vec4 a_Color;
layout (location=3) in float a_Thickness;
layout (location=4) in float a_Fade;

uniform mat4 projection;

out float Thickness;
out float Fade;
out vec4 Color;
out vec3 LocalPosition;

void main()
{
  Thickness=a_Thickness;
  Fade=a_Fade;
  Color=a_Color;
  LocalPosition=a_LocalPosition;

  gl_Position=projection*vec4(a_WorldPosition,1.0);
}



$fragmentshader
#version 330 core

layout (location=0) out vec4 color;

in float Thickness;
in float Fade;
in vec4 Color;
in vec3 LocalPosition;

void main()
{
   // Calculate distance and fill circle with white
    float distance = 1.0 - length(LocalPosition);
    float circle = smoothstep(0.0, Fade, distance);
    circle *= smoothstep(Thickness + Fade, Thickness, distance);

	if (circle == 0.0)
		discard;

    // Set output color
    color = Color;
    color.a*=circle;
	// color.a *= circle;

}