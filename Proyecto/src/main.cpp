#include "WindowGL.h"
#include "Scene.h"
#include <iostream>
#include <vector>
#include <string>


// Datos para controlar el movimiento del mouse
struct MouseState
{
    Scene* scene = nullptr;

    bool dragging = false;

    double lastX = 0.0;
    double lastY = 0.0;
};

void cursorCallback(
    GLFWwindow* window,
    double xpos,
    double ypos
)
{
    MouseState* state = static_cast<MouseState*>(
        glfwGetWindowUserPointer(window)
    );

    if (!state || !state->scene)
    {
        return;
    }

    if (glfwGetMouseButton(
        window,
        GLFW_MOUSE_BUTTON_LEFT
    ) == GLFW_PRESS)
    {
        if (!state->dragging)
        {
            state->dragging = true;

            state->lastX = xpos;
            state->lastY = ypos;

            return;
        }

        float deltaX =
            static_cast<float>(xpos - state->lastX);

        float deltaY =
            static_cast<float>(ypos - state->lastY);

        state->scene->rotateCamera(deltaX, deltaY);

        state->lastX = xpos;
        state->lastY = ypos;
    }
    else
    {
        state->dragging = false;
    }
}


void scrollCallback(
    GLFWwindow* window,
    double,
    double yoffset
)
{
    MouseState* state = static_cast<MouseState*>(
        glfwGetWindowUserPointer(window)
    );

    if (!state || !state->scene)
    {
        return;
    }

    state->scene->zoomCamera(
        static_cast<float>(yoffset)
    );
}

void framebufferCallback(
    GLFWwindow* window,
    int width,
    int height
)
{
    //Actualizamos el area de dibujo
    glViewport(0, 0, width, height);

    //Recuperamos la informacion de la escena
    MouseState* state = static_cast<MouseState*>(
        glfwGetWindowUserPointer(window)
    );

    if (!state || !state->scene)
    {
        return;
    }

    //Actualizamos la proyeccion
    state->scene->updateProjection(width, height);
}

