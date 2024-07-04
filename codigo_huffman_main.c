#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100
#define MAX_CHARS 256

typedef struct Grupo_Nodo_Menor {
    char data;
    unsigned freq;
    struct Grupo_Nodo_Menor *left, *right;
} Grupo_Nodo_Menor;

typedef struct Grupo_Menor {
    unsigned size;
    unsigned capacity;
    Grupo_Nodo_Menor **array;
} Grupo_Menor;


//Crea un nuevo nodo para el árbol de Huffman.
Grupo_Nodo_Menor* nuevo_nodo(char data, unsigned freq) {
    Grupo_Nodo_Menor* temp = (Grupo_Nodo_Menor*) malloc(sizeof(Grupo_Nodo_Menor));
    if (temp==NULL) {
        printf("No fue posible reservar memoria");
        exit(1);
    }
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

//Crea un nuevo grupo mínimo vacío.
Grupo_Menor* crearGrupo_Menor(unsigned capacity) {
    Grupo_Menor* grupoMenor = (Grupo_Menor*) malloc(sizeof(Grupo_Menor));
    if (grupoMenor==NULL) {
        printf("No fue posible reservar memoria");
        exit(1);
    }
    grupoMenor->size = 0;
    grupoMenor->capacity = capacity;
    grupoMenor->array = (Grupo_Nodo_Menor**) malloc(grupoMenor->capacity * sizeof(Grupo_Nodo_Menor*));
    return grupoMenor;
}

//Intercambia dos nodos en el montículo.
void intercambiar_grupo_nodo_menor(Grupo_Nodo_Menor** a, Grupo_Nodo_Menor** b) {
    Grupo_Nodo_Menor* t = *a;
    *a = *b;
    *b = t;
}

//Mantiene la propiedad de grupo mínimo.
void agrupar_menor(Grupo_Menor* grupoMenor) {
    int i = 0;
    while (1) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < grupoMenor->size && grupoMenor->array[left]->freq < grupoMenor->array[smallest]->freq)
            smallest = left;
        if (right < grupoMenor->size && grupoMenor->array[right]->freq < grupoMenor->array[smallest]->freq)
            smallest = right;

        if (smallest == i)
            break;

        Grupo_Nodo_Menor* temp = grupoMenor->array[i];
        grupoMenor->array[i] = grupoMenor->array[smallest];
        grupoMenor->array[smallest] = temp;

        i = smallest;
    }
}

//Verifica si el grupo tiene solo un elemento.
int es_tamano_uno(Grupo_Menor* grupoMenor) {
    return (grupoMenor->size == 1);
}

//Extrae el nodo con la frecuencia mínima del grupo.
Grupo_Nodo_Menor* extraer_menor(Grupo_Menor* grupoMenor) {
    if (grupoMenor->size <= 0)
        return NULL;

    Grupo_Nodo_Menor* root = grupoMenor->array[0];

    if (grupoMenor->size > 1) {
        grupoMenor->array[0] = grupoMenor->array[grupoMenor->size - 1];
        grupoMenor->size--;
        agrupar_menor(grupoMenor);
    } else {
        grupoMenor->size = 0;
    }

    return root;
}


