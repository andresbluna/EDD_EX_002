#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALTURA_ARBOL 100
#define MAX_CARACTERES 256

typedef struct NodoMonticulo {
    char dato;
    unsigned frecuencia;
    struct NodoMonticulo *izquierda, *derecha;
} NodoMonticulo;

typedef struct Monticulo {
    unsigned tamano;
    unsigned capacidad;
    NodoMonticulo **arreglo;
} Monticulo;

NodoMonticulo* funcion_nuevoNodo(char dato, unsigned frecuencia) {
    NodoMonticulo* temp = (NodoMonticulo*) malloc(sizeof(NodoMonticulo));
    temp->izquierda = temp->derecha = NULL;
    temp->dato = dato;
    temp->frecuencia = frecuencia;
    return temp;
}

Monticulo* funcion_crearMonticulo(unsigned capacidad) {
    Monticulo* monticulo = (Monticulo*) malloc(sizeof(Monticulo));
    monticulo->tamano = 0;
    monticulo->capacidad = capacidad;
    monticulo->arreglo = (NodoMonticulo**) malloc(monticulo->capacidad * sizeof(NodoMonticulo*));
    return monticulo;
}

void funcion_intercambiarNodos(NodoMonticulo** a, NodoMonticulo** b) {
    NodoMonticulo* t = *a;
    *a = *b;
    *b = t;
}

void funcion_organizarMonticulo(Monticulo* monticulo, int idx) {
    int menor = idx;
    int izquierda = 2 * idx + 1;
    int derecha = 2 * idx + 2;

    if (izquierda < monticulo->tamano && monticulo->arreglo[izquierda]->frecuencia < monticulo->arreglo[menor]->frecuencia)
        menor = izquierda;

    if (derecha < monticulo->tamano && monticulo->arreglo[derecha]->frecuencia < monticulo->arreglo[menor]->frecuencia)
        menor = derecha;

    if (menor != idx) {
        funcion_intercambiarNodos(&monticulo->arreglo[menor], &monticulo->arreglo[idx]);
        funcion_organizarMonticulo(monticulo, menor);
    }
}

int funcion_esTamanoUno(Monticulo* monticulo) {
    return (monticulo->tamano == 1);
}

NodoMonticulo* funcion_extraerMinimo(Monticulo* monticulo) {
    NodoMonticulo* temp = monticulo->arreglo[0];
    monticulo->arreglo[0] = monticulo->arreglo[monticulo->tamano - 1];
    --monticulo->tamano;
    funcion_organizarMonticulo(monticulo, 0);
    return temp;
}

