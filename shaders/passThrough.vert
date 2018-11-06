#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour;
layout (location = 2) in vec2 TexCoord;

out vec2 st;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position =  projection * view * vec4(position, 1.0f);
    st = TexCoord;
}