#pragma once
#include "texto.h"

void Cifrar(const Abstracto& entrada, Abstracto& salida, unsigned char clave, int rotaciones);
void Descifrar(const Abstracto& entrada, Abstracto& salida, unsigned char clave, int rotaciones);