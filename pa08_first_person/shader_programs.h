#ifndef INCLUDED_SHADER_PROGRAMS

//
// The "shader_programs" module provides all ShaderPrograms.
//

#include <string>
using namespace std;

#include "wrap_gl_inclusion.h"

#include "color.h"
#include "transform.h"

// This macro helps avoid compiler warnings when calling
// glVertexPointer().
#define BUFFER_OFFSET(i) ((char *) NULL + (i))

//
// Enums used by ShaderProgram.
//
enum {
    NO_SUCH_ATTRIBUTE = -1 // any negative value will do
};

class ShaderProgram
//
// virtual class for all shader programs
//
{
protected:
    string name;

private:
    static const GLuint undefinedShaderId = 0xffffffff;
    static GLuint currentProgramId;

    GLuint fragmentShaderId;
    GLuint vertexShaderId;

    const bool getUniformLocation(const string name, GLint &location)
        const;

public:
    GLuint programId;

    const void dumpStatus(void) const;
    static const GLint getCurrentAttributeIndex(const string name);

protected:
    Matrix4 modelViewProjectionMatrix;

    const void select(void) const;

    const GLuint compileShader(const string typeName,
                               GLenum shaderType, string glslSource);
    const void setUniform(const string name, const Matrix4 &matrix, int wh)
        const;
    const void setUniform(const string name, double val) const;
    const void setUniform(const string name, int val) const;
    const void setUniform(const string name, const Vec3 &v) const;
    const void setUniform(const string name, const Mat4 &m) const;

public:
    ShaderProgram(const string name);

    void compileFragmentShader(string glslSource);
    void compileVertexShader(string glslSource);

    static const void disableCurrent(void);
    const virtual void start(void) const = 0;
    void setModelViewProjectionMatrix(
        const Matrix4 &modelViewProjectionMatrix_)
    {
        modelViewProjectionMatrix = modelViewProjectionMatrix_;
    }
};


class ConstantColorShaderProgram : public ShaderProgram
//
// ShaderProgram that passes vertex positions through unchanged, but
// allows color to be specified
//
{
    Color color;

public:
    ConstantColorShaderProgram(string name);

    const void start(void) const;
    void setColor(const Color &color_)
    {
        color = color_;
    }
};


class EadsShaderProgram : public ShaderProgram
//
// ShaderProgram that computes emissive-ambient-diffuse-specular
// (classic OpenGL) lighting
//
{
    Matrix4 normalMatrix;
    Matrix4 worldMatrix;

    Color emittance;
    Rgb ambientReflectivity;
    Rgb maximumDiffuseReflectivity;
    Rgb maximumSpecularReflectivity;
    double specularExponent;

public:

    EadsShaderProgram(void);

    const void start(void) const;

    void setAmbient(const Rgb &ambientReflectivity_)
    {
        ambientReflectivity = ambientReflectivity_;
    }

    void setEmittance(const Color &emittance_)
    {
        emittance = emittance_;
    }

    void setDiffuse(const Rgb &maximumDiffuseReflectivity_)
    {
        maximumDiffuseReflectivity = maximumDiffuseReflectivity_;
    }

    void setSpecular(const Rgb &maximumSpecularReflectivity_,
                     double specularExponent_)
    {
        maximumSpecularReflectivity = maximumSpecularReflectivity_;
        specularExponent = specularExponent_;
    }

    void setNormalMatrix(const Matrix4 &normalMatrix_)
    {
        normalMatrix = normalMatrix_;
    }

    void setWorldMatrix(const Matrix4 &worldMatrix_)
    {
        worldMatrix = worldMatrix_;
    }
};


#define INCLUDED_SHADER_PROGRAMS
#endif // INCLUDED_SHADER_PROGRAMS

