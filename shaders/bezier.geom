#version 330 core
layout (lines_adjacency) in;
layout (line_strip, max_vertices = 150) out;

uniform int nSamples;

const mat4 B = mat4(
-1, 3, -3, 1,
3, -6, 3, 0,
-3, 3, 0, 0,
1, 0, 0, 0
);

void main() {
    mat4 GB = mat4(
    gl_in[0].gl_Position,
    gl_in[1].gl_Position,
    gl_in[2].gl_Position,
    gl_in[3].gl_Position
    ) * B;
    float t = 0.0;
    for (int i = 0; i < nSamples; ++i, t += 1.0 / (nSamples - 1)){
        gl_Position = GB*vec4(t * t * t, t * t, t, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}