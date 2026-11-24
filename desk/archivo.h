#pragma once
#include "texto.h"

Texto Leer_archivo(const char* ruta);
bool Escribir_archivo(const char* ruta, const Texto& t);
bool Agregar_archivo(const char* ruta, const Texto& t);

bool Escribir_bits_archivo(const char* ruta, const Abstracto& a);
bool Agregar_bits_archivo(const char* ruta, const Abstracto& a);
Abstracto Leer_bits_archivo(const char* ruta);


namespace archivo {
    Texto input();
    bool output_comprimido_rle(Texto& t);
    bool output_comprimido_lz78(Texto& t);
}