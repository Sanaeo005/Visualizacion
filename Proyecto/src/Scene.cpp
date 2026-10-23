#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include "SurfaceGenerator.h"

Scene::Scene()
    : axesVAO(0),
      axesVBO(0),
      model(1.0f),
      view(1.0f),
      projection(1.0f),
      modelCenter(0.0f),
      modelRadius(1.0f),
      minHeight(0.0f),
      maxHeight(1.0f),
      rotationAngles(0.0f),
      rotateX(false),
      rotateY(false),
      rotateZ(false),
      rotationSpeed(45.0f),
      framebufferWidth(1000),
      framebufferHeight(700)
{
}

Scene::~Scene()
{
    if (axesVBO != 0)
    {
        glDeleteBuffers(1, &axesVBO);
    }

    if (axesVAO != 0)
    {
        glDeleteVertexArrays(1, &axesVAO);
    }
}

bool Scene::initialize(const OBJData& data)
{
    if (!shader.load(
        "shaders/vertex_shader.glsl",
        "shaders/frag_shader.glsl"
    ))
    {
        return false;
    }

    minHeight = data.minBounds.y;
    maxHeight = data.maxBounds.y;

    //Cada vértice contiene posición (x,y,z)
    //y color (r, g, b)
    float axesVertices[] =
    {
        // Eje X - Rojo
        -2.0f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f,
         2.0f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f,

        // Eje Y - Verde
         0.0f, -2.0f,  0.0f,   0.0f, 1.0f, 0.0f,
         0.0f,  2.0f,  0.0f,   0.0f, 1.0f, 0.0f,

        // Eje Z - Azul
         0.0f,  0.0f, -2.0f,   0.0f, 0.0f, 1.0f,
         0.0f,  0.0f,  2.0f,   0.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &axesVAO);
    glGenBuffers(1, &axesVBO);

    glBindVertexArray(axesVAO);

    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(axesVertices),
        axesVertices,
        GL_STATIC_DRAW
    );

    // Atributo 0: posicion
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    // Atributo 1: color
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    fitCamera(data);

    updateProjection(1000, 700);

    if (!objModel.initialize(data))
    {
        return false;
    }

    return true;
}

void Scene::updateProjection(int width, int height)
{
    // Evitar division entre cero
    if (width <= 0 || height <= 0)
    {
        return;
    }

    framebufferWidth = width;
    framebufferHeight = height;

    // Relación de aspecto de la ventana
    float aspectRatio =
        static_cast<float>(width) /
        static_cast<float>(height);

    // Planos de recorte
    float nearPlane = glm::max(
        0.01f,
        modelRadius * 0.01f
    );

    float farPlane = glm::max(
        100.0f,
        modelRadius * 30.0f
    );

    // Actualizamos la matriz de proyeccion
    projection = glm::perspective(
        glm::radians(45.0f),
        aspectRatio,
        nearPlane,
        farPlane
    );
}


void Scene::fitCamera(const OBJData& data)
{
    modelCenter = (data.minBounds + data.maxBounds) * 0.5f;

    glm::vec3 dimensions =
        data.maxBounds - data.minBounds;

    modelRadius = glm::length(dimensions) * 0.5f;

    if (modelRadius < 0.001f)
    {
        modelRadius = 1.0f;
    }

    float fieldOfView = glm::radians(45.0f);

    float distance =
        modelRadius / glm::sin(fieldOfView * 0.5f);

    distance *= 1.4f;

    camera.setTarget(modelCenter);

    camera.setDistanceLimits(
        modelRadius * 1.1f,
        modelRadius * 20.0f
    );

    camera.setDistance(distance);

    view = camera.getViewMatrix();

    std::cout << "\nCamara orbital ajustada.\n";

    std::cout << "Centro: ("
              << modelCenter.x << ", "
              << modelCenter.y << ", "
              << modelCenter.z << ")\n";

    std::cout << "Radio: "
              << modelRadius << '\n';

    std::cout << "Distancia: "
              << camera.getDistance() << '\n';
}

void Scene::render()
{

    view = camera.getViewMatrix();
    shader.use();

    GLuint programID = shader.getID();

    // Ubicaciones de las matrices
    GLint modelLocation =
        glGetUniformLocation(programID, "model");

    GLint viewLocation =
        glGetUniformLocation(programID, "view");

    GLint projectionLocation =
        glGetUniformLocation(programID, "projection");

    // Enviar matriz de modelo
    glUniformMatrix4fv(
        modelLocation,
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );

    //matriz de vista
    glUniformMatrix4fv(
        viewLocation,
        1,
        GL_FALSE,
        glm::value_ptr(view)
    );

    //Matriz de proyeccion
    glUniformMatrix4fv(
        projectionLocation,
        1,
        GL_FALSE,
        glm::value_ptr(projection)
    );

    // Ubicaciones de las variables del mapa de alturas
    GLint minHeightLocation =
        glGetUniformLocation(programID, "minHeight");

    GLint maxHeightLocation =
        glGetUniformLocation(programID, "maxHeight");

    GLint useHeightColorLocation =
        glGetUniformLocation(programID, "useHeightColor");

    // Envia los limites de altura
    glUniform1f(minHeightLocation, minHeight);
    glUniform1f(maxHeightLocation, maxHeight);

    // Dibujar sistema coordenado
    glUniform1i(useHeightColorLocation, GL_FALSE);

    glBindVertexArray(axesVAO);

    glLineWidth(2.0f);

    glDrawArrays(GL_LINES, 0, 6);

    glBindVertexArray(0);

    //Dibuja el modelo OBJ
    glUniform1i(useHeightColorLocation, GL_TRUE);

    objModel.render();

    //Restaurar el valor
    glUniform1i(useHeightColorLocation, GL_FALSE);
}

