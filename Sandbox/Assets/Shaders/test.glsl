#type vertex

#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

layout (location = 0) out struct VertexOutput
{
    vec3 Position;
    vec2 TexCoord;
} Output;

layout (std140, binding = 0) uniform TransformData
{
    mat4 ModelMatrix;
    mat4 ViewProjectionMatrix;
} u_TransformData;

void main()
{
    Output.Position = vec3(u_TransformData.ModelMatrix * vec4(a_Position, 1.0f));
    Output.TexCoord = a_TexCoord;

    gl_Position = u_TransformData.ViewProjectionMatrix * vec4(Output.Position, 1.0f);
}

#type fragment

#version 460 core

layout (location = 0) in struct VertexOutput
{
    vec3 Position;
    vec2 TexCoord;
} Input;

layout (location = 0) out vec4 o_OutputColor;

layout (binding = 0) uniform sampler2D u_AlbedoTexture;
layout (binding = 1) uniform sampler2D u_NormalTexture;
layout (binding = 2) uniform sampler2D u_MetallicRoughnessTexture;
layout (binding = 3) uniform sampler2D u_EmissiveTexture;

void main()
{
    vec3 color = texture(u_EmissiveTexture, Input.TexCoord).rgb;
    o_OutputColor = vec4(color, 1.0f);
}