#ifndef WINDOW_GL_H
#define WINDOW_GL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class WindowGL
{
private:
    GLFWwindow* window;

    int width;
    int height;

public:
    WindowGL(int width, int height);

    ~WindowGL();

    bool initialize();

    bool shouldClose() const;

    void clear();

    void update();

    void close();

    GLFWwindow* getWindow() const;
};

#endif