void Scene::setRenderMode(RenderMode mode)
{
    objModel.setRenderMode(mode);
}

bool Scene::loadModel(const std::string& filePath)
{
    OBJLoader loader;
    OBJData data;

    // Leer archivo OBJ
    if (!loader.load(filePath, data))
    {
        std::cerr << "No fue posible cargar: "
                  << filePath << std::endl;

        return false;
    }

    // Se actualiza geometria en OpenGL
    if (!objModel.initialize(data))
    {
        std::cerr << "No fue posible preparar el modelo."
                  << std::endl;

        return false;
    }

    //Actualizamos los limites de altura
    minHeight = data.minBounds.y;
    maxHeight = data.maxBounds.y;

    // Ajustamos la cámara al nuevo modelo
    fitCamera(data);

    // Reiniciamos la orientacion al cambiar de modelo
    resetRotation();

    model = glm::mat4(1.0f);

    // Actualizar proyeccion
    updateProjection(framebufferWidth, framebufferHeight);

    //Mostramos informacion en terminal
    std::cout << "\n================================\n";
    std::cout << "Modelo: " << filePath << '\n';

    std::cout << "Vertices: "
              << data.vertices.size() << '\n';

    std::cout << "Caras: "
              << data.faceCount << '\n';

    std::cout << "Triangulos: "
              << data.indices.size() / 3 << '\n';

    std::cout << "Minimo: ("
              << data.minBounds.x << ", "
              << data.minBounds.y << ", "
              << data.minBounds.z << ")\n";

    std::cout << "Maximo: ("
              << data.maxBounds.x << ", "
              << data.maxBounds.y << ", "
              << data.maxBounds.z << ")\n";

    std::cout << "================================\n";

    return true;
}

void Scene::toggleRotationX()
{
    rotateX = !rotateX;
}

void Scene::toggleRotationY()
{
    rotateY = !rotateY;
}

void Scene::toggleRotationZ()
{
    rotateZ = !rotateZ;
}

void Scene::resetRotation()
{
    rotationAngles = glm::vec3(0.0f);

    rotateX = false;
    rotateY = false;
    rotateZ = false;
}


void Scene::update(float deltaTime)
{
    // Actualizar los angulos activos
    if (rotateX)
    {
        rotationAngles.x += rotationSpeed * deltaTime;
    }

    if (rotateY)
    {
        rotationAngles.y += rotationSpeed * deltaTime;
    }

    if (rotateZ)
    {
        rotationAngles.z += rotationSpeed * deltaTime;
    }

    //Angulos entre 0 y 360 grados
    rotationAngles.x = glm::mod(rotationAngles.x, 360.0f);
    rotationAngles.y = glm::mod(rotationAngles.y, 360.0f);
    rotationAngles.z = glm::mod(rotationAngles.z, 360.0f);

    //Matriz de rotacion
    glm::mat4 rotation(1.0f);

    rotation = glm::rotate(
        rotation,
        glm::radians(rotationAngles.x),
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    rotation = glm::rotate(
        rotation,
        glm::radians(rotationAngles.y),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    rotation = glm::rotate(
        rotation,
        glm::radians(rotationAngles.z),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    //Trasladar al centro del modelo
    glm::mat4 translateToCenter = glm::translate(
        glm::mat4(1.0f),
        modelCenter
    );

    //Trasladamos temporalmente al origen
    glm::mat4 translateToOrigin = glm::translate(
        glm::mat4(1.0f),
        -modelCenter
    );

    // Transformacion final
    model = translateToCenter *
            rotation *
            translateToOrigin;
}

void Scene::rotateCamera(float deltaX, float deltaY)
{
    camera.rotate(deltaX, deltaY);
}

void Scene::zoomCamera(float offset)
{
    camera.zoom(offset);
}

bool Scene::loadMathematicalSurface()
{
    // Superficie de 100 x 100 vertices
    OBJData data = SurfaceGenerator::generate(100);

    if (data.vertices.empty() || data.indices.empty())
    {
        std::cerr << "Error al generar la superficie matematica.\n";
        return false;
    }

    // Enviamos la nueva geometria a OpenGL
    if (!objModel.initialize(data))
    {
        std::cerr << "Error al preparar la superficie.\n";
        return false;
    }

    // Se actualizan limites para el shader de alturas
    minHeight = data.minBounds.y;
    maxHeight = data.maxBounds.y;

    // Ajustar la camara
    fitCamera(data);

    // Conservamos las dimensiones actuales de la ventana
    updateProjection(framebufferWidth, framebufferHeight);

    // Reinicia las rotaciones
    resetRotation();
    model = glm::mat4(1.0f);

    // Mostrar informacion
    std::cout << "\n================================\n";
    std::cout << "Superficie matematica\n";
    std::cout << "Funcion: cos(x) + sin(z)\n";
    std::cout << "Dominio: [-2pi, 2pi] x [-2pi, 2pi]\n";

    std::cout << "Vertices: "
              << data.vertices.size() << '\n';

    std::cout << "Triangulos: "
              << data.indices.size() / 3 << '\n';

    std::cout << "Altura minima: "
              << minHeight << '\n';

    std::cout << "Altura maxima: "
              << maxHeight << '\n';

    std::cout << "================================\n";

    return true;
}