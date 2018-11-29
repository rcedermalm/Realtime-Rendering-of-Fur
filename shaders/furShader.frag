#version 430 core

uniform sampler2D mainTexture;

uniform vec3 cameraPosition;
uniform vec3 lightPos;
uniform vec3 lightColor;

in vec2 gTexCoord;
in vec3 gPosition;
in vec3 gTangent;

out vec4 color;

void main()
{
    vec3 light = normalize(lightPos - gPosition);
    vec3 texColor = vec3(texture(mainTexture, gTexCoord));
    float diffuseAmount = length(cross(light, gTangent));
    if(diffuseAmount < 0.7)
        diffuseAmount = 0.7;
    vec3 diffuse = texColor * diffuseAmount;
    vec3 eye = normalize(gPosition - cameraPosition);
    float p = 50;
    vec3 specular = texColor * 0.5 * clamp(pow((dot(gTangent, light) * dot(gTangent, eye) + length(cross(gTangent,light))*length(cross(gTangent,eye))), p), 0.0, 1.0);
    color = vec4(diffuse + specular, 1.0);

}