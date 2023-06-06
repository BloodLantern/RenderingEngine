#version 460
out vec4 FragColor;

in vec2 textureCoords;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, textureCoords);
}