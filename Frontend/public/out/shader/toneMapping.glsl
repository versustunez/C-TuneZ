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
    const float exposure = 1.0;
    const float gamma = 1.0;
    vec3 hdrColor = texture(u_Texture, o_UV).rgb;
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    result = pow(result, vec3(1.0 / gamma));
    color = vec4(result, 1.0);
}