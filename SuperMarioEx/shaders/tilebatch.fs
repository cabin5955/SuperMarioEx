#version 300 es
precision mediump float;
in vec4 ourColor;
in vec2 TexCoords;
layout(location = 0) out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;

void main()
{
    color = vec4(spriteColor, 1.0) * texture(sprite, TexCoords);
}
