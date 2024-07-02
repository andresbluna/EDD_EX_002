#ifndef STRUCT_COD_ARBOL_HUFFMAN_H
#define STRUCT_COD_ARBOL_HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codigo_huffman/struct_cod_arbol_huffman.h"
#include "codigo_huffman/crear_nodos_hoja.h"
#include "codigo_huffman/unir_nodos.h"

#DEFINE MAX_CHAR[256]



typedef struct {
    char cadena_texto;
    struct nodo_arbol_hoffman *izq, *der;
}Hoja;


typedef struct {
    Hoja nodo_hoja;
    struct nodo_arbol_hoffman *izq, *der;
}nodo_arbol_hoffman;

#endif
