#include <stdio.h>
#include <stdlib.h>

#define MAX_TREE_HT 100
#define MAX_CHARS 256

typedef struct Grupo_Nodo_Menor {
    char data;
    unsigned freq;
    struct Grupo_Nodo_Menor *left, *right;
} Grupo_Nodo_Menor;

Grupo_Nodo_Menor* nuevo_nodo(char data, unsigned freq) {
    Grupo_Nodo_Menor* temp = (Grupo_Nodo_Menor*) malloc(sizeof(Grupo_Nodo_Menor));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

int es_hoja(Grupo_Nodo_Menor* root) {
    return !(root->left) && !(root->right);
}

Grupo_Nodo_Menor* loadHuffmanTree(FILE* file) {
    char c;
    if (fscanf(file, "%c", &c) == EOF || c == '0') {
        return NULL;
    }

    if (fscanf(file, "%c", &c) == EOF) {
        return NULL;
    }

    if (c == '1') {
        char data;
        fscanf(file, "%c", &data);
        return nuevo_nodo(data, 0);
    } else {
        Grupo_Nodo_Menor* node = nuevo_nodo('$', 0);
        node->left = loadHuffmanTree(file);
        node->right = loadHuffmanTree(file);
        return node;
    }
}

char* decomprimir_texto(const char* archivo_comprimidoText, Grupo_Nodo_Menor* root, int archivo_comprimidoSize) {
    char* dearchivo_comprimidoText = (char*)malloc(archivo_comprimidoSize * 8); // Worst case scenario
    int index = 0;
    Grupo_Nodo_Menor* current = root;

    for (int i = 0; i < archivo_comprimidoSize; i++) {
        char byte = archivo_comprimidoText[i];
        for (int j = 7; j >= 0; j--) {
            int bit = (byte >> j) & 1;
            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }

            if (es_hoja(current)) {
                dearchivo_comprimidoText[index++] = current->data;
                current = root;
            }
        }
    }

    dearchivo_comprimidoText[index] = '\0';
    return dearchivo_comprimidoText;
}

void freeHuffmanTree(Grupo_Nodo_Menor* root) {
    if (root == NULL) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

int main() {
    const char* archivo_comprimidoFilename = "archivo_comprimido.bin";
    const char* treeFilename = "arbol_huffman.bin";
    const char* outputFilename = "archivo_descomprimido.txt";


    FILE* archivo_comprimidoFile = fopen(archivo_comprimidoFilename, "rb");
    if (archivo_comprimidoFile == NULL) {
        printf("Error al abrir el archivo comprimido.\n");
        return 1;
    }

    fseek(archivo_comprimidoFile, 0, SEEK_END);
    int archivo_comprimidoSize = ftell(archivo_comprimidoFile);
    fseek(archivo_comprimidoFile, 0, SEEK_SET);

    char* archivo_comprimidoText = (char*)malloc(archivo_comprimidoSize);
    fread(archivo_comprimidoText, sizeof(char), archivo_comprimidoSize, archivo_comprimidoFile);
    fclose(archivo_comprimidoFile);

    FILE* treeFile = fopen(treeFilename, "r");
    if (treeFile == NULL) {
        printf("Error al abrir el archivo del árbol de Huffman.\n");
        free(archivo_comprimidoText);
        return 1;
    }

    Grupo_Nodo_Menor* root = loadHuffmanTree(treeFile);
    fclose(treeFile);

    char* dearchivo_comprimidoText = decomprimir_texto(archivo_comprimidoText, root, archivo_comprimidoSize);

    FILE* outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        printf("Error al crear el archivo de salida.\n");
        free(archivo_comprimidoText);
        free(dearchivo_comprimidoText);
        freeHuffmanTree(root);
        return 1;
    }

    fprintf(outputFile, "%s", dearchivo_comprimidoText);
    fclose(outputFile);

    printf("Texto descomprimido guardado en '%s'.\n", outputFilename);

    free(archivo_comprimidoText);
    free(dearchivo_comprimidoText);
    freeHuffmanTree(root);

    return 0;
}
