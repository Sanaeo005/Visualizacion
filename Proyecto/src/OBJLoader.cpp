#include "OBJLoader.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

int OBJLoader::resolveIndex(int index, int vertexCount)
{
    if (index > 0)
    {
        return index - 1;
    }

    if (index < 0)
    {
        return vertexCount + index;
    }

    return -1;
}

bool OBJLoader::load(
    const std::string& filePath,
    OBJData& data
)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "No se pudo abrir: "
                  << filePath << std::endl;

        return false;
    }

    OBJData result;

    std::string line;
    unsigned int lineNumber = 0;

    try
    {
        while (std::getline(file, line))
        {
            ++lineNumber;

            std::istringstream stream(line);
            std::string type;

            stream >> type;

            // Ignorar lineas vacias y comentarios
            if (type.empty() || type[0] == '#')
            {
                continue;
            }

            // Leer vertices
            if (type == "v")
            {
                glm::vec3 vertex;

                if (!(stream >> vertex.x
                             >> vertex.y
                             >> vertex.z))
                {
                    throw std::runtime_error(
                        "Vertice incompleto"
                    );
                }

                if (result.vertices.empty())
                {
                    result.minBounds = vertex;
                    result.maxBounds = vertex;
                }
                else
                {
                    result.minBounds =
                        glm::min(result.minBounds, vertex);

                    result.maxBounds =
                        glm::max(result.maxBounds, vertex);
                }

                result.vertices.push_back(vertex);
            }

            // Leer caras
            else if (type == "f")
            {
                std::vector<unsigned int> faceIndices;

                std::string token;

                while (stream >> token)
                {
                    // Obtener el indice de posicion
                    // antes del primer '/'
                    std::size_t slash = token.find('/');

                    std::string positionIndex =
                        token.substr(0, slash);

                    int rawIndex = std::stoi(positionIndex);

                    int index = resolveIndex(
                        rawIndex,
                        static_cast<int>(
                            result.vertices.size()
                        )
                    );

                    if (index < 0 ||
                        index >= static_cast<int>(
                            result.vertices.size()
                        ))
                    {
                        throw std::runtime_error(
                            "Indice de vertice invalido"
                        );
                    }

                    faceIndices.push_back(
                        static_cast<unsigned int>(index)
                    );
                }

                if (faceIndices.size() < 3)
                {
                    throw std::runtime_error(
                        "Cara con menos de tres vertices"
                    );
                }

                result.faceCount++;

                // Triangulacion en abanico
                for (std::size_t i = 1;
                     i + 1 < faceIndices.size();
                     ++i)
                {
                    result.indices.push_back(faceIndices[0]);
                    result.indices.push_back(faceIndices[i]);
                    result.indices.push_back(faceIndices[i + 1]);
                }
            }
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error en " << filePath
                  << ", linea " << lineNumber
                  << ": " << error.what()
                  << std::endl;

        return false;
    }

    if (result.vertices.empty() || result.indices.empty())
    {
        std::cerr << "El archivo no contiene "
                  << "una malla valida."
                  << std::endl;

        return false;
    }

    data = std::move(result);

    return true;
}