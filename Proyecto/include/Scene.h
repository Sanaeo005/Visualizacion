#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "OBJModel.h"
#include <string>
#include "OrbitCamera.h"

class Scene
{
private:
    GLuint axesVAO;
    GLuint axesVBO;
    ShaderProgram shader;
    OBJModel objModel;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 modelCenter;
    float modelRadius;
    float minHeight;
    float maxHeight;
    glm::vec3 rotationAngles;
    bool rotateX;
    bool rotateY;
    bool rotateZ;
    float rotationSpeed;
    OrbitCamera camera;
    int framebufferWidth;
    int framebufferHeight;

public:
    Scene();

    ~Scene();

    bool initialize(const OBJData& data);

    void render();

    void updateProjection(int width, int height);

    void fitCamera(const OBJData& data);

    void setRenderMode(RenderMode mode);

    bool loadModel(const std::string& filePath);

    bool loadMathematicalSurface();

    void update(float deltaTime);

    void toggleRotationX();

    void toggleRotationY();
    
    void toggleRotationZ();

    void resetRotation();

    void rotateCamera(float deltaX, float deltaY);

    void zoomCamera(float offset);
};

#endif