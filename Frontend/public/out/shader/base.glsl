#type vertex
#version 300 es

precision mediump float;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec3  a_Normal;
layout(location = 2) in vec3  a_Color;
layout(location = 3) in vec3  a_UV;
layout(location = 4) in mat4 a_Transform;

uniform mat4 u_ViewProjection;

out vec3 o_Color;
out vec3 o_Normal;
out vec3 o_UV;
out vec3 o_FragPosition;

void main()
{
    o_Normal = transpose(inverse(mat3(a_Transform))) * a_Normal;
    o_Color = a_Color;
    o_UV = a_UV;
    vec4 pos = a_Transform * vec4(a_Position, 1.0);
    o_FragPosition = pos.xyz;
    gl_Position = u_ViewProjection * pos;
}

#type fragment
#version 300 es

precision mediump float;

in vec3 o_Color;
in vec3 o_Normal;
in vec3 o_UV;
in vec3 o_FragPosition;

layout(location = 0) out vec4 Position;
layout(location = 1) out vec4 Normals;
layout(location = 2) out vec4 Color;

void main()
{
    Position = vec4(o_FragPosition, 1.0);
    Normals = vec4(normalize(o_Normal), 1.0);
    Color.rgb = o_Color;
    Color.a = 1.0;
}