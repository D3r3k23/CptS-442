#version 330

//
// This is the fundamental "EADS" (Emissive-Ambient-Diffuse-Specular)
// shader that is a somewhat simplified version of the traditional
// OpenGL shading model.
//

// material properties
uniform vec3 emittance;
uniform vec3 ambientReflectivity;
uniform vec3 maximumDiffuseReflectivity;
uniform vec3 maximumSpecularReflectivity;
uniform float specularExponent;

// camera propert(y|ies)
uniform vec3 towardsCamera;

// light properties
uniform vec3 irradiance;
uniform vec3 towardsLight;

uniform mat4 viewMatrix;      // transforms vertices into NDC

uniform mat3 normalMatrix;    // transforms normals into world

// per-vertex inputs
in vec4 vertexPosition;
in vec3 vertexNormal;

smooth out vec4 interpolatedVertexColor;
const float EPSILON = 1.0e-5; // for single precision

void main(void)
{
    //
    // ASSIGNMENT (PA05)
    //
    // Add GLSL code here to compute `radiance`, the amount of light
    // reflected (or perhaps given off by) the vertex:
    //
    // - Initialize `radiance` to the `emittance` of the object.
    //   (This will be non-zero if we want the object to appear to be
    //   glowing, but remember that this does not actually emit light
    //   into the scene -- we have to compute the effect of doing that
    //   next.)
    //
    // - All other light sources reflect off the object, so we'll
    //   compute their `reflectivity`, a vec3 (rgb) value, as follows:
    //
    //   * Initialize `reflectivity` to the `ambientReflectivity` of
    //     the object. This kind of reflectivity is a hack to account
    //     for indirect light: It (bogusly) assumes that it doesn't
    //     matter where the light is with respect to the object.
    //
    //   * Reflectivity is calculated in world coordinates, so
    //     transform the `vertexNormal` by the `normalMatrix`. (You
    //     might call the result `worldNormal` to keep things clear.)
    //     Use the GLSL normalize() function to make sure the result
    //     is of unit length.
    //
    //   * Create a local vec3 that points towards the light. The
    //     `towardsLight` uniform already does that and is in world
    //     coordinates, so it doesn't have to be transformed. You
    //     should, however, use normalize() on it. (You'll have to
    //     have a new vec3 variable -- I called mine "towardsLight_"
    //     -- since GLSL won't let you modify a uniform variable.)
    //
    //   * Use the dot() function to compute the dot product of
    //     `worldNormal` and `towardsLight_`. I call it `nDotL`.
    //
    //   * If `nDotL` is positive, the light source is above the
    //     surface's "horizon", so it will be illuminated and you need
    //     to compute its contribution to `reflectivity` as follows:
    //
    //     + To compute the diffuse component, add the product of
    //       `nDotL` and the uniform `maximumDiffuseReflectivity` to
    //       `reflectivity`.
    //
    //     + Compute the Blinn "half vector" vec3 `h` that points
    //       halfway between the light and the camera by adding
    //       `towardsCamera` and `towardsLight_` and normalizing the
    //       result. (Do you see why you don't have to divide by 2?)
    //
    //     + Compute `nDotH`, the dot product of `worldNormal` with
    //       `h`.
    //
    //     + If `nDotH` is positive, the light source makes a
    //       significant specular contribution to the reflectivity,
    //       so:
    //
    //       : add the product of the uniform
    //         `maximumSpecularReflectivity` and the quantity `nDotH`
    //         raised to the power of the uniform variable
    //         `specularExponent` (use the pow() function) to
    //         `reflectivity`
    //
    // - At this point, `reflectivity` is now the total fraction (in
    //   R, G, and B components) of incident light (the uniform
    //   variable `irradiance`) reflected by the object, so just add
    //   (the light's) `irradiance` times `reflectivity` to
    //   `radiance`. The rest of the vertex shader will pass this on
    //   as the color to be interpolated and sent to the pixel shader.
    //

    vec3 radiance = emittance;
    vec3 reflectivity = ambientReflectivity;
    vec3 worldNormal = normalize(normalMatrix * vertexNormal);
    vec3 towardsLight_ = normalize(towardsLight);
    float nDotL = dot(worldNormal, towardsLight_);

    if (nDotL >= 0)
    {
        reflectivity += nDotL * maximumDiffuseReflectivity;
        vec3 h = normalize(towardsCamera + towardsLight_);
        float nDotH = dot(worldNormal, h);

        if (nDotH >= 0)
            reflectivity += maximumSpecularReflectivity * pow(nDotH, specularExponent);
    }

    radiance += irradiance * reflectivity;

    interpolatedVertexColor = vec4(radiance, 1);
#if 0 // debug
    interpolatedVertexColor = vec4(vertexNormal, 1);
#endif

    // the position transform is trivial
    gl_Position = viewMatrix * vertexPosition;
}
