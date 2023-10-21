#type vertex

#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;

layout (location = 0) out vec2 v_TexCoords;

layout (location = 0) uniform mat4 u_ModelViewProjectionMatrix;

void main()
{
    v_TexCoords = a_TexCoords;

    gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 1.0f);
}

#type fragment

#version 460 core

layout (location = 0) in vec2 v_TexCoords;

layout (location = 0) out vec4 o_OutputColor;

layout (binding = 0) uniform sampler2D u_ColorTexture;

void main()
{
    vec3 color = texture(u_ColorTexture, v_TexCoords).rgb;
    o_OutputColor = vec4(color, 1.0f);
}