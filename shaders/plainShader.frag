#version 430 core

uniform sampler2D textureUnit;
uniform vec3 lightPos;
uniform vec3 lightColor;

in vec2 texCoord;
in vec3 vertexNormal;
in vec3 vertexPosition;

out vec4 color;

void main()
{
    vec3 lightDir = normalize(lightPos - vertexPosition);

    float diff = max(dot(vertexNormal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(texture(textureUnit, texCoord));

    // Only diffuse colour because it will be the "skin" of the object
    vec3 result = diffuse * lightColor;
    color = vec4(result, 1.0);

}