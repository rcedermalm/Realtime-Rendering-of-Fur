#version 430 core

const int noOfHairSegments = 4;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 TexCoord;

uniform sampler2D hairDataTexture;

out vec2 texCoord;


void main()
{
    vec2 firstHairStrandPos;

    if(gl_VertexID == 0){
        firstHairStrandPos = vec2( 0.01f, 0.01f );

    } else if (gl_VertexID == 1){
        firstHairStrandPos = vec2( 0.01f, 0.5f );
    } else
        firstHairStrandPos = vec2( 0.01f, 0.98f );

    gl_Position = vec4(vec3(texture(hairDataTexture, firstHairStrandPos)), 1.0f);
    texCoord = TexCoord;


}
