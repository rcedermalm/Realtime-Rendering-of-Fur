#version 150

layout(triangles) in;
layout(line_strip, max_vertices = 12) out;

uniform sampler2D hairDataTexture;
uniform float noOfHairSegments;
uniform float noOfVertices;

in vec2 texCoord[3];
in vec4 teNormal[3];
in vec3 tessCoords[3];
in vec3 teVertexIDs[3];

uniform mat4 view;
uniform mat4 projection;

out vec2 gTexCoord;

float offsetWidth = (1.0f / (noOfHairSegments * 3.0f) ) * 0.5f;
float offsetHeight = (1.0f / noOfVertices) * 0.5f;

vec3 getPositionFromTexture(float vertexIndex, float hairIndex) {
    vec2 hairStrandPos = vec2((hairIndex / noOfHairSegments) + offsetWidth, (vertexIndex / noOfVertices) + offsetHeight);
    return vec3(texture(hairDataTexture, hairStrandPos));
}

vec3 getInterpolatedPosition(int index, int hairIndex){
    vec3 masterHairPos0 = getPositionFromTexture(teVertexIDs[index].x, hairIndex);
    vec3 masterHairPos1 = getPositionFromTexture(teVertexIDs[index].y, hairIndex);
    vec3 masterHairPos2 = getPositionFromTexture(teVertexIDs[index].z, hairIndex);

    vec3 hairPos = tessCoords[index].x * masterHairPos0 +
                      tessCoords[index].y * masterHairPos1 +
                      tessCoords[index].z * masterHairPos2;
    return hairPos;
}

void GenerateHairStrands(int index)
{
    gl_Position = gl_in[index].gl_Position;
    gTexCoord = texCoord[index];
    EmitVertex();

    for(int hairIndex = 0; hairIndex < 4; hairIndex++){
        gl_Position = projection * view * vec4(getInterpolatedPosition(index, hairIndex), 1.0f);
        gTexCoord = texCoord[index];
        EmitVertex();
    }

    EndPrimitive();
}

void main()
{
    for(int i = 0; i < gl_in.length(); i++){
        GenerateHairStrands(i);
    }
}
