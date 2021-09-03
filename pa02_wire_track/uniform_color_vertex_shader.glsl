#version 330

uniform vec3 color;

// per-vertex inputs
in vec4 vertexPosition;

// output color
smooth out vec4 interpolatedColor; // interpolation happens *after* this shader

void main(void)
{
    gl_Position = vertexPosition;
    interpolatedColor = vec4(color.r, color.g, color.b, 1.0);
}
