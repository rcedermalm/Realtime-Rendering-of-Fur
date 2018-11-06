#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>

/// Adapted from http://stackoverflow.com/questions/2795044/easy-framework-for-opengl-shaders-in-c-c

class ShaderProgram {
public:
    /// Constructs a GLSL shader program with V/TC/TE/G/F-shaders located in the specified files
    ShaderProgram(std::string vertex_shader_filename = "",
                  std::string tessellation_control_shader_filename = "",
                  std::string tessellation_eval_shader_filename = "",
                  std::string geometry_shader_filename = "",
                  std::string fragment_shader_filename = "");

    /// Get the GLuint corresponding to the OpenGL shader program
    inline operator GLuint() {
        return prog;
    }

    /// Activate the shader program
    inline void operator()() {
        glUseProgram(prog);
    }

    static const std::string getShaderType(GLuint type);

    GLint MV_Loc, P_Loc, lDir_Loc, camPos_Loc = -1;
    GLint tmpTex = -1;

    ~ShaderProgram();

protected:
    GLuint AttachShader(GLuint shaderType, std::string source);

    void ConfigureShaderProgram();

private:
    std::vector<GLuint> shader_programs_;
    GLuint prog;

    GLuint compile(GLuint type, GLchar const *source);
};