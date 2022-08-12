#type vertex
#version 300 es
layout(location = 0) in vec3  a_Position;
layout(location = 3) in vec2 a_UV;

out vec2 o_UV;

void main() {
    gl_Position = vec4(a_Position, 1.0);
    o_UV = a_UV;
}

#type fragment
#version 300 es
precision highp float;
in vec2 o_UV;

uniform sampler2D u_Texture;
uniform bool u_Horizontal;

out vec4 o_Color;

void main()
{
    float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
    ivec2 size = textureSize(u_Texture, 0);
    vec2 tex_offset =  1.0 / vec2(size.x, size.y);
    vec3 result = texture(u_Texture, o_UV).rgb * weight[0];
    if (u_Horizontal)
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(u_Texture, o_UV + vec2(tex_offset.x * float(i), 0.0)).rgb * weight[i];
            result += texture(u_Texture, o_UV - vec2(tex_offset.x * float(i), 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(u_Texture, o_UV + vec2(0.0, tex_offset.y * float(i))).rgb * weight[i];
            result += texture(u_Texture, o_UV - vec2(0.0, tex_offset.y * float(i))).rgb * weight[i];
        }
    }
    o_Color = vec4(result,1.0);
}
