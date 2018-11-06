#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 texCoord[3]; // [3] because this makes a triangle

out vec2 texCoordG;

void main()
{
    for(int i = 0; i < gl_in.length(); i++){
        gl_Position = gl_in[i].gl_Position;
        texCoordG = texCoord[i];
        EmitVertex();
    }
    EndPrimitive();
}