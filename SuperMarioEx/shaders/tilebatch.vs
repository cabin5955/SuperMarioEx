#version 300 es
layout(location = 0)  in vec3 a_position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in int  a_TexIndex;

out vec2 TexCoords;
out vec4 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = a_TexCoords;
    ourColor = a_Color;
    gl_Position = projection * view * model * vec4(a_position.xyz, 1.0);
}
