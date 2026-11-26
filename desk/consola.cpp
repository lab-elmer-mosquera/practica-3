#include "consola.h"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <print>

using std::cin;
using std::cout;
using std::endl;


static bool es_digito(char c) { return c >= '0' && c <= '9'; }

bool texto_a_int(const Texto& t, int& valor) {
    // Ignorar '\r' (Windows) y espacios al iniciofin
    size_t ini = 0, fin = t.longitud;
    while (ini < fin && (t.datos[ini] == ' ' || t.datos[ini] == '\t' || t.datos[ini] == '\r')) ++ini;
    while (fin > ini && (t.datos[fin-1] == ' ' || t.datos[fin-1] == '\t' || t.datos[fin-1] == '\r')) --fin;

    if (ini == fin) return false;                 // línea vacía

    long long acc = 0;
    for (size_t i = ini; i < fin; ++i) {
        char c = t.datos[i];
        if (!es_digito(c)) return false;          // hay algo que no es dígito
        acc = acc * 10 + (c - '0');
        if (acc > 2147483647LL) return false;     // no cabe en int
    }
    valor = (int)acc;
    return true;
}

namespace consola {

void limpiar_buffer_entrada() {
    // std::cin.clear();
    // if (std::cin.rdbuf()->in_avail() > 0) {
    //     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // }
    std::cin.clear();
    int c;
    while ((c = std::cin.get()) != '\n' && c != EOF) {
        // vaciar hasta el salto de línea
    }
}

void pausar() {
    std::cout << "Presione Enter para continuar...";
    std::cin.get();
}

void limpiar_pantalla() {
    std::cout << "\033[2J\033[1;1H";
}

Texto input() {
    Texto t;
    char c;
    while (std::cin.get(c)) {
        if (c == '\n') break;
        t.agregar_caracter(c);
    }
    return t;
}

int input_int_rango(int min, int max) {
    if (min > max)
        throw std::invalid_argument("input_int_rango: min > max");

    for (int intento = 0; intento < 5; ++intento) {
        std::cout << "\n>> ";
        Texto linea = input();
        int valor;
        if (texto_a_int(linea, valor) && valor >= min && valor <= max)
            return valor;

        std::cout << "entrada invalida"
                     "\nINTENTO:"
                  << intento+1 << "/5" << std::endl;
    }

    std::cout << "Limite de intentos alcanzado" << std::endl;
    exit(EXIT_FAILURE);
}

void println_hex(Abstracto& base){
    cout << "  hex:  ";
    for (size_t i = 0; i < base.longitud; ++i) {
        std::print("{:02X} ", base.datos[i]);
    }
    cout<<endl;
}

}