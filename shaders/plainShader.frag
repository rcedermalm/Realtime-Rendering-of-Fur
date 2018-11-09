#version 430 core

uniform sampler2D textureUnit;

in vec2 texCoord;

out vec4 color;

void main()
{
    color = vec4(vec3(texture(textureUnit, texCoord)), 1.0f);
}