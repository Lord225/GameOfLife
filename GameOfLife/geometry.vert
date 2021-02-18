#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex;

uniform mat4 proj;

out vec2 Tex;

void main()
{
    gl_Position = proj * vec4(vPos, 1.0f);
    Tex = vTex;
}