cmake_minimum_required(VERSION 4.3)

project(practica-3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(practica-3 main.cpp
    rle.h rle.cpp
    lz78.h lz78.cpp
    texto.h texto.cpp
    cifrado.h cifrado.cpp
    archivo.h archivo.cpp
    consola.h consola.cpp
)

target_compile_definitions(practica-3 PRIVATE
    RUTA_INOUT="${CMAKE_SOURCE_DIR}/inout/"
)

file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/inout")

include(GNUInstallDirs)
install(TARGETS practica-3
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
