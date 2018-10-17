#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTex;

flat out vec3 fColor;
out vec2 fTex;

uniform mat4 p;

void main()
{
    gl_Position = p * vec4(aPos, 1.0);
    fColor = vColor;
    fTex = vTex;
}
