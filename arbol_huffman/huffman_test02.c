#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100
#define MAX_CHARS 256
#define MAX_CHILDREN 10

// Estructura para representar un nodo del árbol de Huffman
typedef struct Nodo {
    char data;
    unsigned freq;
    struct Nodo *left, *right;
    struct Nodo *children[MAX_CHILDREN];
    int numChildren;
} Nodo;

// Estructura para representar una cola de prioridad (montículo mínimo)
typedef struct Cola {
    unsigned size;
    unsigned capacity;
    Nodo **array;
} Cola;

// Función para crear un nuevo nodo del árbol
Nodo* crearNodo(char data, unsigned freq) {
    Nodo* nodo = (Nodo*) malloc(sizeof(Nodo));
    nodo->data = data;
    nodo->freq = freq;
    nodo->left = nodo->right = NULL;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        nodo->children[i] = NULL;
    }
    nodo->numChildren = 0;
    return nodo;
}
// Función para insertar hijo en nodo
void insertarHijo(Nodo* padre, Nodo* hijo) {
    if (padre->numChildren < MAX_CHILDREN) {
        padre->children[padre->numChildren] = hijo;
        padre->numChildren++;
    }
}
//Función para hacer recorrido de Preorden
void recorridoPreordenGeneral(Nodo* nodo) {
    if (nodo == NULL)
        return;
    printf("%c ", nodo->data);
    for (int i = 0; i < nodo->numChildren; i++) {
        recorridoPreordenGeneral(nodo->children[i]);
    }
}

//Función para hacer recorrido Post Orden
void recorridoPostordenGeneral(Nodo* nodo) {
    if (nodo == NULL)
        return;
    for (int i = 0; i < nodo->numChildren; i++) {
        recorridoPostordenGeneral(nodo->children[i]);
    }
    printf("%c ", nodo->data);
}

// Función para crear una cola de prioridad vacía
Cola* crearCola(unsigned capacity) {
    Cola* cola = (Cola*) malloc(sizeof(Cola));
    cola->size = 0;
    cola->capacity = capacity;
    cola->array = (Nodo**) malloc(cola->capacity * sizeof(Nodo*));
    return cola;
}

// Función para intercambiar dos nodos en la cola de prioridad
void intercambiarNodos(Nodo** a, Nodo** b) {
    Nodo* temp = *a;
    *a = *b;
    *b = temp;
}

// Función para mantener la propiedad de montículo mínimo
void mantenerMonticulo(Cola* cola, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < cola->size && cola->array[left]->freq < cola->array[smallest]->freq)
        smallest = left;

    if (right < cola->size && cola->array[right]->freq < cola->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        intercambiarNodos(&cola->array[smallest], &cola->array[idx]);
        mantenerMonticulo(cola, smallest);
    }
}

// Función para extraer el nodo con la frecuencia mínima de la cola
Nodo* extraerMinimo(Cola* cola) {
    if (cola->size == 0)
        return NULL;

    Nodo* nodo = cola->array[0];
    cola->array[0] = cola->array[cola->size - 1];
    --cola->size;
    mantenerMonticulo(cola, 0);
    return nodo;
}

