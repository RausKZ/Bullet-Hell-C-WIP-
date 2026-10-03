cmake_minimum_required(VERSION 3.25)
project(BulletHell C)

set(CMAKE_C_STANDARD 99)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Tipo de compilacion" FORCE)
endif()

# MSVC: runtime de C estatico (/MT), asi no hace falta vcruntime140.dll
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# --- raylib: se descarga y se compila como libreria estatica -----------------
include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    URL      https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz
    URL_HASH SHA256=2b3ee1e2120c7a0796b33062c7e9a694dd8a8caa56a96319ac8c8ecf54a90d0b
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(raylib)

# El juego no usa imagenes del portapapeles. Desactivarlo evita que raylib llame
# a Xlib directamente, asi en Linux el binario no enlaza contra libX11
# (GLFW ya carga libX11 y libGL en tiempo de ejecucion con dlopen).
target_compile_definitions(raylib PRIVATE SUPPORT_CLIPBOARD_IMAGE=0)

# --- Juego --------------------------------------------------------------------
add_executable(BulletHell main.c)
target_link_libraries(BulletHell PRIVATE raylib)

if(WIN32)
    # Aplicacion de ventana, sin consola
    set_target_properties(BulletHell PROPERTIES WIN32_EXECUTABLE TRUE)
    if(MSVC)
        target_link_options(BulletHell PRIVATE /ENTRY:mainCRTStartup)
    else()
        # MinGW: mete libgcc y libwinpthread dentro del .exe
        target_link_options(BulletHell PRIVATE -static)
    endif()
elseif(UNIX AND NOT APPLE)
    # --as-needed descarta las librerias que el binario no usa (libGL, libX11...)
    target_link_options(BulletHell PRIVATE -static-libgcc -Wl,--as-needed)
endif()
