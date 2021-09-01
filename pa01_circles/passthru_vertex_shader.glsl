#version 330

in vec4 vertexPosition;
out vec4 vertexColor; // interpolation happens *after* this shader

void main(void)
{
    gl_Position = vertexPosition;

    vertexColor = vec4(1.0, 1.0, 1.0, 1.0);
}
