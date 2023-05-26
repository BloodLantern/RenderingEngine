#version 330
layout (location = 0) in mediump vec3 position;
layout (location = 1) in mediump vec2 uv;
layout (location = 2) in mediump vec3 normal;

out mediump vec2 TexCoord;
uniform mediump mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    TexCoord = uv;
}
