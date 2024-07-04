cmake_minimum_required(VERSION 3.28)
project(EDD_EX_002 C)

set(CMAKE_C_STANDARD 11)

add_executable(EDD_EX_002 codigo_huffman_main.c
        codigo_huffman/arbol_huffman.h
        codigo_huffman/arbol_huffman.c
        decodificador_binario.c
        huffman_test02.c
)
