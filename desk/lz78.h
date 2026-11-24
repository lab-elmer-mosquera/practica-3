#pragma once
#include "texto.h"

struct Entrada;

namespace LZ78 {
    void comprimir(const Texto& entrada, Abstracto& salida);
    void descomprimir(const Abstracto& entrada, Texto& salida);
    Texto ComprimidoLz78_a_texto(const Abstracto& comp);
}

