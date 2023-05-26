#version 330
out lowp vec4 FragColor;

in lowp vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}