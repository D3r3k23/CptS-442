#version 330

uniform vec3 color;
uniform mat4 modelViewProjectionMatrix;

// per-vertex inputs
in vec4 vertexPosition;

// output color
smooth out vec4 interpolatedVertexColor; // interpolation happens *after* this shader

void main(void)
{
    gl_Position = modelViewProjectionMatrix * vertexPosition;
    interpolatedVertexColor = vec4(color.r, color.g, color.b, 1.0);
}
