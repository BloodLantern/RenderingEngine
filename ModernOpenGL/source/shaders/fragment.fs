#version 330
out vec4 FragColor;

in vec2 textureCoords;

in vec3 vertexPosition;
in vec3 vertexNormal;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

uniform vec3 lightPosition;

uniform sampler2D ourTexture;

void main()
{
    float factor = max(dot(normalize(vertexPosition - lightPosition), vertexNormal), 0);
    vec4 diffuse = diffuseColor * factor;
    
    FragColor = (ambientColor + diffuse) * texture(ourTexture, textureCoords);
}