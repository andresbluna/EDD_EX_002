#include "struct_cod_arbol_huffman.h"

nodo_arbol_hoffman* crearArbol(int numSimbolos, char simbolos[], int frecuencias[]) {

    ColaPrioridad *cola = crearColaPrioridad();


    for (int i = 0; i < numSimbolos; ++i) {
        nodo_arbol_hoffman *nodo = malloc(sizeof(nodo_arbol_hoffman));
        if (nodo == NULL) {
            printf("No fue posible reservar memoria");
            exit(1);
        }
        nodo->caracter = simbolos[i];
        nodo->frecuencia = frecuencias[i];
        nodo->izq = nodo->der = NULL;
        insertarEnColaPrioridad(cola, nodo);
    }


    while (numElementosEnCola(cola) > 1) {
        nodo_arbol_hoffman *izq = extraerMinimo(cola);
        nodo_arbol_hoffman *der = extraerMinimo(cola);

        nodo_arbol_hoffman *nuevoNodo = malloc(sizeof(nodo_arbol_hoffman));
        if (nuevoNodo == NULL) {
            printf("No fue posible reservar memoria");
            exit(1);

            nuevoNodo->caracter = '\0';
            nuevoNodo->frecuencia = izq->frecuencia + der->frecuencia;
            nuevoNodo->izq = izq;
            nuevoNodo->der = der;

            insertarEnColaPrioridad(cola, nuevoNodo);
        }


        return extraerMinimo(cola);
    }
}
