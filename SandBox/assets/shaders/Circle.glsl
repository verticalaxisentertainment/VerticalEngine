$vertexshader
#version 450 core
layout (location=0) in vec3 a_WorldPosition;
layout (location=1) in vec3 a_LocalPosition;
layout (location=2) in vec4 a_Color;
layout (location=3) in float a_Thickness;
layout (location=4) in float a_Fade;
layout (location = 5) in int a_EntityID;

uniform mat4 projection;

out float Thickness;
out float Fade;
out vec4 Color;
out vec3 LocalPosition;
out flat int EntityID;

void main()
{
  Thickness=a_Thickness;
  Fade=a_Fade;
  Color=a_Color;
  LocalPosition=a_LocalPosition;
  EntityID=a_EntityID;

  gl_Position=projection*vec4(a_WorldPosition,1.0);
}



$fragmentshader
#version 450 core

layout (location=0) out vec4 color;
layout (location = 1) out int color2;

in float Thickness;
in float Fade;
in vec4 Color;
in vec3 LocalPosition;
in flat int EntityID;

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

    color2=EntityID;
	// color.a *= circle;

}