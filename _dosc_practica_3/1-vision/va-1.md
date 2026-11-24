---
tipo: vision-activa
id: va-1
version: 0.1
estado: Activa
descripcion: Aplicación modular de consola en C++ para la compresión sin pérdida y encriptación reversible de archivos de texto.
---

## Que es

Una aplicación modular en **C++** orientada al procesamiento, protección y verificación de integridad de datos textuales mediante la combinación secuencial de algoritmos de compresión sin pérdida (**RLE** y **LZ78**) y técnicas de encriptación reversible a nivel de bits (**rotación circular** y operación **XOR**), operando sobre archivos de texto con gestión controlada de errores.

## Que no es

- **No es** una herramienta que dependa de librerías externas de compresión o encriptación.
- **No es** una aplicación apoyada en contenedores estándar de la STL (como `std::vector`, `std::map` o `std::unordered_map`) para las estructuras principales de compresión.
- **No es** un sistema de compresión con pérdida (lossy) ni un esquema criptográfico asimétrico moderno.
- **No es** un programa monolítico; exige separación funcional entre sus módulos de procesamiento.

## Como funciona

El sistema opera en dos fases lógicas (transformación y recuperación) sobre un flujo de datos proveniente de archivos de texto:

1. **Lectura y selección:** Carga el texto original desde un archivo y permite elegir el algoritmo de compresión a aplicar (`RLE` o `LZ78`).
2. **Compresión sin pérdida:**
   - **RLE:** Reduce secuencias de símbolos consecutivos repetidos mediante pares de conteo y carácter.
   - **LZ78:** Construye un diccionario de forma dinámica en memoria para codificar subcadenas repetidas en cualquier posición como pares `(índice, carácter)`.
3. **Encriptación reversible:** Toma los datos resultantes de la compresión y aplica por cada byte una rotación de bits hacia la izquierda de $n$ posiciones ($0 < n < 8$), seguida de una operación lógica **XOR** con una clave $K$ de un byte.
4. **Recuperación (Inversa):** Revierte el proceso aplicando nuevamente la operación **XOR** con la misma clave $K$, invierte la rotación de bits y ejecuta la descompresión correspondiente reconstruyendo los datos.
5. **Validación:** Escribe el resultado recuperado en un archivo de salida y verifica que coincida exactamente con el texto original.

## Reglas generales

- **Restricción de `std::string`:** Su uso está permitido única y exclusivamente en la implementación del módulo **RLE**.
- **Gestión manual de memoria:** El diccionario del algoritmo **LZ78** debe implementarse mediante arreglos dinámicos y memoria dinámica justificada, sin almacenar frases completas con `std::string`.
- **Modularidad:** Cada proceso (RLE, LZ78, encriptación/desencriptación e integración) debe desarrollarse y probarse como un módulo independiente organizado mediante funciones antes de integrarse.
- **Tolerancia a fallos:** El sistema debe incorporar manejo de excepciones para interceptar errores en tiempo de ejecución (como fallos de lectura/escritura de archivos o indeterminaciones) sin cierres abruptos.
