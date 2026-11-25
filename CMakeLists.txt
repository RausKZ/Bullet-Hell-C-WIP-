cmake_minimum_required(VERSION 3.25)
project(BulletHell C)

set(CMAKE_C_STANDARD 99)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Tipo de compilacion" FORCE)
endif()

# MSVC: runtime de C estatico (/MT)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# --- raylib ---
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

target_compile_definitions(raylib PRIVATE SUPPORT_CLIPBOARD_IMAGE=0)

# --- JUEGO ---
# Buscamos dinámicamente todos los archivos .c (por si acaso hay errores de tipeo en los nombres)
file(GLOB SOURCES "*.c")

# Este mensaje saldrá en el log de GitHub Actions para confirmar qué archivos leyó
message(STATUS "ARCHIVOS C ENCONTRADOS PARA COMPILAR: ${SOURCES}")

add_executable(BulletHell ${SOURCES})
target_link_libraries(BulletHell PRIVATE raylib)

if(WIN32)
    set_target_properties(BulletHell PROPERTIES WIN32_EXECUTABLE TRUE)
    if(MSVC)
        target_link_options(BulletHell PRIVATE /ENTRY:mainCRTStartup)
    else()
        target_link_options(BulletHell PRIVATE -static)
    endif()
elseif(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    target_link_libraries(BulletHell PRIVATE raylib OpenGL::GL X11::X11 m pthread dl rt)
    target_link_options(BulletHell PRIVATE -static-libgcc -Wl,--as-needed)
endif()

add_custom_command(TARGET BulletHell POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:BulletHell>/Assets
)