int main()
{
    WindowGL window(1000, 700);

    if (!window.initialize())
    {
        std::cerr << "No fue posible iniciar el visualizador."
                  << std::endl;

        return 1;
    }


    // Archivos
    std::vector<std::string> modelPaths =
    {
        "models/Teapot.obj",
        "models/pescado.obj",
        "models/cat.obj",
        "models/casa.obj",
        "models/car.obj",
        "models/bunny.obj",
        "Superficie matematica"
    };

    int currentModel = 0;

    //Cargamos la geometria inicial
    OBJLoader loader;
    OBJData initialData;

    if (!loader.load(modelPaths[currentModel], initialData))
    {
        return 1;
    }

    {
        Scene scene;

        if (!scene.initialize(initialData))
        {
            std::cerr << "No fue posible inicializar la escena."
                      << std::endl;

            return 1;
        }

        std::cout << "\nControles disponibles:\n";
        std::cout << "Flecha derecha - Siguiente modelo\n";
        std::cout << "Flecha izquierda - Modelo anterior\n";
        std::cout << "1 - Nube de puntos\n";
        std::cout << "2 - Malla de lineas\n";
        std::cout << "3 - Superficie\n";
        std::cout << "X - Activar/detener rotacion X\n";
        std::cout << "Y - Activar/detener rotacion Y\n";
        std::cout << "Z - Activar/detener rotacion Z\n";
        std::cout << "R - Reiniciar rotaciones\n";
        std::cout << "Mouse izquierdo - Girar camara\n";
        std::cout << "Rueda del mouse - Acercar/alejar\n";

        GLFWwindow* glfwWindow = window.getWindow();

        MouseState mouseState;

        mouseState.scene = &scene;

        glfwSetWindowUserPointer(
            glfwWindow,
            &mouseState
        );

        glfwSetCursorPosCallback(
            glfwWindow,
            cursorCallback
        );

        glfwSetScrollCallback(
            glfwWindow,
            scrollCallback
        );

        //Detecta cambios en el tamaño de la ventana
        glfwSetFramebufferSizeCallback(
            glfwWindow,
            framebufferCallback
        );

        //Sincronizamos las dimensiones iniciales
        int framebufferWidth;
        int framebufferHeight;

        glfwGetFramebufferSize(
            glfwWindow,
            &framebufferWidth,
            &framebufferHeight
        );

        framebufferCallback(
            glfwWindow,
            framebufferWidth,
            framebufferHeight
        );

        bool previousNext = false;
        bool previousBack = false;
        // Estado anterior de las teclas de rotacion
        bool previousX = false;
        bool previousY = false;
        bool previousZ = false;
        bool previousR = false;
        // Tiempo del fotograma anterior
        double previousTime = glfwGetTime();

        while (!window.shouldClose())
        {


            //Calculamos tiempo transcurrido
            double currentTime = glfwGetTime();

            float deltaTime = static_cast<float>(
                currentTime - previousTime
            );

            previousTime = currentTime;

            //consultamos las teclas
            bool nextPressed =
                glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS;

            bool backPressed =
                glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS;

            //Siguiente modelo
            if (nextPressed && !previousNext)
            {
                int nextModel =
                    (currentModel + 1) %
                    static_cast<int>(modelPaths.size());

                bool loaded = false;

                if (modelPaths[nextModel] == "Superficie matematica")
                {
                    loaded = scene.loadMathematicalSurface();
                }
                else
                {
                    loaded = scene.loadModel(modelPaths[nextModel]);
                }

                if (loaded)
                {
                    currentModel = nextModel;
                }
            }

            //Modelo anterior
            if (backPressed && !previousBack)
            {
                int previousModel =
                    (currentModel - 1 +
                     static_cast<int>(modelPaths.size()))
                    % static_cast<int>(modelPaths.size());

                bool loaded = false;

                if (modelPaths[previousModel] == "Superficie matematica")
                {
                    loaded = scene.loadMathematicalSurface();
                }
                else
                {
                    loaded = scene.loadModel(modelPaths[previousModel]);
                }

                if (loaded)
                {
                    currentModel = previousModel;
                }
            }

            previousNext = nextPressed;
            previousBack = backPressed;

            // Modos de representacion
            if (glfwGetKey(glfwWindow, GLFW_KEY_1) == GLFW_PRESS)
            {
                scene.setRenderMode(RenderMode::Points);
            }

            if (glfwGetKey(glfwWindow, GLFW_KEY_2) == GLFW_PRESS)
            {
                scene.setRenderMode(RenderMode::Wireframe);
            }

            if (glfwGetKey(glfwWindow, GLFW_KEY_3) == GLFW_PRESS)
            {
                scene.setRenderMode(RenderMode::Surface);
            }

            //==================================
            //Controles de rotacion
            //==================================
            bool xPressed =
                glfwGetKey(glfwWindow, GLFW_KEY_X) == GLFW_PRESS;

            bool yPressed =
                glfwGetKey(glfwWindow, GLFW_KEY_Y) == GLFW_PRESS;

            bool zPressed =
                glfwGetKey(glfwWindow, GLFW_KEY_Z) == GLFW_PRESS;

            bool rPressed =
                glfwGetKey(glfwWindow, GLFW_KEY_R) == GLFW_PRESS;

            //Alternar rotacion X
            if (xPressed && !previousX)
            {
                scene.toggleRotationX();
            }

            //Alternar rotacion Y
            if (yPressed && !previousY)
            {
                scene.toggleRotationY();
            }

            //Alternar rotacion Z
            if (zPressed && !previousZ)
            {
                scene.toggleRotationZ();
            }

            //Reiniciar rotaciones
            if (rPressed && !previousR)
            {
                scene.resetRotation();
            }

            previousX = xPressed;
            previousY = yPressed;
            previousZ = zPressed;
            previousR = rPressed;

            scene.update(deltaTime);

            window.clear();

            scene.render();

            window.update();
        }
    }

    return 0;
}