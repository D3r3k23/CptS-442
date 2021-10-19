#version 330

smooth in vec4 interpolatedVertexColor;
out vec4 fragmentColor;

void main(void)
{
    fragmentColor = interpolatedVertexColor;
}
