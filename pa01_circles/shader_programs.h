#ifndef INCLUDED_SHADER_PROGRAMS

//
// The "shader_programs" module provides all ShaderPrograms.
//

#include <string>
using namespace std;

#include "wrap_gl_inclusion.h"


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

    const void select(void) const;

    const GLuint compileShader(const string typeName,
                               GLenum shaderType, string glslSource);
    const void setUniform(const string name, double val) const;
    const void setUniform(const string name, int val) const;

public:
    ShaderProgram(const string name);

    void compileFragmentShader(string glslSource);
    void compileVertexShader(string glslSource);

    static const void disableCurrent(void);
    const virtual void start(void) const = 0;
};


class PassthruShaderProgram : public ShaderProgram
//
// A minimal ShaderProgram that passes vertex coordinates and colors
// through unchanged.
//
{
public:
    PassthruShaderProgram(void);

    const void start(void) const;
};


#define INCLUDED_SHADER_PROGRAMS
#endif // INCLUDED_SHADER_PROGRAMS

