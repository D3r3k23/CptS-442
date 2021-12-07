#version 330

//
// This is the textured fragment shader, intended for use with the
// textured vertex shader. The resulting shader program takes
// fragment colors directly from a texture without any lighting
// calculation.
//

// This enables and disables textures.
uniform bool useTextures = false;

// This is the texture we use when textures are enabled:
uniform sampler2D imageTexture;
// and this
smooth in vec2 interpolatedTextureCoordinates;
// is an "index" into it.

// This is the color we use when textures are disabled:
uniform vec3 color;

// This is the only output of this shader.
out vec4 fragmentColor;


void main(void)
{
    if (useTextures) {
        fragmentColor = vec4(
            texture(imageTexture, interpolatedTextureCoordinates).rgb, 1);
    } else
        fragmentColor = vec4(color, 1);
}
