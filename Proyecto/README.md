# Proyecto de Mitad de Semestre — Visualización

Visualizador de modelos tridimensionales desarrollado en **C++ con OpenGL**. Permite cargar archivos OBJ, explorar su geometría mediante diferentes representaciones y aplicar transformaciones utilizando el teclado y el mouse.

## Funcionalidades

- Carga y visualización de seis modelos OBJ: `Teapot`, `pescado`, `cat`, `casa`, `car` y `bunny`.
- Sistema de ejes coordenados X, Y y Z.
- Representación como **nube de puntos**, **malla de líneas** o **superficie sólida**.
- Mapa de colores basado en la altura del modelo: azul (mínimo), amarillo (intermedio) y rojo (máximo)
- Cálculo de límites geométricos y ajuste de cámara al seleccionar un modelo.
- Rotaciones alrededor de los ejes X, Y y Z.
- Información de vértices, caras, triángulos y coordenadas extremas en la terminal.
- Ajuste de la proyección cuando cambia el tamaño de la ventana.

### Funcionalidades adicionales

- **Cámara orbital:** rotación de la vista arrastrando el mouse y acercamiento/alejamiento con la rueda.
- **Superficie matemática:** generación de una malla de 100 × 100 vértices para la función que viene en el PDF. La malla contiene 10,000 vértices y 19,602 triángulos; sus alturas mínima y máxima se muestran en la terminal owo.

## Tecnologías

- C++17
- OpenGL 3.3 Core (o compatible)
- GLFW, GLEW y GLM
- GLSL
- CMake

## Dependencias en Ubuntu

```bash
sudo apt update
sudo apt install build-essential cmake libglfw3-dev libglew-dev libglm-dev libgl1-mesa-dev
```

## Compilación y ejecución

Desde la carpeta `Proyecto` del repo:

```bash
cmake -S . -B build
cmake --build build
./build/VisualizadorOBJ
```

**NOTA:** ejecutar el programa desde `Proyecto`, ya que los archivos GLSL y OBJ se cargan mediante rutas relativas (`shaders/` y `models/`).

## Controles

| Control | Acción |
|---|---|
| `←` / `→` | Modelo anterior / siguiente modelo |
| `1` | Nube de puntos |
| `2` | Malla de líneas |
| `3` | Superficie sólida |
| `X` | Activar o detener rotación alrededor de X |
| `Y` | Activar o detener rotación alrededor de Y |
| `Z` | Activar o detener rotación alrededor de Z |
| `R` | Reiniciar las rotaciones del modelo |
| Arrastrar con clic izquierdo | Girar la cámara orbital |
| Rueda del mouse | Acercar o alejar |

La **superficie matemática** aparece como una opción adicional al recorrer los modelos con las flechas.

## Organización del proyecto

```text
Proyecto/
├── include/          # Declaraciones de clases
├── src/              # Implementaciones en C++
├── shaders/          # Shaders GLSL
├── models/           # Archivos OBJ
├── CMakeLists.txt    # Configuración de compilación
└── README.md
```

Mi proyecto separa la lectura de archivos (`OBJLoader`), la generación de la superficie matemática (`SurfaceGenerator`), el almacenamiento y dibujo de geometría (`OBJModel`), la cámara (`OrbitCamera`), los shaders (`ShaderProgram`) y la escena (`Scene`).

## Notas finales:

- Los colores se calculan con base en la altura **original** de la geometría, por lo que permanecen asociados al modelo al rotarlo.
- La interfaz se controla mediante teclado y mouse; no se integró Dear ImGui.
