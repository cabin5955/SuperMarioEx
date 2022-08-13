#version 300 es
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texcoords;
layout (location = 2) in vec3 a_normal;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    float u = a_texcoords.x;
    float v = a_texcoords.y;
    
    FragPos = vec3(model * vec4(a_position, 1.0));
    TexCoords = vec2(u,v);
    Normal = a_normal;
    gl_Position = projection * view * model * vec4(a_position.xy, 0.0, 1.0);
}

