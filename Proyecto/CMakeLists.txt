cmake_minimum_required(VERSION 3.16)

project(VisualizadorOBJ LANGUAGES CXX)

# Configuración de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Advertencias del compilador
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
endif()

# Bibliotecas necesarias
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

# Archivos fuente
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Crear ejecutable
add_executable(VisualizadorOBJ ${SOURCES})

# Directorio de cabeceras
target_include_directories(VisualizadorOBJ PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Enlazar bibliotecas
target_link_libraries(VisualizadorOBJ PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
    glm::glm
)
