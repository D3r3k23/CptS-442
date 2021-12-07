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
uniform int nLights;
uniform struct Light {
    vec3 irradiance;
    vec3 towards;
} light[10]; // in later GLSLs, this size can vary

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
    // Copy your previous (PA06) solution here.
    //
    vec3 radiance = emittance;
    vec3 worldNormal = normalize(normalMatrix * vertexNormal);

    for (int i = 0; i < nLights; i++)
    {
        vec3 towardsLight = normalize(light[i].towards);
        float nDotL = dot(worldNormal, towardsLight);

        vec3 reflectivity = ambientReflectivity;
        if (nDotL >= 0)
        {
            reflectivity += nDotL * maximumDiffuseReflectivity;
            vec3 h = normalize(towardsCamera + towardsLight);
            float nDotH = dot(worldNormal, h);

            if (nDotH >= 0)
                reflectivity += maximumSpecularReflectivity * pow(nDotH, specularExponent);
        }
        radiance += light[i].irradiance * reflectivity;
    }

    interpolatedVertexColor = vec4(radiance, 1);
#if 0 // debug
    interpolatedVertexColor = vec4(vertexNormal, 1);
#endif

    // the position transform is trivial
    gl_Position = viewMatrix * vertexPosition;
}
