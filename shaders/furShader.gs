#version 150

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

uniform sampler2D hairDataTexture;
uniform float noOfHairSegments;
uniform float noOfVertices;

in vec2 texCoord[3]; // [3] because this makes a triangle
in vec4 teNormal[3];
in vec3 tessCoords[3];
in vec3 teVertexIDs[3];

uniform mat4 view;
uniform mat4 projection;

out vec2 gTexCoord;

float offsetWidth = (1.0f / (noOfHairSegments * 3.0f) ) * 0.5f;
float offsetHeight = (1.0f / noOfVertices) * 0.5f;

// TODO: FIX SO THAT HAIRINDEX ALSO IS INCLUDED
vec3 getPositionFromTexture(float vertexIndex, int hairIndex) {
    vec2 hairStrandPos = vec2(offsetWidth, (vertexIndex / noOfVertices) + offsetHeight);
    return vec3(texture(hairDataTexture, hairStrandPos));
}

void GenerateHairStrands(int index)
{
    gl_Position = gl_in[index].gl_Position;
    gTexCoord = texCoord[index];
    EmitVertex();

    // TODO: MOVE INTERPOLATION TO FUNCTION
    vec3 hairStrandPos0 = getPositionFromTexture(teVertexIDs[index].x, 0);
    vec3 hairStrandPos1 = getPositionFromTexture(teVertexIDs[index].y, 0);
    vec3 hairStrandPos2 = getPositionFromTexture(teVertexIDs[index].z, 0);
    gl_Position = projection * view * vec4(tessCoords[index].x * hairStrandPos0 +
                       tessCoords[index].y * hairStrandPos1 +
                       tessCoords[index].z * hairStrandPos2, 1.0f);
    gTexCoord = texCoord[index];
    EmitVertex();

    EndPrimitive();
}

void main()
{
    for(int i = 0; i < gl_in.length(); i++){
        GenerateHairStrands(i);
    }
}
