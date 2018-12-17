#version 150

layout(triangles) in;
layout(line_strip, max_vertices = 39) out; // Unfortunately this needs to be hardcoded.. (3*(noOfHairSegments+1))Do not forget to change this
                                           // if you change noOfHairSegments in main.cpp
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Hair uniforms
uniform sampler2D hairDataTexture;
uniform sampler2D randomDataTexture;
uniform float noOfHairSegments;
uniform float noOfVertices;
uniform float nrOfDataVariablesPerMasterHair;

in vec2 teTexCoord[3];
in vec3 teNormal[3];
in vec3 teVertexIDs[3];
in vec3 teTessCoords[3];

out vec2 gTexCoord;
out vec3 gPosition;
out vec3 gTangent;

float offsetWidth = (1.0f / (noOfHairSegments * nrOfDataVariablesPerMasterHair) ) * 0.5f;
float offsetHeight = (1.0f / noOfVertices) * 0.5f;

// Function declarations
void generateHairStrands(int index);
vec3 getInterpolatedPosition(int index, int hairIndex);
vec3 getPositionFromTexture(float vertexIndex, float hairIndex);

void main()
{
    for(int i = 0; i < gl_in.length(); i++){
        generateHairStrands(i);
    }
}

void generateHairStrands(int index){
    vec3 lastPos = (gl_in[index].gl_Position).xyz;
    vec3 firstHairSegmentPos = getInterpolatedPosition(index, 0);

    // Create first hairvertex (the one attached to the object)
    gl_Position = projection * view * model * gl_in[index].gl_Position;
    gTexCoord = teTexCoord[index];
    gPosition = lastPos;
    gTangent = normalize(firstHairSegmentPos - lastPos);
    EmitVertex();

    vec3 randomness = vec3(texture(randomDataTexture, teTexCoord[index]));

    // Create the hairvertices
    for(int hairIndex = 1; hairIndex < noOfHairSegments; hairIndex++){
        vec3 hairPos = getInterpolatedPosition(index, hairIndex);
        hairPos = hairPos + 0.2f * randomness;

        gl_Position = projection * view * vec4(hairPos, 1.0);
        gTexCoord = teTexCoord[index];
        gPosition = hairPos;
        gTangent = normalize(hairPos - lastPos);
        EmitVertex();

        lastPos = hairPos;
    }

    EndPrimitive();
}

vec3 getInterpolatedPosition(int index, int hairIndex){
    vec3 masterHairPos0 = getPositionFromTexture(teVertexIDs[index].x, hairIndex);
    vec3 masterHairPos1 = getPositionFromTexture(teVertexIDs[index].y, hairIndex);
    vec3 masterHairPos2 = getPositionFromTexture(teVertexIDs[index].z, hairIndex);

    vec3 hairPos = teTessCoords[index].x * masterHairPos0 +
                      teTessCoords[index].y * masterHairPos1 +
                      teTessCoords[index].z * masterHairPos2;

    return hairPos;
}

vec3 getPositionFromTexture(float vertexIndex, float hairIndex) {
    vec2 hairStrandPos = vec2((hairIndex / noOfHairSegments) + offsetWidth, (vertexIndex / noOfVertices) + offsetHeight);
    return vec3(texture(hairDataTexture, hairStrandPos));
}
