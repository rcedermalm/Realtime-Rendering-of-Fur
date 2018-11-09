#version 430 core

uniform sampler2D textureUnit;

in vec2 gTexCoord;

out vec4 color;

void main()
{
    color = vec4(vec3(texture(textureUnit, gTexCoord)), 1.0f);
}