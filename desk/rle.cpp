#include "rle.h"
#include <cstddef>
#include <string>
#include <iostream>
#include <print>

using std::cout;
using std::endl;
using std::println;

using std::string;

Texto string_to_texto(const std::string& str) {
    Texto r;
    if (str.empty()) return r;
    r.reservar(str.size() + 1);
    for (size_t i = 0; i < str.size(); ++i) {
        r.datos[i] = str[i];
    }
    r.longitud = str.size();
    r.datos[r.longitud] = '\0';
    return r;
}

namespace RLE {

    void comprimir(const Texto& entrada, Texto& salida) {
        if (&entrada == &salida) return;
        salida.limpiar();

        size_t i = 0;
        while (i < entrada.longitud) {
            char c = entrada.datos[i];
            size_t reps = 1;
            while (i + reps < entrada.longitud && entrada.datos[i + reps] == c) {
                ++reps;
            }
            salida.agregar_caracter(c);
            for (char d : std::to_string(reps)) {
                salida.agregar_caracter(d);
            }
            i += reps;
        }
    }

    void descomprimir(const Texto& comprimido, Texto& salida) {
        if (&comprimido == &salida) return;
        salida.limpiar();

        size_t i = 0;
        while (i < comprimido.longitud) {
            char c = comprimido.datos[i++];
            size_t reps = 0;
            bool hay_digitos = false;
            while (i < comprimido.longitud
                   && comprimido.datos[i] >= '0'
                   && comprimido.datos[i] <= '9') {
                size_t d = static_cast<size_t>(comprimido.datos[i] - '0');
                if (reps > (SIZE_MAX - d) / 10) return;   // overflow
                reps = reps * 10 + d;
                hay_digitos = true;
                ++i;
            }
            if (!hay_digitos || reps == 0) return;        // formato inválido
            salida.reservar(salida.longitud + reps);
            for (size_t j = 0; j < reps; ++j) {
                salida.agregar_caracter(c);
            }
        }
    }
}