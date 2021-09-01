#version 330

in vec4 vertexColor;
out vec4 fragmentColor;

void main(void)
{
    fragmentColor = vertexColor;
}
