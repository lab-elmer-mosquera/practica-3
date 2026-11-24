#include "texto.h"

Abstracto Texto_a_Abstracto(const Texto& t) {
    Abstracto a;
    if (t.longitud == 0) return a;

    // buffer exacto, sin reallocs en el bucle
    delete[] a.datos;
    a.capacidad = t.longitud;
    a.datos     = new unsigned char[a.capacidad];

    for (size_t i = 0; i < t.longitud; ++i) {
        a.datos[i] = static_cast<unsigned char>(t.datos[i]);
    }
    a.longitud = t.longitud;
    return a;
}

Texto Abstracto_a_Texto(const Abstracto& a) {
    Texto t;
    if (a.longitud == 0) return t;

    delete[] t.datos;
    t.capacidad = a.longitud + 1;
    t.datos = new char[t.capacidad];

    for (size_t i = 0; i < a.longitud; ++i) {
        t.datos[i] = static_cast<char>(a.datos[i]);
    }
    t.longitud = a.longitud;
    t.datos[t.longitud] = '\0';
    return t;
}