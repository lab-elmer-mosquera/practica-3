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

// #include "consola.h"
// #include "texto.h"
// #include <print>

// int main() {
//     consola::limpiar_pantalla();

//     std::println("=== Prueba de consola ===");

//     std::println("\n1) input()");
//     std::print("Escribe algo: ");
//     Texto t = consola::input();
//     std::println("Recibido: '{}' (longitud {}).", t.datos, t.longitud);

//     std::println("\n2) pausar()");
//     consola::pausar();

//     std::println("\n3) limpiar_buffer_entrada()");
//     std::print("Escribe un numero: ");
//     int x{};
//     std::cin >> x;
//     std::println("Leido: {}", x);
//     consola::limpiar_buffer_entrada();
//     std::println("Buffer limpio. Ahora input() no deberia saltarse:");
//     std::print("Escribe otra cosa: ");
//     Texto u = consola::input();
//     std::println("Recibido: '{}'.", u.datos);

//     std::println("\n4) limpiar_pantalla()");
//     consola::pausar();
//     consola::limpiar_pantalla();
//     std::println("Pantalla limpiada. Fin.");

//     return 0;
// }