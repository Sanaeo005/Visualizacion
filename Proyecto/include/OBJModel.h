#ifndef OBJ_MODEL_H
#define OBJ_MODEL_H

#include <GL/glew.h>

#include "OBJLoader.h"

// Modos de representacion del modelo
enum class RenderMode
{
    Points,
    Wireframe,
    Surface
};

class OBJModel
{
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    GLsizei indexCount;
    GLsizei vertexCount;

    RenderMode renderMode;

    void release();

public:
    OBJModel();

    ~OBJModel();

    bool initialize(const OBJData& data);

    void setRenderMode(RenderMode mode);

    RenderMode getRenderMode() const;

    void render() const;
};

#endif