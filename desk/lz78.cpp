#include "lz78.h"
#include <cstdint>
#include <string>
#include <format>
#include <stdexcept>

// ---------- Diccionario ----------

struct Par {
    unsigned int padre;
    unsigned char caracter;
};

struct DiccionarioLz78 {
    size_t longitud = 0;
    size_t capacidad = 32;
    Par* datos = new Par[capacidad];

    DiccionarioLz78() = default;

    DiccionarioLz78(const DiccionarioLz78&) = delete;

    DiccionarioLz78& operator=(const DiccionarioLz78&) = delete;

    ~DiccionarioLz78() {
        delete[] datos;
    }

    void agregar(Par par) {
        if (longitud == capacidad) {
            capacidad *= 2;

            Par* nuevo = new Par[capacidad];

            for (size_t i = 0; i < longitud; i++) {
                nuevo[i] = datos[i];
            }

            delete[] datos;
            datos = nuevo;
        }

        datos[longitud++] = par;
    }

    unsigned int buscar(unsigned int padre, unsigned char c) {
        for (size_t i = 0; i < longitud; i++) {
            if (datos[i].padre == padre &&
                datos[i].caracter == c) {
                return i + 1;
            }
        }

        return 0;
    }
};

namespace LZ78 {

    // ---------- Funciones auxiliares ----------

    static void escribir_indice(Abstracto& salida, uint32_t indice) {
        for (int i = 0; i < 4; i++) {
            salida.agregar_byte(indice % 256);
            indice /= 256;
        }
    }

    static uint32_t leer_indice(const Abstracto& entrada, size_t& pos) {
        if (entrada.longitud - pos < 4) {
            throw std::runtime_error("Indice incompleto");
        }

        uint32_t indice = 0;

        for (int i = 0; i < 4; i++) {
            indice |= uint32_t(entrada.datos[pos++]) << (i * 8);
        }

        return indice;
    }

    static void emitir_cadena(
        const DiccionarioLz78& diccionario,
        Texto& salida,
        unsigned int indice
        ) {
        if (indice == 0) {
            return;
        }

        if (indice > diccionario.longitud) {
            throw std::runtime_error("Indice invalido");
        }

        Par par = diccionario.datos[indice - 1];

        emitir_cadena(diccionario, salida, par.padre);

        salida.agregar_caracter(char(par.caracter));
    }

    // ---------- Compresion ----------

    void comprimir(const Texto& entrada, Abstracto& salida) {
        salida.limpiar();

        DiccionarioLz78 diccionario;
        unsigned int padre = 0;

        for (size_t i = 0; i < entrada.longitud; i++) {
            unsigned char c = entrada.datos[i];

            unsigned int indice = diccionario.buscar(padre, c);

            if (indice != 0) {
                padre = indice;
            } else {
                salida.agregar_byte(1);
                escribir_indice(salida, padre);
                salida.agregar_byte(c);

                diccionario.agregar({padre, c});

                padre = 0;
            }
        }

        if (padre != 0) {
            salida.agregar_byte(2);
            escribir_indice(salida, padre);
        }

        salida.agregar_byte(0);
    }

    // ---------- Descompresion ----------

    void descomprimir(const Abstracto& entrada, Texto& salida) {
        salida.limpiar();

        DiccionarioLz78 diccionario;
        size_t pos = 0;

        while (pos < entrada.longitud) {
            unsigned char tipo = entrada.datos[pos++];

            if (tipo == 0) {
                break;
            }

            if (tipo != 1 && tipo != 2) {
                throw std::runtime_error("Tipo invalido");
            }

            unsigned int padre = leer_indice(entrada, pos);

            emitir_cadena(diccionario, salida, padre);

            if (tipo == 1) {
                if (pos >= entrada.longitud) {
                    throw std::runtime_error("Caracter faltante");
                }

                unsigned char c = entrada.datos[pos++];

                salida.agregar_caracter(char(c));

                diccionario.agregar({padre, c});
            }
        }
    }

    // ---------- Representacion textual ----------

    static void agregar_texto(Texto& salida, const std::string& texto) {
        for (char c : texto) {
            salida.agregar_caracter(c);
        }
    }

    static void escribir_caracter(Texto& salida, unsigned char c) {
        if (c == ',' || c == '(' || c == ')' || c == '\\' || c == '-') {
            salida.agregar_caracter('\\');
            salida.agregar_caracter(char(c));
        } else if (c >= 32 && c < 127) {
            salida.agregar_caracter(char(c));
        } else {
            agregar_texto(salida, std::format("\\x{:02X}", unsigned(c)));
        }
    }

    Texto ComprimidoLz78_a_texto(const Abstracto& comp) {
        Texto salida;
        size_t pos = 0;

        while (pos < comp.longitud) {
            unsigned char tipo = comp.datos[pos++];

            if (tipo == 0) {
                break;
            }

            if (tipo != 1 && tipo != 2) {
                throw std::runtime_error("Tipo invalido");
            }

            unsigned int padre = leer_indice(comp, pos);

            if (salida.longitud > 0) {
                salida.agregar_caracter(',');
            }

            salida.agregar_caracter('(');
            agregar_texto(salida, std::to_string(padre));
            salida.agregar_caracter(',');

            if (tipo == 1) {
                if (pos >= comp.longitud) {
                    throw std::runtime_error("Caracter faltante");
                }

                escribir_caracter(salida, comp.datos[pos++]);
            } else {
                salida.agregar_caracter('-');
            }

            salida.agregar_caracter(')');
        }

        return salida;
    }

}