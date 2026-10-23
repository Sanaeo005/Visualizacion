#include "ShaderProgram.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

ShaderProgram::ShaderProgram()
    : programID(0)
{
}

ShaderProgram::~ShaderProgram()
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
    }
}

std::string ShaderProgram::readFile(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "No se pudo abrir el archivo: "
                  << path << std::endl;

        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

GLuint ShaderProgram::compileShader(
    GLenum type,
    const std::string& source
)
{
    GLuint shader = glCreateShader(type);

    const char* shaderSource = source.c_str();

    glShaderSource(shader, 1, &shaderSource, nullptr);

    glCompileShader(shader);

    GLint success = GL_FALSE;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success != GL_TRUE)
    {
        GLint logLength = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<char> log(logLength > 0 ? logLength : 1);

        glGetShaderInfoLog(
            shader,
            static_cast<GLsizei>(log.size()),
            nullptr,
            log.data()
        );

        std::cerr << "Error al compilar shader:\n"
                  << log.data() << std::endl;

        glDeleteShader(shader);

        return 0;
    }

    return shader;
}

bool ShaderProgram::load(
    const std::string& vertexPath,
    const std::string& fragmentPath
)
{
    std::string vertexSource = readFile(vertexPath);
    std::string fragmentSource = readFile(fragmentPath);

    if (vertexSource.empty() || fragmentSource.empty())
    {
        return false;
    }

    GLuint vertexShader = compileShader(
        GL_VERTEX_SHADER,
        vertexSource
    );

    if (vertexShader == 0)
    {
        return false;
    }

    GLuint fragmentShader = compileShader(
        GL_FRAGMENT_SHADER,
        fragmentSource
    );

    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return false;
    }

    GLuint newProgram = glCreateProgram();

    glAttachShader(newProgram, vertexShader);
    glAttachShader(newProgram, fragmentShader);

    glLinkProgram(newProgram);

    GLint success = GL_FALSE;

    glGetProgramiv(newProgram, GL_LINK_STATUS, &success);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (success != GL_TRUE)
    {
        GLint logLength = 0;

        glGetProgramiv(
            newProgram,
            GL_INFO_LOG_LENGTH,
            &logLength
        );

        std::vector<char> log(logLength > 0 ? logLength : 1);

        glGetProgramInfoLog(
            newProgram,
            static_cast<GLsizei>(log.size()),
            nullptr,
            log.data()
        );

        std::cerr << "Error al enlazar programa:\n"
                  << log.data() << std::endl;

        glDeleteProgram(newProgram);

        return false;
    }

    if (programID != 0)
    {
        glDeleteProgram(programID);
    }

    programID = newProgram;

    std::cout << "Shaders cargados correctamente."
              << std::endl;

    return true;
}

void ShaderProgram::use() const
{
    glUseProgram(programID);
}

GLuint ShaderProgram::getID() const
{
    return programID;
}