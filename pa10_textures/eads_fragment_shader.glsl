#version 330

uniform bool useGouraudShading = true;

//
// These are the per-fragment inputs to the shader.
//

// Used if Gouraud shading is enabled:
smooth in vec4 interpolatedVertexColor;

// Used if Phong shading is enabled (i.e. useGouraudShading is false):
smooth in vec3 interpolatedWorldNormal;
smooth in vec3 interpolatedTowardsCamera;

//
// Here's how we support texturing in this shader: It only works if
// Phong shading is enabled (i.e. `useGouraudShading` is false). Given
// that...
//

// This turns textures on and off:
uniform bool useTextures = false;

// If textures are enabled (i.e. `useTextures` is true), we look up
// both ambient and diffuse reflectivities with the texture() built-in
// using this
uniform sampler2D imageTexture;
// as a texture and this
smooth in vec2 interpolatedTextureCoordinates;
// as an "index" into the texture.

uniform float emittanceTextureWeight;
uniform float ambientTextureWeight;
uniform float diffuseTextureWeight;
// specularity is not textured (it's like polish)


// This is the only output of this shader.
out vec4 fragmentColor;

//
// Note that we access the following uniform variables in the fragment
// shader by simply copying the same declarations they had in the EADS
// vertex shader. Setting a uniform variable in OpenGL makes it
// available to all shaders that declare it.
//

// material properties
uniform vec3 emittance;
uniform vec3 ambientReflectivity;
uniform vec3 maximumDiffuseReflectivity;
uniform vec3 maximumSpecularReflectivity;
uniform float specularExponent;

// light properties
uniform int nLights;
uniform struct Light {
    vec3 irradiance;
    vec3 towards;
} light[10]; // in later GLSLs, this size can vary

vec3 getTexturedRadiance(vec3 worldNormal, vec3 towardsCamera)
//
// ASSIGNMENT (PA10)
//
// First, copy your getRadiance() from "eads_vertex_shader.glsl".
// Then make the following modifications:
//
// - Change the name to "getTexturedRadiance()" here and where it's
//   called in main().
//
// - If `useTextures` is true, let a vec3 `textureRgb` be the (".rgb")
//   result of looking up `interpolatedTextureCoordinates` in
//   `imageTexture` using `interpolatedTextureCoordinates` and set
//   `radiance` to the product of `textureRgb` and
//   `emittanceTextureWeight`. We'll use an EadsFragmentShader to the
//   walls and use emittance to set their color. A better way would be
//   to build something like a "textured color fragment shader", but
//   we're lazy! (Maybe someday.)
//
// - If `useTextures` is false, set `radiance` to `emittance` as you
//   did before.
//
// - In the sum over all incident lights,
//
//   * If `useTextures` is true, initialize `reflectivity` to the
//     product of `ambientTextureWeight` and `textureRgb`.
//
//   * If `useTextures` is false, initialze `reflectivity` to
//     `ambientReflectivity` as you did before.
//
//   * If `nDotL` is positive,
//
//     + If `useTextures` is true, increment `reflectivity` by the
//       product of `diffuseTextureWeight`, `textureRgb`, and `nDotL`.
//
//     + If `useTextures` is false, increment `reflectivity` by the
//       product of `maximumDiffuseReflectivity and `nDotL`, as you
//       did before.
//
//   There are no changes to the specular component for reasons of
//   realism that will be explained in lecture.
//
{
    vec3 radiance;
    vec3 textureRgb
    if (useTextures)
    {
        textureRgb = texture(imageTexture, interpolatedTextureCoordinates).rgb;
        radiance = textureRgb * emittanceTextureWeight;
    }
    else
        radiance = emittance;

    for (int i = 0; i < nLights; i++)
    {
        vec3 towardsLight = normalize(light[i].towards);
        float nDotL = dot(worldNormal, towardsLight);

        vec3 reflectivity;
        if (useTextures)
            reflectivity = ambientTextureWeight * textureRgb;
        else
            reflectivity = ambientReflectivity;

        if (nDotL >= 0)
        {
            if (useTextures)
                reflectivity += diffuseTextureWeight * textureRgb * nDotL;
            else
                reflectivity += nDotL * maximumDiffuseReflectivity;

            vec3 h = normalize(towardsCamera + towardsLight);
            float nDotH = dot(worldNormal, h);

            if (nDotH >= 0)
                reflectivity += maximumSpecularReflectivity * pow(nDotH, specularExponent);
        }
        radiance += light[i].irradiance * reflectivity;
    }
    return radiance;

    return light[0].irradiance; // replace this with the correct expression
}

void main(void)
{
    if (useGouraudShading)
        fragmentColor = interpolatedVertexColor;
    else {
        // Notice that we have to normalize the interpolated variables,
        // as interpolation does not, in general, preserve length.
        fragmentColor = vec4(getTexturedRadiance(normalize(interpolatedWorldNormal), normalize(interpolatedTowardsCamera)), 1);
    }
}
