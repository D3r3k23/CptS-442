#version 330

uniform vec3 color;
uniform mat4 modelViewProjectionMatrix;

// per-vertex inputs
in vec4 vertexPosition;
in vec2 textureCoordinates;

// output color (if textures not enabled)
smooth out vec4 interpolatedVertexColor; // interpolation happens *after* this shader
// use only if textures are enabled (this is in the fragment shader)
smooth out vec2 interpolatedTextureCoordinates;

void main(void)
{
    gl_Position = modelViewProjectionMatrix * vertexPosition;
    interpolatedVertexColor = vec4(color.r, color.g, color.b, 1.0);
    interpolatedTextureCoordinates = textureCoordinates;
}
