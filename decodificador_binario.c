#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100
#define MAX_CHARS 256

typedef struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
} MinHeapNode;

MinHeapNode* newNode(char data, unsigned freq) {
    MinHeapNode* temp = (MinHeapNode*) malloc(sizeof(MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

int isLeaf(MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

MinHeapNode* loadHuffmanTree(FILE* file) {
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
        return newNode(data, 0);
    } else {
        MinHeapNode* node = newNode('$', 0);
        node->left = loadHuffmanTree(file);
        node->right = loadHuffmanTree(file);
        return node;
    }
}

char* decompressText(const char* compressedText, MinHeapNode* root, int compressedSize) {
    char* decompressedText = (char*)malloc(compressedSize * 8); // Worst case scenario
    int index = 0;
    MinHeapNode* current = root;

    for (int i = 0; i < compressedSize; i++) {
        char byte = compressedText[i];
        for (int j = 7; j >= 0; j--) {
            int bit = (byte >> j) & 1;
            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }

            if (isLeaf(current)) {
                decompressedText[index++] = current->data;
                current = root;
            }
        }
    }

    decompressedText[index] = '\0';
    return decompressedText;
}

void freeHuffmanTree(MinHeapNode* root) {
    if (root == NULL) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

int main() {
    const char* compressedFilename = "compressed.bin";
    const char* treeFilename = "huffman_tree.bin";
    const char* outputFilename = "decompressed.txt";

    // Leer el archivo comprimido
    FILE* compressedFile = fopen(compressedFilename, "rb");
    if (compressedFile == NULL) {
        printf("Error al abrir el archivo comprimido.\n");
        return 1;
    }

    fseek(compressedFile, 0, SEEK_END);
    int compressedSize = ftell(compressedFile);
    fseek(compressedFile, 0, SEEK_SET);

    char* compressedText = (char*)malloc(compressedSize);
    fread(compressedText, sizeof(char), compressedSize, compressedFile);
    fclose(compressedFile);

    // Leer el árbol de Huffman
    FILE* treeFile = fopen(treeFilename, "r");
    if (treeFile == NULL) {
        printf("Error al abrir el archivo del árbol de Huffman.\n");
        free(compressedText);
        return 1;
    }

    MinHeapNode* root = loadHuffmanTree(treeFile);
    fclose(treeFile);

    // Descomprimir el texto
    char* decompressedText = decompressText(compressedText, root, compressedSize);

    // Guardar el texto descomprimido en un archivo
    FILE* outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        printf("Error al crear el archivo de salida.\n");
        free(compressedText);
        free(decompressedText);
        freeHuffmanTree(root);
        return 1;
    }

    fprintf(outputFile, "%s", decompressedText);
    fclose(outputFile);

    printf("Texto descomprimido guardado en '%s'.\n", outputFilename);

    // Liberar memoria
    free(compressedText);
    free(decompressedText);
    freeHuffmanTree(root);

    return 0;
}
