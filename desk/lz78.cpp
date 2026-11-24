#include "lz78.h"
#include <print>

using std::println;

// ---------- estructuras internas ----------
struct Par {
    unsigned int  padre;
    unsigned char caracter;
};

struct DiccionarioLz78 {
    Par* datos{nullptr};
    size_t longitud{};
    size_t capacidad{};

    DiccionarioLz78() {
        capacidad = 32;
        longitud  = 0;
        datos = new Par[capacidad];
    }
    DiccionarioLz78(const DiccionarioLz78&) = delete;
    DiccionarioLz78& operator=(const DiccionarioLz78&) = delete;

    DiccionarioLz78(DiccionarioLz78&& o) noexcept
        : datos(o.datos), longitud(o.longitud), capacidad(o.capacidad) {
        o.datos = nullptr; o.longitud = 0; o.capacidad = 0;
    }
    DiccionarioLz78& operator=(DiccionarioLz78&& o) noexcept {
        if (this != &o) {
            delete[] datos;
            datos = o.datos; longitud = o.longitud; capacidad = o.capacidad;
            o.datos = nullptr; o.longitud = 0; o.capacidad = 0;
        }
        return *this;
    }
    ~DiccionarioLz78() { delete[] datos; }

    void limpiar() { longitud = 0; }

    void reservar(size_t n) {
        if (n <= capacidad) return;
        while (capacidad < n) capacidad *= 2;
        Par* nuevo = new Par[capacidad];
        for (size_t i = 0; i < longitud; ++i) nuevo[i] = datos[i];
        delete[] datos;
        datos = nuevo;
    }

    void agregar_par(Par par) {
        reservar(longitud + 1);
        datos[longitud++] = par;
    }
};

namespace LZ78 {

// helpers internos

static void escribir_varint(Abstracto& out, unsigned int v) {
    while (v >= 0x80) {
        out.agregar_byte((unsigned char)((v & 0x7F) | 0x80));
        v >>= 7;
    }
    out.agregar_byte((unsigned char)v);
}

static unsigned int leer_varint(const Abstracto& in, size_t& pos) {
    unsigned int v = 0; int shift = 0;
    while (pos < in.longitud) {
        unsigned char b = in.datos[pos++];
        v |= (unsigned int)(b & 0x7F) << shift;
        if (!(b & 0x80)) break;
        shift += 7;
    }
    return v;
}

static int buscar_hijo(const DiccionarioLz78& dict, unsigned int padre, unsigned char c) {
    for (size_t i = 0; i < dict.longitud; ++i)
        if (dict.datos[i].padre == padre && dict.datos[i].caracter == c)
            return (int)(i + 1);
    return 0;
}

static void emitir_cadena(const DiccionarioLz78& dict, Texto& out, unsigned int id) {
    if (id == 0) return;
    emitir_cadena(dict, out, dict.datos[id - 1].padre);
    out.agregar_caracter((char)dict.datos[id - 1].caracter);
}

// API publica

void comprimir(const Texto& entrada, Abstracto& salida) {
    salida.limpiar();
    DiccionarioLz78 dict;

    size_t pos = 0;
    while (pos < entrada.longitud) {
        unsigned int nodo = 0;
        size_t p = pos;

        while (p < entrada.longitud) {
            unsigned char c = (unsigned char)entrada.datos[p];
            int hijo = buscar_hijo(dict, nodo, c);
            if (hijo == 0) break;
            nodo = (unsigned int)hijo;
            ++p;
        }

        if (p < entrada.longitud) {
            unsigned char c = (unsigned char)entrada.datos[p];
            salida.agregar_byte(1);
            escribir_varint(salida, nodo);
            salida.agregar_byte(c);
            dict.agregar_par({nodo, c});
            pos = p + 1;
        } else {
            salida.agregar_byte(2);
            escribir_varint(salida, nodo);
            pos = p;
        }
    }
    salida.agregar_byte(0);
}

void descomprimir(const Abstracto& entrada, Texto& salida) {
    salida.limpiar();
    DiccionarioLz78 dict;

    size_t pos = 0;
    while (pos < entrada.longitud) {
        unsigned char tipo = entrada.datos[pos++];
        if (tipo == 0) break;

        unsigned int padre = leer_varint(entrada, pos);

        if (tipo == 1) {
            if (pos >= entrada.longitud) break;
            unsigned char c = entrada.datos[pos++];
            emitir_cadena(dict, salida, padre);
            salida.agregar_caracter((char)c);
            dict.agregar_par({padre, c});
        } else if (tipo == 2) {
            emitir_cadena(dict, salida, padre);
        }
    }
}

Texto ComprimidoLz78_a_texto(const Abstracto& comp) {
    // --- helpers locales (sólo aquí se usan) ---
    auto escribir_uint = [](Texto& t, unsigned int v) {
        char buf[16]; int n = 0;
        if (v == 0) buf[n++] = '0';
        else while (v) { buf[n++] = char('0' + v % 10); v /= 10; }
        while (n) t.agregar_caracter(buf[--n]);
    };

    auto escribir_caracter = [](Texto& t, unsigned char c) {
        static const char hex[] = "0123456789ABCDEF";
        switch (c) {
        case ',':  t.agregar_caracter('\\'); t.agregar_caracter(',');  return;
        case '(':  t.agregar_caracter('\\'); t.agregar_caracter('(');  return;
        case ')':  t.agregar_caracter('\\'); t.agregar_caracter(')');  return;
        case '\\': t.agregar_caracter('\\'); t.agregar_caracter('\\'); return;
        default:
            if (c >= 32 && c < 127) t.agregar_caracter((char)c);
            else {
                t.agregar_caracter('\\');
                t.agregar_caracter('x');
                t.agregar_caracter(hex[c >> 4]);
                t.agregar_caracter(hex[c & 0xF]);
            }
        }
    };

    // --- recorrido del flujo binario ---
    Texto out;
    size_t pos = 0;
    bool primero = true;

    while (pos < comp.longitud) {
        unsigned char tipo = comp.datos[pos++];
        if (tipo == 0) break;

        unsigned int padre = leer_varint(comp, pos);

        if (!primero) out.agregar_caracter(',');
        primero = false;

        out.agregar_caracter('(');
        escribir_uint(out, padre);
        out.agregar_caracter(',');

        if (tipo == 1) {
            if (pos >= comp.longitud) break;
            escribir_caracter(out, comp.datos[pos++]);
        } else if (tipo == 2) {
            out.agregar_caracter('-');
            // out.agregar_caracter('E');
            // out.agregar_caracter('O');
            // out.agregar_caracter('F');
        } else {
            out.agregar_caracter('?');
        }
        out.agregar_caracter(')');
    }

    return out;
}

}