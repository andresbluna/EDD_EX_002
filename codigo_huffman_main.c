#include <stdio.h>
#include <stdlib.h>
#include "struct_cod_arbol_huffman.h"
#include "crear_nodos_hoja.h"
#include "unir_nodos.h"
#include "generar_codigo_huffman.h"

int main() {
    char simbolos[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    int frecuencias[] = {5, 9, 12, 13, 16, 45};
    int numSimbolos = sizeof(simbolos) / sizeof(simbolos[0]);

    nodo_arbol_hoffman* colaPrioridad[numSimbolos];

    for (int i = 0; i < numSimbolos; ++i) {
        colaPrioridad[i] = crearNodoHoja(simbolos[i], frecuencias[i]);
    }


    nodo_arbol_hoffman* raiz = NULL;

    while (/* condición basada en el tamaño de la cola */) {
        nodo_arbol_hoffman* izq = /* extraer mínimo de la cola */;
        nodo_arbol_hoffman* der = /* extraer mínimo de la cola */;
        nodo_arbol_hoffman* padre = unirLosNodos(izq, der);

    }

    generarCodigoHuffman(raiz);



    return 0;
}