// Inserta un nuevo nodo en el montículo.
void insertar_grupo_menor(Grupo_Menor* grupoMenor, Grupo_Nodo_Menor* grupoMenorNode) {
    ++grupoMenor->size;
    int i = grupoMenor->size - 1;
    while (i && grupoMenorNode->freq < grupoMenor->array[(i - 1) / 2]->freq) {
        grupoMenor->array[i] = grupoMenor->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    grupoMenor->array[i] = grupoMenorNode;
}

//Construye un montículo mínimo a partir de un arreglo de nodos.
void construir_grupo_menor(Grupo_Menor* grupoMenor) {
    for (int i = grupoMenor->size / 2 - 1; i >= 0; i--) {
        int current = i;
        while (1) {
            int smallest = current;
            int left = 2 * current + 1;
            int right = 2 * current + 2;

            if (left < grupoMenor->size && grupoMenor->array[left]->freq < grupoMenor->array[smallest]->freq)
                smallest = left;
            if (right < grupoMenor->size && grupoMenor->array[right]->freq < grupoMenor->array[smallest]->freq)
                smallest = right;

            if (smallest == current)
                break;

            Grupo_Nodo_Menor* temp = grupoMenor->array[current];
            grupoMenor->array[current] = grupoMenor->array[smallest];
            grupoMenor->array[smallest] = temp;

            current = smallest;
        }
    }
}


//Verifica si un nodo es una hoja en el árbol.
int es_hoja(Grupo_Nodo_Menor* root) {
    return !(root->left) && !(root->right);
}

//Crea y construye un montículo mínimo a partir de datos y frecuencias.
Grupo_Menor* crear_y_construir_grupo_menor(char data[], int freq[], int size) {
    Grupo_Menor* grupoMenor = crearGrupo_Menor(size);
    for (int i = 0; i < size; ++i)
        grupoMenor->array[i] = nuevo_nodo(data[i], freq[i]);
    grupoMenor->size = size;
    construir_grupo_menor(grupoMenor);
    return grupoMenor;
}

//Construye el árbol de Huffman.
Grupo_Nodo_Menor* construir_arbol_huffman(char data[], int freq[], int size) {
    Grupo_Nodo_Menor *left, *right, *top;
    Grupo_Menor* grupoMenor = crear_y_construir_grupo_menor(data, freq, size);

    while (!es_tamano_uno(grupoMenor)) {
        left = extraer_menor(grupoMenor);
        right = extraer_menor(grupoMenor);
        top = nuevo_nodo('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertar_grupo_menor(grupoMenor, top);
    }
    return extraer_menor(grupoMenor);
}

//Imprime un arreglo de enteros.
void imprimir_arreglo(int arr[], int n) {
    for (int i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

//Imprime los códigos de Huffman para cada carácter.
void imprimir_codigos(Grupo_Nodo_Menor* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        imprimir_codigos(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        imprimir_codigos(root->right, arr, top + 1);
    }
    if (es_hoja(root)) {
        printf("%c: ", root->data);
        imprimir_arreglo(arr, top);
    }
}

//Almacena los códigos de Huffman en un arreglo.
void almacenar_codigos(Grupo_Nodo_Menor* root, int arr[], int top, char* codes[]) {
    if (root->left) {
        arr[top] = 0;
        almacenar_codigos(root->left, arr, top + 1, codes);
    }
    if (root->right) {
        arr[top] = 1;
        almacenar_codigos(root->right, arr, top + 1, codes);
    }
    if (es_hoja(root)) {
        codes[root->data] = (char*)malloc(top + 1);
        for (int i = 0; i < top; i++) {
            codes[root->data][i] = arr[i] + '0';
        }
        codes[root->data][top] = '\0';
    }
}

//Genera y muestra los códigos de Huffman.
void codigo_huffman(char data[], int freq[], int size) {
    Grupo_Nodo_Menor* root = construir_arbol_huffman(data, freq, size);
    int arr[MAX_TREE_HT], top = 0;
    imprimir_codigos(root, arr, top);
}

//Calcula la frecuencia de cada carácter en el texto.
void calcular_frecuencia(const char* text, int* freq) {
    for (int i = 0; text[i]; i++) {
        freq[(unsigned char)text[i]]++;
    }
}

//Comprime el texto usando los códigos de Huffman.
char* comprimir_texto(const char* text, Grupo_Nodo_Menor* root, int* archivo_comprimidoSize) {
    int arr[MAX_TREE_HT], top = 0;
    char* codes[256] = {NULL};
    almacenar_codigos(root, arr, top, codes);

    char* archivo_comprimidoText = (char*)malloc(strlen(text) * 8); // Worst case scenario
    int index = 0;
    char currentByte = 0;
    int bitCount = 0;

    for (int i = 0; text[i]; i++) {
        char* code = codes[(unsigned char)text[i]];
        for (int j = 0; code[j]; j++) {
            currentByte = (currentByte << 1) | (code[j] - '0');
            bitCount++;
            if (bitCount == 8) {
                archivo_comprimidoText[index++] = currentByte;
                currentByte = 0;
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0) {
        currentByte <<= (8 - bitCount);
        archivo_comprimidoText[index++] = currentByte;
    }

    *archivo_comprimidoSize = index;

    for (int i = 0; i < 256; i++) {
                if (codes[i]) {
            free(codes[i]);
        }
    }

    return archivo_comprimidoText;
}

//Guarda el árbol de Huffman en un archivo.
void guardar_arbol_huffman(Grupo_Nodo_Menor* root, FILE* file) {
    if (root == NULL) {
        fprintf(file, "0");
        return;
    }

    fprintf(file, "1");
    if (es_hoja(root)) {
        fprintf(file, "1%c", root->data);
    } else {
        fprintf(file, "0");
        guardar_arbol_huffman(root->left, file);
        guardar_arbol_huffman(root->right, file);
    }
}

//Carga el árbol de Huffman desde un archivo.
Grupo_Nodo_Menor* cargar_arbol_huffman(FILE* file) {
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
        node->left = cargar_arbol_huffman(file);
        node->right = cargar_arbol_huffman(file);
        return node;
    }
}

//Imprime bits de bytes
void imprimir_bits(unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (byte >> i) & 1);
    }
}



//Función principal-main
int main() {
    const char* filename = "salmo23.txt";
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error al abrir el archivo %s\n", filename);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char* text = (unsigned char*)malloc(fileSize);
    if (text == NULL) {
        printf("Error al asignar memoria\n");
        fclose(file);
        return 1;
    }

    size_t bytesRead = fread(text, 1, fileSize, file);
    fclose(file);

    printf("Tamaño original del archivo: %ld bytes\n", fileSize);

    int freq[MAX_CHARS] = {0};
    calcular_frecuencia(text, freq);

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

    Grupo_Nodo_Menor* root = construir_arbol_huffman(data, freqValues, size);

    int archivo_comprimidoSize;
    char* archivo_comprimidoText = comprimir_texto(text, root, &archivo_comprimidoSize);
    printf("Tamaño comprimido: %d bytes\n", archivo_comprimidoSize);

    FILE* archivo_comprimidoFile = fopen("archivo_comprimido.bin", "wb");
    fwrite(archivo_comprimidoText, sizeof(char), archivo_comprimidoSize, archivo_comprimidoFile);
    fclose(archivo_comprimidoFile);

    FILE* treeFile = fopen("arbol_huffman.bin", "w");
    guardar_arbol_huffman(root, treeFile);
    fclose(treeFile);

    printf("Archivo comprimido y árbol de Huffman guardados.\n");

    free(text);
    free(archivo_comprimidoText);

    return 0;
}



