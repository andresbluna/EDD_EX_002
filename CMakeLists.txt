cmake_minimum_required(VERSION 3.28)
project(EDD_EX_002 C)

set(CMAKE_C_STANDARD 11)

add_executable(EDD_EX_002 main.c
        struct_cod_arbol_huffman.h
        crear_nodos_hoja.h
        comparar_frecuencias.h
        unir_nodos.h
        construir_arbol.h
        generar_codigo_huffman.h
)
