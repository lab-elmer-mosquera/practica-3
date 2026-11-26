#include "archivo.h"
#include <print>
#include <fstream>
#include <cstddef>

// Rutas de entrada y salida
// static const char* RUTA_ENTRADA = RUTA_INOUT "entrada.txt";
// static const char* RUTA_COMPRIMIDO_RLE = RUTA_INOUT "comprimido-rle.txt";
// static const char* RUTA_COMPRIMIDO_LZ78 = RUTA_INOUT "comprimido-lz78.txt";


namespace archivo {
    Texto input(){
        const char* RUTA_ENTRADA = RUTA_INOUT "entrada.txt";
        return Leer_archivo(RUTA_ENTRADA);
    }


    bool output_comprimido_rle(Texto& t){
        const char* RUTA_COMPRIMIDO_RLE = RUTA_INOUT "comprimido-rle.txt";
        return Escribir_archivo(RUTA_COMPRIMIDO_RLE, t);
    }

    bool output_comprimido_lz78(Texto& t){
        const char* RUTA_COMPRIMIDO_LZ78 = RUTA_INOUT "comprimido-lz78.txt";
        return Escribir_archivo(RUTA_COMPRIMIDO_LZ78, t);
    }
}


bool leer_linea(std::ifstream& archivo, Texto& linea) {
    linea.limpiar();
    char c;

    // Lee letra por letra hasta el salto de línea o fin de archivo
    while (archivo.get(c)) {
        if (c == '\r') continue;
        if (c == '\n') {
            return true;
        }
        linea.agregar_caracter(c);
    }

    return linea.longitud > 0;
}

Texto Leer_archivo(const char* ruta) {
    Texto resultado{};
    if (ruta == nullptr) return resultado;

    std::ifstream archivo(ruta);
    if(!archivo.is_open() ) {
        std::println("Error: no se pudo abrir el archivo.");
        return resultado;
    }

    char c{};
    while (archivo.get(c)) {
        // std::println("{}", static_cast<const char*>(linea.datos));
        if (c == '\r') continue;
        resultado.agregar_caracter(c);
    }

    archivo.close();
    return resultado;
}

bool Escribir_archivo(const char* ruta, const Texto& t) {
    if (ruta == nullptr) return false;

    std::ofstream archivo(ruta, std::ios::binary | std::ios::trunc);
    if (!archivo.is_open()) {
        std::println("Error: no se pudo abrir el archivo para escribir.");
        return false;
    }

    if (t.longitud > 0) {
        archivo.write(t.datos, static_cast<std::streamsize>(t.longitud));
    }

    archivo.close();
    return true;
}

bool Agregar_archivo(const char* ruta, const Texto& t) {
    if (ruta == nullptr) return false;

    std::ofstream archivo(ruta, std::ios::binary | std::ios::app);
    if (!archivo.is_open()) {
        std::println("Error: no se pudo abrir el archivo para agregar.");
        return false;
    }

    if (t.longitud > 0) {
        archivo.write(t.datos, static_cast<std::streamsize>(t.longitud));
    }

    archivo.close();
    return true;
}

bool Escribir_bits_archivo(const char* ruta, const Abstracto& a) {
    if (ruta == nullptr) return false;

    std::ofstream archivo(ruta, std::ios::binary | std::ios::trunc);
    if (!archivo.is_open()) {
        std::println("Error: no se pudo abrir el archivo para escribir.");
        return false;
    }

    if (a.longitud > 0) {
        archivo.write(reinterpret_cast<const char*>(a.datos),
                      static_cast<std::streamsize>(a.longitud));
    }

    archivo.close();
    return true;
}

bool Agregar_bits_archivo(const char* ruta, const Abstracto& a) {
    if (ruta == nullptr) return false;

    std::ofstream archivo(ruta, std::ios::binary | std::ios::app);
    if (!archivo.is_open()) {
        std::println("Error: no se pudo abrir el archivo para agregar.");
        return false;
    }

    if (a.longitud > 0) {
        archivo.write(reinterpret_cast<const char*>(a.datos),
                      static_cast<std::streamsize>(a.longitud));
    }

    archivo.close();
    return true;
}

Abstracto Leer_bits_archivo(const char* ruta) {
    Abstracto resultado;
    if (ruta == nullptr) return resultado;

    std::ifstream archivo(ruta, std::ios::binary | std::ios::ate);
    if (!archivo.is_open()) {
        std::println("Error: no se pudo abrir el archivo.");
        return resultado;
    }

    std::streampos tamano = archivo.tellg();
    if (tamano <= 0) return resultado;

    size_t n = static_cast<size_t>(tamano);
    resultado.reservar(n);
    resultado.longitud = n;

    archivo.seekg(0, std::ios::beg);
    archivo.read(reinterpret_cast<char*>(resultado.datos),
                 static_cast<std::streamsize>(n));

    if (archivo.gcount() != static_cast<std::streamsize>(n)) {
        resultado.longitud = 0;   // lectura incompleta
    }

    archivo.close();
    return resultado;
}