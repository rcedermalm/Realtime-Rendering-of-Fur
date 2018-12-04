#include <ShaderProgram.hpp>
#include <FileReader.hpp>

#include <memory>
#include <cstdlib>


BaseShaderProgram::~BaseShaderProgram() {
    for (GLuint shader_program : shader_programs_) {
        glDeleteShader(shader_program);
    }
    glDeleteProgram(prog);
}

GLuint BaseShaderProgram::AttachShader(GLuint shaderType, std::string source) {
    GLuint sh = compile(shaderType, source.c_str());
    shader_programs_.push_back(sh);

    std::cout << "Attached shader of type: '" << getShaderType(shaderType) << "'\n";

    return sh;
}

void BaseShaderProgram::ConfigureShaderProgram() {
    prog = glCreateProgram();

    for (GLuint shader_program : shader_programs_) {
        glAttachShader(prog, shader_program);
    }

    glLinkProgram(prog);

    GLint isLinked;
    glGetProgramiv(prog, GL_LINK_STATUS, (int *) &isLinked);

    if (isLinked == GL_FALSE) {
        GLint length;
        glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &length);
        std::string log(length, ' ');
        glGetShaderInfoLog(prog, length, &length, &log[0]);
        std::cerr << "Failed to link shaderprogram : " << std::endl
        << log << std::endl;
        exit(EXIT_FAILURE);
    } else {
        std::cout << "Shader linking complete!\n";
    }
}

const std::string BaseShaderProgram::getShaderType(GLuint type) {
    std::string name;
    switch (type) {
        case GL_VERTEX_SHADER:
            name = "Vertex Shader";
            break;
        case GL_TESS_CONTROL_SHADER:
            name = "Tessellation Control Shader";
            break;
        case GL_TESS_EVALUATION_SHADER:
            name = "Tessellation Evaluation Shader";
            break;
        case GL_GEOMETRY_SHADER:
            name = "Geometry Shader";
            break;
        case GL_FRAGMENT_SHADER:
            name = "Fragment Shader";
            break;
        case GL_COMPUTE_SHADER:
            name = "Compute Shader";
        default:
            name = "Unknown Shader type";
            break;
    }
    return name;
}

GLuint BaseShaderProgram::compile(GLuint type, GLchar const *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        GLint logSize = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
        std::string log(logSize, ' ');
        glGetShaderInfoLog(shader, logSize, &logSize, &log[0]);
        std::cerr << "Failed to compile shadertype: " << getShaderType(type) << std::endl
        << log << std::endl;
        glDeleteShader(shader); // Don't leak the shader.
        exit(EXIT_FAILURE);
    }
    return shader;
}

ShaderProgram::ShaderProgram(std::string vertex_shader_filename,
        std::string tessellation_control_shader_filename,
        std::string tessellation_eval_shader_filename,
        std::string geometry_shader_filename,
        std::string fragment_shader_filename){

    if (vertex_shader_filename != "") {
        auto v_source = FileReader::ReadFromFile(vertex_shader_filename);
        AttachShader(GL_VERTEX_SHADER, v_source);
    }
    if (tessellation_control_shader_filename != "") {
        auto t_c_source = FileReader::ReadFromFile(tessellation_control_shader_filename);
        AttachShader(GL_TESS_CONTROL_SHADER, t_c_source);
    }
    if (tessellation_eval_shader_filename != "") {
        auto t_e_source = FileReader::ReadFromFile(tessellation_eval_shader_filename);
        AttachShader(GL_TESS_EVALUATION_SHADER, t_e_source);
    }
    if (geometry_shader_filename != "") {
        auto g_source = FileReader::ReadFromFile(geometry_shader_filename);
        AttachShader(GL_GEOMETRY_SHADER, g_source);
    }
    if (fragment_shader_filename != "") {
        auto f_source = FileReader::ReadFromFile(fragment_shader_filename);
        AttachShader(GL_FRAGMENT_SHADER, f_source);
    }

    //Link shaders
    ConfigureShaderProgram();

    //Detach shaders after successful linking
    for (GLuint shader_program : shader_programs_) {
        glDetachShader(prog, shader_program);
    }
}


ComputeShader::ComputeShader(std::string compute_shader_filename){
    if (compute_shader_filename != "") {
        auto v_source = FileReader::ReadFromFile(compute_shader_filename);
        AttachShader(GL_COMPUTE_SHADER, v_source);
    }

    //Link shader
    ConfigureShaderProgram();

    //Detach shader after successful linking
    for (GLuint shader_program : shader_programs_) {
        glDetachShader(prog, shader_program);
    }
}
