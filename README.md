# Práctica 3: Compresión y Cifrado de Texto

Aplicación modular de consola desarrollada en **C++23** para la compresión sin pérdida y encriptación reversible de texto, desarrollada para la **Práctica 3** (UdeA).

---

## Descripción

El sistema procesa texto mediante algoritmos de compresión sin pérdida (**RLE** y **LZ78**) y aplica una capa de protección mediante cifrado simétrico reversible a nivel de bits (rotación circular a la izquierda y operación XOR), permitiendo la recuperación íntegra y validación del texto original.

### Características
- **Compresión sin pérdida:**
  - **RLE (Run-Length Encoding):** Codificación de secuencias consecutivas repetidas.
  - **LZ78:** Codificación basada en diccionario dinámico con gestión manual de memoria.
- **Cifrado reversible a nivel de bits:** Rotación circular de bits ($n$ posiciones) y operación lógica `XOR` con clave de un byte ($K$).
- **Modos de entrada:** Soporte para entrada directa desde consola o mediante lectura/escritura de archivos (`desk/inout/`).
- **Ciclo completo:** Compresión $\rightarrow$ Cifrado $\rightarrow$ Descifrado $\rightarrow$ Descompresión con verificación de integridad.

---

## Estructura del Proyecto

```text
practica-3/
├── _dosc_practica_3/     # Documentación y visión del proyecto
├── desk/                 # Código fuente del programa
│   ├── CMakeLists.txt    # Configuración de compilación CMake
│   ├── main.cpp          # Menú principal y control de flujo
│   ├── rle.h / .cpp      # Módulo del algoritmo RLE
│   ├── lz78.h / .cpp     # Módulo del algoritmo LZ78
│   ├── cifrado.h / .cpp  # Módulo de rotación de bits y XOR
│   ├── texto.h / .cpp    # Estructuras y tipos de datos para cadenas
│   ├── archivo.h / .cpp  # Lectura y persistencia en archivos
│   ├── consola.h / .cpp  # Utilidades para interacción en terminal
│   └── inout/            # Archivos de texto de entrada y salida
└── README.md             # Documentación general
```

---

## Requisitos

- **Compilador C++:** Compatible con el estándar **C++23** (soporte para `<print>`, ej. GCC 14+ o Clang 17+).
- **CMake:** Versión 3.25 o superior.
- **Sistema Operativo:** Linux / macOS / Windows.

---

## Compilación y Ejecución

1. Ir a la carpeta del proyecto:
   ```bash
   cd desk
   ```

2. Configurar la compilación con CMake:
   ```bash
   cmake -B build
   ```

3. Compilar el ejecutable:
   ```bash
   cmake --build build
   ```

4. Ejecutar el programa:
   ```bash
   ./build/practica-3
   ```

---

## 🎮 Menú y Uso

Al ejecutar el programa, se presentará el menú interactivo:

```text
              MENU PRINCIPAL

  1. COMPRIMIR CON RLE DESDE CONSOLA
  2. COMPRIMIR CON LZ78 DESDE CONSOLA

  3. COMPRIMIR CON RLE DESDE ARCHIVO
  4. COMPRIMIR CON LZ78 DESDE ARCHIVO

  0.              SALIR
```

- Las opciones **1** y **2** solicitan el texto directamente por terminal.
- Las opciones **3** y **4** procesan los archivos ubicados en la carpeta `desk/inout/`.
