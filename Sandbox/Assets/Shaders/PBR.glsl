#type vertex

#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Bitangent;

layout(location = 0) out VertexOutput {
    vec3 WorldPosition;
    vec2 TexCoord;
    mat3 TBNMatrix;
} Output;

layout(location = 0) uniform mat4 u_ModelMatrix;
layout(location = 1) uniform mat4 u_ViewProjectionMatrix;

void main()
{
   Output.WorldPosition = vec3(u_ModelMatrix * vec4(a_Position, 1.0f));   
   Output.TexCoord = a_TexCoord;

   vec3 T = normalize(vec3(u_ModelMatrix * vec4(a_Tangent,   0.0f)));
   vec3 B = normalize(vec3(u_ModelMatrix * vec4(a_Bitangent, 0.0f)));
   vec3 N = normalize(vec3(u_ModelMatrix * vec4(a_Normal,    0.0f)));
   Output.TBNMatrix = mat3(T, B, N);

   gl_Position = u_ViewProjectionMatrix * vec4(Output.WorldPosition, 1.0f);
}

#type fragment

#version 460 core

layout(location = 0) in VertexOutput {
    vec3 WorldPosition;
    vec2 TexCoord;
    mat3 TBNMatrix;
} Input;

layout(location = 0) out vec4 OutputColor;

layout(binding = 0) uniform sampler2D u_AlbedoTexture;
layout(binding = 1) uniform sampler2D u_NormalTexture;
layout(binding = 2) uniform sampler2D u_MetallicRoughnessTexture;
layout(binding = 3) uniform sampler2D u_EmissiveTexture;

layout(location = 3) uniform vec3 u_ViewPosition;

const int c_MaxLightCount = 1;
uniform vec3 lightPositions[c_MaxLightCount];
uniform vec3 lightColors[c_MaxLightCount];

const float M_PI = 3.14159265358979323846;
const float epsilon = 0.00001f;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;

    float denom = NdotH2 * (a2 - 1.0f) + 1.0f;
    denom = M_PI * denom * denom;

    return a2 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;

    float denom = NdotV * (1.0f - k) + k;

    return NdotV / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float ggx1 = GeometrySchlickGGX(max(dot(N, V), 0.0f), roughness);
    float ggx2 = GeometrySchlickGGX(max(dot(N, L), 0.0f), roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

float CalculateAttenuation(vec3 fragmentPosition, vec3 lightPosition)
{
    float d = length(lightPosition - fragmentPosition);
    float denom = d * d;

    return 1.0f / denom;
}

void main()
{           
    vec3 albedo = texture(u_AlbedoTexture, Input.TexCoord).rgb;
    vec3 normal = normalize(Input.TBNMatrix * (normalize(texture(u_NormalTexture, Input.TexCoord).rgb * 2.0f - 1.0f)));
    vec2 metalnessRoughness = texture(u_MetallicRoughnessTexture, Input.TexCoord).bg;
    float metalness = metalnessRoughness.r;
    float roughness = metalnessRoughness.g;

    vec3 viewDirection = normalize(u_ViewPosition - Input.WorldPosition);

    vec3 F0 = vec3(0.04f);
    F0 = mix(F0, albedo, metalness);

    vec3 directLighting = vec3(0.0);
    for(int i = 0; i < c_MaxLightCount; ++i)
    {
        vec3 L = normalize(lightPositions[i] - Input.WorldPosition);
        vec3 H = normalize(viewDirection + L);
      
        float attenuation = CalculateAttenuation(Input.WorldPosition, lightPositions[i]);
        vec3 radiance     = lightColors[i] * attenuation;

        vec3 F = FresnelSchlick(max(dot(H, viewDirection), 0.0f), F0);
        float NDF = DistributionGGX(normal, H, roughness);
        float G = GeometrySmith(normal, viewDirection, L, roughness);

        vec3 numerator = NDF * G * F;
        float denumerator = max(4.0f * max(dot(normal, viewDirection), 0.0f) * max(dot(normal, L), 0.0f), epsilon);
        vec3 specular = numerator / denumerator;

        vec3 kS = F;
        vec3 kD = mix(vec3(1.0f) - kS, vec3(0.0f), metalness);

        float NdotL = max(dot(normal, L), 0.0f);
        directLighting += (kD * albedo / M_PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03f) * albedo;

    OutputColor = vec4(ambient + directLighting, 1.0f);
}