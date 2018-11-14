#version 430 core

//const float noOfHairSegments = 4.0f;
//const float noOfVertices = 3.0f;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 TexCoord;

uniform sampler2D hairDataTexture;
uniform float noOfHairSegments;
uniform float noOfVertices;

out vec2 texCoord;


void main()
{
    float offsetWidth = (1.0f / (noOfHairSegments * 3.0f) ) * 0.5f;
    float offsetHeight = (1.0f / noOfVertices) * 0.5f;

    vec2 firstHairStrandPos = vec2(offsetWidth, (gl_VertexID / noOfVertices) + offsetHeight);

    gl_Position = vec4(vec3(texture(hairDataTexture, firstHairStrandPos)), 1.0f);
    texCoord = TexCoord;
}
