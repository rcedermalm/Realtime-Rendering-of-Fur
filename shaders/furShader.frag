#version 430 core

uniform sampler2D mainTexture;

in vec2 gTexCoord;

out vec4 color;

void main()
{
    color = vec4(1.0);//vec4(vec3(texture(mainTexture, gTexCoord)), 1.0f);
}