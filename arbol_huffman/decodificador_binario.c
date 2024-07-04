#include <stdio.h>
#include <stdlib.h>


// Definición de la estructura Nodo para el árbol Huffman
typedef struct Nodo {
    char data;
    unsigned freq;
    struct Nodo *left, *right;
} Nodo;

// Función para crear un nuevo nodo del árbol Huffman
Nodo* crearNodo(char data, unsigned freq) {
    Nodo* nodo = (Nodo*) malloc(sizeof(Nodo));
    nodo->data = data;
    nodo->freq = freq;
    nodo->left = nodo->right = NULL;
    return nodo;
}

// Función para cargar el árbol Huffman desde un archivo binario
Nodo* cargarArbolHuffman(FILE* file) {
    char tipoNodo;
    if (fscanf(file, "%c", &tipoNodo) != 1 || tipoNodo == '0')
        return NULL;
    if (tipoNodo == 'L') {
        char data;
        fscanf(file, "%c", &data);
        return crearNodo(data, 0);
    } else if (tipoNodo == 'I') {
        Nodo* nodo = crearNodo('$', 0);
        nodo->left = cargarArbolHuffman(file);
        nodo->right = cargarArbolHuffman(file);
        return nodo;
    }

    return NULL;
}
// Función para decodificar la cadena comprimida usando el árbol Huffman
char* decodificar(Nodo* raiz, unsigned char* strComprimido, int longitud) {
    char* decodificado = (char*) malloc(longitud * 8 + 1);
    Nodo* current = raiz;
    int bitIndex = 0, decodeIndex = 0;

    for (int i = 0; i < longitud; i++) {
        for (int j = 7; j >= 0; j--) {
            int bit = (strComprimido[i] >> j) & 1;
            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }
            if (current->left == NULL && current->right == NULL) {
                decodificado[decodeIndex++] = current->data;
                current = raiz;
            }
        }
    }
    decodificado[decodeIndex] = '\0';
    return decodificado;
}

// Función principal
int main() {
    FILE* treeFile = fopen("arbol.bin", "rb");
    if (!treeFile) {
        printf("Error al abrir el archivo del árbol.\n");
        return 1;
    }
    Nodo* raiz = cargarArbolHuffman(treeFile);
    fclose(treeFile);

    FILE* comprimidoFile = fopen("comprimido.bin", "rb");
    if (!comprimidoFile) {
        printf("Error al abrir el archivo comprimido.\n");
        return 1;
    }

    fseek(comprimidoFile, 0, SEEK_END);
    long fileSize = ftell(comprimidoFile);
    fseek(comprimidoFile, 0, SEEK_SET);
    unsigned char* comprimido = (unsigned char*) malloc(fileSize);
    fread(comprimido, 1, fileSize, comprimidoFile);
    fclose(comprimidoFile);

    char* textoDecodificado = decodificar(raiz, comprimido, fileSize);
    printf("Texto Decodificado: %s\n", textoDecodificado);

    FILE* salidaFile = fopen("salida.txt", "w");
    if (!salidaFile) {
        printf("Error al abrir el archivo de salida.\n");
        free(textoDecodificado);
        return 1;
    }
    fprintf(salidaFile, "%s", textoDecodificado);
    fclose(salidaFile);

    free(comprimido);
    free(textoDecodificado);
    return 0;
}
