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
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;
uniform sampler2D u_Texture4;
uniform int u_Passes;

out vec4 o_Color;

void main()
{
    vec3 hdrColor = texture(u_Texture, o_UV).rgb;
    vec3 bloomColor = texture(u_Texture1, o_UV).rgb;
    bloomColor += texture(u_Texture2, o_UV).rgb;
    bloomColor += texture(u_Texture3, o_UV).rgb;
    bloomColor += texture(u_Texture4, o_UV).rgb;
    hdrColor += bloomColor;
    o_Color = vec4(hdrColor, 1.0);
}