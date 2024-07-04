cmake_minimum_required(VERSION 3.28)
project(EDD_EX_002 C)

set(CMAKE_C_STANDARD 11)

add_executable(EDD_EX_002 codigo_huffman_main.c
        romantico_viajero_main.c
        codigo_huffman/huffman_tree.h
        codigo_huffman/huffman_tree.c
        decodificador_binario.c
        sample_huffman.c
)
