#version 430 core

uniform sampler2D mainTexture;

in vec2 texCoord;

out vec4 color;

void main()
{
    // The skin will not be visible, therefore it can be set to the colour of the texture
    color = vec4(vec3(texture(mainTexture, texCoord)), 1.0);

}