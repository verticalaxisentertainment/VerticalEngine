$vertexshader
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0); 
}




$fragmentshader
#version 330 core
layout(location = 0) out vec4 FragColor;
// layout(location = 1) out int color2;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec2 resoulation;
const float offset = 1.0 / 300.0;

vec4 applyVignette(vec4 color)
{
    vec2 position = (gl_FragCoord.xy / resoulation) - vec2(0.5);           
    float dist = length(position);

    float radius = 0.5;
    float softness = 0.02;
    float vignette = smoothstep(radius, radius - softness, dist);

    color.rgb = color.rgb - (1.0 - vignette);

    return color;
}

void main()
{ 
    vec3 lightPos=vec3(0.0,0.0,0.5);
    vec3 a=vec3(-0.5,0.5,0.5);
    vec3 b=vec3(0.5,0.5,0.5);

    vec3 wa=a-lightPos;
    vec3 wb=b-lightPos;
    
    vec3 ea=lightPos+(normalize(wa)*5.0);
    vec3 eb=lightPos+(normalize(wb)*5.0);


//    vec2 uv = gl_FragCoord.xy / resoulation;
   
//     uv *=  1.0 - uv.yx;   //vec2(1.0)- uv.yx; -> 1.-u.yx; Thanks FabriceNeyret !
    
//     float vig = uv.x*uv.y * 15.0; // multiply with sth for intensity
    
//     vig = pow(vig, 0.25); 
    
//     FragColor = vig*texture(screenTexture, TexCoords);

    vec2 uv = gl_FragCoord.xy / resoulation.xy;
   
    uv *=  1.0 - uv.yx;   //vec2(1.0)- uv.yx; -> 1.-u.yx; Thanks FabriceNeyret !
    
    float vig = uv.x*uv.y * 15.0; // multiply with sth for intensity
    
    vig = pow(vig, 0.25); // change pow for modifying the extend of the  vignette

    
    FragColor = vec4(vig)*texture(screenTexture, TexCoords);
    // color2=50;
    // FragColor=texture(screenTexture,TexCoords); 
}