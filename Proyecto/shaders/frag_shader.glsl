#version 330 core

in vec3 vertexColor;
in float vertexHeight;

out vec4 FragColor;

// Valores de altura
uniform float minHeight;
uniform float maxHeight;

// Indica si usamos el mapa de alturas
uniform bool useHeightColor;

vec3 heightColor(float height)
{
    vec3 azul = vec3(0.0, 0.0, 1.0);
    vec3 amarillo = vec3(1.0, 1.0, 0.0);
    vec3 rojo = vec3(1.0, 0.0, 0.0);

    // Sin division entre cero
    float range = maxHeight - minHeight;

    if (abs(range) < 0.00001)
    {
        return amarillo;
    }

    //Altura
    float t = (height - minHeight) / range;

    t = clamp(t, 0.0, 1.0);

    // Primera mitad: azul a amarillo
    if (t <= 0.5)
    {
        return mix(
            azul,
            amarillo,
            t * 2.0
        );
    }

    // Segunda mitad: amarillo a rojo
    return mix(
        amarillo,
        rojo,
        (t - 0.5) * 2.0
    );
}

void main()
{
    vec3 color;

    if (useHeightColor)
    {
        color = heightColor(vertexHeight);
    }
    else
    {
        color = vertexColor;
    }

    FragColor = vec4(color, 1.0);
}