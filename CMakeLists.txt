cmake_minimum_required(VERSION 3.28)
project(EDD_EX_002 C)

set(CMAKE_C_STANDARD 11)

add_executable(EDD_EX_002 codigo_huffman_main.c
        codigo_huffman/struct_cod_arbol_huffman.h
        codigo_huffman/crear_nodos_hoja.h
        codigo_huffman/comparar_frecuencias.h
        codigo_huffman/unir_nodos.h
        codigo_huffman/construir_arbol.h
        codigo_huffman/generar_codigo_huffman.h
        romantico_viajero_main.c
)
