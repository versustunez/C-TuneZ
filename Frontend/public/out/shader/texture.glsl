#type vertex
#version 300 es

precision mediump float;

layout(location = 0) in vec3  a_Position;
layout(location = 3) in vec2 a_UV;

out vec2 o_UV;

void main()
{
    o_UV = a_UV;
    gl_Position = vec4(a_Position, 1.0);
}

#type fragment
#version 300 es

precision mediump float;

in vec2 o_UV;

uniform sampler2D u_Texture;

out vec4 color;

void main()
{
    color = texture(u_Texture, o_UV);
}