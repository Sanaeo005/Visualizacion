#include "OrbitCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include <cmath>

OrbitCamera::OrbitCamera()
    : target(0.0f),
      distance(10.0f),
      yaw(40.0f),
      pitch(25.0f),
      minDistance(0.5f),
      maxDistance(100.0f)
{
}

void OrbitCamera::setTarget(const glm::vec3& newTarget)
{
    target = newTarget;
}

void OrbitCamera::setDistance(float newDistance)
{
    distance = glm::clamp(
        newDistance,
        minDistance,
        maxDistance
    );
}

void OrbitCamera::setDistanceLimits(
    float minimum,
    float maximum
)
{
    minDistance = glm::max(0.001f, minimum);
    maxDistance = glm::max(minDistance, maximum);

    setDistance(distance);
}

void OrbitCamera::rotate(float deltaX, float deltaY)
{
    float sensitivity = 0.25f;

    yaw += deltaX * sensitivity;
    pitch -= deltaY * sensitivity;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);
}

void OrbitCamera::zoom(float offset)
{
    float zoomFactor = std::pow(0.9f, offset);

    setDistance(distance * zoomFactor);
}

glm::mat4 OrbitCamera::getViewMatrix() const
{
    float theta = glm::radians(yaw);
    float phi = glm::radians(pitch);

    glm::vec3 position;

    position.x =
        target.x + distance * std::cos(phi) * std::sin(theta);

    position.y =
        target.y + distance * std::sin(phi);

    position.z =
        target.z + distance * std::cos(phi) * std::cos(theta);

    return glm::lookAt(
        position,
        target,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

float OrbitCamera::getDistance() const
{
    return distance;
}