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

typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    MinHeapNode **array;
} MinHeap;

MinHeapNode* newNode(char data, unsigned freq) {
    MinHeapNode* temp = (MinHeapNode*) malloc(sizeof(MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

MinHeap* createMinHeap(unsigned capacity) {
    MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**) malloc(minHeap->capacity * sizeof(MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(MinHeap* minHeap) {
    return (minHeap->size == 1);
}

MinHeapNode* extractMin(MinHeap* minHeap) {
    MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap* minHeap, MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(MinHeap* minHeap) {
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

int isLeaf(MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    MinHeapNode *left, *right, *top;
    MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

void printArr(int arr[], int n) {
    for (int i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

void printCodes(MinHeapNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
    if (isLeaf(root)) {
        printf("%c: ", root->data);
        printArr(arr, top);
    }
}

void storeCodes(MinHeapNode* root, int arr[], int top, char* codes[]) {
    if (root->left) {
        arr[top] = 0;
        storeCodes(root->left, arr, top + 1, codes);
    }
    if (root->right) {
        arr[top] = 1;
        storeCodes(root->right, arr, top + 1, codes);
    }
    if (isLeaf(root)) {
        codes[root->data] = (char*)malloc(top + 1);
        for (int i = 0; i < top; i++) {
            codes[root->data][i] = arr[i] + '0';
        }
        codes[root->data][top] = '\0';
    }
}

void HuffmanCodes(char data[], int freq[], int size) {
    MinHeapNode* root = buildHuffmanTree(data, freq, size);
    int arr[MAX_TREE_HT], top = 0;
    printCodes(root, arr, top);
}

void calculateFrequency(const char* text, int* freq) {
    for (int i = 0; text[i]; i++) {
        freq[(unsigned char)text[i]]++;
    }
}

char* compressText(const char* text, MinHeapNode* root, int* compressedSize) {
    int arr[MAX_TREE_HT], top = 0;
    char* codes[256] = {NULL};
    storeCodes(root, arr, top, codes);

    char* compressedText = (char*)malloc(strlen(text) * 8); // Worst case scenario
    int index = 0;
    char currentByte = 0;
    int bitCount = 0;

    for (int i = 0; text[i]; i++) {
        char* code = codes[(unsigned char)text[i]];
        for (int j = 0; code[j]; j++) {
            currentByte = (currentByte << 1) | (code[j] - '0');
            bitCount++;
            if (bitCount == 8) {
                compressedText[index++] = currentByte;
                currentByte = 0;
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0) {
        currentByte <<= (8 - bitCount);
        compressedText[index++] = currentByte;
    }

    *compressedSize = index;

    for (int i = 0; i < 256; i++) {
                if (codes[i]) {
            free(codes[i]);
        }
    }

    return compressedText;
}

void saveHuffmanTree(MinHeapNode* root, FILE* file) {
    if (root == NULL) {
        fprintf(file, "0");
        return;
    }

    fprintf(file, "1");
    if (isLeaf(root)) {
        fprintf(file, "1%c", root->data);
    } else {
        fprintf(file, "0");
        saveHuffmanTree(root->left, file);
        saveHuffmanTree(root->right, file);
    }
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

int main() {
    const char* filename = "/Users/imagemaker/Desarrollo/EDD/EDD_EX_002/salmo23.txt";
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error al abrir el archivo %s\n", filename);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* text = (char*)malloc(fileSize + 1);
    if (text == NULL) {
        printf("Error al asignar memoria\n");
        fclose(file);
        return 1;
    }

    size_t bytesRead = fread(text, 1, fileSize, file);
    text[bytesRead] = '\0';
    fclose(file);

    printf("Tamaño original del archivo: %ld bytes\n", fileSize);

    int freq[MAX_CHARS] = {0};
    calculateFrequency(text, freq);

    char data[MAX_CHARS];
    int freqValues[MAX_CHARS];
    int size = 0;

    for (int i = 0; i < MAX_CHARS; i++) {
        if (freq[i] > 0) {
            data[size] = (char)i;
            freqValues[size] = freq[i];
            size++;
        }
    }

    MinHeapNode* root = buildHuffmanTree(data, freqValues, size);

    int compressedSize;
    char* compressedText = compressText(text, root, &compressedSize);
    printf("Tamaño comprimido: %d bytes\n", compressedSize);

    FILE* compressedFile = fopen("compressed.bin", "wb");
    fwrite(compressedText, sizeof(char), compressedSize, compressedFile);
    fclose(compressedFile);

    FILE* treeFile = fopen("huffman_tree.bin", "w");
    saveHuffmanTree(root, treeFile);
    fclose(treeFile);

    printf("Archivo comprimido y árbol de Huffman guardados.\n");

    // Simulación de restauración en otro computador
    FILE* compressedReadFile = fopen("compressed.bin", "rb");
    fseek(compressedReadFile, 0, SEEK_END);
    int compressedReadSize = ftell(compressedReadFile);
    fseek(compressedReadFile, 0, SEEK_SET);

    char* compressedReadText = (char*)malloc(compressedReadSize);
    fread(compressedReadText, sizeof(char), compressedReadSize, compressedReadFile);
    fclose(compressedReadFile);

    FILE* treeReadFile = fopen("huffman_tree.bin", "r");
    MinHeapNode* loadedRoot = loadHuffmanTree(treeReadFile);
    fclose(treeReadFile);

    free(text);
    free(compressedText);
    free(compressedReadText);

    return 0;
}

