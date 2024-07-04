#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct {
    int origen;
    int destino;
    float peso;
} Borde;

typedef struct {
    char letra;
    float x;
    float y;
} Punto;

float distancia_euclidiana(Punto p1, Punto p2);
void llenar_matriz(Punto *lista, int cant_puntos, float **mat);
void imprimir_matriz(int cant_puntos, float **mat);
void estimar_ruta(float *costo, Punto *array, int cant_puntos, float **matriz, char *ruta);
void estimar_ruta_aleatoria(float *costo, Punto *array, int cant_puntos, float **matriz, char *ruta);
void prim(float **matriz, int cant_puntos, float **arbol_prim);
void kruskal(float **matriz, int cant_puntos, float **arbol_kruskal);
int find(int padre[], int i);
void union_find(int padre[], int rango[], int x, int y);
int comparar_bordes(const void *a, const void *b);

int main() {
    FILE *mi_archivo;
    mi_archivo = fopen("C:\\Desarrollo\\EDD_EX_002\\rutas_far.txt", "r");
    if (mi_archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        exit(1);
    }

    int cant_puntos, i, j;
    Punto *array;
    float **matriz, costo;
    char *ruta;

    fscanf(mi_archivo, "%d\n", &cant_puntos);

    array = malloc(sizeof(Punto) * cant_puntos);
    if (array == NULL) {
        printf("Error al reservar memoria para array");
        exit(1);
    }

    ruta = malloc(sizeof(char) * (cant_puntos + 2));
    if (ruta == NULL) {
        printf("Error al reservar memoria para ruta");
        exit(1);
    }

    matriz = (float **)malloc(sizeof(float *) * cant_puntos);
    if (matriz == NULL) {
        printf("Error al reservar memoria para matriz");
        exit(1);
    }
    for (i = 0; i < cant_puntos; i++) {
        matriz[i] = (float *)malloc(sizeof(float) * cant_puntos);
        if (matriz[i] == NULL) {
            printf("Error al reservar memoria para matriz[i]");
            exit(1);
        }
    }
    for (i = 0; i < cant_puntos; i++) {
        fscanf(mi_archivo, " %c,%f,%f\n", &array[i].letra, &array[i].x, &array[i].y);
        printf("%c,%.14f,%.14f\n", array[i].letra, array[i].x, array[i].y);
        fflush(stdin);
    }

    llenar_matriz(array, cant_puntos, matriz);
    imprimir_matriz(cant_puntos, matriz);

    float **arbol_prim = malloc(sizeof(float *) * cant_puntos);
    float **arbol_kruskal = malloc(sizeof(float *) * cant_puntos);

    for (i = 0; i < cant_puntos; i++) {
        arbol_prim[i] = malloc(sizeof(float) * cant_puntos);
        if (arbol_prim[i] == NULL) {
            printf("Error al reservar memoria para arbol_prim");
            exit(1);
        }
        arbol_kruskal[i] = malloc(sizeof(float) * cant_puntos);
        if (arbol_kruskal[i] == NULL) {
            printf("Error al reservar memoria para arbol_kruskal");
            exit(1);
        }
        for (j = 0; j < cant_puntos; j++) {
            arbol_prim[i][j] = 0;
            arbol_kruskal[i][j] = 0;
        }
    }

    prim(matriz, cant_puntos, arbol_prim);
    printf("Arbol de expansion minima (Prim):\n");
    imprimir_matriz(cant_puntos, arbol_prim);

    kruskal(matriz, cant_puntos, arbol_kruskal);
    printf("Arbol de expansion minima (Kruskal):\n");
    imprimir_matriz(cant_puntos, arbol_kruskal);

    costo = 0;
    estimar_ruta(&costo, array, cant_puntos, matriz, ruta);
    printf("La mejor ruta (KNN) es %s con un costo de %g\n", ruta, costo);

    costo = 0;
    estimar_ruta_aleatoria(&costo, array, cant_puntos, matriz, ruta);
    printf("La ruta aleatoria es %s con un costo de %g\n", ruta, costo);

    for (i = 0; i < cant_puntos; i++) {
        free(matriz[i]);
        free(arbol_prim[i]);
        free(arbol_kruskal[i]);
    }
    free(matriz);
    free(arbol_prim);
    free(arbol_kruskal);
    free(array);
    free(ruta);
    fclose(mi_archivo);
    return 0;
}

