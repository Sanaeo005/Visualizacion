#include "OBJModel.h"

#include <iostream>

OBJModel::OBJModel()
    : VAO(0),
      VBO(0),
      EBO(0),
      indexCount(0),
      vertexCount(0),
      renderMode(RenderMode::Surface)
{
}

OBJModel::~OBJModel()
{
    release();
}

void OBJModel::release()
{
    if (EBO != 0)
    {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }

    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    indexCount = 0;
    vertexCount = 0;
}

bool OBJModel::initialize(const OBJData& data)
{
    if (data.vertices.empty() || data.indices.empty())
    {
        std::cerr << "El modelo no contiene geometria."
                  << std::endl;

        return false;
    }

    // Liberamos geometria anterior antes de cargar otra
    release();

    indexCount = static_cast<GLsizei>(data.indices.size());
    vertexCount = static_cast<GLsizei>(data.vertices.size());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    //Enviar posiciones a laGPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        data.vertices.size() * sizeof(glm::vec3),
        data.vertices.data(),
        GL_STATIC_DRAW
    );

    // Enviar indices a la GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        data.indices.size() * sizeof(unsigned int),
        data.indices.data(),
        GL_STATIC_DRAW
    );

    //Atributo 0: posicion
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::vec3),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    // El atributo 1 sera un color constante
    glDisableVertexAttribArray(1);

    glVertexAttrib3f(
        1,
        0.85f,
        0.85f,
        0.85f
    );

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::cout << "Geometria enviada a OpenGL."
              << std::endl;

    return true;
}


void OBJModel::setRenderMode(RenderMode mode)
{
    renderMode = mode;
}

RenderMode OBJModel::getRenderMode() const
{
    return renderMode;
}

void OBJModel::render() const
{
    glBindVertexArray(VAO);

    // Color temporal del modelo
    glVertexAttrib3f(
        1,
        0.85f,
        0.85f,
        0.85f
    );

    switch (renderMode)
    {
        case RenderMode::Points:
        {
            glPointSize(3.0f);

            glDrawArrays(
                GL_POINTS,
                0,
                vertexCount
            );

            break;
        }

        case RenderMode::Wireframe:
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            glDrawElements(
                GL_TRIANGLES,
                indexCount,
                GL_UNSIGNED_INT,
                nullptr
            );

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            break;
        }

        case RenderMode::Surface:
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glDrawElements(
                GL_TRIANGLES,
                indexCount,
                GL_UNSIGNED_INT,
                nullptr
            );

            break;
        }
    }

    glBindVertexArray(0);
}