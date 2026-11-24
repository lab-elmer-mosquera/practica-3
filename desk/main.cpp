#include <print>
#include <iostream>
#include <stdexcept>

#include "texto.h"
#include "consola.h"
#include "cifrado.h"
#include "archivo.h"
#include "lz78.h"
#include "rle.h"


using std::cout;
using std::endl;
using std::cin;
using std::print;
using std::println;


void imprimir_menu();

enum class Ctx_Eject {
    SALIR = 0,
    RLE_CONSOLA = 1,
    LZ78_CONSOLA = 2,
    RLE_ARCHIVO = 3,
    LZ78_ARCHIVO = 4
};

int main(){

    imprimir_menu();
    int opcion{};
    try {
        opcion = consola::input_int_rango(0, 4);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    Ctx_Eject control = static_cast<Ctx_Eject>(opcion);

    Texto entrada{};

    Abstracto comprimido{};
    Texto aux_comprimido_legible{};

    if (control == Ctx_Eject::SALIR){
        println("... saliendo");
        return 0;
    } else if (control == Ctx_Eject::LZ78_ARCHIVO || control ==  Ctx_Eject::RLE_ARCHIVO){
        entrada = archivo::input();
    } else if (control == Ctx_Eject::LZ78_CONSOLA || control ==  Ctx_Eject::RLE_CONSOLA){
        print("== INGRESE LA CADENA =="
              "\n>> ");
        entrada = consola::input();
    } else {
        println("... ERROR DESCONOCIDO");
        return 0;
    }

    consola::limpiar_pantalla();
    println("== {} SELECCIONADO ==", ( control == Ctx_Eject::LZ78_CONSOLA || control == Ctx_Eject::LZ78_ARCHIVO ) ? "LZ78" : "RLE");
    println("Texto de entrada: {}", entrada.datos);
    consola::pausar();



    if (control == Ctx_Eject::RLE_CONSOLA || control ==  Ctx_Eject::RLE_ARCHIVO) {
        RLE::comprimir(entrada, aux_comprimido_legible);
        comprimido = Texto_a_Abstracto(aux_comprimido_legible);
    }    else if (control == Ctx_Eject::LZ78_ARCHIVO || control ==  Ctx_Eject::LZ78_CONSOLA){
        LZ78::comprimir(entrada, comprimido);
        aux_comprimido_legible = LZ78::ComprimidoLz78_a_texto(comprimido);
    }


    println("  comprimido: {}", aux_comprimido_legible.datos);

    Abstracto comprimido_cifrado{};
    Abstracto comprimido_descifrado{};
    Texto aux_comprimido_descifrado_legible{};

    Texto descomprimido{};

    println("... cifrando comprimido");
    Cifrar(comprimido, comprimido_cifrado, 0x5A, 3);

    println("Cifrado");
    consola::println_hex(comprimido_cifrado);

    Descifrar(comprimido_cifrado, comprimido_descifrado, 0x5A, 3);


    if (control == Ctx_Eject::LZ78_ARCHIVO || control ==  Ctx_Eject::LZ78_CONSOLA){
        aux_comprimido_descifrado_legible = LZ78::ComprimidoLz78_a_texto(comprimido_descifrado);
        LZ78::descomprimir(comprimido_descifrado, descomprimido);
    } else if (control == Ctx_Eject::RLE_CONSOLA || control ==  Ctx_Eject::RLE_ARCHIVO) {
        aux_comprimido_descifrado_legible = Abstracto_a_Texto(comprimido_descifrado);
        RLE::descomprimir(aux_comprimido_legible, descomprimido);
    }

    println("  comprimido recuperado"
            "\n    {}", aux_comprimido_descifrado_legible.datos);
    println("original recuperado: {}", descomprimido.datos);

    entrada.limpiar();
    comprimido.limpiar();

    return 0;
}

void imprimir_menu(){
    print("\n              MENU PRINCIPAL"
          "\n"
          "\n  1. COMPRIMIR CON RLE DESDE CONSOLA"
          "\n  2. COMPRIMIR CON LZ78 DESDE CONSOLA"
          "\n"
          "\n  3. COMPRIMIR CON RLE DESDE ARCHIVO"
          "\n  4. COMPRIMIR CON LZ78 DESDE ARCHIVO"
          "\n"
          "\n  0.              SALIR");
}