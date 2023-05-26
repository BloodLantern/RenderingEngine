#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;
uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    TexCoord = uv;
}
