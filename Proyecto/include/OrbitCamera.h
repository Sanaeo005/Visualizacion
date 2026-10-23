#ifndef ORBIT_CAMERA_H
#define ORBIT_CAMERA_H

#include <glm/glm.hpp>

class OrbitCamera
{
private:
    glm::vec3 target;

    float distance;
    float yaw;
    float pitch;

    float minDistance;
    float maxDistance;

public:
    OrbitCamera();

    void setTarget(const glm::vec3& newTarget);

    void setDistance(float newDistance);

    void setDistanceLimits(float minimum, float maximum);

    void rotate(float deltaX, float deltaY);

    void zoom(float offset);

    glm::mat4 getViewMatrix() const;

    float getDistance() const;
};

#endif