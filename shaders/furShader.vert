#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 TexCoord;

out vec2 texCoord;
out vec4 vNormal;
out float vertexID;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(position, 1.0);
    texCoord = TexCoord;

    mat3 normalMatrix = mat3(transpose(inverse(view)));
    vNormal = normalize(projection * vec4(normalMatrix * normal, 0.0));

    vertexID = gl_VertexID;

    //vNormal = projection * view * vec4(normal, 0.0); //normalize(projection * view * vec4(normal, 0.0));
}
