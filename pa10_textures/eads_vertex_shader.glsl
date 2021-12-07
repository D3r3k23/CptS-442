#version 330

//
// This is the fundamental "EADS" (Emissive-Ambient-Diffuse-Specular)
// shader that is a somewhat simplified version of the traditional
// OpenGL shading model.
//

uniform bool useGouraudShading = true;

// material properties
uniform vec3 emittance;
uniform vec3 ambientReflectivity;
uniform vec3 maximumDiffuseReflectivity;
uniform vec3 maximumSpecularReflectivity;
uniform float specularExponent;

// camera propert(y|ies)
//
// Note that "towardsCamera" has been replaced by the following:
//

// If true, we're using an orthographic projection. If false, we're
// using a perspective transform.
uniform int useOrthographic;

// This is the equivalent of "towardsCamera", but it should only be
// used if "useOrthographic" is true.
uniform vec3 orthographicTowards;

// When using perspective (i.e. useOrthographic is false), the camera
// is at this position, so the "towards" vector is this value minus
// `vertexPosition`.
uniform vec3 cameraPosition;

// light properties
uniform int nLights;
uniform struct Light {
    vec3 irradiance;
    vec3 towards;
} light[10]; // in later GLSLs, this size can vary

//
// Note that `viewMatrix` has been replaced (in this shader only) by
// the following two matrices:

// This transforms `vertexPosition` into NDC for drawing purposes.
uniform mat4 modelViewProjectionMatrix;

// This transforms `vertexPosition` into world coordinates for
// lighting computation.
uniform mat4 worldMatrix;

uniform mat3 normalMatrix;    // transforms normals into world

// per-vertex inputs
in vec4 vertexPosition;
in vec3 vertexNormal;
in vec2 textureCoordinates;

smooth out vec4 interpolatedVertexColor;
// use only if textures are enabled (this is in the fragment shader)
smooth out vec2 interpolatedTextureCoordinates;

// use only if Phong shading is enabled (i.e. useGouraudShading is false)
smooth out vec3 interpolatedWorldNormal;
smooth out vec3 interpolatedTowardsCamera;

const float EPSILON = 1.0e-5; // for single precision

vec3 getRadiance(vec3 worldNormal, vec3 towardsCamera)
//
// Copy your previous (PA09) solution here.
//
{
    return vec3(0,0,0); // to be replaced
}

void main(void)
{
    //
    // Copy your previous (PA08) solution here.
    //

    //
    // ASSIGNMENT (PA10)
    //
    // If `useGouraudShading` is true, set `radiance` to the result of
    // `getRadiance()` as you did for PA09. If it is false, set
    // `interpolatedWorldNormal` to `worldNormal` and
    // `interpolatedTowardsCamera` to `towardsCamera`.
    //

    interpolatedVertexColor = vec4(radiance, 1);
#if 0 // debug
    interpolatedVertexColor = vec4(vertexNormal, 1);
#endif
    interpolatedTextureCoordinates = textureCoordinates;

    // the position transform is trivial
    gl_Position = modelViewProjectionMatrix * vertexPosition;
}
