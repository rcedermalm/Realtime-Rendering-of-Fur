#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 TexCoord;

out vec2 vTexCoord;
out vec3 vNormal;
out float vVertexID;

void main()
{
    gl_Position = vec4(position, 1.0);
    vTexCoord = TexCoord;
    vNormal = normalize(normal);
    vVertexID = gl_VertexID;
}
