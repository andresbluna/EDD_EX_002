#ifndef UNIR_NODOS_H
#define UNIR_NODOS_H

#include <stdlib.h>
#include "struct_cod_arbol_huffman.h"
#include "codigo_huffman/struct_cod_arbol_huffman.h"
#include "codigo_huffman/crear_nodos_hoja.h"
#include "codigo_huffman/unir_nodos.h"

    nodo_arbol_hoffman* unirLosNodos(nodo_arbol_hoffman* izq, nodo_arbol_hoffman* der) {
    nodo_arbol_hoffman* nodoPadre = (nodo_arbol_hoffman*)malloc(sizeof(nodo_arbol_hoffman));
    if (nodoPadre == NULL) {
        printf("Error al asignar memoria");
        exit(1);


    nodoPadre->caracter = '\0';
    nodoPadre->frecuencia = izq->frecuencia + der->frecuencia;
    nodoPadre->izq = izq;
    nodoPadre->der = der;

    return nodoPadre;
}

#endif
