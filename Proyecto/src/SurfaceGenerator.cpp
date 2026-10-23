#include "SurfaceGenerator.h"

#include <cmath>
#include <iostream>

OBJData SurfaceGenerator::generate(int resolution)
{
    OBJData data;

    if (resolution < 2)
    {
        std::cerr << "Resolucion invalida.\n";
        return data;
    }

    constexpr float PI = 3.14159265358979323846f;

    const float minDomain = -2.0f * PI;
    const float maxDomain =  2.0f * PI;

    const float step =
        (maxDomain - minDomain) / (resolution - 1);

    // Reserva memoria para los vertices
    data.vertices.reserve(resolution * resolution);

    // Genera vertices
    for (int i = 0; i < resolution; ++i)
    {
        float x = minDomain + i * step;

        for (int j = 0; j < resolution; ++j)
        {
            float z = minDomain + j * step;

            float y = std::cos(x) + std::sin(z);

            glm::vec3 vertex(x, y, z);

            // Calculamos limites de la geometria
            if (data.vertices.empty())
            {
                data.minBounds = vertex;
                data.maxBounds = vertex;
            }
            else
            {
                data.minBounds =
                    glm::min(data.minBounds, vertex);

                data.maxBounds =
                    glm::max(data.maxBounds, vertex);
            }

            data.vertices.push_back(vertex);
        }
    }

    //Reserva memoria para los triangulos
    data.indices.reserve(
        (resolution - 1) * (resolution - 1) * 6
    );

    //Conectamos los vertices para formar la malla
    for (int i = 0; i < resolution - 1; ++i)
    {
        for (int j = 0; j < resolution - 1; ++j)
        {
            unsigned int a = i * resolution + j;
            unsigned int b = (i + 1) * resolution + j;
            unsigned int c = i * resolution + j + 1;
            unsigned int d = (i + 1) * resolution + j + 1;

            // Primer triangulo
            data.indices.push_back(a);
            data.indices.push_back(c);
            data.indices.push_back(b);

            // Segundo triangulo
            data.indices.push_back(b);
            data.indices.push_back(c);
            data.indices.push_back(d);

            data.faceCount += 2;
        }
    }

    return data;
}