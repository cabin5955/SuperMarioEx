#version 300 es
layout(location = 0) in vec4 a_position;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int flipx;
uniform int flipy;

void main()
{
    float u = a_position.z;
    float v = a_position.w;
    
    if(flipx == 1)
        u = 1.0 - a_position.z;
    
    if(flipy == 1)
        v = 1.0 - a_position.w;
    
    TexCoords = vec2(u,v);
    gl_Position = projection * view * model * vec4(a_position.xy, 0.0, 1.0);
}
