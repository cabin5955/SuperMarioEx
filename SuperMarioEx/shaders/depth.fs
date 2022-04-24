#version 300 es
precision mediump float;

in vec2 TexCoords;

uniform sampler2D sprite;

layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = texture(sprite, TexCoords); 
}
