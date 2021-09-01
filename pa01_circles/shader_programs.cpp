#ifdef _WIN32
// If this is not defined, VC++ complains about fopen() insecurity.
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
using namespace std;

#include <iostream>
#include <cassert>

// for fopen(3)
#include <stdio.h>

// for stat(2)
#include <sys/types.h>
#include <sys/stat.h>
#if  !defined(_WIN32)
#include <unistd.h>
#endif

#include "check_gl.h"
#include "scene.h"
#include "shader_programs.h"
#include "wrap_gl_inclusion.h"

GLuint ShaderProgram::currentProgramId = 0; // never returned by glGetProgram()


//
// (static) helper functions
//

void checkProgramLog(GLuint programId, string name)
//
// checks the GPU for program linking errors and, if any are found,
// prints them and exits
//
{
    GLint linked;
    GLsizei lenReq;

    CHECK_GL(glGetProgramiv(programId, GL_LINK_STATUS, &linked));
    if (!linked) {
        CHECK_GL(glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &lenReq));
        if (lenReq > 1) {
            GLchar *log = (GLchar *) malloc(lenReq);

            // log is null-terminated, so we don't need is actual length
            CHECK_GL(glGetProgramInfoLog(programId, lenReq, NULL, log));
            cerr << "*** error linking shader program \"" << name << "\":\n"
                 << log << endl;
            free(log);
        }
        exit(EXIT_FAILURE);
    }
}


void checkShaderLog(GLuint shaderId, string typeName, string name)
//
// checks the GPU for program compilation errors and, if any are
// found, prints them and exits
//
{
    GLint compiled;
    GLsizei lenReq;

    CHECK_GL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled));
    if (!compiled) {
        CHECK_GL(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &lenReq));
        if (lenReq > 1) {
            GLchar *log = (GLchar *) malloc(lenReq);

            // log is null-terminated, so we don't need is actual length
            CHECK_GL(glGetShaderInfoLog(shaderId, lenReq, NULL, log));
            cerr << "*** error(s) compiling " << typeName << " shader \""
                 << name << "\":\n" << log << endl;
            free(log);
        }
        exit(EXIT_FAILURE);
    }
}


#ifdef SOLUTION // make this static in the template and solutions
static char *readFile(string fname)
#else
char *readFile(string fname)
#endif
//
// reads the contents of `fname` into a (char) string, including
// NUL-termination allocated on the heap. The caller is responsible
// for free()ing the string.
//
{
    struct stat statBuf;
    char *buf;
    size_t fileSize;

    if (stat(fname.c_str(), &statBuf) < 0) {
        cerr << "Unable to stat \"" << fname << "\" -- exiting\n";
        exit(EXIT_FAILURE);
    }
    FILE *file = fopen(fname.c_str(), "r");
    if (file == NULL) {
        cerr << "Unable to open \""
             << fname
             << "\" for reading -- exiting\n";
        exit(EXIT_FAILURE);
    }
    fileSize = statBuf.st_size;

    // allow for NUL-termination (and some paranoia about sizeof(char))
    buf = (char *) malloc(fileSize * sizeof(char) + 1);

    if (buf == NULL) {
        cerr << "Unable to allocate space for \"" << fname
             << "\" -- exiting\n";
        exit(EXIT_FAILURE);
    }
    if (fread(buf, sizeof(char), fileSize, file) != fileSize) {
        cerr << "Unable to read " << fileSize << " bytes from \""
             << fname << "\" -- exiting\n";
        exit(EXIT_FAILURE);
    }
    buf[fileSize] = '\0';
    fclose(file);
    return buf;
}


//
// NOTE: We are abandoning our usual alphabetical order in this case
// in order to show the base class first and then proceed from easier
// to more complicated shader programs.
//

ShaderProgram::ShaderProgram(string name_)
    : name(name_),
      fragmentShaderId(undefinedShaderId),
      vertexShaderId(undefinedShaderId),
      programId(undefinedShaderId)
//
// allocates a shader program in the GPU
//
{
    CHECK_GL(programId = glCreateProgram());
}


void ShaderProgram::compileFragmentShader(string glslCode)
//
// compiles the fragment shader in `glslCode`
//
{
    // If there's already a fragment shader in this shader program,
    // delete it.
    if (fragmentShaderId != undefinedShaderId)
        glDeleteShader(fragmentShaderId);
    fragmentShaderId = compileShader("fragment", GL_FRAGMENT_SHADER, glslCode);
}


const GLuint ShaderProgram::compileShader(const string typeName,
                                          GLenum shaderType,
                                          string glslSource)