float distancia_euclidiana(Punto p1, Punto p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void llenar_matriz(Punto *lista, int cant_puntos, float **mat) {
    int i, j;
    float distancia;
    for (i = 0; i < cant_puntos; i++) {
        for (j = 0; j <= i; j++) {
            if (i == j) {
                mat[i][j] = -1;
            } else {
                distancia = distancia_euclidiana(lista[i], lista[j]);
                mat[i][j] = distancia;
                mat[j][i] = distancia;
            }
        }
    }
}

void imprimir_matriz(int cant_puntos, float **mat) {
    int i, j;
    for (i = 0; i < cant_puntos; i++) {
        for (j = 0; j < cant_puntos; j++) {
            printf("%0.4f\t", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void estimar_ruta(float *costo, Punto *array, int cant_puntos, float **matriz, char *ruta) {
    int i, j, inicio, actual, siguiente;
    float distancia_minima;

    *costo = 0;
    inicio = 0;

    int visitado[cant_puntos];
    for (i = 0; i < cant_puntos; i++) {
        visitado[i] = 0;
    }

    actual = inicio;
    ruta[0] = array[inicio].letra;
    visitado[inicio] = 1;

    for (i = 1; i < cant_puntos; i++) {
        distancia_minima = INFINITY;
        siguiente = -1;

        for (j = 0; j < cant_puntos; j++) {
            if (!visitado[j] && matriz[actual][j] < distancia_minima) {
                distancia_minima = matriz[actual][j];
                siguiente = j;
            }
        }

        *costo += distancia_minima;

        visitado[siguiente] = 1;
        actual = siguiente;
        ruta[i] = array[siguiente].letra;
    }

    *costo += matriz[actual][inicio];
    ruta[cant_puntos] = array[inicio].letra;
    ruta[cant_puntos + 1] = '\0';
}

void estimar_ruta_aleatoria(float *costo, Punto *array, int cant_puntos, float **matriz, char *ruta) {
    int i, j, inicio, actual, siguiente;
    int *visitado = (int *)malloc(cant_puntos * sizeof(int));


    if (visitado == NULL) {
        printf("Error al reservar memoria para visitado");
        exit(1);
    }

    *costo = 0;
    inicio = 0;

    for (i = 0; i < cant_puntos; i++) {
        visitado[i] = 0;
    }

    actual = inicio;
    ruta[0] = array[inicio].letra;
    visitado[inicio] = 1;

    srand(time(NULL));

    for (i = 1; i < cant_puntos; i++) {
        do {
            siguiente = rand() % cant_puntos;
        } while (visitado[siguiente]);

        *costo += matriz[actual][siguiente];

        visitado[siguiente] = 1;
        actual = siguiente;
        ruta[i] = array[siguiente].letra;
    }

    *costo += matriz[actual][inicio];
    ruta[cant_puntos] = array[inicio].letra;
    ruta[cant_puntos + 1] = '\0';

    free(visitado);
}

void prim(float **matriz, int cant_puntos, float **arbol_prim) {
    int *visitado = (int *)malloc(cant_puntos * sizeof(int));
    if (visitado == NULL) {
        printf("Error al reservar memoria para visitado");
        exit(1);
    }
    float *costos_minimos = (float *)malloc(cant_puntos * sizeof(float));
    if (costos_minimos == NULL) {
        printf("Error al reservar memoria para costos_minimos");
        exit(1);
    }
    int *padre = (int *)malloc(cant_puntos * sizeof(int));
    if (padre == NULL) {
        printf("Error al reservar memoria para array");
        exit(1);
    }

    for (int i = 0; i < cant_puntos; i++) {
        visitado[i] = 0;
        costos_minimos[i] = INFINITY;
    }

    costos_minimos[0] = 0;
    padre[0] = -1;

    for (int count = 0; count < cant_puntos - 1; count++) {
        int u = -1;
        for (int i = 0; i < cant_puntos; i++) {
            if (!visitado[i] && (u == -1 || costos_minimos[i] < costos_minimos[u])) {
                u = i;
            }
        }

        visitado[u] = 1;

        for (int v = 0; v < cant_puntos; v++) {
            if (matriz[u][v] && !visitado[v] && matriz[u][v] < costos_minimos[v]) {
                padre[v] = u;
                costos_minimos[v] = matriz[u][v];
            }
        }
    }

    for (int i = 1; i < cant_puntos; i++) {
        arbol_prim[padre[i]][i] = arbol_prim[i][padre[i]] = matriz[i][padre[i]];
    }

    free(visitado);
    free(costos_minimos);
    free(padre);
}

void kruskal(float **matriz, int cant_puntos, float **arbol_kruskal) {
    typedef struct {
        int origen, destino;
        float peso;
    } Borde;

    Borde *bordes = malloc((cant_puntos * cant_puntos / 2) * sizeof(Borde));
    if (bordes == NULL) {
        printf("Error al reservar memoria para bordes");
        exit(1);
    }

    int e = 0;

    for (int i = 0; i < cant_puntos; i++) {
                for (int j = i + 1; j < cant_puntos; j++) {
            if (matriz[i][j] != -1) {
                bordes[e].origen = i;
                bordes[e].destino = j;
                bordes[e].peso = matriz[i][j];
                e++;
            }
        }
    }

    // Ordenar los bordes por peso
    qsort(bordes, e, sizeof(Borde), comparar_bordes);

    int *padre = malloc(cant_puntos * sizeof(int));
    if (padre == NULL) {
        printf("Error al reservar memoria para padre");
        exit(1);
    }
    int *rango = malloc(cant_puntos * sizeof(int));
    if (rango == NULL) {
        printf("Error al reservar memoria para rango");
        exit(1);
    }

    for (int v = 0; v < cant_puntos; v++) {
        padre[v] = v;
        rango[v] = 0;
    }

    int borde_aceptado = 0;
    int i = 0;

    while (borde_aceptado < cant_puntos - 1 && i < e) {
        Borde siguiente_borde = bordes[i++];
        int x = find(padre, siguiente_borde.origen);
        int y = find(padre, siguiente_borde.destino);

        if (x != y) {
            arbol_kruskal[siguiente_borde.origen][siguiente_borde.destino] = arbol_kruskal[siguiente_borde.destino][siguiente_borde.origen] = siguiente_borde.peso;
            union_find(padre, rango, x, y);
            borde_aceptado++;
        }
    }

    free(bordes);
    free(padre);
    free(rango);
}

int find(int padre[], int i) {
    if (padre[i] != i)
        padre[i] = find(padre, padre[i]);
    return padre[i];
}

void union_find(int padre[], int rango[], int x, int y) {
    int raiz_x = find(padre, x);
    int raiz_y = find(padre, y);

    if (raiz_x != raiz_y) {
        if (rango[raiz_x] > rango[raiz_y]) {
            padre[raiz_y] = raiz_x;
        } else if (rango[raiz_x] < rango[raiz_y]) {
            padre[raiz_x] = raiz_y;
        } else {
            padre[raiz_y] = raiz_x;
            rango[raiz_x]++;
        }
    }
}

int comparar_bordes(const void *a, const void *b) {
    Borde *borde_a = (Borde *)a;
    Borde *borde_b = (Borde *)b;
    return (borde_a->peso > borde_b->peso) - (borde_a->peso < borde_b->peso);
}

