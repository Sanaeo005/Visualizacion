#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>

#include <string>

class ShaderProgram
{
private:
    GLuint programID;

    std::string readFile(const std::string& path);

    GLuint compileShader(
        GLenum type,
        const std::string& source
    );

public:
    ShaderProgram();

    ~ShaderProgram();

    bool load(
        const std::string& vertexPath,
        const std::string& fragmentPath
    );

    void use() const;

    GLuint getID() const;
};

#endif