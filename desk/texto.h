#pragma once
#include <cstddef>

struct Texto {
    char* datos{nullptr};
    size_t longitud{};
    size_t capacidad{};

    Texto() {
        capacidad = 32;
        longitud = 0;
        datos = new char[capacidad];
        datos[0] = '\0';
    }

    Texto(const Texto&)            = delete;
    Texto& operator=(const Texto&) = delete;

    Texto(Texto&& o) noexcept
        : datos(o.datos), longitud(o.longitud), capacidad(o.capacidad) {
        o.datos = nullptr; o.longitud = 0; o.capacidad = 0;
    }

    Texto& operator=(Texto&& o) noexcept {
        if (this != &o) {
            delete[] datos;
            datos = o.datos; longitud = o.longitud; capacidad = o.capacidad;
            o.datos = nullptr; o.longitud = 0; o.capacidad = 0;
        }
        return *this;
    }

    ~Texto() { delete[] datos; }

    void limpiar() {
        longitud = 0;
        datos[0] = '\0';
    }

    void reservar(size_t n) {
        if (n <= capacidad) return;
        size_t nueva = capacidad;
        while (nueva < n) nueva *= 2;
        char* nuevo = new char[nueva];
        for (size_t i = 0; i < longitud; ++i) nuevo[i] = datos[i];
        delete[] datos;
        datos = nuevo;
        capacidad = nueva;
    }

    void agregar_caracter(char c) {
        reservar(longitud + 2);
        datos[longitud++] = c;
        datos[longitud] = '\0';
    }
};

struct Abstracto {
    unsigned char* datos{nullptr};
    size_t longitud{};
    size_t capacidad{};

    Abstracto() {
        capacidad = 32;
        longitud = 0;
        datos = new unsigned char[capacidad];
    }

    Abstracto(const Abstracto&) = delete;
    Abstracto& operator=(const Abstracto&) = delete;

    Abstracto(Abstracto&& o) noexcept
        : datos(o.datos), longitud(o.longitud), capacidad(o.capacidad) {
        o.datos = nullptr; o.longitud = 0; o.capacidad = 0;
    }
    Abstracto& operator=(Abstracto&& o) noexcept {
        if (this != &o) {
            delete[] datos;
            datos = o.datos; longitud = o.longitud; capacidad = o.capacidad;
            o.datos = nullptr; o.longitud = 0; o.capacidad = 0;
        }
        return *this;
    }

    ~Abstracto() { delete[] datos; }

    void limpiar() {
        longitud = 0;
    }

    void reservar(size_t n) {
        if (n <= capacidad) return;
        size_t nueva = capacidad;
        while (nueva < n) nueva *= 2;
        unsigned char* nuevo = new unsigned char[nueva];
        for (size_t i = 0; i < longitud; ++i) nuevo[i] = datos[i];
        delete[] datos;
        datos = nuevo;
        capacidad = nueva;
    }

    void agregar_byte(unsigned char b) {
        reservar(longitud + 1);
        datos[longitud++] = b;
    }
};


Abstracto Texto_a_Abstracto(const Texto& t);

Texto Abstracto_a_Texto(const Abstracto& a);