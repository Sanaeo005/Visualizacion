#include "WindowGL.h"

#include <iostream>

WindowGL::WindowGL(int width, int height)
    : window(nullptr), width(width), height(height)
{
}

WindowGL::~WindowGL()
{
    close();
}

bool WindowGL::initialize()
{
    if (!glfwInit())
    {
        std::cerr << "Error al inicializar GLFW." << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
        width,
        height,
        "Visualizador OBJ",
        nullptr,
        nullptr
    );

    if (!window)
    {
        std::cerr << "Error al crear la ventana." << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    GLenum result = glewInit();

    if (result != GLEW_OK)
    {
        std::cerr << "Error al inicializar GLEW: "
                  << glewGetErrorString(result)
                  << std::endl;

        close();
        return false;
    }

    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    glfwSwapInterval(1);

    std::cout << "OpenGL inicializado correctamente." << std::endl;

    std::cout << "Version OpenGL: "
              << glGetString(GL_VERSION)
              << std::endl;

    return true;
}

bool WindowGL::shouldClose() const
{
    return !window || glfwWindowShouldClose(window);
}

void WindowGL::clear()
{
    glClearColor(0.08f, 0.08f, 0.12f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowGL::update()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void WindowGL::close()
{
    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

GLFWwindow* WindowGL::getWindow() const
{
    return window;
}