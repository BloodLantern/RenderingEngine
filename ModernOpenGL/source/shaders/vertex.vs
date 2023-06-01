#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec2 textureCoords;

out vec3 vertexPosition;
out vec3 vertexNormal;

uniform mat4 mvp;
uniform mat4 model;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);

    textureCoords = uv;

    vertexPosition = (model * vec4(position, 1.0)).xyz;
    vertexNormal = normalize(mat3(model) * normal);
}
