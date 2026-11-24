#include "cifrado.h"

unsigned char rotar_izquierda(unsigned char b, int n) {
    n &= 7;
    if (n == 0) return b;
    return (b << n) | (b >> (8 - n));
}

unsigned char rotar_derecha(unsigned char b, int n) {
    n &= 7;
    if (n == 0) return b;
    return (b >> n) | (b << (8 - n));
}

void Cifrar(const Abstracto& entrada, Abstracto& salida,
            unsigned char clave, int rotaciones) {
    if (&entrada == &salida) return;

    salida.longitud = 0;
    if (entrada.datos == nullptr || entrada.longitud == 0) return;

    salida.reservar(entrada.longitud);
    salida.longitud = entrada.longitud;

    for (size_t i = 0; i < entrada.longitud; ++i) {
        unsigned char b = entrada.datos[i];
        b = rotar_izquierda(b, rotaciones);
        b ^= clave;
        salida.datos[i] = b;
    }
}

void Descifrar(const Abstracto& entrada, Abstracto& salida,
               unsigned char clave, int rotaciones) {
    if (&entrada == &salida) return;

    salida.longitud = 0;
    if (entrada.datos == nullptr || entrada.longitud == 0) return;

    salida.reservar(entrada.longitud);
    salida.longitud = entrada.longitud;

    for (size_t i = 0; i < entrada.longitud; ++i) {
        unsigned char b = entrada.datos[i];
        b ^= clave;
        b = rotar_derecha(b, rotaciones);
        salida.datos[i] = b;
    }
}