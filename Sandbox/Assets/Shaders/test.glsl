#type vertex

#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;

layout (location = 0) out vec2 o_TexCoords;

void main()
{
    o_TexCoords = a_TexCoords;

    gl_Position = vec4(a_Position, 1.0f);
}

#type fragment

#version 460 core

layout (location = 0) in vec2 o_TexCoords;

layout (location = 0) out vec4 OutputColor;

layout (binding = 0) uniform sampler2D u_ColorTexture;

void main()
{
    vec3 color = texture(u_ColorTexture, o_TexCoords).rgb;
    OutputColor = vec4(color, 1.0f);
}