// Función para insertar un nodo en la cola de prioridad
void insertarNodo(Cola* cola, Nodo* nodo) {
    ++cola->size;
    int i = cola->size - 1;
    while (i && nodo->freq < cola->array[(i - 1) / 2]->freq) {
        cola->array[i] = cola->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    cola->array[i] = nodo;
}

// Función para construir el árbol de Huffman
Nodo* construirArbolHuffman(char data[], int freq[], int size) {
    Nodo *left, *right, *top;
    Cola* cola = crearCola(size);

    for (int i = 0; i < size; ++i)
        insertarNodo(cola, crearNodo(data[i], freq[i]));

    while (cola->size != 1) {
        left = extraerMinimo(cola);
        right = extraerMinimo(cola);
        top = crearNodo('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertarNodo(cola, top);
    }

    return extraerMinimo(cola);
}

// Función para verificar si un nodo es una hoja del árbol
int esHoja(Nodo* nodo) {
    return !(nodo->left) && !(nodo->right);
}

// Función para imprimir los códigos de Huffman
void imprimirCodigos(Nodo* nodo, int arr[], int top) {
    if (nodo->left) {
        arr[top] = 0;
        imprimirCodigos(nodo->left, arr, top + 1);
    }
    if (nodo->right) {
        arr[top] = 1;
        imprimirCodigos(nodo->right, arr, top + 1);
    }
    if (esHoja(nodo)) {
        printf("%c: ", nodo->data);
        for (int i = 0; i < top; ++i)
            printf("%d", arr[i]);
        printf("\n");
    }
}

// Función para recorrer el árbol en preorden
void recorridoPreorden(Nodo* nodo) {
    if (nodo == NULL)
        return;
    printf("%c ", nodo->data);
    recorridoPreorden(nodo->left);
    recorridoPreorden(nodo->right);
}

// Función para recorrer el árbol en inorden
void recorridoInorden(Nodo* nodo) {
    if (nodo == NULL)
        return;
    recorridoInorden(nodo->left);
    printf("%c ", nodo->data);
    recorridoInorden(nodo->right);
}

// Función para recorrer el árbol en postorden
void recorridoPostorden(Nodo* nodo) {
    if (nodo == NULL)
        return;
    recorridoPostorden(nodo->left);
    recorridoPostorden(nodo->right);
    printf("%c ", nodo->data);
}

// Función para recorrer el árbol utilizando una pila
void recorridoPila(Nodo* nodo) {
    if (nodo == NULL)
        return;

    Nodo* pila[MAX_TREE_HT];
    int top = -1;
    pila[++top] = nodo;

    while (top >= 0) {
        Nodo* temp = pila[top--];
        printf("%c ", temp->data);

        if (temp->right)
            pila[++top] = temp->right;
        if (temp->left)
            pila[++top] = temp->left;
    }
}

// Función para buscar un elemento en un árbol binario de búsqueda
Nodo* buscarElemento(Nodo* nodo, char data) {
    if (nodo == NULL || nodo->data == data)
        return nodo;

    if (data < nodo->data)
        return buscarElemento(nodo->left, data);
    else
        return buscarElemento(nodo->right, data);
}

// Función para insertar un elemento en un árbol binario de búsqueda
Nodo* insertarElemento(Nodo* nodo, char data, int freq) {
    if (nodo == NULL)
        return crearNodo(data, freq);

    if (data < nodo->data)
        nodo->left = insertarElemento(nodo->left, data, freq);
    else if (data > nodo->data)
        nodo->right = insertarElemento(nodo->right, data, freq);

    return nodo;
}

// Función para calcular la frecuencia de cada carácter en el texto
void calcularFrecuencia(const char* text, int* freq) {
    for (int i = 0; text[i]; i++)
        freq[(unsigned char)text[i]]++;
}

// Función para almacenar los códigos de Huffman en un arreglo de cadenas
void almacenarCodigos(Nodo* nodo, int arr[], int top, char* codigos[]) {
    if (nodo->left) {
        arr[top] = 0;
        almacenarCodigos(nodo->left, arr, top + 1, codigos);
    }
    if (nodo->right) {
        arr[top] = 1;
        almacenarCodigos(nodo->right, arr, top + 1, codigos);
    }
    if (esHoja(nodo)) {
        codigos[nodo->data] = (char*)malloc(top + 1);
        for (int i = 0; i < top; i++)
            codigos[nodo->data][i] = arr[i] + '0';
        codigos[nodo->data][top] = '\0';
    }
}

// Función para comprimir el texto utilizando los códigos de Huffman
char* comprimirTexto(const char* text, Nodo* nodo, int* comprimidoSize) {
    int arr[MAX_TREE_HT], top = 0;
    char* codigos[256] = {NULL};
    almacenarCodigos(nodo, arr, top, codigos);

    char* comprimido = (char*)malloc(strlen(text) * 8);
    int index = 0;
    char currentByte = 0;
    int bitCount = 0;

    for (int i = 0; text[i]; i++) {
        char* codigo = codigos[(unsigned char)text[i]];
        for (int j = 0; codigo[j]; j++) {
            currentByte = (currentByte << 1) | (codigo[j] - '0');
            bitCount++;
            if (bitCount == 8) {
                comprimido[index++] = currentByte;
                currentByte = 0;
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0) {
        currentByte <<= (8 - bitCount);
        comprimido[index++] = currentByte;
    }

    *comprimidoSize = index;

    for (int i = 0; i < 256; i++) {
        if (codigos[i])
            free(codigos[i]);
    }

    return comprimido;
}

// Función para guardar el árbol de Huffman en los archivos
void guardarArbolHuffman(Nodo* nodo, FILE* file) {
    if (nodo == NULL) {
        fputc('0', file);  // Usamos '0' para indicar un nodo nulo
        return;
    }

    if (esHoja(nodo)) {
        fputc('L', file);  // 'L' para indicar que es una hoja
        fputc(nodo->data, file);
    } else {
        fputc('I', file);  // 'I' para indicar que es un nodo interno
        guardarArbolHuffman(nodo->left, file);
        guardarArbolHuffman(nodo->right, file);
    }
}



// Función para cargar el árbol de Huffman desde un archivo
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


// Función principal
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
    calcularFrecuencia(text, freq);

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

    Nodo* raiz = construirArbolHuffman(data, freqValues, size);

    int comprimidoSize;
    char* comprimido = comprimirTexto(text, raiz, &comprimidoSize);
    printf("Tamaño comprimido: %d bytes\n", comprimidoSize);

    // Guardar el archivo comprimido en formato binario
    FILE* comprimidoFile = fopen("comprimido.bin", "wb");
    fwrite(comprimido, sizeof(char), comprimidoSize, comprimidoFile);
    fclose(comprimidoFile);

    // Guardar el árbol de Huffman
    FILE* treeFile = fopen("arbol.bin", "wb");
    guardarArbolHuffman(raiz, treeFile);
    fclose(treeFile);

    // Guardar la representación binaria en un archivo de texto
    FILE* binarioFile = fopen("comprimido_binario.txt", "w");
    if (binarioFile == NULL) {
        printf("Error al abrir el archivo para guardar el binario\n");
        return 1;
    }

    for (int i = 0; i < comprimidoSize; i++) {
        for (int j = 7; j >= 0; j--) {
            fprintf(binarioFile, "%d", (comprimido[i] >> j) & 1);
        }
    }

    fclose(binarioFile);

    printf("Archivo comprimido, árbol de Huffman y representación binaria guardados.\n");

    // Liberar memoria
    free(text);
    free(comprimido);

    return 0;
}



