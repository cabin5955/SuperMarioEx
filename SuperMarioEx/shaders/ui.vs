#version 300 es
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_texcoords;

out vec2 TexCoords;

uniform mat4 model;
// note that we're omitting the view matrix; the view never changes so we basically have an identity view matrix and can therefore omit it.
uniform mat4 projection;

void main()
{
    TexCoords = a_texcoords.xy;
    gl_Position = projection * model * vec4(a_position.xy, 0.0, 1.0);
}
