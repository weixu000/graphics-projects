#version 330 core

in vec3 vNormal;// normal in object space

out vec4 fragColor;

void main()
{
    fragColor = vec4((normalize(vNormal) + 1) / 2, 1.0f);
}