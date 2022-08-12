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
struct Light {
    vec3 Position;
    vec3 Color;
    vec4 Meta;
};
const int NR_LIGHTS = 32;

in vec2 o_UV;

uniform sampler2D u_Texture;// Position Buffer
uniform sampler2D u_Texture1;// Normals
uniform sampler2D u_Texture2;// Color

uniform Light u_Lights[NR_LIGHTS];
uniform vec3 u_ViewPos;
uniform int u_LightCount;

layout(location = 0) out vec4 OutColor;
layout(location = 1) out vec4 Brightness;

void main()
{
    vec4 AlbedoRaw = texture(u_Texture2, o_UV);
    vec3 FragPos = texture(u_Texture, o_UV).rgb;
    vec3 Normal = texture(u_Texture1, o_UV).rgb;
    vec3 Albedo = AlbedoRaw.rgb;
    float Specular = AlbedoRaw.a;

/*    float dDepth= texture(u_Texture3, o_UV, 0).r;
    if (dDepth >= 1.0) discard;*/

    vec3 lighting = Albedo * 0.1;// hard-coded ambient component
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    for (int i = 0; i < u_LightCount; ++i)
    {
        Light light = u_Lights[i];
        float distance = length(light.Position - FragPos);
        vec3 lightColor = light.Color * light.Meta.w;
        vec3 lightDir = normalize(light.Position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lightColor;

        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);
        vec3 specular = lightColor * spec * Specular;

        float attenuation = 1.0 / (1.0 + light.Meta.y * distance + light.Meta.z * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;
    }

    OutColor = vec4(lighting, 1.0);
    float brightness = dot(OutColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0) {
        Brightness = vec4(OutColor.rgb, 1.0);
    } else {
        Brightness = vec4(0.0, 0.0, 0.0, 1.0);
    }
}