//
// compiles and links a shader named `typeName` whose (GLSL) type
// is `shaderType` and whose source is `glslSource`
//
{
    GLuint shaderId = 0;

    //
    // Use glCreateShader() to create `shaderId`. (asserting that it
    // is non-zero.) This method will return this value.
    //
    CHECK_GL(shaderId = glCreateShader(shaderType));
    assert(shaderId != 0);

    //
    // Assign `glslSource` to be the shader's source using
    // glShaderSource().
    //
    // GLSL shader source is a single C string
    const char *source = glslSource.c_str();
    GLint lenSource = glslSource.size();
    CHECK_GL(glShaderSource(shaderId, 1, &source, &lenSource));

    //
    // Compile the shader with `glCompileShader()`.
    //
    CHECK_GL(glCompileShader(shaderId));

    //
    // Call "checkShaderLog" with `shaderId`. This will exit the
    // program if there are any compiler errors.
    //
    checkShaderLog(shaderId, typeName, name);

    assert(programId != undefinedShaderId); // should be already assigned

    //
    // Attach shader `shaderId` to shader program `programId` with
    // glAttachShader().
    //
    CHECK_GL(glAttachShader(programId, shaderId));

    //
    // Call `glLinkProgram()` to link the program `programId`.
    //
    CHECK_GL(glLinkProgram(programId));

    //
    // Call "checkProgramLog" with `programId`. This will exit the
    // program if there are any link errors.
    //
    checkProgramLog(programId, name);

    return shaderId;
}


void ShaderProgram::compileVertexShader(string glslCode)
//
// compiles the vertex shader in `glslCode`
//
{
    // If there's already a vertex shader in this shader program,
    // delete it.
    if (vertexShaderId != undefinedShaderId)
        glDeleteShader(vertexShaderId);
    vertexShaderId = compileShader("vertex", GL_VERTEX_SHADER, glslCode);
}


const void ShaderProgram::disableCurrent(void)
//
// disables the current shader program, re-enabling the default
// (OpenGL) shader program
//
{
    CHECK_GL(glUseProgram(0)); // reenables the fixed pipeline
    currentProgramId = 0;
}


const void ShaderProgram::dumpStatus(void) const
//
// dumps on cout some useful information about the shader (for debugging)
//
{
    int length;
    GLint size;
    GLenum type;
    int nAttr, nUniform;

    cout << "status of shader program id " << programId
         << " (\"" << name << "\")" << endl;

    CHECK_GL(glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &nAttr));
    cout << nAttr << " active attributes:" << endl;
    for (int i = 0; i < nAttr; i++) {
        char attributeName[128];
        CHECK_GL(glGetActiveAttrib(programId,
                                   i, sizeof(attributeName), &length,
                                   &size, &type, attributeName));
        cout << "   " << attributeName << "\n";
    }

    CHECK_GL(glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &nUniform));
    cout << nUniform << " active uniforms:" << endl;
    for (int i = 0; i < nUniform; i++) {
        char uniformName[128];
        CHECK_GL(glGetActiveUniform(programId,
                                    i, sizeof(uniformName), &length,
                                    &size, &type, uniformName));
        cout << "   " << uniformName << "\n";
    }
}



const GLint ShaderProgram::getCurrentAttributeIndex(const string name)
//
// gets the attribute index of the attribute referred to as `name` in
// the current shader program. It returns NO_SUCH_ATTRIBUTE if the
// attribute is not found (or has been optimized away by the GLSL
// compiler).
//
{
    GLint result;
    CHECK_GL(result = glGetAttribLocation(currentProgramId, name.c_str()));
    if (result == -1)
        return NO_SUCH_ATTRIBUTE;
    assert(result >= 0);
    return result;
}


const bool ShaderProgram::getUniformLocation(const string variableName,
                                             GLint &location) const
//
// sets `location` to the location of uniform variable `variableName`,
// returning false and printing an error message iff there is no such
// uniform variable
//
{
    //
    // Use glGetUniformLocation() to get the location `location` of
    // uniform variable `variableName` in `programId`. Return true iff
    // it's found. Output an error message if it isn't.
    //
    CHECK_GL(location = glGetUniformLocation(programId, variableName.c_str()));
    if (location == -1) {
        cerr << "unable to find and set active uniform variable \""
             << variableName << "\"\n"
             << "    in the shader named \"" << name  << "\"\n"
             << "    Perhaps it is not used in a shader (even if it"
                " is declared)\n"
                "    or has been optimized away?\n";
        return false;
    }
    return true;
}


const void ShaderProgram::select(void) const
//
// select this ShaderProgram for execution
//
{
    // Optimization: If this program is the one that's currently
    // enabled, don't bother calling glUseProgram() on it.
    if (programId == currentProgramId)
        return;

    CHECK_GL(glUseProgram(programId));
    currentProgramId = programId;
}


const void ShaderProgram::setUniform(const string name, double val) const
//
// set a uniform double value
//
{
    GLint location;

    if (getUniformLocation(name, location))
        CHECK_GL(glUniform1f(location, static_cast<GLfloat>(val)));
}


const void ShaderProgram::setUniform(const string name, int val) const
//
// set a uniform int value
//
{
    GLint location;

    if (getUniformLocation(name, location))
        CHECK_GL(glUniform1i(location, val));
}


PassthruShaderProgram::PassthruShaderProgram(void)
    : ShaderProgram("PassthruShaderProgram")
//
// reads the GLSL source for the vertex and fragment shaders and
// compiles them
//
{
    char *fileContents;

    fileContents = readFile("passthru_vertex_shader.glsl");
    compileVertexShader(fileContents);
    free(fileContents);

    fileContents = readFile("passthru_fragment_shader.glsl");
    compileFragmentShader(fileContents);
    free(fileContents);
}


const void PassthruShaderProgram::start(void) const
//
// enables the shader program
//
{
    select();

    // If there were any uniform variables to set, we would set them
    // here.
}


