#pragma once
#include <string>
#include <string_view>
#include "texto.h"

namespace RLE {
    void comprimir(const Texto& entrada, Texto& salida);
    void descomprimir(const Texto& comprimido, Texto& salida);
}

