#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <glm/glm.hpp>

#include <string>
#include <vector>

struct OBJData
{
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 minBounds = glm::vec3(0.0f);
    glm::vec3 maxBounds = glm::vec3(0.0f);

    unsigned int faceCount = 0;
};

class OBJLoader
{
private:
    static int resolveIndex(int index, int vertexCount);

public:
    bool load(const std::string& filePath, OBJData& data);
};

#endif