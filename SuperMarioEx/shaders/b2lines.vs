#version 300 es
layout(location = 0) in vec2 v_position;
layout(location = 1) in vec4 v_color;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 f_color;

void main()
{
    f_color = v_color;
    gl_Position = projectionMatrix * viewMatrix * vec4(v_position, 0.0f, 1.0f);
}
