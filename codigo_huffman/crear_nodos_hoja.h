#ifndef CREAR_NODOS_HOJA_H
#define CREAR_NODOS_HOJA_H

#include <stdio.h>
#include <stdlib.h>
#include "struct_cod_arbol_huffman.h"
#include "codigo_huffman/struct_cod_arbol_huffman.h"
#include "codigo_huffman/crear_nodos_hoja.h"
#include "codigo_huffman/unir_nodos.h"

nodo_arbol_hoffman* crearNodoHoja(char simbolo, int frecuencia) {
    nodo_arbol_hoffman *nodo = (nodo_arbol_hoffman*)malloc(sizeof(nodo_arbol_hoffman));
    if (nodo == NULL) {
        printf("Error al asignar memoria para el nodo");
        exit(1);
    }

    nodo->caracter = simbolo;
    nodo->frecuencia = frecuencia;
    nodo->izq = NULL;
    nodo->der = NULL;

    return nodo;
}

#endif
