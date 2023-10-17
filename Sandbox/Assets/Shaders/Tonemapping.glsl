#type vertex

#version 460 core

layout (location = 0) out vec2 o_ScreenPosition;

void main()
{
    if(gl_VertexID == 0) {
        o_ScreenPosition = vec2(1.0, 2.0);
        gl_Position = vec4(1.0, 3.0, 0.0, 1.0);
    }
    else if(gl_VertexID == 1) {
        o_ScreenPosition = vec2(-1.0, 0.0);
        gl_Position = vec4(-3.0, -1.0, 0.0, 1.0);
    }
    else /* if(gl_VertexID == 2) */ {
        o_ScreenPosition = vec2(1.0, 0.0);
        gl_Position = vec4(1.0, -1.0, 0.0, 1.0);
    }
}  

#type fragment

#version 460 core

const float gamma = 2.2f;

layout (location = 0) in vec2 o_ScreenPosition;

layout (location = 0) out vec4 OutputColor;

layout (location = 0) uniform float u_Exposure = 1.0f;
layout (binding = 0) uniform sampler2D u_ScreenTexture;

vec3 ACESFilm(vec3 color)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;

    return clamp(
        ((color * (a * color + b)) / (color * (c * color + d) + e)), 
        vec3(0.0f), 
        vec3(1.0f));
}

void main()
{
    vec3 color = texture(u_ScreenTexture, o_ScreenPosition).rgb * u_Exposure;
    color = ACESFilm(color);

    OutputColor = vec4(pow(color, vec3(1.0f / gamma)), 1.0f);
}  