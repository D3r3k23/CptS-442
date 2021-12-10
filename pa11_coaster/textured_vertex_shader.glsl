#version 330

//
// This is the textured vertex shader, intended for use with the
// textured fragment shader. The resulting shader program takes
// fragment colors directly from a texture without any lighting
// calculation.
//

// This transforms `vertexPosition` into NDC for drawing purposes.
uniform mat4 modelViewProjectionMatrix;

// per-vertex inputs
in vec4 vertexPosition;
in vec2 textureCoordinates;


// use only if textures are enabled (this is in the fragment shader)
smooth out vec2 interpolatedTextureCoordinates;

void main(void)
{
    interpolatedTextureCoordinates = textureCoordinates;

    // the position transform is trivial
    gl_Position = modelViewProjectionMatrix * vertexPosition;
}
