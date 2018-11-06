#version 330 core

uniform sampler2D textureUnit;

in vec2 st;

out vec4 color;

void main()
{
    color = vec4(vec3(texture(textureUnit, st)), 1.0f);
}