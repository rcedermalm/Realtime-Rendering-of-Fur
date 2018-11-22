#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 TexCoord;

uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
out vec3 vertexNormal;
out vec3 vertexPosition;

void main()
{
    gl_Position = projection * view *  vec4(position, 1.0);
    texCoord = TexCoord;
    vertexNormal = normal;
    vertexPosition = position;
}
