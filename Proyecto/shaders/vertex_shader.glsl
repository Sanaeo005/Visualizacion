#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

out vec3 vertexColor;
out float vertexHeight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Color recibido desde OpenGL
    vertexColor = aColor;

    // Altura original del vertice
    vertexHeight = aPosition.y;

    // Transformacion del vertice
    gl_Position =
        projection *
        view *
        model *
        vec4(aPosition, 1.0);
}