void funcion_insertarMonticulo(Monticulo* monticulo, NodoMonticulo* nodoMonticulo) {
    ++monticulo->tamano;
    int i = monticulo->tamano - 1;
    while (i && nodoMonticulo->frecuencia < monticulo->arreglo[(i - 1) / 2]->frecuencia) {
        monticulo->arreglo[i] = monticulo->arreglo[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    monticulo->arreglo[i] = nodoMonticulo;
}

void funcion_construirMonticulo(Monticulo* monticulo) {
    int n = monticulo->tamano - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        funcion_organizarMonticulo(monticulo, i);
}

int funcion_esHoja(NodoMonticulo* raiz) {
    return !(raiz->izquierda) && !(raiz->derecha);
}

Monticulo* funcion_crearYConstruirMonticulo(char dato[], int frecuencia[], int tamano) {
    Monticulo* monticulo = funcion_crearMonticulo(tamano);
    for (int i = 0; i < tamano; ++i)
        monticulo->arreglo[i] = funcion_nuevoNodo(dato[i], frecuencia[i]);
    monticulo->tamano = tamano;
    funcion_construirMonticulo(monticulo);
    return monticulo;
}

NodoMonticulo* funcion_construirArbolHuffman(char dato[], int frecuencia[], int tamano) {
    NodoMonticulo *izquierda, *derecha, *superior;
    Monticulo* monticulo = funcion_crearYConstruirMonticulo(dato, frecuencia, tamano);

    while (!funcion_esTamanoUno(monticulo)) {
        izquierda = funcion_extraerMinimo(monticulo);
        derecha = funcion_extraerMinimo(monticulo);
        superior = funcion_nuevoNodo('$', izquierda->frecuencia + derecha->frecuencia);
        superior->izquierda = izquierda;
        superior->derecha = derecha;
        funcion_insertarMonticulo(monticulo, superior);
    }
    return funcion_extraerMinimo(monticulo);
}

void funcion_imprimirArreglo(int arr[], int n) {
    for (int i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

void funcion_imprimirCodigos(NodoMonticulo* raiz, int arr[], int superior) {
    if (raiz->izquierda) {
        arr[superior] = 0;
        funcion_imprimirCodigos(raiz->izquierda, arr, superior + 1);
    }
    if (raiz->derecha) {
        arr[superior] = 1;
        funcion_imprimirCodigos(raiz->derecha, arr, superior + 1);
    }
    if (funcion_esHoja(raiz)) {
        printf("%c: ", raiz->dato);
        funcion_imprimirArreglo(arr, superior);
    }
}

void funcion_almacenarCodigos(NodoMonticulo* raiz, int arr[], int superior, char* codigos[]) {
    if (raiz->izquierda) {
        arr[superior] = 0;
        funcion_almacenarCodigos(raiz->izquierda, arr, superior + 1, codigos);
    }
    if (raiz->derecha) {
        arr[superior] = 1;
        funcion_almacenarCodigos(raiz->derecha, arr, superior + 1, codigos);
    }
    if (funcion_esHoja(raiz)) {
        codigos[raiz->dato] = (char*)malloc(superior + 1);
        for (int i = 0; i < superior; i++) {
            codigos[raiz->dato][i] = arr[i] + '0';
        }
        codigos[raiz->dato][superior] = '\0';
    }
}

void funcion_codigosHuffman(char dato[], int frecuencia[], int tamano) {
    NodoMonticulo* raiz = funcion_construirArbolHuffman(dato, frecuencia, tamano);
    int arr[MAX_ALTURA_ARBOL], superior = 0;
    funcion_imprimirCodigos(raiz, arr, superior);
}

void funcion_calcularFrecuencia(const char* texto, int* frecuencia) {
    for (int i = 0; texto[i]; i++) {
        frecuencia[(unsigned char)texto[i]]++;
    }
}

char* funcion_comprimirTexto(const char* texto, NodoMonticulo* raiz, int* tamanoComprimido) {
    int arr[MAX_ALTURA_ARBOL], superior = 0;
    char* codigos[256] = {NULL};
    funcion_almacenarCodigos(raiz, arr, superior, codigos);

    char* textoComprimido = (char*)malloc(strlen(texto) * 8); // Peor caso
    int indice = 0;
    char byteActual = 0;
    int contadorBits = 0;

    for (int i = 0; texto[i]; i++) {
        char* codigo = codigos[(unsigned char)texto[i]];
        for (int j = 0; codigo[j]; j++) {
            byteActual = (byteActual << 1) | (codigo[j] - '0');
            contadorBits++;
            if (contadorBits == 8) {
                textoComprimido[indice++] = byteActual;
                byteActual = 0;
                contadorBits = 0;
            }
        }
    }

    if (contadorBits > 0) {
        byteActual <<= (8 - contadorBits);
        textoComprimido[indice++] = byteActual;
    }

    *tamanoComprimido = indice;

    for (int i = 0; i < 256; i++) {
        if (codigos[i]) free(codigos[i]);
    }

    return textoComprimido;
}

void funcion_guardarArbolHuffman(NodoMonticulo* raiz, FILE* archivo) {
    if (raiz == NULL) {
        fprintf(archivo, "0");
        return;
    }

    fprintf(archivo, "1");
    if (funcion_esHoja(raiz)) {
        fprintf(archivo, "1%c", raiz->dato);
    } else {
        fprintf(archivo, "0");
        funcion_guardarArbolHuffman(raiz->izquierda, archivo);
        funcion_guardarArbolHuffman(raiz->derecha, archivo);
    }
}

NodoMonticulo* funcion_cargarArbolHuffman(FILE* archivo) {
    char c;
    if (fscanf(archivo, "%c", &c) == EOF || c == '0') {
        return NULL;
    }

    if (fscanf(archivo, "%c", &c) == EOF) {
        return NULL;
    }

    if (c == '1') {
        char dato;
        fscanf(archivo, "%c", &dato);
        return funcion_nuevoNodo(dato, 0);
    } else {
        NodoMonticulo* nodo = funcion_nuevoNodo('$', 0);
        nodo->izquierda = funcion_cargarArbolHuffman(archivo);
        nodo->derecha = funcion_cargarArbolHuffman(archivo);
        return nodo;
    }
}

int funcion_principal() {
    const char* nombreArchivo = "salmo23.txt";
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return 1;
    }

    fseek(archivo, 0, SEEK_END);
    long tamanoArchivo = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);

    char* texto = (char*)malloc(tamanoArchivo + 1);
    if (texto == NULL) {
        printf("Error al asignar memoria\n");
        fclose(archivo);
        return 1;
    }

    size_t bytesLeidos = fread(texto, 1, tamanoArchivo, archivo);
    texto[bytesLeidos] = '\0';
    fclose(archivo);

    printf("Tamaño original del archivo: %ld bytes\n", tamanoArchivo);

    int frecuencia[MAX_CARACTERES] = {0};
    funcion_calcularFrecuencia(texto, frecuencia);

    char datos[MAX_CARACTERES];
    int valoresFrecuencia[MAX_CARACTERES];
    int tamano = 0;

    for (int i = 0; i < MAX_CARACTERES; i++) {
        if (frecuencia[i] > 0) {
            datos[tamano] = (char)i;
            valoresFrecuencia[tamano] = frecuencia[i];
            tamano++;
        }
    }

    NodoMonticulo* raiz = funcion_construirArbolHuffman(datos, valoresFrecuencia, tamano);

    int tamanoComprimido;
    char* textoComprimido = funcion_comprimirTexto(texto, raiz, &tamanoComprimido);
    printf("Tamaño comprimido: %d bytes\n", tamanoComprimido);

    FILE* archivoComprimido = fopen("comprimido.bin", "wb");
    fwrite(textoComprimido, sizeof(char), tamanoComprimido, archivoComprimido);
    fclose(archivoComprimido);

    FILE* archivoArbol = fopen("arbol_huffman.bin", "w");
    funcion_guardarArbolHuffman(raiz, archivoArbol);
    fclose(archivoArbol);

    printf("Archivo comprimido y árbol de Huffman guardados.\n");

    free(texto);
    free(textoComprimido);

    return 0;
}
