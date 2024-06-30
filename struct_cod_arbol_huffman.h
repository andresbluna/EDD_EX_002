#ifndef STRUCT_COD_ARBOL_HUFFMAN_H
#define STRUCT_COD_ARBOL_HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#DEFINE MAX_CHAR[256]


//definir la estructura del nodo del arbol
typedef struct {
    char cadena_texto;
    struct nodo_arbol_hoffman *izq, *der;
}Hoja;

//definir la estructura del nodo hoja

typedef struct {
    Hoja nodo_hoja;
    struct nodo_arbol_hoffman *izq, *der;
}nodo_arbol_hoffman;

#endif
