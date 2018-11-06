#version 330 core

uniform sampler2D textureUnit;

in vec2 texCoordG;

out vec4 color;

void main()
{
    color = vec4(vec3(texture(textureUnit, texCoordG)), 1.0f);
}