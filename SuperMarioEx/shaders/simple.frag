#version 300 es
precision mediump float;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

layout(location = 0) out vec4 color;

uniform sampler2D sprite;

void main()
{
    color = texture(sprite, TexCoords);
}
