#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct_cod_arbol_huffman.h"
#include "crear_nodos_hoja.h"
#include "unir_nodos.h"
#include "generar_codigo_huffman.h"
#include "construir_arbol.h"


void contarFrecuencias(char* texto, int frecuencias[256]) {
    memset(frecuencias, 0, sizeof(int) * 256);
    for (int i = 0; texto[i]!= '\0'; ++i) {
        frecuencias[(unsigned char)texto[i]]++;
    }
}

int main() {
    FILE* file = fopen("salmo23.txt", "r");
    if (!file) {
        perror("No se pudo abrir el archivo");
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* texto = malloc(length + 1);
    if (!texto) {
        fclose(file);
        perror("No se pudo asignar memoria");
        return EXIT_FAILURE;
    }

    fread(texto, 1, length, file);
    fclose(file);
    texto[length] = '\0';

    int frecuencias[256] = {0};
    contarFrecuencias(texto, frecuencias);


    nodo_arbol_hoffman** colaPrioridad = calloc(256, sizeof(nodo_arbol_hoffman*));
    int numCaracteres = 0;

    for (int i = 0; i < 256; ++i) {
        if (frecuencias[i] > 0) {
            colaPrioridad[numCaracteres++] = crearNodoHoja((char)i, frecuencias[i]);
        }
    }

    nodo_arbol_hoffman* raiz = construir_arbol(colaPrioridad, numCaracteres);

    generarCodigoHuffman(raiz);

    free(texto);
    free(colaPrioridad);

    return 0;
}
