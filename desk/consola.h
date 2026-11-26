#pragma once
#include "texto.h"

namespace consola {

Texto input();
    int input_int_rango(int min, int max);
    void  limpiar_buffer_entrada();
    void  pausar();
    void  limpiar_pantalla();
    void println_hex(Abstracto& base);
}